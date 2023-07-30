//
// Created by Rebbecca Gormond on 7/22/23.
//

#ifndef A6_TRANSACTIONS_3_RUNTIME_TESTING_H
#define A6_TRANSACTIONS_3_RUNTIME_TESTING_H
#define MIN_REPEATS 10
#define MIN_VALUES 10

#include <chrono>

#include "../hash_table/hash_table.h"
#include "../helpers/generator.h"
#include "../self_balancing_tree/self_balancing_binary_search_tree.h"
#include "../structures/person.h"
#include "../structures/record.h"
#include "../structures/structure.h"

namespace s21 {

using std::array;
using std::pair;
using std::string;
using std::vector;

class RuntimeTesting {
 public:
  RuntimeTesting();
  ~RuntimeTesting();

  void SetCapacity(size_t number_of_persons);
  void StartResearch(size_t number_of_repetitions);
  string GetResult();

 private:
  void PrepareSetForTests(size_t number_of_repetitions);
  void TestSetItem();
  void TestGetItem();
  void TestDelItem();
  void TestShowAllItems();
  void TestFindKey();

  HashTable* hash_table_;
  SelfBalancingBinarySearchTree* b_tree_;
  size_t number_of_persons_{};

  vector<pair<string, Person>> test_case_;
  vector<pair<double, double>> result_time_test_;  // set get del show find
};

#endif  // A6_TRANSACTIONS_3_RUNTIME_TESTING_H
}