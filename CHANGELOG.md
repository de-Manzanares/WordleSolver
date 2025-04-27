## 0.3.0

### Added

- Multithreading
    - To improve responsiveness, the solver will distribute the entropy calculations to as many cores as are available.

### Changed

- Rename project and CMake target for consistency
    - `WordleSolver` and `WordleSolver::WordleSolver`, respectively

## 0.2.0

- Transition to building the solver as a library rather than an end-to-end application.

### Added

- New feedback interface
    - The feedback interface is vastly improved—instead of answering several distinct questions, the user supplies a single string of five characters that represents the Wordle game's feedback.

### Changed

- Solves the puzzles with Shannon entropy
    - A great improvement over the previously used methods

## 0.1.0

- A CLI application
- Solves the puzzles with
    - Simple Heuristics
    - Regexes
    - Process of elimination
