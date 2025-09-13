#include "wordle_solver.h"
#include <iostream>

int main() {
  WordleSolver ws;
  for (int i = 0; i < 6; ++i) {
    std::cout << ws.guess() << ' ';
    std::string line;
    std::getline(std::cin, line);
    ws.accept_feedback(line);
  }
  return 0;
}
