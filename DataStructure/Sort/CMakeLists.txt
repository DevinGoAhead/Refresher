cmake_minimum_required(VERSION 3.10)

project(SORT)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(Sort Sort.c Main.c) 