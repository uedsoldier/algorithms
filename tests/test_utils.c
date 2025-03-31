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

// void print_crc16_config(crc_t type) {
//     printf("=== CRC16 Configuration ===\n");

//     printf("Type:           %s\n", get_crc16_implementation(type));
//     printf("Polynomial:     0x%04X\n", CRC16_getPoly(type));
//     printf("Initial value:  0x%04X\n", CRC16_getSeed(type));
//     printf("Final XOR:      0x%04X\n", CRC16_getFinalXOR(type));
//     printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" :
//     "No"); printf("Reflect output: %s\n", CRC_getOutputReflected(type) ?
//     "Yes" : "No"); printf("CRC16 lookup table:    %s\n",
//            CRC16_USE_LOOKUP_TABLE ? "Yes" : "No");

//     printf("====================\n\n");
// }

// void print_crc32_config(crc_t type) {
//     printf("=== CRC32 Configuration ===\n");

//     printf("Type:           %s\n", get_crc32_implementation(type));
//     printf("Polynomial:     0x%08X\n", CRC32_getPoly(type));
//     printf("Initial value:  0x%08X\n", CRC32_getSeed(type));
//     printf("Final XOR:      0x%08X\n", CRC32_getFinalXOR(type));
//     printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" :
//     "No"); printf("Reflect output: %s\n", CRC_getOutputReflected(type) ?
//     "Yes" : "No"); printf("CRC32 lookup table:    %s\n",
//            CRC32_USE_LOOKUP_TABLE ? "Yes" : "No");

//     printf("====================\n\n");
// }
