
/**
 * @file crc.h
 * @author José Roberto Parra Trewartha
 * @brief Cyclic Redundancy Check (CRC) Implementation Library
 * @version 0.1
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024
 *
 * This library provides comprehensive CRC calculation functionality supporting
 * multiple standard CRC algorithms including CRC8, CRC16, and CRC32 variants.
 * It implements both table-driven and direct calculation methods for optimal
 * performance and memory usage trade-offs.
 */

#ifndef CRC_H
#define CRC_H

/**
 * @brief Controls the use of implementation names for CRC algorithms
 *
 * When enabled (set to 1), the library includes string names for each CRC
 * algorithm variant, useful for debugging and logging. Disable to save program
 * memory.
 */
#ifndef CRC_USE_IMPLEMENTATION_NAMES
#define CRC_USE_IMPLEMENTATION_NAMES 1
#endif

/**
 * @brief Controls the use of pre-computed lookup tables for CRC8 calculations
 *
 * When enabled (set to 1), uses pre-computed lookup tables for CRC8
 * calculations. Each table consumes 256 bytes of program memory but
 * significantly improves calculation speed. Disable to save memory at the cost
 * of performance.
 */
#define CRC8_USE_LOOKUP_TABLE 0

/**
 * @brief Controls the use of pre-computed lookup tables for CRC16 calculations
 *
 * When enabled (set to 1), uses pre-computed lookup tables for CRC16
 * calculations. Each table consumes 512 bytes of program memory but
 * significantly improves calculation speed. Disable to save memory at the cost
 * of performance.
 */
#define CRC16_USE_LOOKUP_TABLE 0

/**
 * @brief Controls the use of pre-computed lookup tables for CRC32 calculations
 *
 * When enabled (set to 1), uses pre-computed lookup tables for CRC32
 * calculations. Each table consumes 1024 bytes of program memory but
 * significantly improves calculation speed. Disable to save memory at the cost
 * of performance.
 */
#define CRC32_USE_LOOKUP_TABLE 0

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Enumeration of supported CRC algorithm variants
 *
 * This enumeration defines all supported CRC algorithm variants including:
 * - CRC8 variants (CCITT, CDMA2000, DARC, etc.)
 * - CRC16 variants (XMODEM, CCITT, KERMIT, etc.)
 * - CRC32 variants (IEEE 802.3, MPEG-2, POSIX, etc.)
 *
 * Each variant uses specific parameters (polynomial, initial value, etc.)
 * optimized for particular use cases.
 */
typedef enum crc_t {
    // CRC8 Variants
    CRC8_CCITT = 0, /**< CRC-8/CCITT - Used in I2C communication */
    CRC8_CDMA2000,  /**< CRC-8/CDMA2000 - Used in CDMA systems */
    CRC8_DARC,      /**< CRC-8/DARC - Digital Amateur Radio Communications */
    CRC8_DVB_S2,    /**< CRC-8/DVB-S2 - Digital Video Broadcasting */
    CRC8_EBU,       /**< CRC-8/EBU - European Broadcasting Union */
    CRC8_I_CODE,    /**< CRC-8/I-CODE - RFID applications */
    CRC8_ITU,       /**< CRC-8/ITU - ITU-T I.432.1 standard */
    CRC8_MAXIM,     /**< CRC-8/MAXIM - 1-Wire® bus system */
    CRC8_ROHC,      /**< CRC-8/ROHC - Robust Header Compression */
    CRC8_WCDMA,     /**< CRC-8/WCDMA - 3G mobile communications */
    CRC8_SAE_J1850, /**< CRC-8/SAE-J1850 - Vehicle networks */
    CRC8_SAE_J1850_ZERO, /**< CRC-8/SAE-J1850-ZERO - Modified J1850 */
    CRC8_8H2F,           /**< CRC-8/8H2F - General purpose */

    // CRC16 Variants
    CRC16_XMODEM,       /**< CRC-16/XMODEM - File transfer protocol */
    CRC16_AUG_CCITT,    /**< CRC-16/AUG-CCITT - Augmented CCITT */
    CRC16_CCITT_FALSE,  /**< CRC-16/CCITT-FALSE - Modified CCITT */
    CRC16_GENIBUS,      /**< CRC-16/GENIBUS - Industrial automation */
    CRC16_CCITT_KERMIT, /**< CRC-16/KERMIT - Kermit protocol */
    CRC16_TMS37157,     /**< CRC-16/TMS37157 - Texas Instruments */
    CRC16_RIELLO,       /**< CRC-16/RIELLO - Riello UPS */
    CRC16_A,            /**< CRC-16/A - General purpose */
    CRC16_MCRF4XX,      /**< CRC-16/MCRF4XX - RFID tags */
    CRC16_X25,          /**< CRC-16/X-25 - X.25 protocol */
    CRC16_ARC,          /**< CRC-16/ARC - LHA compression */
    CRC16_BUYPASS,      /**< CRC-16/BUYPASS - Retail systems */
    CRC16_DDS110,       /**< CRC-16/DDS-110 - Data storage */
    CRC16_MAXIM,        /**< CRC-16/MAXIM - MAXIM devices */
    CRC16_USB,          /**< CRC-16/USB - USB protocol */
    CRC16_MODBUS,       /**< CRC-16/MODBUS - Modbus protocol */
    CRC16_DECT_X,       /**< CRC-16/DECT-X - DECT telephony */
    CRC16_DECT_R,       /**< CRC-16/DECT-R - DECT telephony */
    CRC16_DNP,          /**< CRC-16/DNP - DNP3 protocol */
    CRC16_EN13757,      /**< CRC-16/EN-13757 - Metering systems */
    CRC16_T10_DIF,      /**< CRC-16/T10-DIF - SCSI DIF */
    CRC16_TELEDISK,     /**< CRC-16/TELEDISK - Disk imaging */
    CRC16_CDMA2000,     /**< CRC-16/CDMA2000 - CDMA systems */

    // CRC32 Variants
    CRC32_D,      /**< CRC-32/D - General purpose */
    CRC32_Q,      /**< CRC-32/Q - Aeronautical systems */
    CRC32_C,      /**< CRC-32/C - Base-91 encoding */
    CRC32_ISO,    /**< CRC-32/ISO - ISO standard */
    CRC32_BZIP2,  /**< CRC-32/BZIP2 - BZIP2 compression */
    CRC32_MPEG_2, /**< CRC-32/MPEG-2 - MPEG-2 systems */
    CRC32_POSIX,  /**< CRC-32/POSIX - POSIX cksum */
    CRC32_JAMCRC, /**< CRC-32/JAMCRC - JAM STAPL */
    CRC32_XFER,   /**< CRC-32/XFER - XFER protocol */
} crc_t;

/* Function Prototypes */

/**
 * @brief Calculates CRC8 checksum for given data
 *
 * @param data Pointer to input data buffer
 * @param dataLength Length of input data in bytes
 * @param crc_type CRC algorithm variant to use
 * @return uint8_t Calculated CRC8 checksum
 */
uint8_t CRC8(const void *data, uint16_t dataLength, crc_t crc_type);

/**
 * @brief Calculates CRC16 checksum for given data
 *
 * @param data Pointer to input data buffer
 * @param dataLength Length of input data in bytes
 * @param crc_type CRC algorithm variant to use
 * @return uint16_t Calculated CRC16 checksum
 */
uint16_t CRC16(const void *data, uint16_t dataLength, crc_t crc_type);

/**
 * @brief Calculates CRC32 checksum for given data
 *
 * @param data Pointer to input data buffer
 * @param dataLength Length of input data in bytes
 * @param crc_type CRC algorithm variant to use
 * @return uint32_t Calculated CRC32 checksum
 */
uint32_t CRC32(const void *data, uint16_t dataLength, crc_t crc_type);

#if defined(CRC_USE_IMPLEMENTATION_NAMES) && (CRC_USE_IMPLEMENTATION_NAMES == 1)
/**
 * @brief Gets the string name of a CRC8 implementation
 *
 * @param index Index of the CRC8 implementation
 * @return const char* Name of the implementation or NULL if invalid index
 */
const char *get_crc8_implementation(uint8_t index);

/**
 * @brief Gets the string name of a CRC16 implementation
 *
 * @param index Index of the CRC16 implementation
 * @return const char* Name of the implementation or NULL if invalid index
 */
const char *get_crc16_implementation(uint8_t index);

/**
 * @brief Gets the string name of a CRC32 implementation
 *
 * @param index Index of the CRC32 implementation
 * @return const char* Name of the implementation or NULL if invalid index
 */
const char *get_crc32_implementation(uint8_t index);
#endif

/* Internal Helper Functions */

/**
 * @brief Inverts bits in a byte
 * @param data Byte to invert
 * @return uint8_t Inverted byte
 */
static uint8_t bit_invert_Byte(uint8_t data);

/**
 * @brief Inverts bits in a 16-bit integer
 * @param data Integer to invert
 * @return uint16_t Inverted integer
 */
static uint16_t bit_invert_Int16(uint16_t data);

/**
 * @brief Inverts bits in a 32-bit integer
 * @param data Integer to invert
 * @return uint32_t Inverted integer
 */
static uint32_t bit_invert_Int32(uint32_t data);

#endif /*CRC_H*/
