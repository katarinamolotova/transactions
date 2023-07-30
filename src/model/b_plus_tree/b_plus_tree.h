//
// Created by Chastity Eryn on 7/22/23.
//

#ifndef A6_TRANSACTIONS_3_B_PLUS_TREE_H
#define A6_TRANSACTIONS_3_B_PLUS_TREE_H

#define DEFAULT_ORDER 4

#include <algorithm>

#include "../structures/structure.h"
#include "nodes/internal_node.h"
#include "nodes/leaf_node.h"

namespace s21 {

class BPlusTree : public Structure {
 public:
  explicit BPlusTree();
  ~BPlusTree();

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
  [[nodiscard]] size_t GetSize() const;

 private:
  Node* root_;
  int order_;
  size_t size_;

  vector<pair<string, Record>> GetData();
  bool IsEmpty();
  bool StartNewTree(const string& key, Record& value);
  bool InsertIntoLeaf(const string& key, Record& value);
  LeafNode* FindLeafNode(const string& key);
  void InsertIntoParent(Node* old_node, const string& key, Node* new_node);
  bool DeleteLeaf(LeafNode* leaf, const string& key);
  void AdjustRoot();

  template <typename N>
  N* Split(N* node);
  template <typename N>
  void CoalesceOrRedistribute(N* node);
  template <typename N>
  void Coalesce(N* neighbor_node, N* node, InternalNode* parent, int index);
  template <typename N>
  void Redistribute(N* neighbor_node, N* node, int index);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_B_PLUS_TREE_H
