cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(STRING_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_indexOf "./test_indexOf.c" "../../src/STRING/string_custom.c")
add_test(Tester_indexOf tester_indexOf)

add_executable(tester_validateInt "./test_validate_int.c" "../../src/STRING/string_custom.c")
add_test(Tester_validateInt tester_validateInt)

add_executable(tester_validateHex "./test_validate_hex.c" "../../src/STRING/string_custom.c")
add_test(Tester_validateHex tester_validateHex)
