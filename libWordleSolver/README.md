### libWordleSolver

### Table of Contents

- [How to use it](#how-to-use-it)
    - [Interface](#interface)
    - [Feedback format](#feedback-format)
    - [Need to know](#need-to-know)
- [Integration](#integration)
- [Resources](#resources)
- [License](#license)

### How to use it

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

#### Interface:

- `WordleSolver::guess` returns the solver's best guess as a `std::string`.

- `WordleSolver::accept_feedback` accepts a
  `std::string` representing the Wordle game's feedback for our previous guess.

#### Feedback format:

- `x` &mdash; gray
- `y` &mdash; yellow
- `g` &mdash; green

For example:

- :black_large_square::yellow_square::black_large_square::green_square::black_large_square: &mdash;>
  `xyxgx`

- :black_large_square::black_large_square::yellow_square::yellow_square::green_square: &mdash;>
  `xxyyg`

- :green_square::green_square::green_square::green_square::green_square: <sub>(solved)</sub>

#### Need to know:

TODO wordlist thing

### Integration

The easiest way to use the library in your project is through CMake:

```cmake
include(FetchContent)
FetchContent_Declare(
        WordleSolver
        GIT_REPOSITORY https://github.com/de-Manzanares/WordleSolver.git
        GIT_TAG v0.3.1
)
FetchContent_MakeAvailable(WordleSolver)

target_link_libraries(
        <YOUR-TARGET>
        PRIVATE
        WordleSolver::WordleSolver
)
```

Then, in whichever files you'd like to use `WordleSolver`:

```c++
#include <wordle_solver.h>
```

CMake will build
`WordleSolver::WordleSolver`
as a static library and link your target against it.
It will also add `wordle_solver.h` to your target's include paths.

### Resources

The construction of this software was informed by the following:

- [“Entropy (information theory),” Wikipedia](https://en.wikipedia.org/w/index.php?title=Entropy_(information_theory)&oldid=1286902971)
- [3Blue1Brown, Solving Wordle using information theory (YouTube)](https://www.youtube.com/watch?v=v68zYyaEmEA)

### License

This software is made available under the [MIT License](./LICENSE.md). 
