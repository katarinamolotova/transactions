//
// Created by Chastity Eryn on 7/20/23.
//

#include "file_handler.h"

namespace s21 {

bool FileHandler::Import(const string& path) {
  data_.clear();
  bool success = true;
  fstream in(path);
  if (!in.is_open()) return false;

  string line;
  while (getline(in, line)) {
    Person person;

    pair<string, bool> res = ParseLine(line, person);
    if (!res.second) {
      success = false;
      data_.clear();
      break;
    }
    data_.emplace_back(res.first, Record(person));
  }
  return success;
}

pair<string, bool> FileHandler::ParseLine(string line, Person& person) {
  bool success = false;
  size_t pos, count = 0;
  string delimiter = " ";
  string token, key;
  line.append(" ");
  while ((pos = line.find(delimiter)) != std::string::npos) {
    success = false;
    pair<string, bool> str_res;
    pair<int, bool> int_res;
    token = line.substr(0, pos);
    if (count == 0) {
      key = token;
    } else if (count == 1 || count == 2 || count == 4) {
      str_res = CheckString(token);
      if (!str_res.second) break;
    } else if (count == 3 || count == 5) {
      int_res = CheckInt(token);
      if (!int_res.second) break;
    } else {
      break;
    }
    SetValueToPerson(count, str_res.first, int_res.first, person);
    line.erase(0, pos + delimiter.length());
    count++;
    success = true;
  }
  return make_pair(key, (success && count == 6));
}

pair<string, bool> FileHandler::CheckString(string word) {
  if (word.size() < 3 || word[0] != '\"' || word[word.size() - 1] != '\"')
    return make_pair("", false);
  return make_pair(word.substr(1, word.size() - 2), true);
}

pair<int, bool> FileHandler::CheckInt(const string& num) {
  int result = 0;
  try {
    result = std::stoi(num);
  } catch (std::invalid_argument const& ex) {
    return make_pair(0, false);
  } catch (std::out_of_range const& ex) {
    return make_pair(0, false);
  }
  if (result < 0) return make_pair(0, false);
  return make_pair(result, true);
}

void FileHandler::SetValueToPerson(size_t number, const string& str_value,
                                   int int_value, Person& person) {
  if (number == 1)
    person.SetSurname(str_value);
  else if (number == 2)
    person.SetName(str_value);
  else if (number == 3)
    person.SetYearOfBirth(int_value);
  else if (number == 4)
    person.SetCity(str_value);
  else if (number == 5)
    person.SetCoins(int_value);
}

bool FileHandler::Export(const string& path,
                         const vector<pair<string, Record>>& data) {
  ofstream out(path);
  if (!out.is_open()) return false;

  for (const pair<string, Record>& item : data) {
    Person person = item.second.GetValues();
    string line =
        item.first + " \"" + person.GetSurname() + "\" \"" + person.GetName() +
        "\" " + to_string(person.GetYearOfBirth()) + " \"" + person.GetCity() +
        "\" " + to_string(person.GetCoins()) + "\n";
    out << line;
  }

  return true;
}

vector<pair<string, Record>> FileHandler::GetData() { return data_; }

}  // namespace s21
