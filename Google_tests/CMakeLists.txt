cmake_minimum_required(VERSION 3.10)
project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -Ofast -march=native -fopenmp -O0")

# adding the Google_Tests_run target
add_executable(Google_Tests_run VertexSamplingTest.cpp)
add_executable(Speed_Tests speed_tests/SpeedTest.cpp)
add_executable(Var_Finder Var_Finder.cpp)
add_executable(Matrix_Shifting SubMatrixManipulation.cpp)
add_executable(Alias functionality/AliasTest.cpp)

# links with gtest (required for all tests)
target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Speed_Tests gtest gtest_main)
target_link_libraries(Var_Finder gtest gtest_main)
target_link_libraries(Matrix_Shifting gtest gtest_main)
target_link_libraries(Alias gtest gtest_main)

# Find Eigen
find_package(Eigen3 REQUIRED)
target_link_libraries(Google_Tests_run Eigen3::Eigen)
target_link_libraries(Speed_Tests Eigen3::Eigen)
target_link_libraries(Var_Finder Eigen3::Eigen)
target_link_libraries(Matrix_Shifting Eigen3::Eigen)