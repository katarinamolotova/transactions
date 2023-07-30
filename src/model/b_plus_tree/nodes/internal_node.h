//
// Created by Chastity Eryn on 7/23/23.
//

#ifndef A6_TRANSACTIONS_3_INTERNAL_NODE_H
#define A6_TRANSACTIONS_3_INTERNAL_NODE_H

#include <utility>

#include "node.h"

namespace s21 {

class InternalNode : public Node {
 public:
  explicit InternalNode(int order);
  explicit InternalNode(int order, Node* parent);
  ~InternalNode() override;

  [[nodiscard]] bool IsLeaf() const override;
  [[nodiscard]] int Size() const override;
  [[nodiscard]] int MinSize() const override;
  [[nodiscard]] int MaxSize() const override;
  [[nodiscard]] const string FirstKey() const override;

  Node* MinChild();
  [[nodiscard]] string KeyAt(int index) const;
  void SetKeyAt(int index, string key);
  void PopulateNewRoot(Node* old_node, const string& new_key, Node* new_node);
  int InsertNodeAfter(Node* old_node, const string& new_key, Node* new_node);
  void Remove(int index);
  Node* RemoveAndReturnOnlyChild();
  string ReplaceAndReturnFirstKey();
  void MoveHalfTo(InternalNode* recipient);
  void MoveAllTo(InternalNode* recipient, int index_in_parent);
  void MoveFirstToEndOf(InternalNode* recipient);
  void MoveLastToFrontOf(InternalNode* recipient, int parent_index);
  [[nodiscard]] Node* LookUp(const string& key) const;
  int GetNodeIndex(Node* node) const;
  [[nodiscard]] Node* Neighbor(int index) const;

 private:
  vector<pair<string, Node*>> mappings_;
  void CopyHalfFrom(vector<pair<string, Node*>>& mappings);
  void CopyAllFrom(vector<pair<string, Node*>>& mappings);
  void CopyLastFrom(const pair<string, Node*>& pair);
  void CopyFirstFrom(const pair<string, Node*>& pair, int parent_index);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_INTERNAL_NODE_H
