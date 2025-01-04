cmake_minimum_required(VERSION 3.10)

project(MYSTDIO)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(MyStdio MyStdio.c Main.c)
