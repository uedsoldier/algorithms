cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring AES Tests (tests/AES/CMakeLists.txt) ===")

# Create a list to store added tests
set(ADDED_TESTS "")

# Find implementation files
file(GLOB AES_IMPL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/AES/*.c")
message(STATUS "Found AES implementation files: ${AES_IMPL_FILES}")

# Define the list of AES test files
set(AES_TEST_TYPES
    "128_ECB"
    "128_CBC"
    "192_ECB"
    "192_CBC"
    "256_ECB"
    "256_CBC"
)

# Function to configure a test executable
function(configure_aes_test TYPE)
    set(TEST_NAME "test_AES${TYPE}")

    # Debug print to verify test file existence
    message(STATUS "Looking for test file: ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.c")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.c")
        message(STATUS "Test file found: ${TEST_NAME}.c")
    else()
        message(WARNING "Test file not found: ${TEST_NAME}.c")
        return()
    endif()

    # Add executable
    add_executable(${TEST_NAME}
        ${TEST_NAME}.c
        ${AES_IMPL_FILES}
    )

    # Include directories
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/include/AES
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/AES
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link with the main library
    target_link_libraries(${TEST_NAME}
        PRIVATE
            algorithms_lib
    )

    # Set compiler options
    if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TEST_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TEST_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-missing-braces
        )
        # Link math library on non-MSVC platforms
        target_link_libraries(${TEST_NAME} PRIVATE m)
    endif()

    # Add the test
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    # Set comprehensive test properties
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
        FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
        ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
    )

    # Add the test to our list and report
    list(APPEND ADDED_TESTS "${TEST_NAME}")
    set(ADDED_TESTS ${ADDED_TESTS} PARENT_SCOPE)
    message(STATUS "Added test: ${TEST_NAME}")

    # Print configuration summary for this test
    message(STATUS "${TEST_NAME} Test Configuration:")
    get_target_property(INCLUDE_DIRS ${TEST_NAME} INCLUDE_DIRECTORIES)
    message(STATUS "  Include directories: ${INCLUDE_DIRS}")
    get_target_property(LINK_LIBS ${TEST_NAME} LINK_LIBRARIES)
    message(STATUS "  Linked libraries: ${LINK_LIBS}")
    get_target_property(COMPILE_OPTIONS ${TEST_NAME} COMPILE_OPTIONS)
    message(STATUS "  Compile options: ${COMPILE_OPTIONS}")
endfunction()

# Configure all test types
foreach(TEST_TYPE ${AES_TEST_TYPES})  # Fixed variable name
    configure_aes_test(${TEST_TYPE})
endforeach()

# Set the list of tests in parent scope
set(AES_TESTS ${ADDED_TESTS} PARENT_SCOPE)

message(STATUS "Configured tests: ${ADDED_TESTS}")
message(STATUS "=== Finished configuring AES Tests ===")
