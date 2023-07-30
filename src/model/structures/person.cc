//
// Created by Rebbecca Gormond on 7/21/23.
//

#include "person.h"

#include <utility>

namespace s21 {
Person::Person(string surname, string name, int year_of_birth, string city,
               int coins) {
  this->surname_ = std::move(surname);
  this->name_ = std::move(name);
  this->year_of_birth_ = year_of_birth;
  this->city_ = std::move(city);
  this->coins_ = coins;
}

Person::Person(const Person &person) { *this = person; }

Person &Person::operator=(const Person &other) {
  if (other.surname_ != STR_TOKEN) {
    this->surname_ = other.surname_;
  }
  if (other.name_ != STR_TOKEN) {
    this->name_ = other.name_;
  }
  if (other.year_of_birth_ >= DIGIT_TOKEN) {
    this->year_of_birth_ = other.year_of_birth_;
  }
  if (other.city_ != STR_TOKEN) {
    this->city_ = other.city_;
  }
  if (other.coins_ >= DIGIT_TOKEN) {
    this->coins_ = other.coins_;
  }
  return *this;
}

bool Person::operator==(const Person &other) {
  if (other.surname_ != STR_TOKEN) {
    if (this->surname_ != other.surname_) return false;
  }
  if (other.name_ != STR_TOKEN) {
    if (this->name_ != other.name_) return false;
  }
  if (other.year_of_birth_ >= DIGIT_TOKEN) {
    if (this->year_of_birth_ != other.year_of_birth_) return false;
  }
  if (other.city_ != STR_TOKEN) {
    if (this->city_ != other.city_) return false;
  }
  if (other.coins_ >= DIGIT_TOKEN) {
    if (this->coins_ != other.coins_) return false;
  }
  return true;
}

const string &Person::GetSurname() const { return surname_; }

void Person::SetSurname(const string &surname) { surname_ = surname; }

const string &Person::GetName() const { return name_; }

void Person::SetName(const string &name) { name_ = name; }

int Person::GetYearOfBirth() const { return year_of_birth_; }

void Person::SetYearOfBirth(int yearOfBirth) { year_of_birth_ = yearOfBirth; }

const string &Person::GetCity() const { return city_; }

void Person::SetCity(const string &city) { city_ = city; }

int Person::GetCoins() const { return coins_; }

void Person::SetCoins(int coins) { coins_ = coins; }

bool Person::IsNull() {
  return this->surname_.empty() && this->name_.empty() && this->city_.empty() &&
         this->coins_ == 0 && this->year_of_birth_ == 0;
}

}  // namespace s21