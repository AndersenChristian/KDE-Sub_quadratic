cmake_minimum_required(VERSION 3.27)
project(KDE_Sub_quadratic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Ofast -march=native -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Ofast -march=native")

include_directories(Sqnn)
include_directories(Sqnn/src)

# Find FLANN using pkg-config
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(FLANN REQUIRED flann)

# Find Eigen
find_package(Eigen3 REQUIRED)

# Include directories
#include_directories(cpp lib)

# Include directories for FLANN
# include_directories(${FLANN_INCLUDE_DIRS})

# Define variables for source files
set(MAIN_SOURCES
        Sqnn/src/main.cpp
        Sqnn/src/main.h
        Sqnn/src/KdeUsingMrpt.h
)

file(GLOB API_SOURCES Sqnn/src/API/*.h)
file(GLOB KDE_SOURCES Sqnn/src/KDE/*.h)

# Add executable with source files grouped by category
add_executable(KDE_Sub_quadratic
        ${MAIN_SOURCES}
        ${API_SOURCES}
        ${KDE_SOURCES}
)

#target_link_libraries(KDE_Sub_quadratic ${FLANN_LIBRARIES})
#target_link_libraries(KDE_Sub_quadratic gomp)
target_link_libraries(KDE_Sub_quadratic Eigen3::Eigen)

# Add SIMD flags based on your requirements and compiler
# For example, for GCC or Clang, to enable AVX2:
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(KDE_Sub_quadratic PRIVATE "-mavx2")
    # You can add more specific flags here based on your needs
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # For Visual Studio, use appropriate /arch flag
    target_compile_options(KDE_Sub_quadratic PRIVATE "/arch:AVX2")
endif ()