//
// Created by Chastity Eryn on 7/19/23.
//

#include "self_balancing_binary_search_tree.h"

namespace s21 {

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree() {
  root_ = nullptr;
  size_ = 0;
  nil_ = new Node();
}

SelfBalancingBinarySearchTree::~SelfBalancingBinarySearchTree() {
  size_t start_size = size_;
  for (size_t i = 0; i < start_size; i++) Del(MinNode(root_)->data.first);
  delete nil_;
}

bool SelfBalancingBinarySearchTree::Set(string key, Record value) {
  size_t start_size = size_;
  Node* node = new Node(std::make_pair(key, value), nil_, nil_, nullptr,
                        (root_ == nullptr) ? BLACK : RED);
  if (size_ == 0)
    root_ = node;
  else
    InsertTreeElem(root_, node);
  size_++;

  if (start_size == size_)
    delete node;
  else
    InsertFixupColor(node);

  nil_->parent = MaxNode(root_);
  return start_size < size_;
}

Person SelfBalancingBinarySearchTree::Get(string key) {
  Node* node = FindNode(key, root_);
  if (node == nil_) return {};
  return node->data.second.GetValues();
}

bool SelfBalancingBinarySearchTree::Exists(string key) {
  return (FindNode(key, root_) != nil_);
}

bool SelfBalancingBinarySearchTree::Del(string key) {
  Node* node = FindNode(key, root_);
  if (node == nil_) return false;
  DeleteTreeElem(node);
  delete node;
  return true;
}

bool SelfBalancingBinarySearchTree::Update(string key, const Person& value) {
  Node* node = FindNode(key, root_);
  if (node == nil_) return false;
  Person temp = node->data.second.GetValues();
  temp = value;
  node->data.second.SetValues(temp);
  return true;
}

vector<string> SelfBalancingBinarySearchTree::Keys() {
  vector<string> result;
  vector<Node*> delete_later;
  if (size_ > 0) {
    for (Iterator it(MinNode(root_)); it.GetNode() != nil_; it++) {
      if (it->second.IsAlive())
        result.push_back(it->first);
      else
        delete_later.push_back(it.GetNode());
    }
  }
  DeleteListOfNode(delete_later);
  return result;
}

bool SelfBalancingBinarySearchTree::Rename(string old_key, string new_key) {
  if (old_key == new_key) return true;
  if (Exists(new_key)) return false;
  Node* node = FindNode(old_key, root_);
  if (node == nil_) return false;
  Record record = node->data.second;
  DeleteTreeElem(node);
  delete node;
  Set(new_key, record);
  return true;
}

long SelfBalancingBinarySearchTree::Ttl(string key) {
  Node* node = FindNode(key, root_);
  if (node == nil_) return 0;
  return node->data.second.RemainingTime();
}

vector<string> SelfBalancingBinarySearchTree::Find(const Person& value) {
  vector<string> result;
  vector<Node*> delete_later;
  if (size_ > 0) {
    for (Iterator it(MinNode(root_)); it.GetNode() != nil_; it++) {
      if (!it->second.IsAlive())
        delete_later.push_back(it.GetNode());
      else if (it->second.GetValues() == value)
        result.push_back(it->first);
    }
  }
  DeleteListOfNode(delete_later);
  return result;
}

vector<Person> SelfBalancingBinarySearchTree::ShowAll() {
  vector<Person> result;
  vector<Node*> delete_later;
  if (size_ > 0) {
    for (Iterator it(MinNode(root_)); it.GetNode() != nil_; it++) {
      if (it->second.IsAlive())
        result.push_back(it->second.GetValues());
      else
        delete_later.push_back(it.GetNode());
    }
  }
  DeleteListOfNode(delete_later);
  return result;
}

pair<bool, int> SelfBalancingBinarySearchTree::Upload(string path) {
  FileHandler file_handler;
  if (!file_handler.Import(path)) return make_pair(false, 0);

  int count = 0;
  for (const pair<string, Record>& item : file_handler.GetData()) {
    if (Set(item.first, item.second)) count++;
  }
  return make_pair(true, count);
}

pair<bool, int> SelfBalancingBinarySearchTree::Export(string path) {
  vector<pair<string, Record>> data;
  if (size_ > 0)
    for (Iterator it(MinNode(root_)); it.GetNode() != nil_; it++)
      data.emplace_back(it->first, it->second);

  if (!FileHandler::Export(path, data)) return make_pair(false, 0);
  return make_pair(true, static_cast<int>(data.size()));
}

size_t SelfBalancingBinarySearchTree::GetSize() { return size_; }

//  iterator

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::Iterator::GetNode() {
  return node_;
}

pair<string, Record>& SelfBalancingBinarySearchTree::Iterator::operator*() {
  return this->node_->data;
}

pair<string, Record>* SelfBalancingBinarySearchTree::Iterator::operator->() {
  return &this->node_->data;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::Iterator::operator++(int) {
  return node_ = FindNextElem(node_);
}

bool SelfBalancingBinarySearchTree::Iterator::operator!=(
    const SelfBalancingBinarySearchTree::Iterator& it) {
  return this->node_ != it.node_;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::Iterator::FindNextElem(
    SelfBalancingBinarySearchTree::Node* node) {
  if (node->right == nullptr) {
    node = node->parent;
  } else if (node->right->right != nullptr) {
    node = MinNode(node->right);
  } else {
    Node* start_node = node;
    Node* p = node->parent;
    while (p != nullptr && node == p->right) {
      node = p;
      p = p->parent;
    }
    node = (p == nullptr) ? start_node->right : p;
  }
  return node;
}

//  rb_tree

void SelfBalancingBinarySearchTree::DeleteListOfNode(
    const vector<SelfBalancingBinarySearchTree::Node*>& nodes) {
  for (Node* node : nodes) DeleteTreeElem(node);
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::FindNode(
    const string& key, SelfBalancingBinarySearchTree::Node* root) {
  Node* res = nil_;
  if (root_ != nullptr && root != nullptr) {
    if (key < root->data.first)
      res = FindNode(key, root->left);
    else if (key > root->data.first)
      res = FindNode(key, root->right);
    else
      res = root;
  }

  bool is_deleted = false;
  if (res != nil_ && !res->data.second.IsAlive()) {
    DeleteTreeElem(res);
    is_deleted = true;
    delete res;
  }

  return (is_deleted) ? nil_ : res;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::MinNode(
    SelfBalancingBinarySearchTree::Node* root) {
  if (root->left->left != nullptr) root = MinNode(root->left);
  return root;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::MaxNode(
    SelfBalancingBinarySearchTree::Node* root) {
  if (root->right->right != nullptr) root = MaxNode(root->right);
  return root;
}

void SelfBalancingBinarySearchTree::InsertTreeElem(
    SelfBalancingBinarySearchTree::Node* root,
    SelfBalancingBinarySearchTree::Node* elem) {
  if (elem->data.first < root->data.first) {
    if (root->left == nil_) {
      root->left = elem;
      elem->parent = root;
    } else {
      InsertTreeElem(root->left, elem);
    }
  } else if (elem->data.first > root->data.first) {
    if (root->right == nil_) {
      root->right = elem;
      elem->parent = root;
    } else {
      InsertTreeElem(root->right, elem);
    }
  } else {
    this->size_--;
  }
}

void SelfBalancingBinarySearchTree::InsertFixupColor(
    SelfBalancingBinarySearchTree::Node* elem) {
  while (elem != root_ && elem->parent->color == RED) {
    if (elem->parent == elem->parent->parent->right)
      elem = InsertFixupRightBranch(elem);
    else
      elem = InsertFixupLeftBranch(elem);
  }
  root_->color = BLACK;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::InsertFixupRightBranch(
    SelfBalancingBinarySearchTree::Node* elem) {
  Node* uncle = elem->parent->parent->left;
  if (uncle->color == RED) {
    uncle->color = BLACK;
    elem->parent->color = BLACK;
    elem->parent->parent->color = RED;
    elem = elem->parent->parent;
  } else {
    if (elem == elem->parent->left) {
      elem = elem->parent;
      RightRotate(elem);
    }
    elem->parent->color = BLACK;
    elem->parent->parent->color = RED;
    LeftRotate(elem->parent->parent);
  }
  return elem;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::InsertFixupLeftBranch(
    SelfBalancingBinarySearchTree::Node* elem) {
  Node* uncle = elem->parent->parent->right;
  if (uncle->color == RED) {
    uncle->color = BLACK;
    elem->parent->color = BLACK;
    elem->parent->parent->color = RED;
    elem = elem->parent->parent;
  } else {
    if (elem == elem->parent->right) {
      elem = elem->parent;
      LeftRotate(elem);
    }
    elem->parent->color = BLACK;
    elem->parent->parent->color = RED;
    RightRotate(elem->parent->parent);
  }
  return elem;
}

void SelfBalancingBinarySearchTree::LeftRotate(
    SelfBalancingBinarySearchTree::Node* root) {
  Node* new_root = root->right;
  root->right = new_root->left;
  if (new_root->left != nil_) new_root->left->parent = root;
  new_root->parent = root->parent;
  if (root->parent == nullptr)
    root_ = new_root;
  else if (root == root->parent->left)
    root->parent->left = new_root;
  else
    root->parent->right = new_root;
  new_root->left = root;
  root->parent = new_root;
}

void SelfBalancingBinarySearchTree::RightRotate(
    SelfBalancingBinarySearchTree::Node* root) {
  Node* new_root = root->left;
  root->left = new_root->right;
  if (new_root->right != nil_) new_root->right->parent = root;
  new_root->parent = root->parent;
  if (root->parent == nullptr)
    root_ = new_root;
  else if (root == root->parent->right)
    root->parent->right = new_root;
  else
    root->parent->left = new_root;
  new_root->right = root;
  root->parent = new_root;
}

void SelfBalancingBinarySearchTree::Transplant(
    SelfBalancingBinarySearchTree::Node* delete_node,
    SelfBalancingBinarySearchTree::Node* new_node) {
  if (delete_node->parent == nullptr)
    root_ = new_node;
  else if (delete_node == delete_node->parent->left)
    delete_node->parent->left = new_node;
  else
    delete_node->parent->right = new_node;
  new_node->parent = delete_node->parent;
}

void SelfBalancingBinarySearchTree::DeleteTreeElem(
    SelfBalancingBinarySearchTree::Node* elem) {
  Node* child;
  Node* sup_node = elem;
  bool start_color = sup_node->color;
  if (elem->left == nil_) {
    child = elem->right;
    Transplant(elem, elem->right);
  } else if (elem->right == nil_) {
    child = elem->left;
    Transplant(elem, elem->left);
  } else {
    sup_node = MinNode(elem->right);
    start_color = sup_node->color;
    child = sup_node->right;
    if (sup_node->parent == elem) {
      child->parent = sup_node;
    } else {
      Transplant(sup_node, sup_node->right);
      sup_node->right = elem->right;
      sup_node->right->parent = sup_node;
    }
    Transplant(elem, sup_node);
    sup_node->left = elem->left;
    sup_node->left->parent = sup_node;
    sup_node->color = elem->color;
  }
  if (start_color == BLACK) DeleteFixupColor(child);
  size_--;
}

void SelfBalancingBinarySearchTree::DeleteFixupColor(
    SelfBalancingBinarySearchTree::Node* elem) {
  while (elem != root_ && elem->color == BLACK) {
    if (elem == elem->parent->left) {
      elem = DeleteFixupLeftBranch(elem);
    } else {
      elem = DeleteFixupRightBranch(elem);
    }
  }
  elem->color = BLACK;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::DeleteFixupLeftBranch(
    SelfBalancingBinarySearchTree::Node* elem) {
  Node* brother = elem->parent->right;
  if (brother->color == RED) {
    brother->color = BLACK;
    elem->parent->color = RED;
    LeftRotate(elem->parent);
    brother = elem->parent->right;
  }
  if (brother->left->color == BLACK && brother->right->color == BLACK) {
    brother->color = RED;
    elem = elem->parent;
  } else {
    if (brother->right->color == BLACK) {
      brother->left->color = BLACK;
      brother->color = RED;
      RightRotate(brother);
      brother = elem->parent->right;
    }
    brother->color = elem->parent->color;
    elem->parent->color = BLACK;
    brother->right->color = BLACK;
    LeftRotate(elem->parent);
    elem = root_;
  }
  return elem;
}

SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::DeleteFixupRightBranch(
    SelfBalancingBinarySearchTree::Node* elem) {
  Node* brother = elem->parent->left;
  if (brother->color == RED) {
    brother->color = BLACK;
    elem->parent->color = RED;
    RightRotate(elem->parent);
    brother = elem->parent->left;
  }
  if (brother->left->color == BLACK && brother->right->color == BLACK) {
    brother->color = RED;
    elem = elem->parent;
  } else {
    if (brother->left->color == BLACK) {
      brother->right->color = BLACK;
      brother->color = RED;
      LeftRotate(brother);
      brother = elem->parent->left;
    }
    brother->color = elem->parent->color;
    elem->parent->color = BLACK;
    brother->left->color = BLACK;
    RightRotate(elem->parent);
    elem = root_;
  }
  return elem;
}

}  // namespace s21