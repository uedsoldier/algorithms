cmake_minimum_required(VERSION 3.12)

project(algorithms
    VERSION 0.1.0
    DESCRIPTION "Algorithms library and tests"
    LANGUAGES C
)

message(STATUS "=== Configuring Project Root (CMakeLists.txt) ===")

# Get processor count
include(ProcessorCount)
ProcessorCount(N)
if(N EQUAL 0)
    set(N 1)
    message(STATUS "No processor count detected, defaulting to 1")
else()
    message(STATUS "Detected ${N} processors")
endif()

# Make it available to all subprojects
set(PROCESSOR_COUNT ${N} CACHE STRING "Number of processors to use for parallel builds")

# Enable testing
enable_testing()

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Create interface library for includes
add_library(project_includes INTERFACE)
target_include_directories(project_includes INTERFACE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/BASE64
    ${CMAKE_SOURCE_DIR}/include/AES
    ${CMAKE_SOURCE_DIR}/include/CHECKSUM8
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/BASE64
    ${CMAKE_SOURCE_DIR}/src/AES
    ${CMAKE_SOURCE_DIR}/src/CHECKSUM8

)

# Add source directory if it exists
if(EXISTS "${CMAKE_SOURCE_DIR}/src")
    add_subdirectory(src)
endif()

# Add the tests directory
add_subdirectory(tests)

# Set compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wno-missing-braces")
endif()

# Print configuration information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "Parallel jobs: ${PROCESSOR_COUNT}")

# Print include directories for debugging
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include directories:")
foreach(dir ${inc_dirs})
    message(STATUS "  ${dir}")
endforeach()

message(STATUS "=== Finished configuring Project Root ===")
