#include "Words.hpp"

#include <iostream>

int main() {
  for (const auto &word : Words::solutions) {
    for (const auto ch : word) {
      std::cout << ch;
    }
    std::cout << '\n';
  }

  for (const auto &word : Words::all) {
    for (const auto ch : word) {
      std::cout << ch;
    }
    std::cout << '\n';
  }
}