cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring CRC8 Tests ===")

# Define CRC8 variant enum mapping (matching crc.h)
set(CRC8_ENUM_CCITT 0)
set(CRC8_ENUM_CDMA2000 1)
set(CRC8_ENUM_DARC 2)
set(CRC8_ENUM_DVBS2 3)
set(CRC8_ENUM_EBU 4)
set(CRC8_ENUM_ICODE 5)
set(CRC8_ENUM_ITU 6)
set(CRC8_ENUM_MAXIM 7)
set(CRC8_ENUM_ROHC 8)
set(CRC8_ENUM_WCDMA 9)
set(CRC8_ENUM_SAEJ1850 10)
set(CRC8_ENUM_SAEJ1850_ZERO 11)
set(CRC8_ENUM_AUTOSAR 12)
set(CRC8_ENUM_BLUETOOTH 13)
set(CRC8_ENUM_GSMA 14)
set(CRC8_ENUM_GSMB 15)
set(CRC8_ENUM_HITAG 16)
set(CRC8_ENUM_LTE 17)

# Define the list of CRC8 test types
set(CRC8_TEST_TYPES
    CCITT_LOOKUP
    CCITT_NO_LOOKUP
    CDMA2000_LOOKUP
    CDMA2000_NO_LOOKUP
    MAXIM_LOOKUP
    MAXIM_NO_LOOKUP
    DARC_LOOKUP
    DARC_NO_LOOKUP
    DVBS2_LOOKUP
    DVBS2_NO_LOOKUP
    EBU_LOOKUP
    EBU_NO_LOOKUP
    ICODE_LOOKUP
    ICODE_NO_LOOKUP
    ITU_LOOKUP
    ITU_NO_LOOKUP
    BLUETOOTH_LOOKUP
    BLUETOOTH_NO_LOOKUP
    GSMA_LOOKUP
    GSMA_NO_LOOKUP
    GSMB_LOOKUP
    GSMB_NO_LOOKUP
    HITAG_LOOKUP
    HITAG_NO_LOOKUP
    LTE_LOOKUP
    LTE_NO_LOOKUP
)

# Function to configure a CRC8 test executable
function(configure_crc8_test TYPE)
    message(STATUS "\n=== Configuring test for type: ${TYPE} ===")

    set(TEST_NAME "CRC8_${TYPE}_tester")
    set(SOURCE_FILE "test_CRC8_generic.c")

    # Extract the CRC variant name
    string(REGEX REPLACE "_(LOOKUP|NO_LOOKUP)$" "" CRC_VARIANT "${TYPE}")

    # Get the enum value for this variant
    set(ENUM_VAR_NAME "CRC8_ENUM_${CRC_VARIANT}")
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
            "CRC8_USE_LOOKUP_TABLE=${USE_LOOKUP_TABLE}"
            "CRC_USE_IMPLEMENTATION_NAMES=1"
            "CRC8_VARIANT=${CRC_ENUM_VALUE}"  # Use the numeric enum value
            "VARIANT_LOWERCASE_crcs=${VARIANT_LOWERCASE}_crcs"
    )

    # Debug output
    message(STATUS "Configuration for ${TEST_NAME}:")
    message(STATUS "  Variant: ${CRC_VARIANT}")
    message(STATUS "  Enum Value: ${CRC_ENUM_VALUE}")
    message(STATUS "  Lookup Table: ${USE_LOOKUP_TABLE}")
    message(STATUS "  Array Name: expected_crc8_${VARIANT_LOWERCASE}_crcs")

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

# Configure all CRC8 test types
foreach(TEST_TYPE ${CRC8_TEST_TYPES})
    configure_crc8_test(${TEST_TYPE})
endforeach()

message(STATUS "=== Finished configuring CRC8 Tests ===")
