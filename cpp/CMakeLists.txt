cmake_minimum_required(VERSION 3.24)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp main.cpp AES.cpp AES.h GF2_8.cpp GF2_8.h)
