//
// Created by Chastity Eryn on 7/20/23.
//

#ifndef A6_TRANSACTIONS_3_FILE_HANDLER_H
#define A6_TRANSACTIONS_3_FILE_HANDLER_H

#include <fstream>
#include <vector>

#include "../structures/record.h"

namespace s21 {

using std::fstream;
using std::make_pair;
using std::ofstream;
using std::pair;
using std::to_string;
using std::vector;

class FileHandler {
 public:
  FileHandler() = default;
  ~FileHandler() = default;

  bool Import(const string& path);
  static bool Export(const string& path,
                     const vector<pair<string, Record>>& data);
  vector<pair<string, Record>> GetData();

 private:
  vector<pair<string, Record>> data_;

  static pair<string, bool> ParseLine(
      string line,
      Person& person);  //  возвращает ключ и ошибку, запись меняется по ссылке
  static void SetValueToPerson(size_t number, const string& str_value,
                               int int_value, Person& person);
  static pair<string, bool> CheckString(string word);
  static pair<int, bool> CheckInt(const string& num);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_FILE_HANDLER_H
