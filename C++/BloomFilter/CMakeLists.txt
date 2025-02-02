cmake_minimum_required(VERSION 3.30)

project(BLOOMFILTER)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -g)
add_executable(BloomFilter Main.cc)