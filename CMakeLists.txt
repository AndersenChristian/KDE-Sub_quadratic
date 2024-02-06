cmake_minimum_required(VERSION 3.27)
project(KDE_Sub_quadratic)

set(CMAKE_CXX_STANDARD 20)

include_directories(Sqnn)
include_directories(Sqnn/src)

add_executable(KDE_Sub_quadratic
        Sqnn/src/main.cpp
        Sqnn/src/main.h)
