add_library(_wordle_solver STATIC)

add_library(wordle_solver::lib ALIAS _wordle_solver)

target_sources(
        _wordle_solver
        PRIVATE
        src/wordle_solver.cpp
)
target_include_directories(
        _wordle_solver
        PUBLIC
        include
)
target_link_libraries(
        _wordle_solver
        PRIVATE
        common-options
)
