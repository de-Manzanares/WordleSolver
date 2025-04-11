#ifndef WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_
#define WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_

#include <fstream>
#include <iomanip>
#include <vector>

/**
 * @class WordleSolver
 * @brief Solve a wordle puzzle using Shannon's entropy formula
 */
class WordleSolver {
 public:
  /**
   * @note  Requires text files as named below to be accessible. \n
   *        "all_words.txt" is the large word list of all valid guesses -- good
   *        for choosing high entropy words \n
   *        "all_solutions.txt" is the smaller list of words that are
   *        possible solutions to the puzzle
   */
  WordleSolver()
      : all_words{load_wl("all_words.txt")},
        all_solutions{load_wl("all_solutions.txt")},
        letters_go_here{std::vector(5, '0')},
        letters_dont_go_here{std::vector(5, '0')} {}

  void feedback_loop();

 private:
  std::vector<std::string> all_words;     ///< all valid guesses
  std::vector<std::string> all_solutions; ///< all possible solutions

  /// pre-calculated highest-entropy word from all_words
  inline static std::pair<double, std::string> STARTING_WORD = {5.94723,
                                                                "tarse"};

  std::vector<std::pair<double, std::string>> entropies; ///< word entropies

  std::vector<char> include_letters{}; ///< solution must have these
  std::vector<char> exclude_letters{}; ///< solution must not have these

  /// solution must have these letters here
  std::vector<char> letters_go_here;
  std::vector<int> pos_positions{}; ///< helper for word pruning

  /// solution must not have these letters here
  std::vector<char> letters_dont_go_here;
  std::vector<int> neg_positions{}; ///< helper for word pruning

  /// the words we've narrowed it down to so far
  std::vector<std::string> possible_words{};

  /**
   * @brief   Apparently huge static variables cause compilation to eat ALL THE
   *          RAM. So, text files it is.
   * @param   file_name The word list to load
   * @return  The words in the list
   */
  static std::vector<std::string> load_wl(std::string_view file_name);
};

#endif // WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_
