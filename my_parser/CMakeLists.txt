cmake_minimum_required(VERSION 3.24)
project(my_parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(my_parser main.cpp parser.h parser.cpp)
