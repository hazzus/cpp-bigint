cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
               big_integer_testing.cpp
               big_integer.h
               big_integer.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc
               vector/my_vector.h
               vector/my_vector.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -pedantic -O0")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

endif()

target_link_libraries(big_integer_testing -lgmp -lgmpxx -lpthread)
