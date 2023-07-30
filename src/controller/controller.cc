//
// Created by Gwynesse Tandy on 7/20/23.
//

#include "controller.h"

#include <iostream>

namespace s21 {

void s21::controller::SetModel(StoreType store_type) {
  model_.SetModel(store_type);
}

std::string s21::controller::Set(const std::vector<std::string>& tokens) {
  return model_.Set(tokens);
}

std::string s21::controller::Get(const std::string& key) {
  return model_.Get(key);
}

std::string s21::controller::Del(const std::string& key) {
  return model_.Del(key);
}

std::string s21::controller::Update(const std::vector<std::string>& tokens) {
  return model_.Update(tokens);
}

std::vector<std::string> s21::controller::Keys() { return model_.Keys(); }

std::string s21::controller::Rename(const std::string& key_1,
                                    const std::string& key_2) {
  return model_.Rename(key_1, key_2);
}

std::string s21::controller::TTL(const std::string& key) {
  return model_.TTL(key);
}

std::vector<std::string> s21::controller::Find(
    const std::vector<std::string>& tokens) {
  return model_.Find(tokens);
}

std::vector<Person> s21::controller::ShowAll() { return model_.ShowAll(); }

std::string s21::controller::Upload(const std::string& path) {
  return model_.Upload(path);
}

std::string s21::controller::Export(const std::string& path) {
  return model_.Export(path);
}

bool s21::controller::Exists(const std::string& key) {
  return model_.Exists(key);
}

std::string controller::TemporalCharacteristics(int items_number,
                                                int iterations_number) {
  std::string result =
      model_.TemporalCharacteristics(items_number, iterations_number);
  return result;
}

}  // namespace s21
