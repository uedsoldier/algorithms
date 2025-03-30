cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring CHECKSUM8 Tests (tests/CHECKSUM8/CMakeLists.txt) ===")

# Create a list to store test names
set(ADDED_TESTS "")

# Find implementation files
file(GLOB CHECKSUM8_IMPL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/CHECKSUM8/*.c")
message(STATUS "Found CHECKSUM8 implementation files: ${CHECKSUM8_IMPL_FILES}")

# Define test types
set(CHECKSUM8_TEST_TYPES
    "XOR"
    "2COMPLEMENT"
    "MODULO256"
)

# Function to configure a test executable
function(configure_checksum8_test TYPE)
    set(TEST_NAME "CHECKSUM8_${TYPE}")

    # Add executable
    add_executable(${TEST_NAME}_tester
        test_CHECKSUM8_${TYPE}.c
        ${CHECKSUM8_IMPL_FILES}
    )

    # Include directories
    target_include_directories(${TEST_NAME}_tester
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/include/CHECKSUM8
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/CHECKSUM8
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link with the main library
    target_link_libraries(${TEST_NAME}_tester
        PRIVATE
            algorithms_lib
    )

    # Set compiler options
    if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TEST_NAME}_tester PRIVATE /W4)
    else()
        target_compile_options(${TEST_NAME}_tester PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-missing-braces
        )
        # Link math library on non-MSVC platforms
        target_link_libraries(${TEST_NAME}_tester PRIVATE m)
    endif()

    # Add the test
    add_test(
        NAME ${TEST_NAME}_test
        COMMAND ${TEST_NAME}_tester
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    # Set comprehensive test properties
    set_tests_properties(${TEST_NAME}_test PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
        FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
        ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
    )

    # Add the test to our list and report
    list(APPEND ADDED_TESTS "${TEST_NAME}_test")
    set(ADDED_TESTS ${ADDED_TESTS} PARENT_SCOPE)
    message(STATUS "Added test: ${TEST_NAME}_test")

    # Print configuration summary for this test
    message(STATUS "${TEST_NAME} Test Configuration:")
    get_target_property(INCLUDE_DIRS ${TEST_NAME}_tester INCLUDE_DIRECTORIES)
    message(STATUS "  Include directories: ${INCLUDE_DIRS}")
    get_target_property(LINK_LIBS ${TEST_NAME}_tester LINK_LIBRARIES)
    message(STATUS "  Linked libraries: ${LINK_LIBS}")
    get_target_property(COMPILE_OPTIONS ${TEST_NAME}_tester COMPILE_OPTIONS)
    message(STATUS "  Compile options: ${COMPILE_OPTIONS}")
endfunction()

# Configure all test types
foreach(TEST_TYPE ${CHECKSUM8_TEST_TYPES})
    configure_checksum8_test(${TEST_TYPE})
endforeach()

# Set the list of tests in parent scope
set(CHECKSUM8_TESTS ${ADDED_TESTS} PARENT_SCOPE)

message(STATUS "Configured tests: ${ADDED_TESTS}")
message(STATUS "=== Finished configuring CHECKSUM8 Tests ===")
