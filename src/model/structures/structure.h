//
// Created by Chastity Eryn on 7/19/23.
//

#ifndef A6_TRANSACTIONS_3_STRUCTURE_H
#define A6_TRANSACTIONS_3_STRUCTURE_H

#include <vector>

#include "../helpers/file_handler.h"
#include "record.h"

namespace s21 {

using std::pair;
using std::string;
using std::vector;

class Structure {
 public:
  virtual bool Set(string key, Record value) = 0;
  virtual Person Get(string key) = 0;
  virtual bool Exists(string key) = 0;
  virtual bool Del(string key) = 0;
  virtual bool Update(string key, const Person& value) = 0;
  virtual vector<string> Keys() = 0;
  virtual bool Rename(string old_key, string new_key) = 0;
  virtual long Ttl(string key) = 0;
  virtual vector<string> Find(const Person& value) = 0;
  virtual vector<Person> ShowAll() = 0;
  virtual pair<bool, int> Upload(string path) = 0;
  virtual pair<bool, int> Export(string path) = 0;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_STRUCTURE_H
