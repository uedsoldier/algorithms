/**
 * @file test_utils.c
 * @brief Implementation of common test utilities
 */
#include "test_utils.h"

#include <stdio.h>
#include <string.h>

void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

bool bytes_equal(const uint8_t *s1, size_t len1, const uint8_t *s2,
                 size_t len2) {
    if (len1 != len2) return false;
    if (s1 == NULL && s2 == NULL) return true;
    if (s1 == NULL || s2 == NULL) return false;
    return memcmp(s1, s2, len1) == 0;
}

bool test_crc8(const uint8_t *data, size_t len, uint8_t expected_crc,
               crc_t crc_type) {
    uint8_t calculated_crc = CRC8(data, len, crc_type);
    bool crc_matches = (calculated_crc == expected_crc);

    // Print info
    printf("Expected CRC8: 0x%02X\n", expected_crc);
    printf("Calculated CRC8: 0x%02X\n", calculated_crc);

    printf("CRC8 matches: %s\n", crc_matches ? "YES" : "NO");
    return crc_matches;
}

bool test_crc16(const uint8_t *data, size_t len, uint16_t expected_crc,
                crc_t crc_type) {
    uint16_t calculated_crc = CRC16(data, len, crc_type);
    bool crc_matches = (calculated_crc == expected_crc);
    // Print info
    printf("Expected CRC16: 0x%04X\n", expected_crc);
    printf("Calculated CRC16: 0x%04X\n", calculated_crc);
    printf("CRC16 matches: %s\n", crc_matches ? "YES" : "NO");

    return crc_matches;
}

bool test_crc32(const uint8_t *data, size_t len, uint32_t expected_crc,
                crc_t crc_type) {
    uint32_t calculated_crc = CRC32(data, len, crc_type);
    bool crc_matches = (calculated_crc == expected_crc);

    // Print info
    printf("Expected CRC32: 0x%08X\n", expected_crc);
    printf("Calculated CRC32: 0x%08X\n", calculated_crc);
    printf("CRC32 matches: %s\n", crc_matches ? "YES" : "NO");

    return crc_matches;
}
