#include "feedback.h"

using Code_element = std::bitset<Feedback::feedback_char_bits>;
using Code = std::bitset<Feedback::feedback_word_bits>;

Code Feedback::feedback(const std::string &guess, const std::string &solution) {
  Code code;
  for (int i = 0; i < WordleSolver::word_len; ++i) {
    set_bits(&code, feedback_element(guess[i], solution[i], solution), i);
  }
  return code;
}

std::string Feedback::feedback_string(const std::string &guess,
                                      const std::string &solution) {
  std::string outcome;
  for (int i = 0; i < WordleSolver::word_len; ++i) {
    outcome += feedback_element_char(guess[i], solution[i], solution);
  }
  return outcome;
}

void Feedback::set_bits(Code *target, const Code_element source,
                        const std::size_t section) {
  const auto index = (target->size() - 1) - (section * 2);
  target->set(index, source[1]);
  target->set(index - 1, source[0]);
}

Code_element Feedback::feedback_element(const char ch_test,
                                        const char ch_solution,
                                        const std::string &solution) {
  Code_element code_elem{};
  const bool not_in_solution =
      (std::find(solution.begin(), solution.end(), ch_test) == solution.end());
  if (not_in_solution) {
    return code_elem;
  }
  if (ch_test == ch_solution) {
    code_elem.set(1);
    return code_elem;
  }
  code_elem.set(0);
  return code_elem;
}

char Feedback::feedback_element_char(const char ch_test, const char ch_solution,
                                     const std::string &solution) {
  const bool not_in_solution =
      (std::find(solution.begin(), solution.end(), ch_test) == solution.end());
  if (not_in_solution) {
    return 'x';
  }
  if (ch_test == ch_solution) {
    return 'g';
  }
  return 'y';
}
