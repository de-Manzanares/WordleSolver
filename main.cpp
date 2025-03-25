#include "dictionary.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

#define LOG_Y

static std::map<char, int> lettermap;

std::map<std::string, int> eval_scores(const std::vector<std::string> &vstr);

std::vector<std::pair<int, std::string>>
list_words_by_score(const std::map<std::string, int> &scores);

int main() {
  // fill letter frequency map -------------------------------------------------
  for (const auto &word : wordlist) {
    for (const auto letter : word) {
      lettermap[letter]++;
    }
  }
#ifdef LOG_Y
  for (const auto [letter, freq] : lettermap) {
    std::cout << letter << " " << freq << '\n';
  }
#endif

  // make commonality/uniqueness score map--------------------------------------
  std::map<std::string, int> cu_scores = eval_scores(wordlist);

#ifdef LOG_Y
  for (const auto &[word, score] : cu_scores) {
    std::cout << word << " " << score << '\n';
  }
#endif

  // sort words based on cu_scores ---------------------------------------------
  std::vector<std::pair<int, std::string>> r_cu_scores =
      list_words_by_score(cu_scores);

#ifdef LOG_Y
  for (const auto &[cu_score, word] : r_cu_scores) {
    std::cout << cu_score << " " << word << '\n';
  }
#endif

  // print first guess ---------------------------------------------------------
  std::cout << '\n' << r_cu_scores.back().second << '\n';

  std::vector<std::string> known_substrings{};
  std::vector<char> include_letters{};
  std::vector<char> exclude_letters{};
  std::vector<char> letters_go_here(5, '0');
  std::vector<int> pos_positions{};
  std::vector<char> letters_dont_go_here(5, '0');
  std::vector<int> neg_positions{};
  std::vector<std::string> possible_words{};
  possible_words.reserve(wordlist.size());

  // begin feedback loop -------------------------------------------------------
  for (int i = 0; i < 5; ++i) {

    std::stringstream ss{};
    std::string input{};
    std::string substring{};
    char ch{};

    std::cout << "\nInput values as prompted, separated by spaces\n\n";

    std::cout << "Letters to exclude: ";
    std::getline(std::cin, input);
    ss.str(input);
    ss.clear();
    while (ss >> ch) {
      exclude_letters.push_back(ch);
    }

#ifdef LOG_Y
    for (const auto c : exclude_letters) {
      std::cout << c << " ";
    }
    std::cout << '\n';
#endif

    std::cout << "Letters to include: ";
    std::getline(std::cin, input);

    ss.str(input);
    ss.clear();
    while (ss >> ch) {
      include_letters.push_back(ch);
    }

#ifdef LOG_Y
    for (const auto c : include_letters) {
      std::cout << c << " ";
    }
    std::cout << '\n';
#endif

    std::cout << "Known substrings: ";

    std::getline(std::cin, input);

    ss.str(input);
    ss.clear();
    while (ss >> substring) {
      known_substrings.push_back(substring);
    }

#ifdef LOG_Y
    for (const auto &strings : known_substrings) {
      std::cout << strings << " ";
    }
    std::cout << '\n';
#endif

    std::cout << "Letters in known positions: ";

    std::getline(std::cin, input);

    if (!input.empty()) {
      for (auto j = 0; j < 5; ++j) {
        if (input[j] != '0') {
          pos_positions.push_back(j);
        }
        letters_go_here[j] = input[j];
      }
    }

    std::sort(pos_positions.begin(), pos_positions.end());
    pos_positions.erase(std::unique(pos_positions.begin(), pos_positions.end()),
                        pos_positions.end());

#ifdef LOG_Y
    for (const auto ch : letters_go_here) {
      std::cout << ch;
    }
    std::cout << '\n';
#endif

    std::cout << "Letters must not be here: ";

    std::getline(std::cin, input);

    if (!input.empty()) {
      for (auto j = 0; j < 5; ++j) {
        if (input[j] != '0') {
          neg_positions.push_back(j);
        }
        letters_dont_go_here[j] = input[j];
      }
    }

    std::sort(neg_positions.begin(), neg_positions.end());
    neg_positions.erase(std::unique(neg_positions.begin(), neg_positions.end()),
                        neg_positions.end());

#ifdef LOG_Y
    for (const auto ch : letters_dont_go_here) {
      std::cout << ch;
    }
    std::cout << '\n';
#endif

    // exclude letters
    if (i == 0) {
      for (const auto &word : wordlist) {
        bool found{false};
        for (const auto letter : exclude_letters) {
          if (word.find(letter) != std::string::npos) {
            found = true;
            break;
          }
        }
        if (!found) {
          possible_words.push_back(word);
        }
      }
    } else {
      for (auto word = possible_words.begin(); word != possible_words.end();) {
        bool found{false};
        for (const auto letter : exclude_letters) {
          if (word->find(letter) != std::string::npos) {
            found = true;
            break;
          }
        }
        if (found) {
          word = possible_words.erase(word);
        } else {
          ++word;
        }
      }
    }

    // include letters
    for (auto word = possible_words.begin(); word != possible_words.end();) {
      bool found{true};
      for (const auto letter : include_letters) {
        if (word->find(letter) == std::string::npos) {
          found = false;
          break;
        }
      }
      if (!found) {
        word = possible_words.erase(word);
      } else {
        ++word;
      }
    }

    // include substrings
    for (auto word = possible_words.begin(); word != possible_words.end();) {
      bool found{true};
      for (const auto &substring : known_substrings) {
        if (word->find(substring) == std::string::npos) {
          found = false;
          break;
        }
      }
      if (!found) {
        word = possible_words.erase(word);
      } else {
        ++word;
      }
    }

    // letters in known positions
    for (auto word = possible_words.begin(); word != possible_words.end();) {
      bool found{true};
      for (const auto index : pos_positions) {
        if (word->at(index) != letters_go_here[index]) {
          found = false;
          break;
        }
      }
      if (!found) {
        word = possible_words.erase(word);
      } else {
        ++word;
      }
    }

    // letters don't go here
    for (auto word = possible_words.begin(); word != possible_words.end();) {
      bool found{false};
      for (const auto index : neg_positions) {
        if (word->at(index) == letters_dont_go_here[index]) {
          found = true;
          break;
        }
      }
      if (found) {
        word = possible_words.erase(word);
      } else {
        ++word;
      }
    }

    auto scores = eval_scores(possible_words);
    auto list_scores = list_words_by_score(scores);
    std::cout << "\n\n" << list_scores.back().second << "\n\n";
  }
  return 0;
}

std::map<std::string, int> eval_scores(const std::vector<std::string> &vstr) {
  std::map<std::string, int> scores;
  for (const auto &word : vstr) {
    int score{};
    std::string seenletters;
    for (const auto letter : word) {
      if (seenletters.find(letter) == std::string::npos) {
        score += lettermap[letter];
        seenletters += letter;
      }
    }
    score *= seenletters.size(); // NOLINT
    scores[word] = score;
  }
  return scores;
}

std::vector<std::pair<int, std::string>>
list_words_by_score(const std::map<std::string, int> &scores) {
  std::vector<std::pair<int, std::string>> score_list;
  score_list.reserve(scores.size());
  for (const auto &[word, cu_score] : scores) {
    score_list.emplace_back(cu_score, word);
  }
  std::sort(score_list.begin(), score_list.end());
  return score_list;
}
