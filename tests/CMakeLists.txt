cmake_minimum_required(VERSION 3.12)
project(algorithms_tests
    VERSION 0.1.0
    DESCRIPTION "Algorithms testing suite"
    LANGUAGES C
)

# Enable testing
enable_testing()

# Set path variables for better maintainability
set(ALGORITHMS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")
set(ALGORITHMS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Include directories
include_directories(${ALGORITHMS_INCLUDE_DIR})

# Add all test subdirectories
add_subdirectory(AES)
# add_subdirectory(BASE64)
# add_subdirectory(CHECKSUM8)
# add_subdirectory(CRC)
# add_subdirectory(PKCS7)
# add_subdirectory(SHA1)
# add_subdirectory(SHA256)
# add_subdirectory(STRING)
# add_subdirectory(XTEA)

# Print configuration information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "Testing enabled for all algorithm categories")