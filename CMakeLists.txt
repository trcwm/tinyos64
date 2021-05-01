
cmake_minimum_required(VERSION 3.12)
project(tinyos64)

set(CMAKE_CXX_STANDARD 17)

set(SRC 
    src/main.cpp)

add_executable(tinyos64 ${SRC})

