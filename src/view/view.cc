//
// Created by Chastity Eryn on 7/19/23.
//

#include "view.h"

namespace s21 {

void View::Start() {
  StoreType store_type = GetStoreType();
  if (store_type == NonDefined) {
    PrintError("Wrong option");
  } else if (store_type == Research) {
    ResearchProcessing();
  } else {
    controller_.SetModel(store_type);
    StartMainCycle();
  }
}

StoreType View::GetStoreType() {
  std::cout << "select the store type to be used:\n"
               "\t1. HashTable\n"
               "\t2. SelfBalancingBinarySearchTree\n"
               "\t3. BPlusTree.\n"
               "\t4. Research on temporal characteristics.\n";

  std::string input;
  getline(std::cin, input);
  if (input == "1") {
    return HTable;
  } else if (input == "2") {
    return Tree;
  } else if (input == "3") {
    return BPTree;
  } else if (input == "4") {
    return Research;
  } else {
    return NonDefined;
  }
}

void View::PrintError(const std::string &message) {
  std::cout << message << "\n";
}

void View::StartMainCycle() {
  std::string input;
  while (input != "Q") {
    std::cout << "Enter command. Enter Q to exit\n";
    getline(std::cin, input);
    RewindBuffer();
    if (input != "Q" && !input.empty()) InputProcessing(input);
  }
  std::cout << "Exit\n";
}

void View::InputProcessing(const std::string &input) {
  std::vector<std::string> tokens;
  Split(input, tokens);
  if (tokens[0] == "SET") {
    ExecuteSet(tokens);
  } else if (tokens[0] == "GET") {
    ExecuteGet(tokens);
  } else if (tokens[0] == "EXISTS") {
    ExecuteExists(tokens);
  } else if (tokens[0] == "DEL") {
    ExecuteDel(tokens);
  } else if (tokens[0] == "UPDATE") {
    ExecuteUpdate(tokens);
  } else if (tokens[0] == "KEYS") {
    ExecuteKeys(tokens);
  } else if (tokens[0] == "RENAME") {
    ExecuteRename(tokens);
  } else if (tokens[0] == "TTL") {
    ExecuteTTL(tokens);
  } else if (tokens[0] == "FIND") {
    ExecuteFind(tokens);
  } else if (tokens[0] == "SHOWALL") {
    ExecuteShowAll(tokens);
  } else if (tokens[0] == "UPLOAD") {
    ExecuteUpload(tokens);
  } else if (tokens[0] == "EXPORT") {
    ExecuteExport(tokens);
  } else {
    PrintError("No such command");
  }
}

void View::ExecuteSet(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 7, 9)) {
    std::string result = controller_.Set(tokens);
    std::cout << result << "\n";
  }
}

void View::ExecuteGet(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    std::string result = controller_.Get(tokens[1]);
    std::cout << result << "\n";
  }
}

void View::ExecuteDel(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    std::string result = controller_.Del(tokens[1]);
    std::cout << result << "\n";
  }
}

void View::ExecuteUpdate(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 7)) {
    std::string result = controller_.Update(tokens);
    std::cout << result << "\n";
  }
}

void View::ExecuteKeys(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 1)) {
    std::vector<std::string> keys = controller_.Keys();

    for (int i = 0; i < (int)keys.size(); ++i) {
      std::cout << (i + 1) << ") " << keys[i] << "\n";
    }
  }
}

void View::ExecuteRename(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 3)) {
    std::string result = controller_.Rename(tokens[1], tokens[2]);
    std::cout << result << "\n";
  }
}

void View::ExecuteTTL(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    std::string result = controller_.TTL(tokens[1]);
    std::cout << result << "\n";
  }
}

void View::ExecuteFind(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 6)) {
    std::vector<std::string> keys = controller_.Find(tokens);
    if (!keys.empty() && keys[0] == "CAST ERROR") {
      PrintError("ERROR: unable to cast string to int");
    } else {
      for (int i = 0; i < (int)keys.size(); ++i) {
        std::cout << (i + 1) << ") " << keys[i] << "\n";
      }
    }
  }
}

void View::ExecuteShowAll(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 1)) {
    std::vector<Person> persons = controller_.ShowAll();
    std::vector<int> column_width{4, 20, 20, 16, 16, 10};

    PrintCenter("N", column_width[0]);
    PrintCenter("Surname", column_width[1]);
    PrintCenter("Name", column_width[2]);
    PrintCenter("Year Of Birth", column_width[3]);
    PrintCenter("City", column_width[4]);
    PrintCenter("Coins", column_width[5]);
    printf("\n");

    for (int i = 0; i < (int)persons.size(); ++i) {
      PrintCenter(std::to_string(i + 1), column_width[0]);
      Person person = persons[i];
      PrintCenter(person.GetSurname(), column_width[1]);
      PrintCenter(person.GetName(), column_width[2]);
      PrintCenter(std::to_string(person.GetYearOfBirth()), column_width[3]);
      PrintCenter(person.GetCity(), column_width[4]);
      PrintCenter(std::to_string(person.GetCoins()), column_width[5]);
      printf("\n");
    }
  }
}
void View::PrintCenter(const string &message, int width) {
  int left_space = (width - (int)message.length()) / 2;
  int right_space = width - (int)message.length() - left_space;
  printf("%*c%s%*c|", left_space, ' ', message.c_str(), right_space, ' ');
}

void View::ExecuteUpload(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    std::string result = controller_.Upload(tokens[1]);
    std::cout << result << "\n";
  }
}

void View::ExecuteExport(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    std::string result = controller_.Export(tokens[1]);
    std::cout << result << "\n";
  }
}

void View::ExecuteExists(const std::vector<std::string> &tokens) {
  if (CheckTokensNumber(tokens.size(), 2)) {
    bool exists = controller_.Exists(tokens[1]);
    std::cout << (exists ? "true" : "false") << "\n";
  }
}

void View::RewindBuffer() { std::cin.clear(); }

bool View::CheckTokensNumber(size_t token_size, int correct_number) {
  bool result = true;
  if (token_size != correct_number) {
    PrintError("Wrong number of input parameters");
    result = false;
  }
  return result;
}

bool View::CheckTokensNumber(size_t token_size, int correct_number_1,
                             int correct_number_2) {
  bool result = true;
  if (!(token_size == correct_number_1 || token_size == correct_number_2)) {
    PrintError("Wrong number of input parameters");
    result = false;
  }
  return result;
}

void View::Split(const std::string &str, std::vector<std::string> &out) {
  char delim = ' ';
  size_t start;
  size_t end = 0;
  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    out.push_back(str.substr(start, end - start));
  }
}

void View::ResearchProcessing() {
  int items_number, iterations_number;
  if (SetResearchParameters(items_number, iterations_number)) {
    std::cout << controller_.TemporalCharacteristics(items_number,
                                                     iterations_number);
  }
}

bool View::SetResearchParameters(int &items_number, int &iterations_number) {
  std::string input_1, input_2;
  std::cout << "Enter the number of items in the store\n";
  getline(std::cin, input_1);
  std::cout << "Enter number of iterations\n";
  getline(std::cin, input_2);
  if ((std::all_of(input_1.begin(), input_1.end(), ::isdigit)) &&
      (std::all_of(input_2.begin(), input_2.end(), ::isdigit))) {
    items_number = stoi(input_1);
    iterations_number = stoi(input_2);
    return true;
  } else {
    PrintError("Wrong input format");
    return false;
  }
}

}  // namespace s21
