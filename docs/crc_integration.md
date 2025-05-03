# CRC Library Integration Guide

## Method 1: Direct File Integration

### 1. File Setup
Create the following directory structure in your project:
```
your_project/
├── include/
│   └── CRC/
│       └── crc.h
└── src/
    └── CRC/
        └── crc.c
```

### 2. Configuration Options
The library supports several configuration options in `crc.h`:
- `CRC_USE_IMPLEMENTATION_NAMES` (0 or 1) - Enable/disable algorithm names
- `CRC_USE_LOOKUP_TABLE` (0 or 1) - Enable/disable lookup tables for better performance

## Method 2: CMake Integration

### 1. Add as Static Library
Create a `CMakeLists.txt` file:

```cmake
# Add CRC library
add_library(crc_lib STATIC
    src/CRC/crc.c
)

target_include_directories(crc_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Link with your main project
target_link_libraries(your_project PRIVATE crc_lib)
```

## Usage Example

```c
#include "CRC/crc.h"

int main() {
    const char* data = "Hello, World!";
    size_t length = strlen(data);

    // Calculate CRC8
    uint8_t crc8_result = CRC8(data, length, CRC8_CCITT);

    // Calculate CRC16
    uint16_t crc16_result = CRC16(data, length, CRC16_CCITT_FALSE);

    // Calculate CRC32
    uint32_t crc32_result = CRC32(data, length, CRC32_D);

    return 0;
}
```

## Dependencies
- Standard C library
- C compiler supporting C99 or later

## Compilation Flags
When compiling, ensure you:
1. Add the include directory to your include path
2. Use C99 or later standard
