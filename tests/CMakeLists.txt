cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring Tests Root (tests/CMakeLists.txt) ===")

# Enable testing for this directory
enable_testing()

# Add test utilities library
add_library(test_utils STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.c
)

target_include_directories(test_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/CRC
)

# Link test_utils with algorithms_lib
target_link_libraries(test_utils
    PUBLIC
        algorithms_lib
)

# Add subdirectories for each set of tests
add_subdirectory(AES)
add_subdirectory(BASE64)
add_subdirectory(CHECKSUM8)
add_subdirectory(CRC)

message(STATUS "=== Finished configuring Tests Root ===")
