add_executable(wordle-solver)

target_sources(
        wordle-solver
        PRIVATE
        wordle-solver.cpp
)
target_link_libraries(
        wordle-solver
        PRIVATE
        common-options
        WordleSolver::WordleSolver
)
