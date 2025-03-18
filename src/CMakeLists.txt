cmake_minimum_required(VERSION 3.12)

# Define the main library
add_library(algorithms_lib
    # Core files
    # Add your core source files here if any
    
    # Algorithm implementations
    AES/AES128.c
    AES/AES192.c
    AES/AES256.c
    AES/AES_common.c
    BASE64/base64.c
    CHECKSUM8/checksum8.c
    CRC/crc.c
    PKCS7/PKCS7.c
    SHA1/sha1.c
    SHA256/sha256.c
    XTEA/xtea.c
)

# Set include directories for the library
target_include_directories(algorithms_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set compiler options based on build type
if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(algorithms_lib PRIVATE /W4)  # High warning level for MSVC
else()
    target_compile_options(algorithms_lib PRIVATE -Wall -Wextra -Wpedantic -fPIC)  # POSIX shared library support
endif()

# Set optimization and debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(algorithms_lib PRIVATE -g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(algorithms_lib PRIVATE -O2)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_options(algorithms_lib PRIVATE -g -O2)
endif()

# Add subdirectories for algorithm categories
# This allows each algorithm to have its own CMakeLists.txt if needed
add_subdirectory(AES)
add_subdirectory(BASE64)
add_subdirectory(CHECKSUM8)
add_subdirectory(CRC)
add_subdirectory(PKCS7)
add_subdirectory(SHA1)
add_subdirectory(SHA256)
add_subdirectory(XTEA)
