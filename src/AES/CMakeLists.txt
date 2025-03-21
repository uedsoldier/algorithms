# AES source files
set(AES_SOURCES
    AES128.c
    AES192.c
    AES256.c
    AES_common.c
)

# Add these files to the parent target
target_sources(algorithms_lib
    PRIVATE
        ${AES_SOURCES}
)

# Include directories
target_include_directories(algorithms_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)