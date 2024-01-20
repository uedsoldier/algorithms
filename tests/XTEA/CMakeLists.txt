cmake_minimum_required(VERSION 3.20)

set( CMAKE_CXX_COMPILER "g++")
set( CMAKE_C_COMPILER "gcc")

# set the project name
project(XTEA_testing VERSION 0.1.0)

enable_testing()

add_executable(tester_XTEA_ECB_8rounds "./test_XTEA_ECB_8rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_ECB_8rounds tester_XTEA_ECB_8rounds)

add_executable(tester_XTEA_ECB_16rounds "./test_XTEA_ECB_16rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_ECB_16rounds tester_XTEA_ECB_16rounds)

add_executable(tester_XTEA_ECB_32rounds "./test_XTEA_ECB_32rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_ECB_32rounds tester_XTEA_ECB_32rounds)

add_executable(tester_XTEA_ECB_64rounds "./test_XTEA_ECB_64rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_ECB_64rounds tester_XTEA_ECB_64rounds)

add_executable(tester_XTEA_ECB_128rounds "./test_XTEA_ECB_128rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_ECB_128rounds tester_XTEA_ECB_128rounds)

add_executable(tester_XTEA_CBC_8rounds "./test_XTEA_CBC_8rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_CBC_8rounds tester_XTEA_CBC_8rounds)

add_executable(tester_XTEA_CBC_16rounds "./test_XTEA_CBC_16rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_CBC_16rounds tester_XTEA_CBC_16rounds)

add_executable(tester_XTEA_CBC_32rounds "./test_XTEA_CBC_32rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_CBC_32rounds tester_XTEA_CBC_32rounds)

add_executable(tester_XTEA_CBC_64rounds "./test_XTEA_CBC_64rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_CBC_64rounds tester_XTEA_CBC_64rounds)

add_executable(tester_XTEA_CBC_128rounds "./test_XTEA_CBC_128rounds.c" "../../src/XTEA/XTEA.c" "../../src/PKCS7/PKCS7.c")
add_test(Tester_XTEA_CBC_128rounds tester_XTEA_CBC_128rounds)
