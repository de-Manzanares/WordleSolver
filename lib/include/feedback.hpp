#ifndef WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
#define WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_

#include <bitset>
#include <algorithm>

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
  static std::bitset<10> feedback_word(const std::string &guess,
                                       const std::string &solution) {
    std::bitset<10> outcome;
    for (int i = 0; i < 5; ++i) {
      set_bits(&outcome, feedback_char(guess[i], solution[i], solution), i);
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
   * @param   ch0 Character n of str0
   * @param   ch1 Character n of str1
   * @param   str Word
   * @return 00 -   grey    - no match \n
   *         01 -   yellow  - found in word \n
   *         10 -   green   - the character is in the correct position
   */
  static std::bitset<2> feedback_char(const char ch0, const char ch1,
                                      const std::string &str) {
    std::bitset<2> outcome{};

    const auto it = std::find(str.begin(), str.end(), ch0);
    const bool found = it != str.end(); // NOLINT move to if-init statement

    if (!found) {
      return outcome;
    }
    if (ch0 == ch1) {
      outcome.set(1);
      return outcome;
    }
    outcome.set(0);
    return outcome;
  }
};

#endif // WORDLE_SOLVER_INCLUDE_FEEDBACK_HPP_
