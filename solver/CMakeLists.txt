add_library(_WordleSolver STATIC)

add_library(WordleSolver::WordleSolver ALIAS _WordleSolver)

target_sources(
        _WordleSolver
        PRIVATE
        src/data.cpp
        src/wordle_solver.cpp
)
target_include_directories(
        _WordleSolver
        PUBLIC
        include
)
target_link_libraries(
        _WordleSolver
        PRIVATE
        common-options
)
