//
// Created by Chastity Eryn on 7/23/23.
//

#ifndef A6_TRANSACTIONS_3_LEAF_NODE_H
#define A6_TRANSACTIONS_3_LEAF_NODE_H

#include "internal_node.h"

namespace s21 {

class LeafNode : public Node {
 public:
  explicit LeafNode(int order);
  explicit LeafNode(int order, Node* parent);
  ~LeafNode() override;

  LeafNode* next;

  [[nodiscard]] bool IsLeaf() const override;
  [[nodiscard]] int Size() const override;
  [[nodiscard]] int MinSize() const override;
  [[nodiscard]] int MaxSize() const override;
  [[nodiscard]] const string FirstKey() const override;
  vector<pair<string, Record>> GetData();
  void SetNewValue(const string& key, const Person& value);

  pair<bool, int> CreateAndInsertRecord(const string& key, Record value);
  void Insert(const string& key, Record& value);
  [[nodiscard]] Record LookUp(const string& key) const;
  int RemoveAndDeleteRecord(const string& key);
  void MoveHalfTo(LeafNode* recipient);
  void MoveAllTo(LeafNode* recipient, int);
  void MoveFirstToEndOf(LeafNode* recipient);
  void MoveLastToFrontOf(LeafNode* recipient, int parent_index);

 private:
  vector<pair<string, Record>> mappings_;
  void CopyHalfFrom(vector<pair<string, Record>>& mappings);
  void CopyAllFrom(vector<pair<string, Record>>& mappings);
  void CopyLastFrom(const pair<string, Record>& pair);
  void CopyFirstFrom(const pair<string, Record>& pair, int parent_index);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_LEAF_NODE_H
