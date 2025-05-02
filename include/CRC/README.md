
# CRC Library Debug Levels Guide

## Debug Level Categories

### ERROR (Level 1)
Critical issues that prevent correct operation:
```c
CRC_ERROR("Null pointer provided");
CRC_ERROR("Invalid CRC type: %u", crc_type);
CRC_ERROR("Invalid polynomial");
CRC_ERROR("Lookup table not found");
```

### WARN (Level 2)
Potential issues that don't prevent operation but might indicate problems:
```c
// Currently no WARN messages implemented
// Suggested additions:
CRC_WARN("Input data length is zero");
CRC_WARN("Using fallback calculation method - lookup table not available");
```

### INFO (Level 3)
High-level operation status:
```c
// Start/end of operations
CRC_INFO("Starting CRC8/16/32 calculation");
CRC_INFO("CRC8/16/32 calculation completed - Result: 0x%02X/04X/08X");
```

### DEBUG (Level 4)
Detailed operational information:
```c
CRC_DEBUG("Using polynomial: 0x%02X/04X/08X");
CRC_DEBUG("Initial seed: 0x%02X/04X/08X");
CRC_DEBUG("Input reflection: %s");
CRC_DEBUG("Output reflection: %s");
CRC_DEBUG("Processing %zu bytes");
CRC_DEBUG("Final XOR value: 0x%02X/04X/08X");
CRC_DEBUG("Using lookup table method");
```

### TRACE (Level 5)
Most detailed level, showing internal calculations:
```c
CRC_TRACE("Processing byte: 0x%02X");
CRC_TRACE("After input reflection: 0x%02X");
CRC_TRACE("After XOR with input: 0x%02X/04X/08X");
CRC_TRACE("Bit %d: MSB=%d, CRC=0x%02X/04X/08X");
CRC_TRACE("Lookup result: 0x%02X/04X/08X -> 0x%02X/04X/08X");
CRC_TRACE("After output reflection: 0x%02X/04X/08X -> 0x%02X/04X/08X");
```

## Usage Guidelines

1. **Production Environment**
   ```c
   #define CRC_DEBUG_ENABLE 1
   #define CRC_ACTIVE_DEBUG_LEVEL CRC_DEBUG_LEVEL_ERROR
   ```
   Only critical errors will be logged.

2. **Development Environment**
   ```c
   #define CRC_DEBUG_ENABLE 1
   #define CRC_ACTIVE_DEBUG_LEVEL CRC_DEBUG_LEVEL_INFO
   ```
   Errors and operation status will be logged.

3. **Testing Environment**
   ```c
   #define CRC_DEBUG_ENABLE 1
   #define CRC_ACTIVE_DEBUG_LEVEL CRC_DEBUG_LEVEL_DEBUG
   ```
   Detailed operational information will be logged.

4. **Debugging Environment**
   ```c
   #define CRC_DEBUG_ENABLE 1
   #define CRC_ACTIVE_DEBUG_LEVEL CRC_DEBUG_LEVEL_TRACE
   ```
   All debug information will be logged.

## Memory Usage Considerations

Debug message verbosity impacts both code size and execution time:

- ERROR: Minimal impact (~100 bytes per message)
- WARN/INFO: Low impact (~150 bytes per message)
- DEBUG: Medium impact (~200 bytes per message)
- TRACE: Highest impact (~250 bytes per message)

For embedded systems with limited resources, consider:
1. Using ERROR level only in production
2. Limiting DEBUG/TRACE messages to critical sections
3. Using conditional compilation to remove unused debug levels
