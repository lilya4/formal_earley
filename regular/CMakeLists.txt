cmake_minimum_required(VERSION 3.20)
project(task_8)

add_subdirectory(googletest-main)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run task_8.cpp task_8.h test.cpp)
add_executable(run task_8.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
