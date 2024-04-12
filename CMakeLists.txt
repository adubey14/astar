cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(a_star)

set(CMAKE_C_STANDARD 11)

add_library(a_star_helper SHARED search.c)
add_executable(a_star main.c)
set(CMAKE_C_FLAGS "-Wno-incompatible-pointer-types")
target_link_libraries(a_star a_star_helper m)
