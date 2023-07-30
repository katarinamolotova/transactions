//
// Created by Rebbecca Gormond on 7/21/23.
//

#include "record.h"

namespace s21 {

Record::Record(Person& data, size_t life_time) {
  this->values_ = data;
  this->life_time_ = life_time;
  this->created_time_ = time(nullptr);
  this->is_alive_ = true;

  if (life_time != 0) {
    this->is_immortal_ = false;
  }
}

bool Record::IsAlive() {
  if (!is_alive_) return false;

  if (!this->is_immortal_ &&
      static_cast<size_t>(time(nullptr)) - this->created_time_ >= life_time_) {
    this->is_alive_ = false;
  }
  return is_alive_;
}

Person Record::GetValues() const { return values_; }

void Record::SetValues(Person& values) { values_ = values; }

time_t Record::GetCreatedTime() const { return created_time_; }

void Record::SetCreatedTime(time_t createdTime) { created_time_ = createdTime; }

size_t Record::GetLifeTime() const { return life_time_; }

void Record::SetLifeTime(size_t lifeTime) { life_time_ = lifeTime; }

void Record::SetIsAlive(bool isAlive) { is_alive_ = isAlive; }

bool Record::IsImmortal() const { return is_immortal_; }

void Record::SetIsImmortal(bool isImmortal) { is_immortal_ = isImmortal; }

long Record::RemainingTime() {  // -1 if record immortal
  if (!this->is_alive_) return 0;
  if (is_immortal_) return -1;
  time_t delta_time = time(nullptr) - created_time_;
  if (static_cast<size_t>(delta_time) < life_time_) {
    return static_cast<long>(life_time_ - delta_time);
  } else {
    this->is_alive_ = false;
    return 0;
  }
}
}  // namespace s21