cmake_minimum_required(VERSION 3.30)

project(wordle_solver)

set(CMAKE_CXX_STANDARD 17)

add_executable(wordle_solver main.cpp)
add_executable(frequency_by_position frequency_by_position.cpp)
