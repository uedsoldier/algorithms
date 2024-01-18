cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(checksum8_testing VERSION 0.1.0)

add_executable(test "test.c" "../../src/CHECKSUM8/checksum8.c")

