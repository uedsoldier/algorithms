/**
 * @file checksum8.h
 * @brief Implementation of 8-bit checksum calculation functions
 * @author Eng. José Roberto Parra Trewartha
 * @version 1.0
 */
#ifndef CHECKSUM8_H
#define CHECKSUM8_H

#define CHECKSUM8_USE_IMPLEMENTATION_NAMES 1

#include <stdint.h>

/**
 * @brief Enumeration of available 8-bit checksum calculation methods
 */
typedef enum CHECKSUM8_t {
    CHECKSUM8_XOR,         /**< XOR-based checksum */
    CHECKSUM8_modulo256,   /**< Modulo 256 sum checksum */
    CHECKSUM8_2complement  /**< Two's complement checksum */
} CHECKSUM8_t;

/**
 * @brief Calculate 8-bit checksum using specified method
 *
 * This function calculates an 8-bit checksum over the provided data using
 * one of the implemented methods.
 *
 * @param data Pointer to data buffer to calculate checksum for
 * @param len Length of data in bytes
 * @param checksum_type Type of checksum algorithm to use
 * @return 8-bit checksum value
 */
uint8_t checksum8(void *data, uint16_t len, CHECKSUM8_t checksum_type);

#if defined(CHECKSUM8_USE_IMPLEMENTATION_NAMES) && \
    (CHECKSUM8_USE_IMPLEMENTATION_NAMES == 1)
/**
 * @brief String names for each checksum implementation
 * 
 * These strings provide human-readable names for each checksum method.
 * Only available when CHECKSUM8_USE_IMPLEMENTATION_NAMES is defined and set to 1.
 */
static const char checksum8_implementation_0[] = "XOR";
static const char checksum8_implementation_1[] = "modulo 256";
static const char checksum8_implementation_2[] = "2's complement";
static const char *const checksum8_implementations[] = {
    checksum8_implementation_0,
    checksum8_implementation_1,
    checksum8_implementation_2
};
#endif

#endif /*CHECKSUM8_H*/
