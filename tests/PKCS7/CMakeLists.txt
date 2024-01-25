cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(PKCS7_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_PKCS7_1len_block128 "./test_1_128.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_PKCS7_1len_block128 tester_PKCS7_1len_block128)

add_executable(tester_PKCS7_2len_block128 "./test_2_128.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_PKCS7_2len_block128 tester_PKCS7_2len_block128)

add_executable(tester_PKCS7_10len_block128 "./test_10_128.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_PKCS7_10len_block128 tester_PKCS7_10len_block128)

add_executable(tester_PKCS7_5len_block256 "./test_5_256.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_PKCS7_5len_block256 tester_PKCS7_5len_block256)