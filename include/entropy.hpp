#ifndef WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
#define WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <unordered_map>

// begin helpers ---------------------------------------------------------------

namespace internal::entropy {
/**
 * @param [in,out] target Set the bits, as dictated by the section parameter, to
 *        the source bits
 * @param source The bit values we wish target to take on
 * @param section One of 5 partitions of the target bitset
 */
inline void set_bits(std::bitset<10> *target, const std::bitset<2> source,
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
inline std::bitset<2> feedback_char(const char ch0, const char ch1,
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
} // namespace internal::entropy

// end helpers -----------------------------------------------------------------
// begin public api ------------------------------------------------------------

/**
 * @brief   Used by the solver to order words based on expected information
 * @details Uses Shannon's entropy formula to calculate the expected bits of a
 *          word based on the feedback from being tested against every other
 *          word.
 * @param   feedback The feedback map - used to calculate probability of the
 *          type of feedback, which is in turn used to evaluate expected
 *          information
 * @return  The expected information, in bits, for a word
 * @note    The solver first generates a map of feedback for a word, then passes
 *          that map to this function to get the entropy for that word
 */
inline double
entropy(const std::unordered_map<std::bitset<10>, double> &feedback) {
  double neg_entropy{};
  double sum{};
  for (const auto &[res, freq] : feedback) {
    sum += freq;
  }
  for (const auto &[res, freq] : feedback) {
    const auto prob = freq / sum;
    neg_entropy += prob * std::log2(prob);
  }
  return -neg_entropy; // two wrongs makes a right?
}

/**
 * @brief   Used by the solver to create a map of feedback types for a certain
 *          word.
 * @param   str0 test string
 * @param   str1 target string
 * @return  Feedback for each character of the test string: grey (00), yellow
 *          (01), or green (10).
 */
inline std::bitset<10> feedback_word(const std::string &str0,
                                     const std::string &str1) {
  using namespace internal::entropy;
  std::bitset<10> outcome;
  for (int i = 0; i < 5; ++i) {
    set_bits(&outcome, feedback_char(str0[i], str1[i], str1), i);
  }
  return outcome;
}

// end public api --------------------------------------------------------------

#endif // WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
