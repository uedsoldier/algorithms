cmake_minimum_required(VERSION 3.28)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(AES_testing VERSION 0.1.0)

add_executable(test test.c "../../src/AES/AES.c")

