cmake_minimum_required(VERSION 3.27)
project(KDE_Sub_quadratic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(Sqnn)
include_directories(Sqnn/src)

# Define variables for source files
set(MAIN_SOURCES
        Sqnn/src/main.cpp
        Sqnn/src/main.h
        Sqnn/src/ANN/ANN.h
)

file(GLOB API_SOURCES Sqnn/src/API/*.h)
file(GLOB KDE_SOURCES Sqnn/src/KDE/*.h)

# Add executable with source files grouped by category
add_executable(KDE_Sub_quadratic
        ${MAIN_SOURCES}
        ${API_SOURCES}
        ${KDE_SOURCES}
)

# Add SIMD flags based on your requirements and compiler
# For example, for GCC or Clang, to enable AVX2:
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(KDE_Sub_quadratic PRIVATE "-mavx2")
    # You can add more specific flags here based on your needs
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # For Visual Studio, use appropriate /arch flag
    target_compile_options(KDE_Sub_quadratic PRIVATE "/arch:AVX2")
endif()