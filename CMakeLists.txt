cmake_minimum_required(VERSION 3.27)
project(KDE_Sub_quadratic)

set(CMAKE_CXX_STANDARD 20)

include_directories(Sqnn)
include_directories(Sqnn/src)

# Define variables for source files
set(MAIN_SOURCES
        Sqnn/src/main.cpp
        Sqnn/src/main.h
)

file(GLOB API_SOURCES Sqnn/src/API/*.h)
file(GLOB KDE_SOURCES Sqnn/src/KDE/*.h)

# Add executable with source files grouped by category
add_executable(KDE_Sub_quadratic
        ${MAIN_SOURCES}
        ${API_SOURCES}
        ${KDE_SOURCES}
)