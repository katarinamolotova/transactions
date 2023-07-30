//
// Created by Rebbecca Gormond on 7/22/23.
//

#include "runtime_testing.h"

namespace s21 {

RuntimeTesting::RuntimeTesting() {
  hash_table_ = new HashTable();
  b_tree_ = new SelfBalancingBinarySearchTree();
}

RuntimeTesting::~RuntimeTesting() {
  delete hash_table_;
  delete b_tree_;
}

string RuntimeTesting::GetResult() {
  string result =
      "Research result for a key-value store with number of users = " +
      std::to_string(number_of_persons_) +
      " \nA total of iterations were performed = " +
      std::to_string(test_case_.size()) + ".\n\n";

  string pattern[] = {" Set ", " Get ", " Delete ", " Show All ", " Find key "};
  for (int i = 0; i < result_time_test_.size(); i++) {
    result += "Test case:" + pattern[i] + " \nHashTable duration = " +
              std::to_string(result_time_test_[i].first) +
              " seconds\nSelfBalancingBinarySearchTree duration = " +
              std::to_string(result_time_test_[i].second) + " seconds\n\n";
  }
  return result;
}

void RuntimeTesting::StartResearch(size_t number_of_repetitions) {
  PrepareSetForTests(number_of_repetitions);
  TestSetItem();
  TestGetItem();
  TestDelItem();
  TestShowAllItems();
  TestFindKey();
}

void RuntimeTesting::TestSetItem() {
  array<double, 2> result{};
  for (int i = 0; i < result.size(); ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& j : test_case_) {
      if (i == 0) {
        hash_table_->Set(j.first, Record(j.second));
      } else {
        b_tree_->Set(j.first, Record(j.second));
      }
    }
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    result[i] = duration.count() / static_cast<double>(test_case_.size());
  }
  result_time_test_.emplace_back(result[0], result[1]);
}

void RuntimeTesting::TestGetItem() {
  array<double, 2> result{};
  for (int i = 0; i < result.size(); ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& j : test_case_) {
      if (i == 0) {
        hash_table_->Get(j.first);
      } else {
        b_tree_->Get(j.first);
      }
    }
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    result[i] = duration.count() / static_cast<double>(test_case_.size());
  }
  result_time_test_.emplace_back(result[0], result[1]);
}

void RuntimeTesting::TestDelItem() {
  array<double, 2> result{};
  for (int i = 0; i < result.size(); ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& j : test_case_) {
      if (i == 0) {
        hash_table_->Del(j.first);
      } else {
        b_tree_->Del(j.first);
      }
    }
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    result[i] = duration.count() / static_cast<double>(test_case_.size());
  }
  result_time_test_.emplace_back(result[0], result[1]);
}

void RuntimeTesting::TestShowAllItems() {
  array<double, 2> result{};
  for (int i = 0; i < result.size(); ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& j : test_case_) {
      if (i == 0) {
        hash_table_->ShowAll();
      } else {
        b_tree_->ShowAll();
      }
    }
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    result[i] = duration.count() / static_cast<double>(test_case_.size());
  }
  result_time_test_.emplace_back(result[0], result[1]);
}

void RuntimeTesting::TestFindKey() {
  array<double, 2> result{};
  for (int i = 0; i < result.size(); ++i) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for (auto& j : test_case_) {
      if (i == 0) {
        hash_table_->Find(j.second);
      } else {
        b_tree_->Find(j.second);
      }
    }
    std::chrono::duration<double> duration =
        std::chrono::high_resolution_clock::now() - start_time;
    result[i] = duration.count() / static_cast<double>(test_case_.size());
  }
  result_time_test_.emplace_back(result[0], result[1]);
}

void RuntimeTesting::PrepareSetForTests(size_t number_of_repetitions) {
  size_t n = (number_of_repetitions < MIN_REPEATS) ? MIN_REPEATS
                                                   : number_of_repetitions;
  auto generator = Generator(n);
  while (!generator.IsEmpty()) {
    test_case_.emplace_back(generator.GetRandomPerson());
  }
}

void RuntimeTesting::SetCapacity(size_t number_of_persons) {
  this->number_of_persons_ =
      (number_of_persons < MIN_VALUES) ? MIN_VALUES : number_of_persons;
  auto generator = Generator(number_of_persons_);
  pair<string, Person> record;

  while (!generator.IsEmpty()) {
    record = generator.GetRandomPerson();
    hash_table_->Set(record.first, Record(record.second));
    b_tree_->Set(record.first, Record(record.second));
  }
}
}  // namespace s21