/**
 * @file test_utils.c
 * @brief Implementation of common test utilities
 */
#include "test_utils.h"

#include <stdio.h>

#include "crc.h"

void print_hex(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void print_crc8_config(crc_t type) {
    printf("=== CRC8 Configuration ===\n");

    printf("Type:           %s\n", get_crc8_implementation(type));
    printf("Polynomial:     0x%02X\n", CRC8_getPoly(type));
    printf("Initial value:  0x%02X\n", CRC8_getSeed(type));
    printf("Final XOR:      0x%02X\n", CRC8_getFinalXOR(type));
    printf("Reflect input:  %s\n", CRC_getInputReflected(type) ? "Yes" : "No");
    printf("Reflect output: %s\n", CRC_getOutputReflected(type) ? "Yes" : "No");
    printf("CRC8 lookup table:    %s\n", CRC8_USE_LOOKUP_TABLE ? "Yes" : "No");

    printf("====================\n\n");
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
