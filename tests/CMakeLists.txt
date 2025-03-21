cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring Tests Root (tests/CMakeLists.txt) ===")

# Enable testing for this directory
enable_testing()

# Add subdirectories for each set of tests
add_subdirectory(AES)
add_subdirectory(BASE64)
add_subdirectory(CHECKSUM8)

message(STATUS "=== Finished configuring Tests Root ===")
