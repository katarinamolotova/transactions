//
// Created by Rebbecca Gormond on 7/22/23.
//

#include "generator.h"

namespace s21 {

Generator::Generator(size_t number_of_generations) {
  number_of_generations_ = number_of_generations;
}

pair<string, Person> Generator::GetRandomPerson() {
  if (number_of_generations_ > 0) {
    string key = GetNewString(KEY_LENGTH_MAX, true);
    Person result = Person(GetNewString(RandomGenerate(2, SURNAME_LENGTH_MAX)),
                           GetNewString(RandomGenerate(2, NAME_LENGTH_MAX)),
                           RandomGenerate(YEAR_OF_BIRTH_MIN, YEAR_OF_BIRTH_MAX),
                           GetNewString(RandomGenerate(3, CITY_LENGTH_MAX)),
                           RandomGenerate(COINS_MIN, COINS_MAX));
    number_of_generations_--;
    return {key, result};
  }
  return {};
}

bool Generator::IsEmpty() const { return number_of_generations_ == 0; }

int Generator::RandomGenerate(size_t from, size_t to) {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> dist(static_cast<int>(from),
                                          static_cast<int>(to));
  return dist(engine);
}

string Generator::GetNewString(size_t length, bool key) {
  string result;
  int i = 0;
  if (!key) {
    result +=
        static_cast<char>(RandomGenerate(ASCII_UPPER_MIN, ASCII_UPPER_MAX));
    i = 1;
  }
  for (; i < static_cast<int>(length); i++) {
    result +=
        static_cast<char>(RandomGenerate(ASCII_LOWER_MIN, ASCII_LOWER_MAX));
  }
  return result;
}

}  // namespace s21

#include "generator.h"
