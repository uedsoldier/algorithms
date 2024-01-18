cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(PKCS7_testing VERSION 0.1.0)

add_executable(test test.c "../../src/PKCS7/PKCS7.c" "../../src/AES/AES.c")

