#include "data.h"
#include "feedback.hpp"
#include "wordle_solver.h"

#include <chrono>
#include <iostream>
#include <map>

int main() {
  const std::vector<std::string> solutions =
      WordleSolver::load_wordlist(&ALL_SOLUTIONS_TXT[0], ALL_SOLUTIONS_TXT_LEN);

  constexpr std::size_t unsolved{0};
  const std::string solved{"ggggg"};

  std::map<int, int> guesses;
  guesses[unsolved] = 0;
  guesses[1] = 0;
  std::size_t counter{};

  const auto start = std::chrono::high_resolution_clock::now();

  std::cout << "progress: ";
  for (const auto &solution : solutions) {
    WordleSolver solver;
    std::string guess;
    std::string feedback;

    for (int i = 1; i <= 10; ++i) {
      guess = solver.guess();
      feedback = Feedback::feedback_string(guess, solution);
      if (feedback == solved) {
        ++guesses[i];
        break;
      }
      solver.accept_feedback(feedback);
    }
    if (feedback != solved) {
      ++guesses[unsolved];
    }
    ++counter;
    if (counter % 100 == 0) {
      std::cout << '.';
      std::cout.flush();
    }
  }
  std::cout << "noice!\n\n";

  const auto end = std::chrono::high_resolution_clock::now();
  const auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  double average{};

  for (const auto &[score, freq] : guesses) {
    std::cout << score << ' ' << freq << '\n';
    average += static_cast<double>(score) * static_cast<double>(freq) /
               static_cast<double>(solutions.size());
  }
  std::cout << '\n';
  std::cout << "Average time to solve: " << average << " guesses\n";
  std::cout << "Test duration: " << duration.count() << " ms\n";
}
