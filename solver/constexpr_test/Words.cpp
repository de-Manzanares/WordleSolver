#include "Words.hpp"

extern const std::array<Words::Word_t, Words::n_words> Words::all =
    Words::load(ALL_WORDS_TXT);
extern const std::array<Words::Word_t, Words::n_solutions> Words::solutions =
    Words::load(ALL_SOLUTIONS_TXT);