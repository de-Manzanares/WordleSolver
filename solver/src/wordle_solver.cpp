#include "wordle_solver.h"
#include "entropy.hpp"
#include "feedback.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <unordered_map>

std::vector<std::string>
WordleSolver::load_wl(const std::string_view file_name) {
  std::vector<std::string> wl;
  if (!std::filesystem::exists(file_name)) {
    std::cerr << "Cannot find file '" << file_name << "' in search path:\n"
              << std::filesystem::current_path();
    std::exit(EXIT_FAILURE);
  }
  if (std::ifstream iFile((file_name.data())); iFile.is_open()) {
    std::string line;
    while (std::getline(iFile, line)) {
      wl.push_back(line);
    }
  } else {
    std::cerr << "Failed to open file " << file_name << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return wl;
}

void WordleSolver::accept_feedback(const std::string &feedback) {
  _feedback = feedback;
  update_guess_list();
}

void WordleSolver::update_guess_list() {
  // the pruning methods may not remove the last guess if there are duplicate
  // letters in the guess, eg "nanny"
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
  for (int i = 0; i < 5; ++i) {
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

void WordleSolver::sort_remove_duplicates(std::vector<char> *v) {
  std::sort(v->begin(), v->end());
  v->erase(std::unique(v->begin(), v->end()), v->end());
}

void WordleSolver::prune_guess_list() {
  process_exclude_letters();
  process_include_letters();
  process_yellow_letters();
  process_green_letters();
}

void WordleSolver::process_exclude_letters() {
  _exclude_letters.erase(
      std::remove_if(_exclude_letters.begin(), _exclude_letters.end(),
                     [&](const auto ch_ex) {
                       return std::find(_include_letters.begin(),
                                        _include_letters.end(),
                                        ch_ex) != _include_letters.end();
                     }),
      _exclude_letters.end());

  if (_iteration == 1) {
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
    _entropies.clear();
    _entropies.reserve(_guess_list.size());

    auto select_list = [&]() -> const std::vector<std::string> * {
      if (_iteration == 0 && _guess_list.size() > 5) {
        return &_all_words;
      }
      if (_guess_list.size() <= 5) {
        return &_guess_list;
      }
      return &_all_solutions;
    };

    for (const auto &word0 : *select_list()) {
      std::unordered_map<std::bitset<10>, double> feedback;
      for (const auto &word1 : _guess_list) {
        ++feedback[Feedback::feedback_word_bitset(word0, word1)];
      }
      double score = Entropy::entropy<decltype(feedback)::key_type>(feedback);
      _entropies.emplace_back(score, word0);
    }

    std::sort(_entropies.begin(), _entropies.end(), std::less{});
    _last_guess = _entropies.back().second;
    guess = _last_guess;
  }
  ++_iteration;
  return guess;
}
