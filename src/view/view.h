//
// Created by Chastity Eryn on 7/19/23.
//

#ifndef A6_TRANSACTIONS_3_VIEW_H
#define A6_TRANSACTIONS_3_VIEW_H

#include <iostream>

#include "../controller/controller.h"

namespace s21 {

class View {
 public:
  void Start();

 private:
  static StoreType GetStoreType();
  void StartMainCycle();
  void InputProcessing(const std::string& command);
  void ExecuteSet(const std::vector<std::string>& tokens);
  void ExecuteGet(const std::vector<std::string>& tokens);
  void ExecuteDel(const std::vector<std::string>& tokens);
  void ExecuteUpdate(const std::vector<std::string>& tokens);
  void ExecuteKeys(const std::vector<std::string>& tokens);
  void ExecuteRename(const std::vector<std::string>& tokens);
  void ExecuteTTL(const std::vector<std::string>& tokens);
  void ExecuteFind(const std::vector<std::string>& tokens);
  void ExecuteShowAll(const std::vector<std::string>& tokens);
  void ExecuteUpload(const std::vector<std::string>& tokens);
  void ExecuteExport(const std::vector<std::string>& tokens);
  void ExecuteExists(const std::vector<std::string>& tokens);
  static bool CheckTokensNumber(size_t token_size, int correct_number);
  static bool CheckTokensNumber(size_t token_size, int correct_number_1,
                                int correct_number_2);
  static void RewindBuffer();
  static void Split(const std::string& str, std::vector<std::string>& out);
  static void PrintError(const std::string& message);
  static void PrintCenter(const std::string& message, int width);
  void ResearchProcessing();
  static bool SetResearchParameters(int& items_number, int& iterations_number);

  controller controller_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_VIEW_H
