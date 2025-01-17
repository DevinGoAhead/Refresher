cmake_minimum_required(VERSION 3.10)

project(MYSTDIO)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -g -lpthread)
add_executable(ProdcConsu Task.hpp BufferQueue.hpp Global.h Main.cc)
