cmake_minimum_required(VERSION 3.10)
project(socket C)

set(CMAKE_C_STANDARD 99)

add_library(socket fork_3.c)