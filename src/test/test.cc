//
// Created by Chastity Eryn on 7/21/23.
//

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../model/b_plus_tree/b_plus_tree.h"
#include "../model/hash_table/hash_table.h"
#include "../model/self_balancing_tree/self_balancing_binary_search_tree.h"

class TestSelfBinaryTree : public testing::Test {
 protected:
  s21::SelfBalancingBinarySearchTree tree;
  s21::Person person;
  s21::Record record;

  void SetUp() {
    person = s21::Person("Petrov", "Alex", 2000, "Moscow", 100);
    record = s21::Record(person);
  }

  void TearDown() {}
};

TEST_F(TestSelfBinaryTree, Set) {
  bool success = tree.Set("key", record);

  EXPECT_EQ(success, true);
  EXPECT_EQ(tree.GetSize(), 1);
}

TEST_F(TestSelfBinaryTree, GetSuccess) {
  tree.Set("key", record);
  s21::Person res_person = tree.Get("key");

  EXPECT_EQ(person == res_person, true);
}

TEST_F(TestSelfBinaryTree, GetFail) {
  s21::Person res_person = tree.Get("key");

  EXPECT_EQ(res_person.IsNull(), true);
}

TEST_F(TestSelfBinaryTree, ExistSuccess) {
  tree.Set("key", record);
  bool exist = tree.Exists("key");

  EXPECT_EQ(exist, true);
}

TEST_F(TestSelfBinaryTree, ExistFail) {
  bool exist = tree.Exists("key");

  EXPECT_EQ(exist, false);
}

TEST_F(TestSelfBinaryTree, DelSuccess) {
  tree.Set("key", record);
  bool success = tree.Del("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(tree.GetSize(), 0);
}

TEST_F(TestSelfBinaryTree, DelFail) {
  bool success = tree.Del("key");

  EXPECT_EQ(success, false);
}

TEST_F(TestSelfBinaryTree, UpdateSuccess) {
  tree.Set("key", record);
  s21::Person res = tree.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = tree.Update("key", update_person);
  res = tree.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestSelfBinaryTree, UpdateSuccessWithNotCompletePerson) {
  tree.Set("key", record);
  s21::Person res = tree.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("-", "-", -1, "-", 1000);
  bool success = tree.Update("key", update_person);
  res = tree.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetName() == "Alex", true);
  EXPECT_EQ(res.GetYearOfBirth(), 2000);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestSelfBinaryTree, UpdateFail) {
  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = tree.Update("key", update_person);

  EXPECT_EQ(success, false);
}

TEST_F(TestSelfBinaryTree, KeysEmpty) {
  std::vector<std::string> keys = tree.Keys();

  EXPECT_EQ(keys.empty(), true);
}

TEST_F(TestSelfBinaryTree, Keys) {
  tree.Set("key", record);
  std::vector<std::string> keys = tree.Keys();

  EXPECT_EQ(keys.size(), 1);
  EXPECT_EQ(keys[0] == "key", true);
}

TEST_F(TestSelfBinaryTree, RenameSuccess) {
  tree.Set("key", record);
  bool success = tree.Rename("key", "new_key");

  EXPECT_EQ(tree.Exists("new_key"), true);
  EXPECT_EQ(tree.Exists("key"), false);
  EXPECT_EQ(success, true);
}

TEST_F(TestSelfBinaryTree, RenameIfNewKeyExists) {
  tree.Upload("../datasets/file.dat");
  tree.Set("key", record);
  bool success = tree.Rename("key", "key1");

  EXPECT_EQ(tree.Exists("key1"), true);
  EXPECT_EQ(tree.Exists("key"), true);
  EXPECT_EQ(tree.GetSize(), 6);
  EXPECT_EQ(success, false);
}

TEST_F(TestSelfBinaryTree, UploadToEmptyTree) {
  std::pair<bool, int> res = tree.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
}

TEST_F(TestSelfBinaryTree, UploadToNotEmptyTree) {
  tree.Set("key1", record);
  std::pair<bool, int> res = tree.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 4);
}

TEST_F(TestSelfBinaryTree, UploadFailWrongOrderData) {
  std::pair<bool, int> res = tree.Upload("../datasets/bad_order_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestSelfBinaryTree, UploadFailNotExistsFile) {
  std::pair<bool, int> res = tree.Upload("../datasets/not_exist.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestSelfBinaryTree, UploadFailCountData) {
  std::pair<bool, int> res = tree.Upload("../datasets/bad_count_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestSelfBinaryTree, UploadFailWrongNumber) {
  std::pair<bool, int> res = tree.Upload("../datasets/bad_number_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestSelfBinaryTree, ExportSuccess) {
  tree.Upload("../datasets/file.dat");
  std::pair<bool, int> res = tree.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
}

TEST_F(TestSelfBinaryTree, ExportSuccessEmptyTree) {
  std::pair<bool, int> res = tree.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestSelfBinaryTree, FindOnePersonSuccess) {
  tree.Upload("../datasets/file.dat");

  s21::Person need_person("Сидоров", "Сергей", 1847, "Суздаль", 12312313);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res[0] == "key3", true);
  EXPECT_EQ(res.size(), 1);
}

TEST_F(TestSelfBinaryTree, FindLotOfPersonSuccess) {
  tree.Upload("../datasets/file.dat");

  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res.size(), 2);
  EXPECT_EQ(res[0] == "key1", true);
  EXPECT_EQ(res[1] == "key2", true);
}

TEST_F(TestSelfBinaryTree, FindEmptyTree) {
  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestSelfBinaryTree, ShowAllEmpty) {
  std::vector<s21::Person> res = tree.ShowAll();

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestSelfBinaryTree, ShowAllSuccess) {
  tree.Upload("../datasets/file.dat");
  std::vector<s21::Person> res = tree.ShowAll();

  EXPECT_EQ(res.size(), 5);
  EXPECT_EQ(res[0].GetCoins(), 55);
  EXPECT_EQ(res[1].GetName() == "Василий", true);
  EXPECT_EQ(res[2].GetYearOfBirth(), 1847);
}

TEST_F(TestSelfBinaryTree, TtlIsImmortal) {
  tree.Upload("../datasets/file.dat");

  EXPECT_EQ(tree.Ttl("key1"), -1);
}

TEST_F(TestSelfBinaryTree, TtlIsAlive) {
  s21::Record record_not_immortal(person, 10);
  tree.Set("key", record_not_immortal);
  EXPECT_EQ(tree.Ttl("key"), 10);
}

TEST_F(TestSelfBinaryTree, TtlCheckTime) {
  s21::Record record_not_immortal(person, 10);
  tree.Set("key", record_not_immortal);
  sleep(2);
  EXPECT_EQ(tree.Ttl("key"), 8);
}

TEST_F(TestSelfBinaryTree, TtlIsNotAlive) {
  s21::Record record_not_immortal(person, 1);
  tree.Set("key", record_not_immortal);
  sleep(2);
  EXPECT_EQ(tree.Ttl("key"), 0);
}

class TestHashTable : public testing::Test {
 protected:
  s21::HashTable table;
  s21::Person person;
  s21::Record record;

  void SetUp() {
    person = s21::Person("Petrov", "Alex", 2000, "Moscow", 100);
    record = s21::Record(person);
  }

  void TearDown() {}
};

TEST_F(TestHashTable, Set) {
  bool success = table.Set("key", record);

  EXPECT_EQ(success, true);
  EXPECT_EQ(table.GetSize(), 1);
}

TEST_F(TestHashTable, GetSuccess) {
  table.Set("key", record);
  s21::Person res_person = table.Get("key");

  EXPECT_EQ(person == res_person, true);
}

TEST_F(TestHashTable, GetFail) {
  s21::Person res_person = table.Get("key");

  EXPECT_EQ(res_person.IsNull(), true);
}

TEST_F(TestHashTable, ExistSuccess) {
  table.Set("key", record);
  bool exist = table.Exists("key");

  EXPECT_EQ(exist, true);
}

TEST_F(TestHashTable, ExistFail) {
  bool exist = table.Exists("key");

  EXPECT_EQ(exist, false);
}

TEST_F(TestHashTable, DelSuccess) {
  table.Set("key", record);
  bool success = table.Del("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(table.GetSize(), 0);
}

TEST_F(TestHashTable, DelFail) {
  bool success = table.Del("key");

  EXPECT_EQ(success, false);
}

TEST_F(TestHashTable, UpdateSuccess) {
  table.Set("key", record);
  s21::Person res = table.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = table.Update("key", update_person);
  res = table.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestHashTable, UpdateSuccessWithNotCompletePerson) {
  table.Set("key", record);
  s21::Person res = table.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("-", "-", -1, "-", 1000);
  bool success = table.Update("key", update_person);
  res = table.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetName() == "Alex", true);
  EXPECT_EQ(res.GetYearOfBirth(), 2000);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestHashTable, UpdateFail) {
  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = table.Update("key", update_person);

  EXPECT_EQ(success, false);
}

TEST_F(TestHashTable, KeysEmpty) {
  std::vector<std::string> keys = table.Keys();

  EXPECT_EQ(keys.empty(), true);
}

TEST_F(TestHashTable, Keys) {
  table.Set("key", record);
  std::vector<std::string> keys = table.Keys();

  EXPECT_EQ(keys.size(), 1);
  EXPECT_EQ(keys[0] == "key", true);
}

TEST_F(TestHashTable, RenameSuccess) {
  table.Set("key", record);
  bool success = table.Rename("key", "new_key");

  EXPECT_EQ(table.Exists("new_key"), true);
  EXPECT_EQ(table.Exists("key"), false);
  EXPECT_EQ(success, true);
}

TEST_F(TestHashTable, UploadToEmptyTree) {
  std::pair<bool, int> res = table.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
}

TEST_F(TestHashTable, UploadToNotEmptyTree) {
  table.Set("key1", record);
  std::pair<bool, int> res = table.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 4);
}

TEST_F(TestHashTable, UploadFailWrongOrderData) {
  std::pair<bool, int> res = table.Upload("../datasets/bad_order_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestHashTable, UploadFailNotExistsFile) {
  std::pair<bool, int> res = table.Upload("../datasets/not_exist.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestHashTable, UploadFailCountData) {
  std::pair<bool, int> res = table.Upload("../datasets/bad_count_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestHashTable, UploadFailWrongNumber) {
  std::pair<bool, int> res = table.Upload("../datasets/bad_number_data.dat");

  EXPECT_EQ(res.first, false);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestHashTable, ExportSuccess) {
  table.Upload("../datasets/file.dat");
  std::pair<bool, int> res = table.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
}

TEST_F(TestHashTable, ExportSuccessEmptyTree) {
  std::pair<bool, int> res = table.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestHashTable, FindOnePersonSuccess) {
  table.Upload("../datasets/file.dat");

  s21::Person need_person("Сидоров", "Сергей", 1847, "Суздаль", 12312313);
  std::vector<std::string> res = table.Find(need_person);

  EXPECT_EQ(res[0] == "key3", true);
  EXPECT_EQ(res.size(), 1);
}

TEST_F(TestHashTable, FindLotOfPersonSuccess) {
  table.Upload("../datasets/file.dat");

  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = table.Find(need_person);

  EXPECT_EQ(res.size(), 2);
  EXPECT_EQ(res[0] == "key1", true);
  EXPECT_EQ(res[1] == "key2", true);
}

TEST_F(TestHashTable, FindEmptyTree) {
  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = table.Find(need_person);

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestHashTable, ShowAllEmpty) {
  std::vector<s21::Person> res = table.ShowAll();

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestHashTable, ShowAllSuccess) {
  table.Upload("../datasets/file.dat");
  std::vector<s21::Person> res = table.ShowAll();

  EXPECT_EQ(res.size(), 5);
  EXPECT_EQ(res[0].GetCoins(), 55);
  EXPECT_EQ(res[1].GetName() == "Василий", true);
  EXPECT_EQ(res[2].GetYearOfBirth(), 1847);
}

TEST_F(TestHashTable, TtlIsImmortal) {
  table.Upload("../datasets/file.dat");

  EXPECT_EQ(table.Ttl("key1"), -1);
}

TEST_F(TestHashTable, TtlIsAlive) {
  s21::Record record_not_immortal(person, 10);
  table.Set("key", record_not_immortal);
  EXPECT_EQ(table.Ttl("key"), 10);
}

TEST_F(TestHashTable, TtlCheckTime) {
  s21::Record record_not_immortal(person, 10);
  table.Set("key", record_not_immortal);
  sleep(2);
  EXPECT_EQ(table.Ttl("key"), 8);
}

TEST_F(TestHashTable, TtlIsNotAlive) {
  s21::Record record_not_immortal(person, 1);
  table.Set("key", record_not_immortal);
  sleep(2);
  s21::Person empty = table.Get("key");
  EXPECT_EQ(empty.IsNull(), true);
  EXPECT_EQ(table.Ttl("key"), 0);
}

TEST_F(TestHashTable, RenameIfNewKeyExists) {
  table.Upload("../datasets/file.dat");
  table.Set("key", record);
  bool success = table.Rename("key", "key1");

  EXPECT_EQ(table.Exists("key1"), true);
  EXPECT_EQ(table.Exists("key"), true);
  EXPECT_EQ(table.GetSize(), 6);
  EXPECT_EQ(success, false);
}

class TestBPlusTree : public testing::Test {
 protected:
  s21::BPlusTree tree;
  s21::Person person;
  s21::Record record;

  void SetUp() {
    person = s21::Person("Petrov", "Alex", 2000, "Moscow", 100);
    record = s21::Record(person);
  }

  void TearDown() {}
};

TEST_F(TestBPlusTree, Set) {
  bool success = tree.Set("key", record);

  EXPECT_EQ(success, true);
  EXPECT_EQ(tree.GetSize(), 1);
}

TEST_F(TestBPlusTree, GetSuccess) {
  tree.Set("key", record);
  s21::Person res_person = tree.Get("key");

  EXPECT_EQ(person == res_person, true);
}

TEST_F(TestBPlusTree, GetFail) {
  s21::Person res_person = tree.Get("key");

  EXPECT_EQ(res_person.IsNull(), true);
}

TEST_F(TestBPlusTree, ExistSuccess) {
  tree.Set("key", record);
  bool exist = tree.Exists("key");

  EXPECT_EQ(exist, true);
}

TEST_F(TestBPlusTree, ExistFail) {
  bool exist = tree.Exists("key");

  EXPECT_EQ(exist, false);
}

TEST_F(TestBPlusTree, DelSuccess) {
  tree.Set("key", record);
  bool success = tree.Del("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(tree.GetSize(), 0);
}

TEST_F(TestBPlusTree, DelFail) {
  bool success = tree.Del("key");

  EXPECT_EQ(success, false);
}

TEST_F(TestBPlusTree, UpdateSuccess) {
  tree.Set("key", record);
  s21::Person res = tree.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = tree.Update("key", update_person);
  res = tree.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestBPlusTree, UpdateSuccessWithNotCompletePerson) {
  tree.Set("key", record);
  s21::Person res = tree.Get("key");
  EXPECT_EQ(res.GetCoins(), 100);

  s21::Person update_person("-", "-", -1, "-", 1000);
  bool success = tree.Update("key", update_person);
  res = tree.Get("key");

  EXPECT_EQ(success, true);
  EXPECT_EQ(res.GetName() == "Alex", true);
  EXPECT_EQ(res.GetYearOfBirth(), 2000);
  EXPECT_EQ(res.GetCoins(), 1000);
}

TEST_F(TestBPlusTree, UpdateFail) {
  s21::Person update_person("Petrov", "Alex", 2000, "Moscow", 1000);
  bool success = tree.Update("key", update_person);

  EXPECT_EQ(success, false);
}

TEST_F(TestBPlusTree, KeysEmpty) {
  std::vector<std::string> keys = tree.Keys();

  EXPECT_EQ(keys.empty(), true);
}

TEST_F(TestBPlusTree, Keys) {
  tree.Set("key", record);
  std::vector<std::string> keys = tree.Keys();

  EXPECT_EQ(keys.size(), 1);
  EXPECT_EQ(keys[0] == "key", true);
}

TEST_F(TestBPlusTree, RenameSuccess) {
  tree.Set("key", record);
  bool success = tree.Rename("key", "new_key");

  EXPECT_EQ(tree.Exists("new_key"), true);
  EXPECT_EQ(tree.Exists("key"), false);
  EXPECT_EQ(tree.GetSize(), 1);
  EXPECT_EQ(success, true);
}

TEST_F(TestBPlusTree, UploadToEmptyTree) {
  std::pair<bool, int> res = tree.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
  EXPECT_EQ(tree.GetSize(), 5);
}

TEST_F(TestBPlusTree, UploadToNotEmptyTree) {
  tree.Set("key1", record);
  std::pair<bool, int> res = tree.Upload("../datasets/file.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 4);
  EXPECT_EQ(tree.GetSize(), 5);
}

TEST_F(TestBPlusTree, ExportSuccess) {
  tree.Upload("../datasets/file.dat");
  std::pair<bool, int> res = tree.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 5);
  EXPECT_EQ(tree.GetSize(), 5);
}

TEST_F(TestBPlusTree, ExportSuccessEmptyTree) {
  std::pair<bool, int> res = tree.Export("../datasets/export.dat");

  EXPECT_EQ(res.first, true);
  EXPECT_EQ(res.second, 0);
}

TEST_F(TestBPlusTree, FindOnePersonSuccess) {
  tree.Upload("../datasets/file.dat");

  s21::Person need_person("Сидоров", "Сергей", 1847, "Суздаль", 12312313);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res[0] == "key3", true);
  EXPECT_EQ(res.size(), 1);
}

TEST_F(TestBPlusTree, FindLotOfPersonSuccess) {
  tree.Upload("../datasets/file.dat");

  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res.size(), 2);
  EXPECT_EQ(res[0] == "key1", true);
  EXPECT_EQ(res[1] == "key2", true);
}

TEST_F(TestBPlusTree, FindEmptyTree) {
  s21::Person need_person("-", "-", -1, "-", 55);
  std::vector<std::string> res = tree.Find(need_person);

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestBPlusTree, ShowAllEmpty) {
  std::vector<s21::Person> res = tree.ShowAll();

  EXPECT_EQ(res.size(), 0);
}

TEST_F(TestBPlusTree, ShowAllSuccess) {
  tree.Upload("../datasets/file.dat");
  std::vector<s21::Person> res = tree.ShowAll();

  EXPECT_EQ(res.size(), 5);
  EXPECT_EQ(res[0].GetCoins(), 55);
  EXPECT_EQ(res[1].GetName() == "Василий", true);
  EXPECT_EQ(res[2].GetYearOfBirth(), 1847);
}

TEST_F(TestBPlusTree, TtlIsImmortal) {
  tree.Upload("../datasets/file.dat");

  EXPECT_EQ(tree.Ttl("key1"), -1);
}

TEST_F(TestBPlusTree, TtlIsAlive) {
  s21::Record record_not_immortal(person, 10);
  tree.Set("key", record_not_immortal);
  EXPECT_EQ(tree.Ttl("key"), 10);
}

TEST_F(TestBPlusTree, TtlCheckTime) {
  s21::Record record_not_immortal(person, 10);
  tree.Set("key", record_not_immortal);
  sleep(2);
  EXPECT_EQ(tree.Ttl("key"), 8);
}

TEST_F(TestBPlusTree, TtlIsNotAlive) {
  s21::Record record_not_immortal(person, 1);
  tree.Set("key", record_not_immortal);
  sleep(2);
  EXPECT_EQ(tree.Ttl("key"), 0);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
