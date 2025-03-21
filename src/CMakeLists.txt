cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring Source Root (src/CMakeLists.txt) ===")

# Set output directories for this directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find implementation files
file(GLOB BASE64_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/BASE64/*.c")
file(GLOB AES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/AES/*.c")
file(GLOB PKCS7_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/PKCS7/*.c")

# Define the algorithms library
add_library(algorithms_lib
    ${BASE64_SOURCES}
    ${AES_SOURCES}
    ${PKCS7_SOURCES}
    # Add other algorithm implementations as they become available
)

# Set include directories for the library
target_include_directories(algorithms_lib
    PUBLIC
        # Public headers
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/BASE64
        ${CMAKE_SOURCE_DIR}/include/AES
        ${CMAKE_SOURCE_DIR}/include/PKCS7
    PRIVATE
        # Private implementation headers
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/BASE64
        ${CMAKE_CURRENT_SOURCE_DIR}/AES
        ${CMAKE_CURRENT_SOURCE_DIR}/PKCS7
)

# Print include directories for debugging
get_target_property(INCLUDE_DIRS algorithms_lib INCLUDE_DIRECTORIES)
message(STATUS "algorithms_lib include directories:")
foreach(dir ${INCLUDE_DIRS})
    message(STATUS "  ${dir}")
endforeach()

# Set output name and properties
set_target_properties(algorithms_lib PROPERTIES
    OUTPUT_NAME "algorithms"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add option for debugging include paths
option(DEBUG_INCLUDE_PATHS "Enable debugging of include paths during compilation" OFF)

# Set compiler options based on compiler
if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(algorithms_lib PRIVATE /W4)
else()
    target_compile_options(algorithms_lib PRIVATE -Wall -Wextra -Wpedantic -Wno-missing-braces)
    if(NOT WIN32)
        target_compile_options(algorithms_lib PRIVATE -fPIC)
    endif()
endif()

# Add debug include paths functionality
if(DEBUG_INCLUDE_PATHS)
    target_compile_definitions(algorithms_lib PRIVATE DEBUG_INCLUDE_PATHS)
    
    # Add compiler-specific options to show include paths during compilation
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
        target_compile_options(algorithms_lib PRIVATE -H)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(algorithms_lib PRIVATE /showIncludes)
    endif()
    
    message(STATUS "Include path debugging enabled")
endif()

# Install targets
install(TARGETS algorithms_lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Print debug information
message(STATUS "Library output information:")
message(STATUS "  Binary dir: ${CMAKE_BINARY_DIR}")
message(STATUS "  Runtime output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Library output: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "  Archive output: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

get_target_property(LIB_LOCATION algorithms_lib LIBRARY_OUTPUT_DIRECTORY)
message(STATUS "  algorithms_lib output location: ${LIB_LOCATION}")

message(STATUS "=== Finished configuring Source Root ===")
