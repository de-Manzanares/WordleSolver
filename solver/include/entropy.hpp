#ifndef WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
#define WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_

#include <algorithm>
#include <bitset>
#include <cmath>

class Entropy {
 public:
  /**
   * @brief   Calculates the expected information from a word.\n
   *          Used by the solver to order words based on expected information
   * @details Uses Shannon's entropy formula to calculate the expected
   *          information, in bits, of a word
   * @param   feedback The feedback map - used to calculate the probability of a
   *          feedback pattern, which is in turn used to evaluate expected
   *          information
   * @return  The expected information, in bits, for a word
   * @note    The solver first generates a map of feedback for a word, then
   *          passes that map to this function to get the entropy for that word
   */
  template <typename Map> static double entropy(Map &feedback) {
    double neg_entropy{};
    double sum{};
    for (const auto &[res, freq] : feedback) {
      sum += freq;
    }
    for (const auto &[res, freq] : feedback) {
      const auto prob = freq / sum;
      neg_entropy += prob * std::log2(prob);
    }
    return -neg_entropy; // two wrongs make a right ... right?
  }
};

#endif // WORDLE_SOLVER_INCLUDE_ENTROPY_HPP_
