cmake_minimum_required(VERSION 3.0)
project(a_star)

set(CMAKE_C_STANDARD 11)

add_executable(bidirectional_a_star search.c)
set(CMAKE_C_FLAGS "-Wno-incompatible-pointer-types")
