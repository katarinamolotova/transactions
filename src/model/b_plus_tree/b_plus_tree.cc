//
// Created by Chastity Eryn on 7/22/23.
//

#include "b_plus_tree.h"

#include <utility>

namespace s21 {

BPlusTree::BPlusTree() {
  root_ = nullptr;
  order_ = DEFAULT_ORDER;
  size_ = 0;
}

BPlusTree::~BPlusTree() {
  if (root_ && root_->IsLeaf())
    delete dynamic_cast<LeafNode*>(root_);
  else if (root_)
    delete dynamic_cast<InternalNode*>(root_);

  root_ = nullptr;
  size_ = 0;
}

size_t BPlusTree::GetSize() const { return size_; }

bool BPlusTree::Set(string key, Record value) {
  if (Exists(key)) return false;
  if (IsEmpty())
    StartNewTree(key, value);
  else
    InsertIntoLeaf(key, value);
  size_++;
  return true;
}

Person BPlusTree::Get(string key) {
  if (size_ == 0) return {};
  LeafNode* node = FindLeafNode(key);
  return (node) ? node->LookUp(key).GetValues() : Person();
}

bool BPlusTree::Exists(string key) { return !Get(key).IsNull(); }

bool BPlusTree::Del(string key) {
  bool success = false;
  if (!IsEmpty()) {
    LeafNode* node = FindLeafNode(key);
    success = DeleteLeaf(node, key);
  }
  if (success) size_--;
  return success;
}

bool BPlusTree::Update(string key, const Person& value) {
  if (size_ == 0 || !Exists(key)) return false;
  LeafNode* node = FindLeafNode(key);
  if (!node) return false;
  Person temp = node->LookUp(key).GetValues();
  temp = value;
  node->SetNewValue(key, temp);
  return true;
}

vector<string> BPlusTree::Keys() {
  vector<string> result;
  vector<pair<string, Record>> data = GetData();

  result.reserve(data.size());
  for (const pair<string, Record>& item : data) result.push_back(item.first);

  return result;
}

bool BPlusTree::Rename(string old_key, string new_key) {
  if (Exists(new_key)) return false;
  if (old_key == new_key) return true;
  LeafNode* node = FindLeafNode(old_key);
  if (!node) return false;
  Record record = node->LookUp(old_key);
  bool res = Del(old_key);
  res &= Set(new_key, record);
  return res;
}

long BPlusTree::Ttl(string key) {
  LeafNode* node = FindLeafNode(key);
  return node ? node->LookUp(key).RemainingTime() : 0;
}

vector<string> BPlusTree::Find(const Person& value) {
  vector<string> result;
  vector<pair<string, Record>> data = GetData();

  for (const auto& item : data) {
    if (item.second.GetValues() == value) result.push_back(item.first);
  }

  return result;
}

vector<Person> BPlusTree::ShowAll() {
  vector<Person> result;
  vector<pair<string, Record>> data = GetData();

  result.reserve(data.size());
  for (const pair<string, Record>& item : data)
    result.push_back(item.second.GetValues());

  return result;
}

pair<bool, int> BPlusTree::Upload(string path) {
  FileHandler file_handler;
  if (!file_handler.Import(path)) return make_pair(false, 0);

  int count = 0;
  for (const pair<string, Record>& item : file_handler.GetData())
    if (Set(item.first, item.second)) count++;
  return make_pair(true, count);
}

pair<bool, int> BPlusTree::Export(string path) {
  vector<pair<string, Record>> result = GetData();

  FileHandler file_handler;
  if (!file_handler.Export(path, result)) return make_pair(false, 0);
  return make_pair(true, static_cast<int>(result.size()));
}

vector<pair<string, Record>> BPlusTree::GetData() {
  vector<pair<string, Record>> result;
  LeafNode* node;
  if (root_ == nullptr)
    return result;
  else if (root_->IsLeaf())
    node = dynamic_cast<LeafNode*>(root_);
  else
    node =
        dynamic_cast<LeafNode*>(dynamic_cast<InternalNode*>(root_)->MinChild());

  while (node) {
    auto data = node->GetData();
    for (pair<string, Record>& item : data) {
      bool exists = false;
      for (const pair<string, Record>& pair : result) {
        if (item.first == pair.first) {
          exists = true;
          break;
        }
      }
      if (!exists) result.emplace_back(item.first, item.second);
    }
    node = node->next;
  }

  return result;
}

//  b+ tree

bool BPlusTree::IsEmpty() { return !root_; }

bool BPlusTree::StartNewTree(const string& key, Record& value) {
  auto* new_leaf_node = new LeafNode(order_);
  bool success = new_leaf_node->CreateAndInsertRecord(key, value).first;
  root_ = new_leaf_node;
  return success;
}

bool BPlusTree::InsertIntoLeaf(const string& key, Record& value) {
  LeafNode* leaf = FindLeafNode(key);
  if (!leaf) return false;

  pair<bool, int> temp = leaf->CreateAndInsertRecord(key, value);
  int new_size = temp.second;
  if (new_size > leaf->MaxSize()) {
    LeafNode* new_leaf = Split(leaf);
    new_leaf->next = leaf->next;
    leaf->next = new_leaf;
    string new_key = new_leaf->FirstKey();
    InsertIntoParent(leaf, new_key, new_leaf);
  }
  return temp.first;
}

LeafNode* BPlusTree::FindLeafNode(const string& key) {
  if (IsEmpty()) return nullptr;
  Node* node = root_;

  while (!node->IsLeaf()) {
    auto* internal_node = dynamic_cast<InternalNode*>(node);
    node = internal_node->LookUp(key);
  }

  auto* leaf = dynamic_cast<LeafNode*>(node);
  bool is_deleted = false;
  Record record = leaf->LookUp(key);
  if (!record.GetValues().IsNull() && !record.IsAlive()) {
    DeleteLeaf(leaf, key);
    is_deleted = true;
  }

  return is_deleted ? nullptr : leaf;
}

void BPlusTree::InsertIntoParent(Node* old_node, const string& key,
                                 Node* new_node) {
  auto* parent = dynamic_cast<InternalNode*>(old_node->parent);
  if (parent == nullptr) {
    root_ = new InternalNode(order_);
    parent = dynamic_cast<InternalNode*>(root_);
    old_node->parent = parent;
    new_node->parent = parent;
    parent->PopulateNewRoot(old_node, key, new_node);
  } else {
    int new_size = parent->InsertNodeAfter(old_node, key, new_node);
    if (new_size > parent->MaxSize()) {
      InternalNode* new_internal_node = Split(parent);
      string new_key = new_internal_node->ReplaceAndReturnFirstKey();
      InsertIntoParent(parent, new_key, new_internal_node);
    }
  }
}

bool BPlusTree::DeleteLeaf(LeafNode* leaf, const string& key) {
  if (!leaf || leaf->LookUp(key).GetValues().IsNull()) return false;

  int new_size = leaf->RemoveAndDeleteRecord(key);
  if (new_size == -1)
    return false;
  else if (new_size < leaf->MinSize())
    CoalesceOrRedistribute(leaf);

  return true;
}

template <typename N>
void BPlusTree::CoalesceOrRedistribute(N* node) {
  if (node == root_) {
    AdjustRoot();
  } else {
    auto parent = static_cast<InternalNode*>(node->parent);
    int index_node_in_parent = parent->GetNodeIndex(node);
    if (index_node_in_parent == -1) return;
    int neighbor_index =
        (index_node_in_parent == 0) ? 1 : index_node_in_parent - 1;
    N* neighbor_node = static_cast<N*>(parent->Neighbor(neighbor_index));
    if (node->Size() + neighbor_node->Size() <= neighbor_node->MaxSize()) {
      Coalesce(neighbor_node, node, parent, index_node_in_parent);
    } else {
      Redistribute(neighbor_node, node, index_node_in_parent);
    }
  }
}

void BPlusTree::AdjustRoot() {
  if (!root_->IsLeaf() && root_->Size() == 1) {
    auto node = dynamic_cast<InternalNode*>(root_);
    root_ = dynamic_cast<InternalNode*>(root_)->RemoveAndReturnOnlyChild();
    root_->parent = nullptr;
    delete node;
  } else {
    delete root_;
    root_ = nullptr;
  }
}

template <typename N>
void BPlusTree::Redistribute(N* neighbor_node, N* node, int index) {
  if (index == 0)
    neighbor_node->MoveFirstToEndOf(node);
  else
    neighbor_node->MoveLastToFrontOf(node, index);
}

template <typename N>
void BPlusTree::Coalesce(N* neighbor_node, N* node, InternalNode* parent,
                         int index) {
  if (index == 0) {
    std::swap(node, neighbor_node);
    index = 1;
  }
  node->MoveAllTo(neighbor_node, index);
  parent->Remove(index);
  if (parent->Size() < parent->MinSize()) CoalesceOrRedistribute(parent);
  delete node;
}

template <typename N>
N* BPlusTree::Split(N* node) {
  N* new_node = new N(order_, node->parent);
  node->MoveHalfTo(new_node);
  return new_node;
}

}  // namespace s21