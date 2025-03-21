# BASE64 source files
set(BASE64_SOURCES
    base64.c
)

# Add these files to the parent target
target_sources(algorithms_lib
    PRIVATE
        ${BASE64_SOURCES}
)

# Include directories
target_include_directories(algorithms_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)