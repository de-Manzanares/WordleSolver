# WordleSolver

A small C++ interface for solving
[Wordle puzzles](https://www.nytimes.com/games/wordle/index.html)
using
[Shannon entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)).

(my first venture into concurrent programming! ...
don't worry, it works just fine)

### Table of contents

- [How to use it](#how-to-use-it)
    - [Interface](#interface)
    - [Feedback format](#feedback-format)
    - [Need to know](#need-to-know)
- [Performance](#performance)
- [Integration](#integration)
- [Resources](#resources)
- [License](#license)

## How to use it

Example code for a CLI Wordle-solving application:

```c++
#include <iostream>
#include <wordle_solver.h>

int main() {
  WordleSolver ws;
  for (int i = 0; i < 6; ++i) {
    std::cout << ws.guess() << ' ';
    std::string line;
    std::getline(std::cin, line);
    ws.accept_feedback(line);
  }
  return 0;
}
```

### Interface:

- `WordleSolver::guess` returns the solver's best guess as a `std::string`.

- `WordleSolver::accept_feedback` accepts a
  `std::string` representing the Wordle game's feedback for our previous guess.

### Feedback format:

- `x` &mdash; gray
- `y` &mdash; yellow
- `g` &mdash; green

For example:

- :black_large_square::yellow_square::black_large_square::green_square::black_large_square: &mdash;>
  `xyxgx`

- :black_large_square::black_large_square::yellow_square::yellow_square::green_square: &mdash;>
  `xxyyg`

- :green_square::green_square::green_square::green_square::green_square: <sub>(solved)</sub>

### Need to know:

Currently, the files in [/data](./data) need to be copied into the working
directory of any executable that uses `WordleSolver`.
There are plans to embed the data into the library at compile time so that the
aforementioned restriction is no longer an issue.

## Performance

The solver averages 3.48 guesses-to-solve across the 2,320 currently tested
[solution words](./data/all_solutions.txt).

The [test.cpp](./test/test.cpp) output below shows the distribution of guesses-to-solve:

```text
0 0 <- unsolved puzzles
1 0
2 79
3 1201
4 907
5 117
6 16

Average guesses-to-solve: 3.47845
Test duration: 3912 ms
```

Evaluating the entropy of a word &mdash; let alone thousands of words &mdash; is computationally expensive.
To improve responsiveness, the solver will distribute the entropy calculations to as many cores as are available.

## Integration

The easiest way to use the solver in your project is through CMake:

```cmake
include(FetchContent)
FetchContent_Declare(
        WordleSolver
        GIT_REPOSITORY https://github.com/de-Manzanares/WordleSolver.git
        GIT_TAG v0.3.0
)
FetchContent_MakeAvailable(WordleSolver)

target_link_libraries(
        <YOUR-TARGET>
        PRIVATE
        WordleSolver::WordleSolver
)
```

Then, in whichever files you'd like to use the `WordleSolver` interface:

```c++
#include <wordle_solver.h>
```

CMake will build
`WordleSolver::WordleSolver`
as a static library and link your target against it.
It will also add `wordle_solver.h` to your target's include paths.

Remember to copy the files from [/data](./data) into the working directory of your executable.

## Resources

The construction of this software was informed by the following:

- [“Entropy (information theory),” Wikipedia](https://en.wikipedia.org/w/index.php?title=Entropy_(information_theory)&oldid=1286902971)
- [3Blue1Brown, Solving Wordle using information theory (YouTube)](https://www.youtube.com/watch?v=v68zYyaEmEA)

## License

This software is made available under the [MIT License](./LICENSE.md). 
