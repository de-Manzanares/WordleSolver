#ifndef WORDLE_SOLVER_INCLUDE_WORDS_H_
#define WORDLE_SOLVER_INCLUDE_WORDS_H_

#include "data.h"
#include "wordle_solver.h"

namespace Words {

using Word_t = std::array<char, 5>;
static constexpr std::size_t n_words = ALL_WORDS_TXT.size() / 6;
static constexpr std::size_t n_solutions = ALL_SOLUTIONS_TXT.size() / 6;

extern const std::array<Word_t, n_words> all;
extern const std::array<Word_t, n_solutions> solutions;

template <std::size_t N>
auto load(const std::array<unsigned char, N> &array)
    -> std::array<Word_t, N / 6> {

  std::array<Word_t, N / 6> wordlist{};

  std::size_t j{};
  std::size_t k{};

  for (std::size_t i = 0; i < N; ++i) {
    if (array[i] != '\n') {
      wordlist[j][k++] = array[i];
    } else if (array[i] == '\n') {
      ++j;
      k = 0;
    }
  }
  return wordlist;
}

}; // namespace Words

#endif // WORDLE_SOLVER_INCLUDE_WORDS_H_
