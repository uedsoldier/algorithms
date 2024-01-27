cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(SHA256_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_SHA256_1 "./testSHA256_1.c" "../../src/SHA256/SHA256.c")
add_test(Tester_SHA256_1 tester_SHA256_1)

add_executable(tester_SHA256_2 "./testSHA256_2.c" "../../src/SHA256/SHA256.c")
add_test(Tester_SHA256_2 tester_SHA256_2)

add_executable(tester_SHA256_3 "./testSHA256_3.c" "../../src/SHA256/SHA256.c")
add_test(Tester_SHA256_3 tester_SHA256_3)