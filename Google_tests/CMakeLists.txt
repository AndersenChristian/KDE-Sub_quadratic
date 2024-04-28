cmake_minimum_required(VERSION 3.10)
project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Ofast -march=native -fopenmp -O0")

# adding the Google_Tests_run target
add_executable(Google_Tests_run GeoTest.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
#target_link_libraries(Google_Tests_run DateConverter_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)

# Find Eigen
find_package(Eigen3 REQUIRED)
target_link_libraries(Google_Tests_run Eigen3::Eigen)