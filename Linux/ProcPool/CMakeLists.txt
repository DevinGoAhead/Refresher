cmake_minimum_required(VERSION 3.10)

project(ProcPool)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(ProcPool ProcPool.hpp Tasks.hpp Pipe.hpp Global.h Main.cc)