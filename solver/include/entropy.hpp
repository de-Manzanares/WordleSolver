#ifndef WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
#define WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <unordered_map>

class Entropy {
 public:
  /**
   * @brief   Calculates the expected information from a word.\n
   *          Used by the solver to order words based on expected information
   * @details Uses Shannon's entropy formula to calculate the expected
   *          information, in bits, of a word
   * @param   feedback The feedback map - used to calculate probability of the
   *          type of feedback, which is in turn used to evaluate expected
   *          information
   * @return  The expected information, in bits, for a word
   * @note    The solver first generates a map of feedback for a word, then
   *          passes that map to this function to get the entropy for that word
   */
  template <typename T>
  static double entropy(const std::unordered_map<T, double> &feedback) {
    double neg_entropy{};
    double sum{};
    for (const auto &[res, freq] : feedback) {
      sum += freq;
    }
    for (const auto &[res, freq] : feedback) {
      const auto prob = freq / sum;
      neg_entropy += prob * std::log2(prob);
    }
    return -neg_entropy; // two wrongs makes a right ... right?
  }
};

#endif // WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
