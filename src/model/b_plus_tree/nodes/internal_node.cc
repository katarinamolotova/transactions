//
// Created by Chastity Eryn on 7/23/23.
//

#include "internal_node.h"

namespace s21 {

InternalNode::InternalNode(int order) : Node(order) {}

InternalNode::InternalNode(int order, Node *parent) : Node(order, parent) {}

InternalNode::~InternalNode() {
  for (const auto &mapping : mappings_) {
    delete mapping.second;
  }
}

bool InternalNode::IsLeaf() const { return false; }

int InternalNode::Size() const { return static_cast<int>(mappings_.size()); }

int InternalNode::MinSize() const { return (order + 1) / 2; }

int InternalNode::MaxSize() const { return order; }

const string InternalNode::FirstKey() const {
  return mappings_[1].first;
  ;
}

Node *InternalNode::MinChild() { return mappings_.front().second; }

Node *InternalNode::LookUp(const string &key) const {
  auto pos = mappings_.begin();
  while (pos != mappings_.end() && key >= pos->first) ++pos;
  --pos;
  return pos->second;
}

void InternalNode::PopulateNewRoot(Node *old_node, const string &new_key,
                                   Node *new_node) {
  mappings_.emplace_back(DUMMY_KEY, old_node);
  mappings_.emplace_back(new_key, new_node);
}

int InternalNode::InsertNodeAfter(Node *old_node, const string &new_key,
                                  Node *new_node) {
  auto iter = mappings_.begin();
  for (; iter != mappings_.end() && iter->second != old_node; ++iter)
    mappings_.insert(iter + 1, make_pair(new_key, new_node));
  return Size();
}

string InternalNode::ReplaceAndReturnFirstKey() {
  string new_key = mappings_[0].first;
  mappings_[0].first = DUMMY_KEY;
  return new_key;
}

void InternalNode::MoveHalfTo(InternalNode *recipient) {
  recipient->CopyHalfFrom(mappings_);
  for (size_t i = MinSize(); i < mappings_.size(); ++i) mappings_.pop_back();
}

void InternalNode::CopyHalfFrom(vector<pair<string, Node *>> &mappings) {
  for (size_t i = MinSize(); i < mappings_.size(); ++i) {
    mappings[i].second->parent = this;
    mappings_.push_back(mappings[i]);
  }
}

int InternalNode::GetNodeIndex(Node *node) const {
  for (int i = 0; i < Size(); ++i) {
    if (mappings_[i].second == node) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

Node *InternalNode::Neighbor(int index) const {
  return mappings_[index].second;
}

Node *InternalNode::RemoveAndReturnOnlyChild() {
  Node *child = mappings_.front().second;
  mappings_.pop_back();
  return child;
}

string InternalNode::KeyAt(int index) const { return mappings_[index].first; }

void InternalNode::SetKeyAt(int index, string key) {
  mappings_[index].first = std::move(key);
}

void InternalNode::MoveFirstToEndOf(InternalNode *recipient) {
  recipient->CopyLastFrom(mappings_.front());
  mappings_.erase(mappings_.begin());
  dynamic_cast<InternalNode *>(parent)->SetKeyAt(1, mappings_.front().first);
}

void InternalNode::MoveLastToFrontOf(InternalNode *recipient,
                                     int parent_index) {
  recipient->CopyFirstFrom(mappings_.back(), parent_index);
  mappings_.pop_back();
}

void InternalNode::CopyLastFrom(const pair<string, Node *> &pair) {
  mappings_.push_back(pair);
  mappings_.back().first = mappings_.back().second->FirstKey();
  mappings_.back().second->parent = this;
}

void InternalNode::CopyFirstFrom(const pair<string, Node *> &pair,
                                 int parent_index) {
  mappings_.front().first =
      dynamic_cast<InternalNode *>(parent)->KeyAt(parent_index);
  mappings_.insert(mappings_.begin(), pair);
  mappings_.front().first = DUMMY_KEY;
  mappings_.front().second->parent = this;
  dynamic_cast<InternalNode *>(parent)->SetKeyAt(parent_index,
                                                 mappings_.front().first);
}

void InternalNode::MoveAllTo(InternalNode *recipient, int index_in_parent) {
  mappings_[0].first =
      dynamic_cast<InternalNode *>(parent)->KeyAt(index_in_parent);
  recipient->CopyAllFrom(mappings_);
  mappings_.clear();
}

void InternalNode::Remove(int index) {
  mappings_.erase(mappings_.begin() + index);
}

void InternalNode::CopyAllFrom(vector<pair<string, Node *>> &mappings) {
  for (const auto &mapping : mappings) {
    mapping.second->parent = this;
    mappings_.push_back(mapping);
  }
}

}  // namespace s21