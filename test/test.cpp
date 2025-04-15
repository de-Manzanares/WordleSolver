#include "feedback.hpp"
#include "wordle_solver.h"

#include <iostream>

int main() {
  const std::vector<std::string> solutions =
      WordleSolver::load_wl("../all_solutions.txt");

  // for (const auto &solution : solutions) {
  //   std::system("./wordle_solver");
  // }
}
