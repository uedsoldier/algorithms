cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(SHA1_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_SHA1_1 "./testSHA1_1.c" "../../src/SHA1/SHA1.c")
add_test(Tester_SHA1_1 tester_SHA1_1)

add_executable(tester_SHA1_2 "./testSHA1_2.c" "../../src/SHA1/SHA1.c")
add_test(Tester_SHA1_2 tester_SHA1_2)

add_executable(tester_SHA1_3 "./testSHA1_3.c" "../../src/SHA1/SHA1.c")
add_test(Tester_SHA1_3 tester_SHA1_3)