#ifndef WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
#define WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_

#include "wordle_solver.h"

#include <algorithm>
#include <bitset>

class Feedback {
 public:
  /// number of bits for a character's feedback code
  static constexpr int FEEDBACK_CHAR_SIZE = 2;

  /// number of bits for a word's feedback code
  static constexpr int FEEDBACK_WORD_SIZE =
      FEEDBACK_CHAR_SIZE * WordleSolver::WORD_SIZE;

  using Code_element = std::bitset<FEEDBACK_CHAR_SIZE>;
  using Code = std::bitset<FEEDBACK_WORD_SIZE>;

  /**
   * @brief   Used by the solver to create a map of feedback types for a certain
   *          word.
   * @param   guess test string
   * @param   solution target string
   * @return  Feedback for each character of the test string: gray (00), yellow
   *          (01), or green (10).
   */
  static Code feedback(const std::string &guess, const std::string &solution) {
    Code code;
    for (int i = 0; i < WordleSolver::WORD_SIZE; ++i) {
      set_bits(&code, feedback_element(guess[i], solution[i], solution), i);
    }
    return code;
  }

  /**
   * @brief   Gives feedback by comparing guess to solution
   * @param   guess test string
   * @param   solution target string
   * @return  Feedback for each character of the test string: gray (x), yellow
   *          (y), or green (g). \n\n
   *          For example, "tarse" (guess) to "refer" (solution) would give
   *          "xxyxy"
   */
  static std::string feedback_string(const std::string &guess,
                                     const std::string &solution) {
    std::string outcome;
    for (int i = 0; i < WordleSolver::WORD_SIZE; ++i) {
      outcome += feedback_element_char(guess[i], solution[i], solution);
    }
    return outcome;
  }

 private:
  /**
   * @param [in,out] target Set the bits, as dictated by the section parameter,
   * to the source bits
   * @param source The bit values we wish target to take on
   * @param section One of 5 partitions of the target bitset
   */
  static void set_bits(Code *target, const Code_element source,
                       const std::size_t section) {
    const auto index = (target->size() - 1) - (section * 2);
    target->set(index, source[1]);
    target->set(index - 1, source[0]);
  }

  /**
   * @brief Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return 00 -   gray    - no match \n
   *         01 -   yellow  - found in solution \n
   *         10 -   green   - the character is in the correct position
   */
  static Code_element feedback_element(const char ch_test,
                                       const char ch_solution,
                                       const std::string &solution) {
    Code_element code_elem{};
    const bool not_in_solution = (std::find(solution.begin(), solution.end(),
                                            ch_test) == solution.end());
    if (not_in_solution) {
      return code_elem;
    }
    if (ch_test == ch_solution) {
      code_elem.set(1);
      return code_elem;
    }
    code_elem.set(0);
    return code_elem;
  }

  /**
   * @brief   Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return  `x` -   gray    - no match \n
   *          `y` -   yellow  - found in solution \n
   *          `g` -   green   - the character is in the correct position
   */
  static char feedback_element_char(const char ch_test, const char ch_solution,
                                    const std::string &solution) {
    const bool not_in_solution = (std::find(solution.begin(), solution.end(),
                                            ch_test) == solution.end());
    if (not_in_solution) {
      return 'x';
    }
    if (ch_test == ch_solution) {
      return 'g';
    }
    return 'y';
  }
};

#endif // WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
