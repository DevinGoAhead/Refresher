cmake_minimum_required(VERSION 3.10)

project(MYSHELL)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(MyShell MyShell.c)