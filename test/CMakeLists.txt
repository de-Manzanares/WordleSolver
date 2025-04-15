add_executable(test)

target_sources(
        test
        PRIVATE
        test.cpp
)
target_link_libraries(
        test
        PRIVATE
        common-options
        wordle_solver::lib
)
