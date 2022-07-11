cmake_minimum_required(VERSION 3.23)
project(lgn)

set(CMAKE_CXX_STANDARD 17)

add_executable(lgn main.cpp headers/chat.h headers/sha1.h cpp/chat.cpp cpp/sha1.cpp)
