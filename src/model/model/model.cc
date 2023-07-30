//
// Created by Gwynesse Tandy on 7/21/23.
//

#include "model.h"

#include <sstream>

namespace s21 {

Model::~Model() {
  delete structure_;
  delete runtime_testing_;
}

void Model::SetModel(StoreType store_type) {
  if (store_type == HTable) {
    structure_ = new HashTable();
  } else if (store_type == Tree) {
    structure_ = new SelfBalancingBinarySearchTree();
  } else if (store_type == BPTree) {
    structure_ = new BPlusTree();
  }
}

std::string Model::Set(const vector<std::string>& tokens) {
  Record record = BuildRecord(tokens);
  std::string result;
  if (record.IsAlive()) {
    result = (structure_->Set(tokens[1], record)) ? "OK" : "key already exists";
  } else {
    result = "ERROR: unable to cast string to int ";
  }
  return result;
}

std::string Model::Get(const string& key) {
  Person person = structure_->Get(key);
  return person.IsNull() ? "(null)" : PersonToString(person);
}

std::string Model::Del(const string& key) {
  return structure_->Del(key) ? "true" : "false";
}

std::string Model::Update(const vector<std::string>& tokens) {
  std::string result;
  if (ValidateIncompleteTokens(tokens, 2)) {
    if (structure_->Update(tokens[1], BuildIncompletePerson(tokens, 2))) {
      result = "OK";
    } else {
      result = "Key doesn't exist";
    }
  } else {
    result = "ERROR: unable to cast string to int";
  }
  return result;
}

std::vector<std::string> Model::Keys() { return structure_->Keys(); }

std::string Model::Rename(const string& key_1, const string& key_2) {
  return (structure_->Rename(key_1, key_2)) ? "OK" : "(null)";
}

std::string Model::TTL(const string& key) {
  long response = structure_->Ttl(key);
  if (response == -1) {
    return "infinity";
  } else if (response == 0) {
    return "(null)";
  } else {
    return to_string(response);
  }
}

std::vector<std::string> Model::Find(const vector<std::string>& tokens) {
  std::vector<std::string> result;
  if (ValidateIncompleteTokens(tokens, 1)) {
    result = structure_->Find(BuildIncompletePerson(tokens, 1));
  } else {
    result.emplace_back("CAST ERROR");
  }
  return result;
}

std::vector<Person> Model::ShowAll() { return structure_->ShowAll(); }

std::string Model::Upload(const std::string& path) {
  pair<bool, int> response = structure_->Upload(path);
  std::stringstream fmt;
  if (response.first) {
    fmt << "OK " << response.second;
  } else {
    fmt << "ERROR: upload failed";
  }
  return fmt.str();
}
std::string Model::Export(const std::string& path) {
  pair<bool, int> response = structure_->Export(path);
  std::stringstream fmt;
  if (response.first) {
    fmt << "OK " << response.second;
  } else {
    fmt << "ERROR: export failed";
  }
  return fmt.str();
}

bool Model::Exists(const string& key) { return structure_->Exists(key); }

bool Model::ValidateSetTokens(const vector<std::string>& tokens) {
  bool valid_numbers =
      std::all_of(tokens[4].begin(), tokens[4].end(), ::isdigit) &&
      std::all_of(tokens[6].begin(), tokens[6].end(), ::isdigit);
  if (valid_numbers && tokens.size() == 9) {
    if (!std::all_of(tokens[8].begin(), tokens[8].end(), ::isdigit) ||
        tokens[7] != "EX") {
      valid_numbers = false;
    }
  }
  return valid_numbers;
}

bool Model::ValidateIncompleteTokens(const vector<std::string>& tokens,
                                     int start) {
  return (tokens[start + 2] == "-" ||
          std::all_of(tokens[start + 2].begin(), tokens[start + 2].end(),
                      ::isdigit)) &&
         (tokens[start + 4] == "-" ||
          std::all_of(tokens[start + 4].begin(), tokens[start + 4].end(),
                      ::isdigit));
}

Record Model::BuildRecord(const vector<std::string>& tokens) {
  Record temp_record;
  if (ValidateSetTokens(tokens)) {
    Person temp_person(tokens[2], tokens[3], stoi(tokens[4]), tokens[5],
                       stoi(tokens[6]));
    if (tokens.size() == 9) {
      temp_record = Record(temp_person, stoi(tokens[8]));
    } else {
      temp_record = Record(temp_person, 0);
    }
  } else {
    temp_record.SetIsAlive(false);
  }
  return temp_record;
}

Person Model::BuildIncompletePerson(const vector<std::string>& tokens,
                                    int start) {
  return Person{tokens[start] == "-" ? "-" : tokens[start],
                tokens[start + 1] == "-" ? "-" : tokens[start + 1],
                tokens[start + 2] == "-" ? -1 : stoi(tokens[start + 2]),
                tokens[start + 3] == "-" ? "-" : tokens[start + 3],
                tokens[start + 4] == "-" ? -1 : stoi(tokens[start + 4])};
}

std::string Model::PersonToString(const Person& person) {
  std::stringstream fmt;
  fmt << person.GetSurname() << " " << person.GetName() << " "
      << person.GetYearOfBirth() << " " << person.GetCity() << " "
      << person.GetCoins();
  return fmt.str();
}

std::string Model::TemporalCharacteristics(int items_number,
                                           int iterations_number) {
  runtime_testing_ = new RuntimeTesting();
  runtime_testing_->SetCapacity(items_number);
  runtime_testing_->StartResearch(iterations_number);
  return runtime_testing_->GetResult();
}

}  // namespace s21
