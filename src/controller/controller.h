//
// Created by Gwynesse Tandy on 7/20/23.
//

#ifndef A6_TRANSACTIONS_3_CONTROLLER_H
#define A6_TRANSACTIONS_3_CONTROLLER_H

#include "../model/model/model.h"

namespace s21 {

class controller {
 public:
  void SetModel(StoreType store_type);
  std::string Set(const std::vector<std::string>& tokens);
  std::string Get(const std::string& key);
  std::string Del(const std::string& key);
  std::string Update(const std::vector<std::string>& tokens);
  std::vector<std::string> Keys();
  std::string Rename(const std::string& key_1, const std::string& key_2);
  std::string TTL(const std::string& key);
  std::vector<std::string> Find(const std::vector<std::string>& tokens);
  std::vector<Person> ShowAll();
  std::string Upload(const std::string& path);
  std::string Export(const std::string& path);
  bool Exists(const std::string& key);
  std::string TemporalCharacteristics(int items_number, int iterations_number);

 private:
  Model model_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_CONTROLLER_H
