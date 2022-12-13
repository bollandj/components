cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(test_synth C)
set(EXECUTABLE_NAME test_synth)
set(EXECUTABLE ${EXECUTABLE_NAME})

set(SRC_FILES
  test/test_synth.c
  wave.c
  )

add_executable(${EXECUTABLE} ${SRC_FILES})

target_include_directories(${EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${EXECUTABLE} ncurses)