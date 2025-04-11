#ifndef WORDLE_SOLVER_ENTROPY_HPP_
#define WORDLE_SOLVER_ENTROPY_HPP_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <unordered_map>

/**
 * @param   ch0 Character n of str0
 * @param   ch1 Character n of str1
 * @param   str Word
 * @return 00 -   grey    - no match \n
 *         01 -   yellow  - found in word \n
 *         10 -   green   - ch0 is in the correct position
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

inline std::bitset<10> feedback_word(const std::string &str0,
                                     const std::string &str1) {
  std::bitset<10> outcome;
  for (int i = 0; i < 5; ++i) {
    set_bits(&outcome, feedback_char(str0[i], str1[i], str1), i);
  }
  return outcome;
}

inline double prob_x(const double x, const double y) { return x / y; }

inline double surprisal(const double prob) { return -std::log2(prob); }

using freq_map = std::unordered_map<std::bitset<10>, double>;

inline double entropy(const freq_map &feedback) {
  double entropy{};
  double sum{};
  for (const auto &[res, freq] : feedback) {
    sum += freq;
  }
  for (const auto &[res, freq] : feedback) {
    const auto prob = prob_x(freq, sum);
    entropy += prob * std::log2(prob);
  }
  return -entropy;
}

#endif // WORDLE_SOLVER_ENTROPY_HPP_
