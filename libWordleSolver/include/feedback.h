#ifndef WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
#define WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_

#include "wordle_solver.h"

#include <algorithm>
#include <bitset>

class Feedback {
public:
  /// number of bits for a character's feedback code
  static constexpr int feedback_char_bits = 2;

  /// number of bits for a word's feedback code
  static constexpr int feedback_word_bits =
      feedback_char_bits * WordleSolver::word_len;

  using Code_element = std::bitset<feedback_char_bits>;
  using Code = std::bitset<feedback_word_bits>;

  /**
   * @brief   Used by the solver to create a map of feedback types for a certain
   *          word.
   * @param   guess test string
   * @param   solution target string
   * @return  Feedback for each character of the test string: gray (00), yellow
   *          (01), or green (10).
   */
  static Code feedback(const std::string &guess, const std::string &solution);

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
                                     const std::string &solution);

private:
  /**
   * @param [in,out] target Set the bits, as dictated by the section parameter,
   * to the source bits
   * @param source The bit values we wish target to take on
   * @param section One of 5 partitions of the target bitset
   */
  static void set_bits(Code *target, Code_element source, std::size_t section);

  /**
   * @brief Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return 00 -   gray    - no match \n
   *         01 -   yellow  - found in solution \n
   *         10 -   green   - the character is in the correct position
   */
  static Code_element feedback_element(char ch_test, char ch_solution,
                                       const std::string &solution);

  /**
   * @brief   Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return  `x` -   gray    - no match \n
   *          `y` -   yellow  - found in solution \n
   *          `g` -   green   - the character is in the correct position
   */
  static char feedback_element_char(char ch_test, char ch_solution,
                                    const std::string &solution);
};

#endif // WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
