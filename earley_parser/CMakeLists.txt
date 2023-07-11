cmake_minimum_required(VERSION 3.20)
project(earley)

add_subdirectory(googletest-main)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run earley.cpp earley.h test.cpp)
add_executable(run earley.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
