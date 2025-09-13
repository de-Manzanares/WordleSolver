add_executable(wordle-solver-test)

target_sources(
        wordle-solver-test
        PRIVATE
        wordle_solver_test.cpp
)
target_link_libraries(
        wordle-solver-test
        PRIVATE
        common-options
        WordleSolver::WordleSolver
)

add_test(NAME WordleSolverTest COMMAND wordle-solver-test)
