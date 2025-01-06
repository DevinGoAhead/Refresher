cmake_minimum_required(VERSION 3.10)

project(NAMEDPIPE)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -g)
add_executable(Server Global.h Server.cc)
add_executable(Client Global.h Client.cc)