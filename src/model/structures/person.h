//
// Created by Rebbecca Gormond on 7/20/23.
//

#ifndef A6_TRANSACTIONS_3_PERSON_H
#define A6_TRANSACTIONS_3_PERSON_H

#define STR_TOKEN "-"
#define DIGIT_TOKEN 0

#include <iostream>
#include <string>

namespace s21 {

using std::string;

class Person {
 public:
  Person() = default;
  Person(string surname, string name, int year_of_birth, string city,
         int coins);
  Person(const Person &person);
  ~Person() = default;

  Person &operator=(const Person &other);
  bool operator==(const Person &other);

  [[nodiscard]] const string &GetSurname() const;
  [[nodiscard]] const string &GetName() const;
  [[nodiscard]] int GetYearOfBirth() const;
  [[nodiscard]] const string &GetCity() const;
  [[nodiscard]] int GetCoins() const;

  void SetSurname(const string &surname);
  void SetName(const string &name);
  void SetYearOfBirth(int yearOfBirth);
  void SetCity(const string &city);
  void SetCoins(int coins);
  bool IsNull();

 private:
  string surname_;
  string name_;
  int year_of_birth_ = 0;
  string city_;
  int coins_ = 0;
};
}  // namespace s21

#endif  // A6_TRANSACTIONS_3_PERSON_H
