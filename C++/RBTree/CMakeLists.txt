cmake_minimum_required(VERSION 3.30)

project(RBTREE)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(RBTree RBTree.cc)