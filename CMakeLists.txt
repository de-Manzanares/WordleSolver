cmake_minimum_required(VERSION 3.30)

project(wordle_solver)

set(CMAKE_CXX_STANDARD 17)

set(CompilerOptions -Wall -Wextra -Wpedantic -Werror)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)

add_library(
        common-options
        INTERFACE
)
target_compile_options(
        common-options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_executable(wordle_solver)
target_sources(
        wordle_solver
        PRIVATE
        src/main.cpp
        src/solver.cpp
)
target_include_directories(
        wordle_solver
        PRIVATE
        include
)
target_link_libraries(
        wordle_solver
        PRIVATE
        common-options
)
