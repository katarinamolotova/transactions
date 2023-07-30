//
// Created by Chastity Eryn on 7/22/23.
//

#ifndef A6_TRANSACTIONS_3_NODE_H
#define A6_TRANSACTIONS_3_NODE_H

#define DUMMY_KEY "-"

#include <string>
#include <vector>

#include "../../structures/record.h"

namespace s21 {

using std::make_pair;
using std::pair;
using std::string;
using std::vector;

class Node {
 public:
  explicit Node(int order) : order(order), parent(nullptr) {}
  explicit Node(int order, Node* parent) : order(order), parent(parent) {}
  virtual ~Node() = default;

  [[nodiscard]] virtual bool IsLeaf() const = 0;
  [[nodiscard]] virtual int Size() const = 0;
  [[nodiscard]] virtual int MinSize() const = 0;
  [[nodiscard]] virtual int MaxSize() const = 0;
  [[nodiscard]] virtual const string FirstKey() const = 0;

  int order;
  Node* parent;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_NODE_H
