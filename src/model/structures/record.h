//
// Created by Rebbecca Gormond on 7/20/23.
//

#ifndef A6_TRANSACTIONS_3_RECORD_H
#define A6_TRANSACTIONS_3_RECORD_H

#include "person.h"

namespace s21 {

class Record {
 public:
  Record() = default;
  explicit Record(Person& data, size_t life_time = 0);

  bool IsAlive();
  void SetValues(Person& values);
  void SetCreatedTime(time_t createdTime);
  void SetLifeTime(size_t lifeTime);
  void SetIsAlive(bool isAlive);
  void SetIsImmortal(bool isImmortal);
  long RemainingTime();  // -1 если бессмертный

  [[nodiscard]] Person GetValues() const;
  [[nodiscard]] bool IsImmortal() const;
  [[nodiscard]] time_t GetCreatedTime() const;
  [[nodiscard]] size_t GetLifeTime() const;

 private:
  Person values_;
  time_t created_time_ = 0;
  size_t life_time_ = 0;
  bool is_alive_ = false;
  bool is_immortal_ = true;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_3_RECORD_H
