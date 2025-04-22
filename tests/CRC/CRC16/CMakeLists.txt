cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring CRC16 Tests ===")

# Define CRC16 variant enum mapping (matching crc.h)
set(CRC16_ENUM_XMODEM 0)


# Define the list of CRC16 test types
set(CRC16_TEST_TYPES
    XMODEM_LOOKUP
    XMODEM_NO_LOOKUP
)

# Function to configure a CRC16 test executable
function(configure_crc16_test TYPE)
    message(STATUS "\n=== Configuring test for type: ${TYPE} ===")

    set(TEST_NAME "CRC16_${TYPE}_tester")
    set(SOURCE_FILE "test_CRC16_generic.c")

    # Extract the CRC variant name
    string(REGEX REPLACE "_(LOOKUP|NO_LOOKUP)$" "" CRC_VARIANT "${TYPE}")

    # Get the enum value for this variant
    set(ENUM_VAR_NAME "CRC16_ENUM_${CRC_VARIANT}")
    if(DEFINED ${ENUM_VAR_NAME})
        set(CRC_ENUM_VALUE ${${ENUM_VAR_NAME}})
        message(STATUS "Found enum value for ${CRC_VARIANT}: ${CRC_ENUM_VALUE}")
    else()
        message(FATAL_ERROR "No enum value defined for variant: ${CRC_VARIANT}")
    endif()

    # Convert to lowercase for array name (still needed for test data arrays)
    string(TOLOWER "${CRC_VARIANT}" VARIANT_LOWERCASE)

    # Determine LOOKUP usage
    if(TYPE MATCHES "NO_LOOKUP$")
        set(USE_LOOKUP_TABLE 0)
    else()
        set(USE_LOOKUP_TABLE 1)
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
            ${CMAKE_CURRENT_SOURCE_DIR}/test_data
    )

    # Set compile definitions using the enum value
    target_compile_definitions(${TEST_NAME}
        PRIVATE
            "CRC16_USE_LOOKUP_TABLE=${USE_LOOKUP_TABLE}"
            "CRC_USE_IMPLEMENTATION_NAMES=1"
            "CRC16_VARIANT=${CRC_ENUM_VALUE}"  # Use the numeric enum value
            "VARIANT_LOWERCASE_crcs=${VARIANT_LOWERCASE}_crcs"
    )

    # Debug output
    message(STATUS "Configuration for ${TEST_NAME}:")
    message(STATUS "  Variant: ${CRC_VARIANT}")
    message(STATUS "  Enum Value: ${CRC_ENUM_VALUE}")
    message(STATUS "  Lookup Table: ${USE_LOOKUP_TABLE}")
    message(STATUS "  Array Name: expected_crc16_${VARIANT_LOWERCASE}_crcs")

    get_target_property(ACTUAL_DEFS ${TEST_NAME} COMPILE_DEFINITIONS)
    message(STATUS "Compile definitions: ${ACTUAL_DEFS}")

    # Rest of the configuration remains the same...
    target_link_libraries(${TEST_NAME}
        PRIVATE
            algorithms_lib
            test_utils
    )

    if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TEST_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TEST_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-missing-braces
        )
        target_link_libraries(${TEST_NAME} PRIVATE m)
    endif()

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )

    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
        FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
        ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
    )

    list(APPEND ADDED_TESTS "${TEST_NAME}")
    set(ADDED_TESTS ${ADDED_TESTS} PARENT_SCOPE)
    message(STATUS "Successfully configured test: ${TEST_NAME}")
endfunction()

# Configure all CRC16 test types
foreach(TEST_TYPE ${CRC16_TEST_TYPES})
    configure_crc16_test(${TEST_TYPE})
endforeach()

# Add CRC16 Validation Test
add_executable(CRC16_validation_tester
    test_CRC16_validation.c
    ${CRC_IMPL_FILES}
)

target_include_directories(CRC16_validation_tester
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/CRC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/CRC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_data
)

target_compile_definitions(CRC16_validation_tester
    PRIVATE
        "CRC_USE_IMPLEMENTATION_NAMES=1"
        "CRC16_USE_LOOKUP_TABLE=1"  # Enable lookup tables for validation tests
)

target_link_libraries(CRC16_validation_tester
    PRIVATE
        algorithms_lib
        test_utils
)

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(CRC16_validation_tester PRIVATE /W4)
else()
    target_compile_options(CRC16_validation_tester PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-missing-braces
    )
    target_link_libraries(CRC16_validation_tester PRIVATE m)
endif()

add_test(
    NAME CRC16_validation_test
    COMMAND CRC16_validation_tester
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

set_tests_properties(CRC16_validation_test PROPERTIES
    TIMEOUT 30
    PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
    FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
    ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
)


message(STATUS "=== Finished configuring CRC16 Tests ===")
