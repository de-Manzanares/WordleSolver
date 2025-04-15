#ifndef WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
#define WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_

#include <algorithm>
#include <bitset>

class Feedback {
 public:
  /**
   * @brief   Used by the solver to create a map of feedback types for a certain
   *          word.
   * @param   guess test string
   * @param   solution target string
   * @return  Feedback for each character of the test string: grey (00), yellow
   *          (01), or green (10).
   */
  static std::bitset<10> feedback_word_bitset(const std::string &guess,
                                              const std::string &solution) {
    std::bitset<10> outcome;
    for (int i = 0; i < 5; ++i) {
      set_bits(&outcome, feedback_char_bitset(guess[i], solution[i], solution),
               i);
    }
    return outcome;
  }

  /**
   * @brief   Gives feedback by comparing guess to solution
   * @param   guess test string
   * @param   solution target string
   * @return  Feedback for each character of the test string: grey (x), yellow
   *          (y), or green (g). \n\n
   *          For example "tarse" (guess) to "refer" (solution) would give
   *          "xxyxy"
   */
  static std::string feedback_word_string(const std::string &guess,
                                          const std::string &solution) {
    std::string outcome;
    for (int i = 0; i < 5; ++i) {
      outcome += feedback_char_char(guess[i], solution[i], solution);
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
  static void set_bits(std::bitset<10> *target, const std::bitset<2> source,
                       const int section) {
    const auto index = (target->size() - 1) - (section * 2);

    target->set(index, source[1]);
    target->set(index - 1, source[0]);
  }

  /**
   * @brief Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return 00 -   grey    - no match \n
   *         01 -   yellow  - found in word \n
   *         10 -   green   - the character is in the correct position
   */
  static std::bitset<2> feedback_char_bitset(const char ch_test,
                                             const char ch_solution,
                                             const std::string &solution) {
    std::bitset<2> outcome{};

    const auto it = std::find(solution.begin(), solution.end(), ch_test);
    const bool found = it != solution.end(); // NOLINT move to if-init statement

    if (!found) {
      return outcome;
    }
    if (ch_test == ch_solution) {
      outcome.set(1);
      return outcome;
    }
    outcome.set(0);
    return outcome;
  }

  /**
   * @brief   Give feedback for a specific character
   * @param   ch_test Character n of test string
   * @param   ch_solution Character n of solution string
   * @param   solution solution string
   * @return  `x` -   grey    - no match \n
   *          `y` -   yellow  - found in word \n
   *          `g` -   green   - the character is in the correct position
   */
  static char feedback_char_char(const char ch_test, const char ch_solution,
                                 const std::string &solution) {

    const auto it = std::find(solution.begin(), solution.end(), ch_test);
    const bool found = it != solution.end(); // NOLINT move to if-init statement

    if (!found) {
      return 'x';
    }
    if (ch_test == ch_solution) {
      return 'g';
    }
    return 'y';
  }
};

#endif // WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
