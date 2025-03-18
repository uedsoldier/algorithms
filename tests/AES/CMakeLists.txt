cmake_minimum_required(VERSION 3.12)
project(aes_testing
    VERSION 0.1.0
    DESCRIPTION "AES functions testing suite"
    LANGUAGES C
)

# Enable testing for the current directory and below
enable_testing()

# Set path variables for better maintainability
set(ALGORITHMS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
set(ALGORITHMS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include")

# Include directories - add both source and include directories
# This allows headers to be found whether they're in src or include
include_directories(
    ${ALGORITHMS_SOURCE_DIR}
    ${ALGORITHMS_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..  # Add repository root for relative includes
)

# Define common compiler warnings based on compiler
if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(COMPILER_WARNINGS /W4)
else()
    set(COMPILER_WARNINGS -Wall -Wextra -Wpedantic -fPIC)
endif()

# Function to create test executable with common settings
function(add_aes_test TEST_NAME)
    # Parse additional source files
    set(TEST_SOURCES ${ARGN})
    
    # Check if the test file exists with the "test_" prefix
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_${TEST_NAME}.c")
        set(TEST_FILE "test_${TEST_NAME}.c")
    # Also check for the alternative naming pattern
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}_test.c")
        set(TEST_FILE "${TEST_NAME}_test.c")
    else()
        message(STATUS "Test file for ${TEST_NAME} not found, skipping...")
        return()
    endif()

    # Create the test executable
    add_executable(${TEST_NAME}_tester
        "${TEST_FILE}"
        ${TEST_SOURCES}
    )

    # Add include directories specifically for this target
    target_include_directories(${TEST_NAME}_tester PRIVATE
        ${ALGORITHMS_SOURCE_DIR}
        ${ALGORITHMS_SOURCE_DIR}/AES
        ${ALGORITHMS_SOURCE_DIR}/PKCS7
        ${ALGORITHMS_INCLUDE_DIR}
        ${ALGORITHMS_INCLUDE_DIR}/AES
        ${ALGORITHMS_INCLUDE_DIR}/PKCS7
    )

    # Add compiler warnings
    target_compile_options(${TEST_NAME}_tester PRIVATE ${COMPILER_WARNINGS})

    # Link math library (MSVC does not need -lm)
    if(NOT MSVC)
        target_link_libraries(${TEST_NAME}_tester PRIVATE m)
    endif()

    # Set optimization and debug flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${TEST_NAME}_tester PRIVATE -g -O0)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TEST_NAME}_tester PRIVATE -O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        target_compile_options(${TEST_NAME}_tester PRIVATE -g -O2)
    endif()

    # Enable sanitizers in Debug mode if supported
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
        include(CheckCCompilerFlag)
        check_c_compiler_flag("-fsanitize=address" HAS_SANITIZER)
        if(HAS_SANITIZER)
            target_compile_options(${TEST_NAME}_tester PRIVATE
                -fsanitize=address
                -fsanitize=undefined
                -fno-omit-frame-pointer
            )
            target_link_options(${TEST_NAME}_tester PRIVATE
                -fsanitize=address
                -fsanitize=undefined
            )
        endif()
    endif()

    # Add the test
    add_test(
        NAME ${TEST_NAME}_test
        COMMAND ${TEST_NAME}_tester
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    # Set test properties with proper pass/fail conditions
    set_tests_properties(${TEST_NAME}_test PROPERTIES
        TIMEOUT 30 # Timeout in seconds
        LABELS "${TEST_NAME}"
        PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
        FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
        WILL_FAIL FALSE
    )
endfunction()

# Common AES dependencies
set(AES_COMMON_SOURCES
    "${ALGORITHMS_SOURCE_DIR}/AES/AES_common.c"
    "${ALGORITHMS_SOURCE_DIR}/PKCS7/PKCS7.c"
)

# Group tests by AES variant
set(AES128_TESTS
    AES128_ECB
    AES128_CBC
)

set(AES192_TESTS
    AES192_ECB
    AES192_CBC
)

set(AES256_TESTS
    AES256_ECB
    AES256_CBC
)

# Add AES-128 tests
foreach(test ${AES128_TESTS})
    add_aes_test(${test} 
        "${ALGORITHMS_SOURCE_DIR}/AES/AES128.c"
        ${AES_COMMON_SOURCES}
    )
endforeach()

# Add AES-192 tests
foreach(test ${AES192_TESTS})
    add_aes_test(${test}
        "${ALGORITHMS_SOURCE_DIR}/AES/AES192.c"
        ${AES_COMMON_SOURCES}
    )
endforeach()

# Add AES-256 tests
foreach(test ${AES256_TESTS})
    add_aes_test(${test}
        "${ALGORITHMS_SOURCE_DIR}/AES/AES256.c"
        ${AES_COMMON_SOURCES}
    )
endforeach()

# Print test configuration
message(STATUS "Configured AES tests:")
message(STATUS "  AES-128 tests: ${AES128_TESTS}")
message(STATUS "  AES-192 tests: ${AES192_TESTS}")
message(STATUS "  AES-256 tests: ${AES256_TESTS}")
