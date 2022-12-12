cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(test_midi_project C)

add_executable(test_midi test/test_midi.c midi.c)
target_include_directories(test_midi PRIVATE .)