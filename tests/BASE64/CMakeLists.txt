cmake_minimum_required(VERSION 3.12)

message(STATUS "=== Configuring BASE64 Tests (tests/BASE64/CMakeLists.txt) ===")

# Create a list to store test names
set(ADDED_TESTS "")

# Find implementation files
file(GLOB BASE64_IMPL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/BASE64/*.c")
message(STATUS "Found BASE64 implementation files: ${BASE64_IMPL_FILES}")

# Define the BASE64 test executable
add_executable(BASE64_tester
    test_BASE64.c
    ${BASE64_IMPL_FILES}
)

# Include directories - complete set of possible include paths
target_include_directories(BASE64_tester
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/BASE64
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/BASE64
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with the main library
target_link_libraries(BASE64_tester
    PRIVATE
        algorithms_lib
)

# Set compiler options
if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(BASE64_tester PRIVATE /W4)
else()
    target_compile_options(BASE64_tester PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wno-missing-braces
    )
    # Link math library on non-MSVC platforms
    target_link_libraries(BASE64_tester PRIVATE m)
endif()

# Add the test
add_test(
    NAME BASE64_test
    COMMAND BASE64_tester
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Set comprehensive test properties
set_tests_properties(BASE64_test PROPERTIES
    TIMEOUT 30
    PASS_REGULAR_EXPRESSION "Final result: ALL TESTS PASSED"
    FAIL_REGULAR_EXPRESSION "(Final result: SOME TESTS FAILED)|(Sanitizer)"
    ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
)

# Add the test to our list and report
list(APPEND ADDED_TESTS "BASE64_test")
message(STATUS "Added test: BASE64_test")

# Set the list of tests in parent scope
set(BASE64_TESTS ${ADDED_TESTS} PARENT_SCOPE)

# Print configuration summary
message(STATUS "BASE64 Test Configuration:")
get_target_property(INCLUDE_DIRS BASE64_tester INCLUDE_DIRECTORIES)
message(STATUS "  Include directories: ${INCLUDE_DIRS}")
get_target_property(LINK_LIBS BASE64_tester LINK_LIBRARIES)
message(STATUS "  Linked libraries: ${LINK_LIBS}")
get_target_property(COMPILE_OPTIONS BASE64_tester COMPILE_OPTIONS)
message(STATUS "  Compile options: ${COMPILE_OPTIONS}")

message(STATUS "=== Finished configuring BASE64 Tests ===")
