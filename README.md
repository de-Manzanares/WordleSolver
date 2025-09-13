## wordle-solver

A multithreaded,
[entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory))-based,
CLI,
[Wordle](https://www.nytimes.com/games/wordle/index.html)-solving tool.

The
[library](./libWordleSolver)
it's built on
is designed with a fairly clean API, so it shouldn't be too hard for a person
to tack on any kind of front-end they like.

### Table of contents

- [Performance](#performance)
- [Resources](#resources)
- [License](#license)

### Performance

The solver averages 3.48 guesses-to-solve across the 2,320 currently tested
[solution words](./word_lists/all_solutions.txt).

The [test](./test/test_solver.cpp) output below shows the distribution of guesses-to-solve:

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

Remember to copy the files from [/word_lists](./word_lists) into the working
directory of your executable.

### Resources

The construction of this software was informed by the following:

- [“Entropy (information theory),” Wikipedia](https://en.wikipedia.org/w/index.php?title=Entropy_(information_theory)&oldid=1286902971)
- [3Blue1Brown, Solving Wordle using information theory (YouTube)](https://www.youtube.com/watch?v=v68zYyaEmEA)

### License

This software is made available under the [MIT License](./LICENSE.md). 
