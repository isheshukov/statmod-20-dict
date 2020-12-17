cmake_minimum_required(VERSION 3.5)

project(dict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(words_alpha.txt words_alpha.txt COPYONLY)
configure_file(text text COPYONLY)

add_executable(dict main.cpp)
