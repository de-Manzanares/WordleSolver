#include "small_wl.h"

#include <algorithm>
#include <iostream>
#include <map>

int main() {
  std::vector<std::map<char, int>> frequency_by_position(5);
  for (const auto &word : small_word_list) {
    for (int i = 0; i < 5; ++i) {
      ++frequency_by_position[i][word[i]];
    }
  }

  for (int i = 0; i < 5; ++i) {
    std::cout << "Position " << i + 1 << ": " << std::endl;
    std::vector<std::pair<int, char>> freq_to_char;
    freq_to_char.reserve(26);
    for (const auto &[ch, freq] : frequency_by_position[i]) {
      freq_to_char.emplace_back(freq, ch);
    }
    std::sort(freq_to_char.begin(), freq_to_char.end(), std::greater{});
    for (const auto &[freq, ch] : freq_to_char) {
      std::cout << ch << " " << freq << std::endl;
    }
    std::cout << std::endl;
  }
}
