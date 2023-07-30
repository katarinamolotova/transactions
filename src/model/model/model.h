//
// Created by Gwynesse Tandy on 7/21/23.
//

#ifndef A6_TRANSACTIONS_3_MODEL_H
#define A6_TRANSACTIONS_3_MODEL_H

#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>

#include "../b_plus_tree/b_plus_tree.h"
#include "../hash_table/hash_table.h"
#include "../runtime_testing/runtime_testing.h"
#include "../self_balancing_tree/self_balancing_binary_search_tree.h"
#include "../structures/person.h"
#include "../structures/record.h"
#include "../structures/structure.h"

namespace s21 {

enum StoreType { HTable, Tree, NonDefined, Research, BPTree };

class Model {
 public:
  void SetModel(StoreType store_type);
  ~Model();
  std::string Set(const std::vector<std::string>& tokens);
  std::string Get(const std::string& key);
  std::string Del(const std::string& key);
  std::string Update(const std::vector<std::string>& tokens);
  std::vector<std::string> Keys();
  std::string Rename(const std::string& key_1, const std::string& key_2);
  std::string TTL(const std::string& key);
  std::vector<std::string> Find(const std::vector<std::string>& tokens);
  std::vector<Person> ShowAll();
  std::string Upload(const std::string& path);
  std::string Export(const std::string& path);
  bool Exists(const std::string& key);
  std::string TemporalCharacteristics(int items_number, int iterations_number);

 private:
  static bool ValidateSetTokens(const vector<std::string>& tokens);
  static bool ValidateIncompleteTokens(const vector<std::string>& tokens,
                                       int start);
  static Record BuildRecord(const vector<std::string>& tokens);
  static Person BuildIncompletePerson(const vector<std::string>& tokens,
                                      int start);
  static std::string PersonToString(const Person& person);

  Structure* structure_;
  RuntimeTesting* runtime_testing_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_MODEL_H
