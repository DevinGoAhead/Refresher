cmake_minimum_required(VERSION 3.10)

project(MYSTDIO)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -g -lpthread)
add_executable(ProdcConsu BlockQueue.hpp Global.h Main.cc)
