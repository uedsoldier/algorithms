cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(AES_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_AES128ECB "./test_AES128_ECB.c" "../../src/AES/AES128.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_executable(tester_AES128CBC "./test_AES128_CBC.c" "../../src/AES/AES128.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_test(Tester_AES128_ECB tester_AES128ECB)
add_test(Tester_AES128_CBC tester_AES128CBC)

add_executable(tester_AES192ECB "./test_AES192_ECB.c" "../../src/AES/AES192.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_executable(tester_AES192CBC "./test_AES192_CBC.c" "../../src/AES/AES192.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_test(Tester_AES192_ECB tester_AES192ECB)
add_test(Tester_AES192_CBC tester_AES192CBC)

add_executable(tester_AES256ECB "./test_AES256_ECB.c" "../../src/AES/AES256.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_executable(tester_AES256CBC "./test_AES256_CBC.c" "../../src/AES/AES256.c" "../../src/PKCS7/PKCS7.c" "../../src/AES/AES_common.c")
add_test(Tester_AES256_ECB tester_AES256ECB)
add_test(Tester_AES256_CBC tester_AES256CBC)
