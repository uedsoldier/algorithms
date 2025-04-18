cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring CRC Tests (tests/CRC/CMakeLists.txt) ===")

# Create a list to store added tests
set(ADDED_TESTS "")

# Find implementation files
file(GLOB CRC_IMPL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/CRC/*.c")
message(STATUS "Found CRC implementation files: ${CRC_IMPL_FILES}")

# Add CRC8 tests subdirectory
add_subdirectory(CRC8)

# Add CRC16 tests subdirectory
add_subdirectory(CRC16)

# Add CRC32 tests subdirectory
add_subdirectory(CRC32)

# Set the list of tests in parent scope
set(CRC_TESTS ${ADDED_TESTS} PARENT_SCOPE)

message(STATUS "Configured tests: ${ADDED_TESTS}")
message(STATUS "=== Finished configuring CRC Tests ===")
