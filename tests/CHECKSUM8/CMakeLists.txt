cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(checksum8_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_checksum8_xor "./test_checksum8_xor.c" "../../src/CHECKSUM8/checksum8.c")
add_test(Tester_checksum8_xor tester_checksum8_xor)

add_executable(tester_checksum8_modulo256 "./test_checksum8_modulo256.c" "../../src/CHECKSUM8/checksum8.c")
add_test(Tester_checksum8_modulo256 tester_checksum8_modulo256)

add_executable(tester_checksum8_2scomplement "./test_checksum8_2scomplement.c" "../../src/CHECKSUM8/checksum8.c")
add_test(Tester_checksum8_2scomplement tester_checksum8_2scomplement)

