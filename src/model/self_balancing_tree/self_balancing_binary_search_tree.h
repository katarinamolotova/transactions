//
// Created by Chastity Eryn on 7/19/23.
//

#ifndef A6_TRANSACTIONS_3_SELF_BALANCING_BINARY_SEARCH_TREE_H
#define A6_TRANSACTIONS_3_SELF_BALANCING_BINARY_SEARCH_TREE_H

#include "../structures/structure.h"

namespace s21 {

#define RED true
#define BLACK false

class SelfBalancingBinarySearchTree : public Structure {
 public:
  SelfBalancingBinarySearchTree();
  ~SelfBalancingBinarySearchTree();

  bool Set(string key, Record value) override;
  Person Get(string key) override;
  bool Exists(string key) override;
  bool Del(string key) override;
  bool Update(string key, const Person& value) override;
  vector<string> Keys() override;
  bool Rename(string old_key, string new_key) override;
  long Ttl(string key) override;  //
  vector<string> Find(const Person& value) override;
  vector<Person> ShowAll() override;
  pair<bool, int> Upload(string path) override;
  pair<bool, int> Export(string path) override;
  size_t GetSize();

 private:
  class Node {
   public:
    Node *left, *right, *parent;
    pair<string, Record> data;
    bool color;
    explicit Node(const pair<string, Record>& data = pair<string, Record>(),
                  Node* left = nullptr, Node* right = nullptr,
                  Node* parent = nullptr, bool color = BLACK) {
      this->data = data;
      this->left = left;
      this->right = right;
      this->parent = parent;
      this->color = color;
    }
  };

  Node* root_;
  Node* nil_;
  size_t size_;

  class Iterator {
   public:
    explicit Iterator(Node* node) : node_(node) {}
    Node* GetNode();
    pair<string, Record>& operator*();
    pair<string, Record>* operator->();
    Node* operator++(int);
    bool operator!=(const Iterator& it);

   private:
    Node* node_;
    Node* FindNextElem(Node* node);
  };

  void DeleteListOfNode(const vector<Node*>&);
  Node* FindNode(const string& key, Node* root);
  static Node* MinNode(Node* root);
  static Node* MaxNode(Node* root);

  void InsertTreeElem(Node* root, Node* elem);
  void InsertFixupColor(Node* elem);
  Node* InsertFixupRightBranch(Node* elem);
  Node* InsertFixupLeftBranch(Node* elem);

  void DeleteTreeElem(Node* elem);
  void DeleteFixupColor(Node* elem);
  Node* DeleteFixupRightBranch(Node* elem);
  Node* DeleteFixupLeftBranch(Node* elem);

  void LeftRotate(Node* root);
  void RightRotate(Node* root);
  void Transplant(Node* delete_node, Node* new_node);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_SELF_BALANCING_BINARY_SEARCH_TREE_H
