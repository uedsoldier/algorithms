cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring CRC8 Tests ===")

# Define the list of CRC8 test types
set(CRC8_TEST_TYPES
    8_CCITT_LOOKUP

)

# Function to configure a CRC8 test executable
function(configure_crc8_test TYPE)
    set(TEST_NAME "CRC${TYPE}_tester")
    set(SOURCE_FILE "test_CRC${TYPE}.c")

    # Debug print to verify test file existence
    message(STATUS "Looking for test file: ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
        message(STATUS "Test file found: ${SOURCE_FILE}")
    else()
        message(WARNING "Test file not found: ${SOURCE_FILE}")
        return()
    endif()

    # Add executable
    add_executable(${TEST_NAME}
        ${SOURCE_FILE}
        ${CRC_IMPL_FILES}
    )

    # Include directories
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/include/CRC
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/src/CRC
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Add common compile definitions for all CRC tests
    target_compile_definitions(${TEST_NAME}
        PRIVATE
            CRC_USE_IMPLEMENTATION_NAMES=1
    )

    # Add specific compile definitions based on LOOKUP/NO_LOOKUP type
    if(TYPE MATCHES ".*LOOKUP$")
        target_compile_definitions(${TEST_NAME}
            PRIVATE
                CRC8_USE_LOOKUP_TABLE=1
        )
    elseif(TYPE MATCHES ".*NO_LOOKUP$")
        target_compile_definitions(${TEST_NAME}
            PRIVATE
                CRC8_USE_LOOKUP_TABLE=0
        )
    endif()

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
endfunction()

# Configure all CRC8 test types
foreach(TEST_TYPE ${CRC8_TEST_TYPES})
    configure_crc8_test(${TEST_TYPE})
endforeach()

message(STATUS "=== Finished configuring CRC8 Tests ===")
