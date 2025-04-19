#ifndef WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_
#define WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_

#include <fstream>
#include <iomanip>
#include <vector>

/**
 * @class WordleSolver
 * @brief Solve a wordle puzzle using Shannon's entropy formula
 */
class WordleSolver {
 public:
  /**
   * @note  Requires text files as named below to be accessible. \n
   *        "all_words.txt" is the large word list of all valid guesses -- good
   *        for choosing high entropy words \n
   *        "all_solutions.txt" is the smaller list of words that are
   *        possible solutions to the puzzle
   */
  WordleSolver()
      : _all_words{load_wordlist("all_words.txt")},
        _all_solutions{load_wordlist("all_solutions.txt")},
        _letters_go_here{std::vector(WORD_SIZE, '0')},
        _letters_dont_go_here{std::vector(WORD_SIZE, '0')} {}

  /**
   * @brief   Give the next guess
   * @return  The guess
   */
  std::string guess();

  /**
   * @brief   Read feedback into _feedback from std::cin
   * @details 'x' - gray \n
   *          'y' - yellow \n
   *          'g' - green
   */
  void accept_feedback(const std::string &feedback);

  /**
   * @brief   Apparently, huge static variables cause compilation to eat ALL THE
   *          RAM. So, text files it is.
   * @param   file_name The word list to load
   * @return  The words in the list
   */
  static std::vector<std::string> load_wordlist(std::string_view file_name);

  static constexpr int WORD_SIZE = 5;

 private:
  /// all valid guesses, loaded from text file
  std::vector<std::string> _all_words;
  /// all possible solutions, loaded from a text file
  std::vector<std::string> _all_solutions;

  /// word entropies, i.e., the expected information
  std::vector<std::pair<double, std::string>> _entropies;

  /// used by other methods for refining the next guess
  std::string _last_guess = "tarse";
  std::string _feedback; ///< feedback from last guess (eg "xxygx")
  int _iteration{};      ///< used for some control flows

  std::vector<char> _include_letters; ///< solution must have
  std::vector<char> _exclude_letters; ///< solution must not have

  std::vector<char> _letters_go_here; ///< solution has these letters here
  std::vector<int> _pos_positions;    ///< helper for correctly placed letters

  /// solution must not have these letters here
  std::vector<char> _letters_dont_go_here;
  std::vector<int> _neg_positions; ///< helper for incorrectly placed letters

  /// the words we've narrowed it down to so far
  std::vector<std::string> _guess_list;

  /**
   * @brief   Based on the feedback, update the lists of possible solutions
   * @details Calls WordleSolver::update_internals() and
   *          WordleSolver::prune_guess_list()
   */
  void update_guess_list();

  /**
   * @brief Use the feedback to set the parameters for the next round of pruning
   */
  void update_internals();

  /**
   * @brief Prune the guess list
   */
  void prune_guess_list();

  void interpret_feedback();
  static void sort_remove_duplicates(std::vector<char> *vec);
  void process_exclude_letters();
  void prefer_include_over_exclude();
  void process_first_exclude_letters();
  void process_include_letters();
  void process_yellow_letters();
  void process_green_letters();

  [[nodiscard]] auto eval_entropies() const -> std::string;

  [[nodiscard]] auto select_wordlist() const
      -> const std::vector<std::string> *;

  [[nodiscard]] auto
  get_best_word(std::vector<std::string>::const_iterator first,
                std::vector<std::string>::const_iterator last) const
      -> std::pair<double, std::string>;
};

#endif // WORDLE_SOLVER_INCLUDE_WORDLE_SOLVER_HPP_
