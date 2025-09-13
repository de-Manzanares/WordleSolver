cmake_minimum_required(VERSION 3.23)
project(WordleSolver VERSION 0.3.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CompilerOptions -Wall -Wextra -Wpedantic -Werror)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)
set(ReleaseCompilerOptions -O3 -ffast-math)

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
        $<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:GNU,Clang>:${ReleaseCompilerOptions}>>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_subdirectory(wordle-solver)
add_subdirectory(libWordleSolver)
enable_testing()
add_subdirectory(test)

add_custom_target(copy_word_lists ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/word_lists
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/word_lists
        COMMENT "copying word lists to ${CMAKE_BINARY_DIR}/bin"
        DEPENDS wordle-solver wordle-solver-test
)
