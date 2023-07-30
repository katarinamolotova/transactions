//
// Created by Rebbecca Gormond on 7/20/23.
//

#include "../helpers/file_handler.h"
#include "../structures/structure.h"

#ifndef A6_TRANSACTIONS_3_HASH_TABLE_H
#define A6_TRANSACTIONS_3_HASH_TABLE_H

namespace s21 {

using std::pair;
using std::string;
using std::vector;

class HashTable : public Structure {
 public:
  HashTable();
  ~HashTable();

  bool Set(string key, Record value) override;
  Person Get(string key) override;
  bool Exists(string key) override;
  bool Del(string key) override;
  bool Update(string key, const Person& value) override;
  vector<string> Keys() override;
  bool Rename(string old_key, string new_key) override;
  long Ttl(string key) override;
  vector<string> Find(const Person& value) override;
  vector<Person> ShowAll() override;
  pair<bool, int> Upload(string path) override;
  pair<bool, int> Export(string path) override;

  [[maybe_unused]] [[nodiscard]] size_t GetSize() const;

 private:
  size_t GetHashCode(const string& key);
  static pair<string, Record> PrepareForInsert(const string&, const Record&);

  vector<pair<string, Record> > table_;
  FileHandler file_handler_;
  size_t size_ = 0;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_HASH_TABLE_H
