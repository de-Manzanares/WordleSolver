#ifndef ENTROPY_INL
#define ENTROPY_INL

#include <algorithm>
#include <bitset>
#include <cmath>

template <typename Map> double Entropy::entropy(Map &feedback) {
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

#endif // ENTROPY_INL