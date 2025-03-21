cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring AES Tests (tests/AES/CMakeLists.txt) ===")

# Define the list of AES test files
set(AES_TESTS
    test_AES128_ECB
    test_AES128_CBC
    test_AES192_ECB
    test_AES192_CBC
    test_AES256_ECB
    test_AES256_CBC
)

# Create a list to store added tests
set(ADDED_TESTS "")

# Process each test
foreach(TEST_NAME ${AES_TESTS})
    # Create the test executable
    add_executable(${TEST_NAME} ${TEST_NAME}.c)

    # Link with algorithms library
    target_link_libraries(${TEST_NAME}
        PRIVATE
            algorithms_lib
    )

    # Register the test with CTest
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    # Set test properties
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
        FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
    )

    # Add to our list of tests
    list(APPEND ADDED_TESTS ${TEST_NAME})
    message(STATUS "Added test: ${TEST_NAME}")
endforeach()

# Set the list of tests in parent scope
set(AES_TESTS ${ADDED_TESTS} PARENT_SCOPE)

message(STATUS "=== Finished configuring AES Tests ===")
