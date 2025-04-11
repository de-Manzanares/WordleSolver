#ifndef WORDLE_SOLVER_INCLUDE_SOLVER_HPP_
#define WORDLE_SOLVER_INCLUDE_SOLVER_HPP_

#include "entropy.hpp"

#include <fstream>
#include <iomanip>
#include <vector>

class WordleSolver {
 public:
  static std::vector<std::string> load_wl(const std::string &file);

  WordleSolver() {
    letters_go_here = std::vector<char>(5, '0');
    letters_dont_go_here = std::vector<char>(5, '0');
    all_words = load_wl("all_words.txt");
    all_solutions = load_wl("all_solutions.txt");
  }

  void feedback_loop();

 private:
  std::vector<std::string> all_words;
  std::vector<std::string> all_solutions;

  std::pair<double, std::string> STARTING_WORD = {5.94723, "tarse"};
  std::vector<std::pair<double, std::string>> entropies;

  std::vector<char> include_letters{};
  std::vector<char> exclude_letters{};
  std::vector<char> letters_go_here;
  std::vector<int> pos_positions{};
  std::vector<char> letters_dont_go_here;
  std::vector<int> neg_positions{};
  std::vector<std::string> possible_words{};
};

#endif // WORDLE_SOLVER_INCLUDE_SOLVER_HPP_