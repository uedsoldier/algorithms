cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(AES_testing VERSION 0.1.0)

enable_testing()

add_executable(main test.c "../../src/AES/AES.c" "../../src/PKCS7/PKCS7.c" )
add_executable(tester_AES128ECB test_AES128_ECB.c "../../src/AES/AES.c" "../../src/PKCS7/PKCS7.c" )
add_executable(tester_AES128CBC test_AES128_CBC.c "../../src/AES/AES.c" "../../src/PKCS7/PKCS7.c" )
add_test(Tester_AES128_ECB tester_AES128ECB)
add_test(Tester_AES128_CBC tester_AES128CBC)
