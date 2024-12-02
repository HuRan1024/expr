cmake_minimum_required(VERSION 3.0)

project(expr)

set(CMAKE_C_STANDARD 17)

add_executable(expr src/expr.c test/test.c)