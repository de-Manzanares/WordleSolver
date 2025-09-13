add_library(WordleSolver)

add_library(WordleSolver::WordleSolver ALIAS WordleSolver)

target_sources(
        WordleSolver
        PRIVATE
        src/wordle_solver.cpp
        src/feedback.cpp
)
target_include_directories(
        WordleSolver
        PUBLIC
        include
)
target_link_libraries(
        WordleSolver
        PRIVATE
        common-options
)
