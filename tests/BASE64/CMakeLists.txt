cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(base64_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_BASE64_1 "./tester_BASE64_1.c" "../../src/BASE64/base64.c")
add_test(Tester_BASE64_1 tester_BASE64_1)

add_executable(tester_BASE64_2 "./tester_BASE64_2.c" "../../src/BASE64/base64.c")
add_test(Tester_BASE64_2 tester_BASE64_2)
