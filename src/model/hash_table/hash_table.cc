//
// Created by Rebbecca Gormond on 7/20/23.
//

#include "hash_table.h"

#include <utility>

namespace s21 {

HashTable::HashTable() { this->table_.resize(1024); }

HashTable::~HashTable() = default;

bool HashTable::Set(string key, Record value) {
  if (Exists(key)) return false;
  size_t hashIndex = GetHashCode(key);
  if (hashIndex >= table_.capacity()) {
    table_.resize(hashIndex * 2);
  }
  while (hashIndex < table_.capacity() - 1 &&
         table_.at(hashIndex).second.IsAlive()) {
    hashIndex++;
  }
  table_.at(hashIndex) = PrepareForInsert(key, value);
  size_++;
  return true;
}

Person HashTable::Get(string key) {
  size_t hashIndex = GetHashCode(key);

  while (hashIndex < table_.capacity() - 1) {
    if (table_.at(hashIndex).first == key &&
        table_.at(hashIndex).second.IsAlive()) {
      return table_.at(hashIndex).second.GetValues();
    } else if (!table_.at(hashIndex).second.IsAlive()) {
      this->Del(key);
    }
    hashIndex++;
  }
  return {};
}

bool HashTable::Exists(string key) {
  size_t hashIndex = GetHashCode(key);

  while (hashIndex < table_.capacity() - 1) {
    if (table_.at(hashIndex).first == key &&
        table_.at(hashIndex).second.IsAlive()) {
      return true;
    }
    hashIndex++;
  }
  return false;
}

bool HashTable::Del(string key) {
  if (!Exists(key)) return false;

  size_t hashIndex = GetHashCode(key);
  while (hashIndex < table_.capacity() - 1 &&
         table_.at(hashIndex).first != key &&
         table_.at(hashIndex).second.IsAlive()) {
    hashIndex++;
  }
  if (hashIndex + 1 == table_.capacity()) return false;
  table_.at(hashIndex).second.SetIsAlive(false);
  size_--;
  return true;
}

bool HashTable::Update(string key, const Person& value) {
  size_t hashIndex = GetHashCode(key);
  if (table_.at(hashIndex).second.IsAlive()) {
    table_.at(hashIndex).second.SetValues(const_cast<Person&>(value));
    return true;
  } else if (!table_.at(hashIndex).second.IsAlive()) {
    this->Del(key);
  }
  return false;
}

vector<string> HashTable::Keys() {
  vector<string> keys;
  for (pair<string, Record> i : table_) {
    if (i.second.IsAlive()) {
      keys.emplace_back(i.first);
    } else {
      this->Del(i.first);
    }
  }
  return keys;
}

bool HashTable::Rename(string old_key, string new_key) {
  size_t old_hash = GetHashCode(old_key);
  size_t new_hash = GetHashCode(new_key);

  if (!Exists(new_key) && table_.at(old_hash).second.IsAlive() &&
      !table_.at(new_hash).second.IsAlive()) {
    table_.at(old_hash).first = new_key;
    table_.at(new_hash) = table_.at(old_hash);
    table_.at(old_hash).first.clear();
    table_.at(old_hash).second.SetIsAlive(false);
    return true;
  }
  return false;
}

long HashTable::Ttl(string key) {
  size_t hashIndex = GetHashCode(key);
  long time = table_.at(hashIndex).second.RemainingTime();
  if (!table_.at(hashIndex).second.IsAlive()) {
    this->Del(key);
  }
  return time;
}

vector<string> HashTable::Find(const Person& value) {
  vector<string> result;
  for (const pair<string, Record>& i : table_) {
    if (i.first.empty()) continue;
    if (i.second.GetValues() == value) {
      result.emplace_back(i.first);
    }
  }
  return result;
}

vector<Person> HashTable::ShowAll() {
  vector<Person> result;
  for (pair<string, Record> i : table_) {
    if (i.second.IsAlive()) {
      result.emplace_back(i.second.GetValues());
    } else {
      this->Del(i.first);
    }
  }
  return result;
}

pair<bool, int> HashTable::Upload(string path) {
  pair<bool, int> counter = {false, 0};
  int count = 0;
  if (!file_handler_.Import(path)) return counter;

  for (const pair<string, Record>& i : file_handler_.GetData()) {
    if (Exists(i.first)) continue;

    size_t hash = GetHashCode(i.first);
    table_.at(hash) = i;
    counter.first = true;
    count++;
  }
  counter.second = count;
  size_ += counter.second;
  return counter;
}

pair<bool, int> HashTable::Export(string path) {
  vector<pair<string, Record>> result;
  for (const pair<string, Record>& i : table_) {
    if (i.first.empty()) continue;
    result.emplace_back(i);
  }
  return {file_handler_.Export(path, result), result.size()};
}

size_t HashTable::GetHashCode(const string& key) {
  unsigned long hash = 0;
  for (char word : key) hash += word;
  return hash % this->table_.capacity();
}

pair<string, Record> HashTable::PrepareForInsert(const string& key,
                                                 const Record& record) {
  pair<string, Record> result;
  result.first = key;
  result.second = record;
  return result;
}

[[maybe_unused]] size_t HashTable::GetSize() const { return this->size_; }
}  // namespace s21