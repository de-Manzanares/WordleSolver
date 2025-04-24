#include "wordle_solver.h"

#include "data.h"
#include "entropy.hpp"
#include "feedback.hpp"

#include <algorithm>
#include <filesystem>
#include <future>
#include <iostream>
#include <thread>
#include <unordered_map>

WordleSolver::WordleSolver()
    : _all_words{load_wordlist(&ALL_WORDS_TXT[0], ALL_SOLUTIONS_TXT_LEN)},
      _all_solutions{
          load_wordlist(&ALL_SOLUTIONS_TXT[0], ALL_SOLUTIONS_TXT_LEN)},
      _letters_go_here{std::vector(WORD_SIZE, '0')},
      _letters_dont_go_here{std::vector(WORD_SIZE, '0')} {}

std::vector<std::string>
WordleSolver::load_wordlist(const std::string_view file_name) {
  std::vector<std::string> wordlist;
  if (!std::filesystem::exists(file_name)) {
    std::cerr << "Cannot find file '" << file_name << "' in search path:\n"
              << std::filesystem::current_path();
    std::exit(EXIT_FAILURE);
  }
  if (std::ifstream in_file((file_name.data())); in_file.is_open()) {
    std::string line;
    while (std::getline(in_file, line)) {
      wordlist.push_back(line);
    }
  } else {
    std::cerr << "Failed to open file " << file_name << '\n';
    std::exit(EXIT_FAILURE);
  }
  return wordlist;
}

std::vector<std::string>
WordleSolver::load_wordlist(const char *characters,
                            const unsigned int length) {
  std::vector<std::string> wordlist;
  wordlist.reserve(length / (WORD_SIZE + 1));
  std::string word;

  for (unsigned int i = 0; i < length; ++i) {
    if (*std::next(characters, i) != '\n') {
      word += *std::next(characters, i);
    } else if (*std::next(characters, i) == '\n') {
      wordlist.push_back(word);
      word.clear();
    }
  }
  return wordlist;
}

void WordleSolver::accept_feedback(const std::string &feedback) {
  _feedback = feedback;
  update_guess_list();
}

void WordleSolver::update_guess_list() {
  // the pruning methods may not remove the last guess if there are duplicate
  // letters in the guess, e.g. "nanny"
  _guess_list.erase(
      std::remove(_guess_list.begin(), _guess_list.end(), _last_guess),
      _guess_list.end());

  update_internals();
  prune_guess_list();
}

void WordleSolver::update_internals() {
  _pos_positions.clear();
  _neg_positions.clear();

  interpret_feedback();

  sort_remove_duplicates(&_exclude_letters);
  sort_remove_duplicates(&_include_letters);
}

void WordleSolver::interpret_feedback() {
  for (int i = 0; i < WORD_SIZE; ++i) {
    switch (_feedback[i]) {
    case 'x':
      _exclude_letters.push_back(_last_guess[i]);
      break;
    case 'y':
      _include_letters.push_back(_last_guess[i]);
      _neg_positions.push_back(i);
      _letters_dont_go_here[i] = _last_guess[i];
      break;
    case 'g':
      _include_letters.push_back(_last_guess[i]);
      _pos_positions.push_back(i);
      _letters_go_here[i] = _last_guess[i];
      break;
    default:;
    }
  }
}

void WordleSolver::sort_remove_duplicates(std::vector<char> *vec) {
  std::sort(vec->begin(), vec->end());
  vec->erase(std::unique(vec->begin(), vec->end()), vec->end());
}

void WordleSolver::prune_guess_list() {
  process_exclude_letters();
  process_include_letters();
  process_yellow_letters();
  process_green_letters();
}

void WordleSolver::process_exclude_letters() {
  prefer_include_over_exclude();

  if (_iteration == 1) {
    process_first_exclude_letters();
  } else {
    for (auto word = _guess_list.begin(); word != _guess_list.end();) {
      bool found{false};
      for (const auto letter : _exclude_letters) {
        if (word->find(letter) != std::string::npos) {
          found = true;
          break;
        }
      }
      if (found) {
        word = _guess_list.erase(word);
      } else {
        ++word;
      }
    }
  }
}

void WordleSolver::prefer_include_over_exclude() {
  _exclude_letters.erase(
      std::remove_if(_exclude_letters.begin(), _exclude_letters.end(),
                     [&](const auto ch_ex) {
                       return std::find(_include_letters.begin(),
                                        _include_letters.end(),
                                        ch_ex) != _include_letters.end();
                     }),
      _exclude_letters.end());
}

void WordleSolver::process_first_exclude_letters() {
  for (const auto &word : _all_solutions) {
    bool found{false};
    for (const auto letter : _exclude_letters) {
      if (word.find(letter) != std::string::npos) {
        found = true;
        break;
      }
    }
    if (!found) {
      _guess_list.push_back(word);
    }
  }
}

void WordleSolver::process_include_letters() {
  for (auto word = _guess_list.begin(); word != _guess_list.end();) {
    bool found{true};
    for (const auto letter : _include_letters) {
      if (word->find(letter) == std::string::npos) {
        found = false;
        break;
      }
    }
    if (!found) {
      word = _guess_list.erase(word);
    } else {
      ++word;
    }
  }
}

void WordleSolver::process_yellow_letters() {
  for (auto word = _guess_list.begin(); word != _guess_list.end();) {
    bool found{false};
    for (const auto index : _neg_positions) {
      if (word->at(index) == _letters_dont_go_here[index]) {
        found = true;
        break;
      }
    }
    if (found) {
      word = _guess_list.erase(word);
    } else {
      ++word;
    }
  }
}

void WordleSolver::process_green_letters() {
  for (auto word = _guess_list.begin(); word != _guess_list.end();) {
    bool found{true};
    for (const auto index : _pos_positions) {
      if (word->at(index) != _letters_go_here[index]) {
        found = false;
        break;
      }
    }
    if (!found) {
      word = _guess_list.erase(word);
    } else {
      ++word;
    }
  }
}

std::string WordleSolver::guess() {
  std::string guess;
  if (_iteration == 0) {
    guess = _last_guess;
  } else if (_guess_list.size() == 1) {
    _last_guess = _guess_list[0];
    guess = _last_guess;
  } else {
    if (_guess_list.empty()) {
      std::cerr << "No solution: \n"
                << "\t- Solution is missing from dictionary, or\n"
                << "\t- Incorrect user input\n";
      std::exit(EXIT_FAILURE);
    }
    _last_guess = eval_entropies();
    guess = _last_guess;
  }
  ++_iteration;
  return guess;
}

const std::vector<std::string> *WordleSolver::select_wordlist() const {
  static constexpr int cutoff{5};

  if (_iteration == 0 && _guess_list.size() > cutoff) {
    return &_all_words;
  }
  if (_guess_list.size() <= cutoff) {
    return &_guess_list;
  }
  return &_all_solutions;
}

std::string WordleSolver::eval_entropies() const {
  std::string best_word{};
  const auto *const wordlist = select_wordlist();

  if (constexpr auto cutoff{999}; wordlist->size() > cutoff) {
    const auto thread_count = std::max(1U, std::thread::hardware_concurrency());
    const auto chunk_size =
        static_cast<std::ptrdiff_t>(wordlist->size() / thread_count);

    std::vector<std::vector<std::string>::const_iterator> ranges;
    ranges.reserve(thread_count + 1);

    for (unsigned i = 0; i < thread_count; ++i) {
      ranges.emplace_back(std::next(wordlist->cbegin(), i * chunk_size));
    }
    ranges.emplace_back(wordlist->cend());

    std::vector<std::future<std::pair<double, std::string>>> futures;
    futures.reserve(thread_count);

    for (unsigned i = 0; i < thread_count; ++i) {
      futures.emplace_back(std::async(std::launch::async, [this, &ranges, i] {
        return get_best_word(ranges[i], ranges[i + 1]);
      }));
    }

    std::vector<std::pair<double, std::string>> best_words;
    best_words.reserve(futures.size());

    for (auto &future : futures) {
      best_words.emplace_back(future.get());
    }

    auto [max_score, max_word] =
        std::make_pair(std::numeric_limits<double>::min(), std::string{});

    for (const auto &[score, word] : best_words) {
      if (score > max_score) {
        max_score = score;
        best_word = word;
      }
    }
  } else {
    best_word = get_best_word(wordlist->cbegin(), wordlist->cend()).second;
  }
  return best_word;
}

std::pair<double, std::string> WordleSolver::get_best_word(
    const std::vector<std::string>::const_iterator first,
    const std::vector<std::string>::const_iterator last) const {

  std::vector<std::pair<double, std::string>> entropies;
  entropies.reserve(std::distance(first, last));

  for (auto it = first; it != last; ++it) {
    std::unordered_map<Feedback::Code, double> feedback_to_freq;

    for (const auto &word : _guess_list) {
      ++feedback_to_freq[Feedback::feedback(*it, word)];
    }
    double score = Entropy::entropy(feedback_to_freq);
    entropies.emplace_back(score, *it);
  }

  auto [max_score, max_word] =
      std::make_pair(std::numeric_limits<double>::min(), std::string{});

  for (const auto &[score, word] : entropies) {
    if (score > max_score) {
      max_score = score;
      max_word = word;
    }
  }
  return {max_score, max_word};
}
