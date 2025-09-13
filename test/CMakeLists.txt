add_executable(test-solver)
target_sources(
        test-solver
        PRIVATE
        test_solver.cpp
)
target_link_libraries(
        test-solver
        PRIVATE
        common-options
        WordleSolver::WordleSolver
)
add_test(NAME TestSolver COMMAND test-solver)
