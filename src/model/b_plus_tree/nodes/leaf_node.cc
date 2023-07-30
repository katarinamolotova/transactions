//
// Created by Chastity Eryn on 7/23/23.
//

#include "leaf_node.h"

namespace s21 {

LeafNode::LeafNode(int order) : Node(order), next(nullptr) {}

LeafNode::LeafNode(int order, Node *parent)
    : Node(order, parent), next(nullptr) {}

LeafNode::~LeafNode() {}

bool LeafNode::IsLeaf() const { return true; }

int LeafNode::MaxSize() const { return order - 1; }

int LeafNode::MinSize() const { return order / 2; }

const string LeafNode::FirstKey() const { return mappings_[0].first; }

int LeafNode::Size() const { return static_cast<int>(mappings_.size()); }

vector<pair<string, Record>> LeafNode::GetData() { return mappings_; }

void LeafNode::SetNewValue(const string &key, const Person &value) {
  for (auto &mapping : mappings_)
    if (mapping.first == key)
      mapping.second.SetValues(const_cast<Person &>(value));
}

pair<bool, int> LeafNode::CreateAndInsertRecord(const string &key,
                                                Record value) {
  if (LookUp(key).GetValues().IsNull())
    Insert(key, value);
  else
    return make_pair(false, static_cast<int>(mappings_.size()));
  return make_pair(true, static_cast<int>(mappings_.size()));
}

Record LeafNode::LookUp(const string &key) const {
  for (const auto &mapping : mappings_)
    if (mapping.first == key) return mapping.second;
  return {};
}

void LeafNode::Insert(const string &key, Record &value) {
  auto pos = mappings_.begin();
  while (pos != mappings_.end() && pos->first < key) ++pos;

  mappings_.insert(pos, make_pair(key, value));
}

void LeafNode::MoveHalfTo(LeafNode *recipient) {
  recipient->CopyHalfFrom(mappings_);
  for (size_t i = MinSize(); i < mappings_.size(); ++i) mappings_.pop_back();
}

void LeafNode::CopyHalfFrom(vector<pair<string, Record>> &mappings) {
  for (size_t i = MinSize(); i < mappings.size(); ++i)
    mappings_.push_back(mappings[i]);
}

int LeafNode::RemoveAndDeleteRecord(const string &key) {
  auto pos = mappings_.begin();
  while (pos != mappings_.end() && pos->first != key) ++pos;
  if (pos == mappings_.end()) return -1;

  mappings_.erase(pos);
  return static_cast<int>(mappings_.size());
}

void LeafNode::MoveFirstToEndOf(LeafNode *recipient) {
  recipient->CopyLastFrom(mappings_.front());
  mappings_.erase(mappings_.begin());
  dynamic_cast<InternalNode *>(parent)->SetKeyAt(1, mappings_.front().first);
}

void LeafNode::MoveLastToFrontOf(LeafNode *recipient, int parent_index) {
  recipient->CopyFirstFrom(mappings_.back(), parent_index);
  mappings_.pop_back();
}

void LeafNode::CopyLastFrom(const pair<string, Record> &pair) {
  mappings_.push_back(pair);
}

void LeafNode::CopyFirstFrom(const pair<string, Record> &pair,
                             int parent_index) {
  mappings_.insert(mappings_.begin(), pair);
  dynamic_cast<InternalNode *>(parent)->SetKeyAt(parent_index,
                                                 mappings_.front().first);
}

void LeafNode::MoveAllTo(LeafNode *recipient, int) {
  recipient->CopyAllFrom(mappings_);
  mappings_.clear();
  recipient->next = this->next;
}

void LeafNode::CopyAllFrom(vector<pair<string, Record>> &mappings) {
  for (const auto &mapping : mappings) mappings_.push_back(mapping);
}

}  // namespace s21
