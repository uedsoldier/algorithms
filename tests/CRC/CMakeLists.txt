cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(CRC_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_CRC8 "./test_CRC8.c" "../../src/CRC/crc.c")
add_test(Tester_CRC8 tester_CRC8)

add_executable(tester_CRC16 "./test_CRC16.c" "../../src/CRC/crc.c")
add_test(Tester_CRC16 tester_CRC16)

add_executable(tester_CRC32 "./test_CRC32.c" "../../src/CRC/crc.c")
add_test(Tester_CRC32 tester_CRC32)