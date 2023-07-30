//
// Created by Rebbecca Gormond on 7/22/23.
//

#include <array>
#include <cstdlib>
#include <random>

#ifndef A6_TRANSACTIONS_3_GENERATOR_H
#define A6_TRANSACTIONS_3_GENERATOR_H

#define KEY_LENGTH_MAX 10
#define SURNAME_LENGTH_MAX 7
#define NAME_LENGTH_MAX 6
#define CITY_LENGTH_MAX 6

#define YEAR_OF_BIRTH_MIN 1970
#define YEAR_OF_BIRTH_MAX 2023

#define ASCII_LOWER_MIN 97
#define ASCII_LOWER_MAX 121

#define ASCII_UPPER_MIN 65
#define ASCII_UPPER_MAX 90

#define COINS_MIN 0
#define COINS_MAX 1000

#include "../structures/person.h"

namespace s21 {
using std::pair;

class Generator {
 public:
  pair<string, Person> GetRandomPerson();
  [[nodiscard]] bool IsEmpty() const;
  explicit Generator(size_t number_of_generations);

 private:
  size_t number_of_generations_;
  static int RandomGenerate(size_t from, size_t to);
  static string GetNewString(size_t length, bool key = false);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_GENERATOR_H
