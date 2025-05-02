/**
 * @file crc.c
 * @author José Roberto Parra Trewartha
 * @brief Cyclic Redundancy Check (CRC) source file
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

#include "crc.h"

/**
 * @brief Inverts bits in a data value of specified length
 * @param data Pointer to the data to invert
 * @param bit_length Number of bits to invert (must be 8, 16, or 32)
 * @return uint32_t Inverted value (cast as needed)
 */
static uint32_t bit_invert(const void *data, uint8_t bit_length) {
    uint32_t input = 0;
    uint32_t inverted = 0;

    // Validate input parameters
    if (data == NULL ||
        (bit_length != 8 && bit_length != 16 && bit_length != 32)) {
        return 0;
    }

    // Copy input data based on bit length
    switch (bit_length) {
        case 8:
            input = *(const uint8_t *)data;
            break;
        case 16:
            input = *(const uint16_t *)data;
            break;
        case 32:
            input = *(const uint32_t *)data;
            break;
    }

    // Perform bit inversion
    for (uint8_t i = 0; i < bit_length; i++) {
        if ((input & (1UL << i)) != 0) {
            inverted |= (1UL << (bit_length - 1 - i));
        }
    }

    return inverted;
}

/**
 * @brief Inverts bits in a byte
 * @param data Byte to invert
 * @return uint8_t Inverted byte
 */
static uint8_t bit_invert_Byte(uint8_t data) {
    return (uint8_t)bit_invert(&data, 8);
}

/**
 * @brief Inverts bits in a 16-bit integer
 * @param data Integer to invert
 * @return uint16_t Inverted integer
 */
static uint16_t bit_invert_Int16(uint16_t data) {
    return (uint16_t)bit_invert(&data, 16);
}

/**
 * @brief Inverts bits in a 32-bit integer
 * @param data Integer to invert
 * @return uint32_t Inverted integer
 */
static uint32_t bit_invert_Int32(uint32_t data) {
    return bit_invert(&data, 32);
}

#if defined(CRC_USE_IMPLEMENTATION_NAMES) && (CRC_USE_IMPLEMENTATION_NAMES == 1)
/**
 * @brief Array containing all CRC implementation names
 */
static const char *const crc_implementations[] = {
    // CRC8 implementations
    "CRC8_CCITT", "CRC8_CDMA2000", "CRC8_DARC", "CRC8_DVB_S2", "CRC8_EBU",
    "CRC8_I_CODE", "CRC8_ITU", "CRC8_MAXIM", "CRC8_ROHC", "CRC8_WCDMA",
    "CRC8_SAE_J1850", "CRC8_SAE_J1850_ZERO", "CRC8_AUTOSAR","CRC8_BLUETOOTH",
    "CRC8_GSM_A", "CRC8_GSM_B", "CRC8_HITAG", "CRC8_LTE",

    // CRC16 implementations
    "CRC16_XMODEM", "CRC16_AUG_CCITT", "CRC16_CCITT_FALSE", "CRC16_GENIBUS",
    "CRC16_CCITT_KERMIT", "CRC16_TMS37157", "CRC16_RIELLO", "CRC16_A",
    "CRC16_MCRF4XX", "CRC16_X25", "CRC16_ARC", "CRC16_BUYPASS", "CRC16_DDS110",
    "CRC16_MAXIM", "CRC16_USB", "CRC16_MODBUS", "CRC16_DECT_X", "CRC16_DECT_R",
    "CRC16_DNP", "CRC16_EN13757", "CRC16_T10_DIF", "CRC16_TELEDISK",
    "CRC16_CDMA2000",

    // CRC32 implementations
    "CRC32_D", "CRC32_Q", "CRC32_C", "CRC32_ISO", "CRC32_BZIP2", "CRC32_MPEG_2",
    "CRC32_POSIX", "CRC32_JAMCRC", "CRC32_XFER"

    // Add more implementations here if needed
};

const char *get_crc_implementation_name(crc_t index) {
    if (index < CRC_IMPL_COUNT) {
        return crc_implementations[index];
    }
    CRC_ERROR_SIMPLE("Invalid CRC implementation index");
    return NULL;
}
#endif

uint8_t CRC8_getPoly(crc_t crc_type) {
    uint8_t retVal;
    switch (crc_type) {
        case CRC8_CCITT:
        case CRC8_ITU:
        case CRC8_ROHC:
            retVal = 0x07;
            break;
        case CRC8_EBU:
        case CRC8_I_CODE:
        case CRC8_SAE_J1850:
        case CRC8_SAE_J1850_ZERO:
        case CRC8_GSM_A:
        case CRC8_HITAG:
            retVal = 0x1D;
            break;
        case CRC8_AUTOSAR:
            retVal = 0x2F;
            break;
        case CRC8_MAXIM:
            retVal = 0x31;
            break;
        case CRC8_DARC:
            retVal = 0x39;
            break;
        case CRC8_CDMA2000:
        case CRC8_WCDMA:
        case CRC8_LTE:
            retVal = 0x9B;
            break;
        case CRC8_DVB_S2:
            retVal = 0xD5;
            break;
        case CRC8_BLUETOOTH:
            retVal = 0xA7;
            break;
        case CRC8_GSM_B:
            retVal = 0x49;
            break;
        default:
            retVal = 0;  // Caso sin sentido
            break;
    }
    return retVal;
}

uint16_t CRC16_getPoly(crc_t crc_type) {
    uint16_t retVal;
    switch (crc_type) {
        case CRC16_CCITT_FALSE:
        case CRC16_XMODEM:
        case CRC16_AUG_CCITT:
        case CRC16_GENIBUS:
        case CRC16_CCITT_KERMIT:
        case CRC16_TMS37157:
        case CRC16_RIELLO:
        case CRC16_MCRF4XX:
        case CRC16_X25:
        case CRC16_A:
            retVal = 0x1021;
            break;
        case CRC16_ARC:
        case CRC16_BUYPASS:
        case CRC16_DDS110:
        case CRC16_MAXIM:
        case CRC16_USB:
        case CRC16_MODBUS:
            retVal = 0x8005;
            break;
        case CRC16_DECT_R:
        case CRC16_DECT_X:
            retVal = 0x0589;
            break;
        case CRC16_DNP:
        case CRC16_EN13757:
            retVal = 0x3D65;
            break;
        case CRC16_T10_DIF:
            retVal = 0x8BB7;
            break;
        case CRC16_TELEDISK:
            retVal = 0xA097;
            break;
        case CRC16_CDMA2000:
            retVal = 0xC867;
            break;
        default:
            retVal = 0;
            break;  // Caso sin sentido
    }

    return retVal;
}

uint32_t CRC32_getPoly(crc_t crc_type) {
    uint32_t retVal;
    switch (crc_type) {
        case CRC32_D:
            retVal = 0xA833982BUL;
            break;
        case CRC32_Q:
            retVal = 0x814141ABUL;
            break;
        case CRC32_C:
            retVal = 0x1EDC6F41UL;
            break;
        case CRC32_ISO:
        case CRC32_BZIP2:
        case CRC32_MPEG_2:
        case CRC32_POSIX:
        case CRC32_JAMCRC:
            retVal = 0x04C11DB7UL;
            break;
        case CRC32_XFER:
            retVal = 0x000000AFUL;
            break;
        // case CRC32_K1:
        // 	retVal = 0x741B8CD7UL;
        // 	break;
        // case CRC32_K2:
        // 	retVal = 0x32583499UL;
        // 	break;
        default:
            retVal = 0UL;
            break;  // Caso sin sentido
    }
    return retVal;
}

uint8_t CRC8_getSeed(crc_t crc_type) {
    uint8_t retVal;
    switch (crc_type) {
        case CRC8_I_CODE:
            retVal = 0xFD;
            break;
        case CRC8_ROHC:
        case CRC8_EBU:
        case CRC8_CDMA2000:
        case CRC8_SAE_J1850:
        case CRC8_AUTOSAR:
        case CRC8_HITAG:
            retVal = 0xFF;
            break;
        default:
            retVal = 0x00;
            break;  // Rest of cases
    }
    return retVal;
}

uint16_t CRC16_getSeed(crc_t crc_type) {
    uint16_t retVal;
    switch (crc_type) {
        case CRC16_CCITT_FALSE:
        case CRC16_GENIBUS:
        case CRC16_MCRF4XX:
        case CRC16_X25:
        case CRC16_USB:
        case CRC16_MODBUS:
        case CRC16_CDMA2000:
            retVal = 0xFFFF;
            break;
        case CRC16_AUG_CCITT:
            retVal = 0x1D0F;
            break;
        case CRC16_DDS110:
            retVal = 0x800D;
            break;
        case CRC16_TMS37157:
            retVal = 0x89EC;
            break;
        case CRC16_RIELLO:
            retVal = 0xB2AA;
            break;
        case CRC16_A:
            retVal = 0xC6C6;
            break;
        default:
            retVal = 0x0000;
            break;  // Rest of cases
    }
    return retVal;
}

uint32_t CRC32_getSeed(crc_t crc_type) {
    uint32_t retVal;
    switch (crc_type) {
        case CRC32_Q:
        case CRC32_POSIX:
        case CRC32_XFER:
            retVal = 0UL;
            break;
        default:
            retVal = 0xFFFFFFFFUL;
            break;  // El resto de los casos
    }
    return retVal;
}

bool CRC_getInputReflected(crc_t crc_type) {
    bool retVal;
    switch (crc_type) {
        // CRC8
        case CRC8_CCITT:
        case CRC8_ITU:
        case CRC8_I_CODE:
        case CRC8_CDMA2000:
        case CRC8_DVB_S2:
        case CRC8_SAE_J1850_ZERO:
        case CRC8_SAE_J1850:
        case CRC8_AUTOSAR:
        case CRC8_GSM_A:
        case CRC8_GSM_B:
        case CRC8_HITAG:
        case CRC8_LTE:
        // CRC16
        case CRC16_GENIBUS:
        case CRC16_XMODEM:
        case CRC16_CCITT_FALSE:
        case CRC16_AUG_CCITT:
        case CRC16_DECT_R:
        case CRC16_DECT_X:
        case CRC16_EN13757:
        case CRC16_BUYPASS:
        case CRC16_DDS110:
        case CRC16_T10_DIF:
        case CRC16_TELEDISK:
        case CRC16_CDMA2000:
        // CRC32
        case CRC32_Q:
        case CRC32_MPEG_2:
        case CRC32_XFER:
        case CRC32_BZIP2:
        case CRC32_POSIX:
            retVal = false;
            break;
        default:
            retVal = true;
            break;  // Rest of cases
    }

    return retVal;
}

bool CRC_getOutputReflected(crc_t crc_type) {
    bool retVal;
    switch (crc_type) {
        // CRC8
        case CRC8_CCITT:
        case CRC8_ITU:
        case CRC8_I_CODE:
        case CRC8_CDMA2000:
        case CRC8_DVB_S2:
        case CRC8_SAE_J1850_ZERO:
        case CRC8_SAE_J1850:
        case CRC8_AUTOSAR:
        case CRC8_GSM_A:
        case CRC8_GSM_B:
        case CRC8_HITAG:
        case CRC8_LTE:
        // CRC16
        case CRC16_GENIBUS:
        case CRC16_XMODEM:
        case CRC16_CCITT_FALSE:
        case CRC16_AUG_CCITT:
        case CRC16_DECT_R:
        case CRC16_DECT_X:
        case CRC16_EN13757:
        case CRC16_BUYPASS:
        case CRC16_DDS110:
        case CRC16_T10_DIF:
        case CRC16_TELEDISK:
        case CRC16_CDMA2000:
        // CRC32
        case CRC32_Q:
        case CRC32_MPEG_2:
        case CRC32_XFER:
        case CRC32_BZIP2:
        case CRC32_POSIX:
            retVal = false;
            break;
        default:
            retVal = true;
            break;  // Rest of cases
    }
    return retVal;
}

uint8_t CRC8_getFinalXOR(crc_t crc_type) {
    uint8_t retVal;
    switch (crc_type) {
        case CRC8_ITU:
            retVal = 0x55;
            break;
        case CRC8_SAE_J1850:
        case CRC8_AUTOSAR:
        case CRC8_GSM_B:
            retVal = 0xFF;
            break;
        default:
            retVal = 0x00;
            break;  // Rest of cases
    }
    return retVal;
}

uint16_t CRC16_getFinalXOR(crc_t crc_type) {
    uint16_t retVal;
    switch (crc_type) {
        case CRC16_XMODEM:
        case CRC16_AUG_CCITT:
        case CRC16_CCITT_FALSE:
        case CRC16_DECT_X:
        case CRC16_BUYPASS:
        case CRC16_DDS110:
        case CRC16_T10_DIF:
        case CRC16_TELEDISK:
        case CRC16_CDMA2000:
            retVal = 0x0000;
            break;
        case CRC16_GENIBUS:
        case CRC16_EN13757:
        case CRC16_X25:
        case CRC16_MAXIM:
        case CRC16_USB:
        case CRC16_DNP:
            retVal = 0xFFFF;
            break;
        case CRC16_DECT_R:
            retVal = 0x0001;
            break;
        default:
            retVal = 0;
            break;  // Posibles casos restantes...
    }
    return retVal;
}

uint32_t CRC32_getFinalXOR(crc_t crc_type) {
    uint32_t retVal;
    switch (crc_type) {
        case CRC32_BZIP2:
        case CRC32_POSIX:
        case CRC32_D:
        case CRC32_C:
        case CRC32_ISO:
            retVal = 0xFFFFFFFFUL;
            break;
        default:
            retVal = 0UL;
            break;  // Posibles casos restantes...
    }
    return retVal;
}

crc_error_t CRC8_Calculate(const void *data, size_t data_len, crc_t crc_type, uint8_t *result){
    // Validate input parameters
    if (data == NULL || result == NULL) {
        CRC_ERROR_SIMPLE("CRC8: NULL data pointer");
        return CRC_ERROR_NULL_DATA;
    }

    // Validate CRC type
    if (crc_type > CRC8_LTE || crc_type < CRC8_CCITT) {
        CRC_ERROR("CRC8: Invalid CRC type: %u", crc_type);
        return CRC_ERROR_INVALID_TYPE;
    }

    // Get polynomial
    uint8_t poly = CRC8_getPoly(crc_type);
    if (poly == 0) {
        CRC_ERROR_SIMPLE("CRC8: Invalid polynomial");
        return CRC_ERROR_INVALID_POLYNOMIAL;
    }
    CRC_INFO_SIMPLE("CRC8: Starting calculation");
    CRC_DEBUG("CRC8: Using polynomial: 0x%02X", poly);

    uint8_t crc = CRC8_getSeed(crc_type);
    CRC_DEBUG("CRC8: Initial seed: 0x%02X", crc);

#if defined(CRC8_USE_LOOKUP_TABLE) && (CRC8_USE_LOOKUP_TABLE == 1)
    const uint8_t *p_table = NULL;
    CRC_DEBUG_SIMPLE("CRC8: Using lookup table method");
    // Select lookup table based on polynomial
    switch (poly) {
#ifdef CRC8_0x07_LOOKUP_TABLE
        case 0x07:
            p_table = CRC8_0x07_table;
            break;
#endif
#ifdef CRC8_0x1D_LOOKUP_TABLE
        case 0x1D:
            p_table = CRC8_0x1D_table;
            break;
#endif
#ifdef CRC8_0x31_LOOKUP_TABLE
        case 0x31:
            p_table = CRC8_0x31_table;
            break;
#endif
#ifdef CRC8_0x39_LOOKUP_TABLE
        case 0x39:
            p_table = CRC8_0x39_table;
            break;
#endif
#ifdef CRC8_0x9B_LOOKUP_TABLE
        case 0x9B:
            p_table = CRC8_0x9B_table;
            break;
#endif
#ifdef CRC8_0xD5_LOOKUP_TABLE
        case 0xD5:
            p_table = CRC8_0xD5_table;
            break;
#endif
#ifdef CRC8_0x2F_LOOKUP_TABLE
        case 0x2F:
            p_table = CRC8_0x2F_table;
            break;
#endif
#ifdef CRC8_0xA7_LOOKUP_TABLE
        case 0xA7:
            p_table = CRC8_0xA7_table;
            break;
#endif
#ifdef CRC8_0x49_LOOKUP_TABLE
        case 0x49:
            p_table = CRC8_0x49_table;
            break;
#endif
        default:
            return CRC_ERROR_LOOKUP_TABLE;
    }

#if defined(CRC8_USE_LOOKUP_TABLE) && (CRC8_USE_LOOKUP_TABLE == 1)
    if (p_table == NULL) {
        CRC_ERROR_SIMPLE("CRC8: Lookup table not found");
        return CRC_ERROR_LOOKUP_TABLE;
    }
#endif
#else
    CRC_WARN("CRC8: Using direct calculation method, lookup table not used");
#endif
    if (data_len > 0) {
        uint8_t *_buf = (uint8_t *)data;
        bool input_reflected = CRC_getInputReflected(crc_type);
        bool output_reflected = CRC_getOutputReflected(crc_type);
        CRC_DEBUG("CRC8: Input reflection: %s", input_reflected ? "yes" : "no");
        CRC_DEBUG("CRC8: Output reflection: %s", output_reflected ? "yes" : "no");
        CRC_DEBUG("CRC8: Processing %zu bytes", data_len);
        while (data_len-- != 0) {
            uint8_t b = *_buf++;
            CRC_TRACE("CRC8: Processing byte: 0x%02X", b);
            b = input_reflected ? bit_invert_Byte(b) : b;
            if (input_reflected) {
                CRC_TRACE("CRC8: After input reflection: 0x%02X", b);
            }
    #if !defined(CRC8_USE_LOOKUP_TABLE) || (CRC8_USE_LOOKUP_TABLE != 1)
            crc = crc ^ b;
            CRC_TRACE("CRC8: After XOR with input: 0x%02X", crc);
            for (uint8_t i = 0; i != 8; i++){
                crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ poly) : (uint8_t)(crc << 1);
                CRC_TRACE("CRC8: Bit %d: MSB=%d, CRC=0x%02X", i, msb ? 1 : 0, crc);
            }
    #else
        uint8_t old_crc = crc;
        crc = p_table[b ^ crc];
        CRC_TRACE("CRC8: Lookup result: 0x%02X -> 0x%02X", old_crc, crc);
    #endif
        }
        if (output_reflected) {
            uint8_t pre_reflect = crc;
            crc = bit_invert_Byte(crc);
            CRC_TRACE("CRC8: After output reflection: 0x%02X -> 0x%02X", pre_reflect, crc);
        }
    } else{
        CRC_WARN("Input data length is zero");
    }

    uint8_t final_xor = CRC8_getFinalXOR(crc_type);
    CRC_DEBUG("CRC8: Final XOR value: 0x%02X", final_xor);

    *result = crc ^ final_xor;
    CRC_INFO("CRC8 result: 0x%02X", *result);

    return CRC_SUCCESS;
}

uint8_t CRC8(const void *data, size_t data_len, crc_t crc_type) {
    uint8_t result = 0;
    if (CRC8_Calculate(data, data_len, crc_type, &result) != CRC_SUCCESS) {
        return 0; // Return 0 on error
    }
    return result;
}

crc_error_t CRC16_Calculate(const void *data, size_t data_len, crc_t crc_type, uint16_t *result) {
    // Validate input parameters
    if (data == NULL || result == NULL) {
        CRC_ERROR_SIMPLE("CRC16: NULL data pointer");
        return CRC_ERROR_NULL_DATA;
    }

    // Validate CRC type
    if (crc_type > CRC16_CDMA2000 || crc_type < CRC16_XMODEM) {
        CRC_ERROR("CRC16: Invalid CRC type: %u", crc_type);
        return CRC_ERROR_INVALID_TYPE;
    }

    // Get polynomial
    uint16_t poly = CRC16_getPoly(crc_type);
    if (poly == 0) {
        CRC_ERROR_SIMPLE("CRC16: Invalid polynomial");
        return CRC_ERROR_INVALID_POLYNOMIAL;
    }
    CRC_DEBUG("CRC16: Using polynomial: 0x%04X", poly);
    CRC_INFO_SIMPLE("CRC16: Starting calculation");
    uint16_t crc = CRC16_getSeed(crc_type);
    CRC_DEBUG("CRC16: Initial seed: 0x%04X", crc);

#if defined(CRC16_USE_LOOKUP_TABLE) && (CRC16_USE_LOOKUP_TABLE == 1)
    const uint16_t *p_table = NULL;
    CRC_DEBUG_SIMPLE("CRC16: Using lookup table method");
    // Select lookup table based on polynomial
    switch (poly) {
#ifdef CRC16_0x1021_LOOKUP_TABLE
        case 0x1021:
            p_table = CRC16_0x1021_table;
            break;
#endif
#ifdef CRC16_0x8005_LOOKUP_TABLE
        case 0x8005:
            p_table = CRC16_0x8005_table;
            break;
#endif
#ifdef CRC16_0x0589_LOOKUP_TABLE
        case 0x0589:
            p_table = CRC16_0x0589_table;
            break;
#endif
#ifdef CRC16_0x3D65_LOOKUP_TABLE
        case 0x3D65:
            p_table = CRC16_0x3D65_table;
            break;
#endif
#ifdef CRC16_0x8BB7_LOOKUP_TABLE
        case 0x8BB7:
            p_table = CRC16_0x8BB7_table;
            break;
#endif
#ifdef CRC16_0xA097_LOOKUP_TABLE
        case 0xA097:
            p_table = CRC16_0xA097_table;
            break;
#endif
#ifdef CRC16_0xC867_LOOKUP_TABLE
        case 0xC867:
            p_table = CRC16_0xC867_table;
            break;
#endif
        default:
            return CRC_ERROR_LOOKUP_TABLE;
    }

#if defined(CRC16_USE_LOOKUP_TABLE) && (CRC16_USE_LOOKUP_TABLE == 1)
    if (p_table == NULL) {
        CRC_ERROR_SIMPLE("CRC16: Lookup table not found");
        return CRC_ERROR_LOOKUP_TABLE;
    }
    #else
    CRC_WARN("CRC16: Using direct calculation method, lookup table not used");
    #endif
#endif
    if (data_len > 0) {
        uint8_t *_buf = (uint8_t *)data;
        bool input_reflected = CRC_getInputReflected(crc_type);
        bool output_reflected = CRC_getOutputReflected(crc_type);
        CRC_DEBUG("CRC16: Input reflection: %s", input_reflected ? "yes" : "no");
        CRC_DEBUG("CRC16: Output reflection: %s", output_reflected ? "yes" : "no");

        CRC_DEBUG("CRC16: Processing %zu bytes", data_len);
        while (data_len-- != 0) {
            uint8_t b = *_buf++;
            CRC_TRACE("CRC16: Processing byte: 0x%02X", b);
            b = input_reflected ? bit_invert_Byte(b) : b;
            if (input_reflected) {
                CRC_TRACE("CRC16: After input reflection: 0x%02X", b);
            }
    #if !defined(CRC16_USE_LOOKUP_TABLE) || (CRC16_USE_LOOKUP_TABLE != 1)
            crc = crc ^ (uint16_t)((uint16_t)b << 8);
            CRC_TRACE("CRC16: After XOR with input: 0x%04X", crc);
            for (uint8_t i = 0; i != 8; i++) {
                crc = (crc & 0x8000) ? (crc << 1) ^ poly : (crc << 1);
                CRC_TRACE("CRC16: Bit %d: MSB=%d, CRC=0x%04X", i, msb ? 1 : 0, crc);
            }
    #else
        uint16_t old_crc = crc;
        crc = (crc << 8) ^ p_table[(crc >> 8) ^ b];
        CRC_TRACE("CRC16: Lookup result: 0x%04X -> 0x%04X", old_crc, crc);
    #endif
        }

        if (output_reflected) {
            uint16_t pre_reflect = crc;
            crc = bit_invert_Int16(crc);
            CRC_TRACE("CRC16: After output reflection: 0x%04X -> 0x%04X", pre_reflect, crc);
        }
    } else {
        CRC_WARN("Input data length is zero");
    }

    uint16_t final_xor = CRC16_getFinalXOR(crc_type);
    CRC_DEBUG("CRC16: Final XOR value: 0x%04X", final_xor);

    *result = crc ^ final_xor;
    CRC_INFO("CRC16 result: 0x%04X", *result);

    return CRC_SUCCESS;
}

uint16_t CRC16(const void *data, size_t data_len, crc_t crc_type) {
    uint16_t result = 0;
    if (CRC16_Calculate(data, data_len, crc_type, &result) != CRC_SUCCESS) {
        return 0; // Return 0 on error
    }
    return result;
}

crc_error_t CRC32_Calculate(const void *data, size_t data_len, crc_t crc_type, uint32_t *result) {
    // Validate input parameters
    if (data == NULL || result == NULL) {
        CRC_ERROR_SIMPLE("CRC32: NULL data pointer");
        return CRC_ERROR_NULL_DATA;
    }

    // Validate CRC type
    if (crc_type > CRC32_XFER || crc_type < CRC32_D) {
        CRC_ERROR("CRC32: Invalid CRC type: %u", crc_type);
        return CRC_ERROR_INVALID_TYPE;
    }

    // Get polynomial
    uint32_t poly = CRC32_getPoly(crc_type);
    if (poly == 0) {
        CRC_ERROR_SIMPLE("CRC32: Invalid polynomial");
        return CRC_ERROR_INVALID_POLYNOMIAL;
    }
    CRC_DEBUG("CRC32: Using polynomial: 0x%08lX", poly);
    CRC_INFO_SIMPLE("CRC32: Starting calculation");
    uint32_t crc = CRC32_getSeed(crc_type);

#if defined(CRC32_USE_LOOKUP_TABLE) && (CRC32_USE_LOOKUP_TABLE == 1)
    const uint32_t *p_table = NULL;
    CRC_DEBUG("CRC32: Using lookup table method");
    switch (poly) {
#ifdef CRC32_0xA833982B_LOOKUP_TABLE
        case 0xA833982BUL:
            p_table = CRC32_0xA833982B_table;
            break;
#endif
#ifdef CRC32_0x814141AB_LOOKUP_TABLE
        case 0x814141ABUL:
            p_table = CRC32_0x814141AB_table;
            break;
#endif
#ifdef CRC32_0x1EDC6F41_LOOKUP_TABLE
        case 0x1EDC6F41UL:
            p_table = CRC32_0x1EDC6F41_table;
            break;
#endif
#ifdef CRC32_0x04C11DB7_LOOKUP_TABLE
        case 0x04C11DB7UL:
            p_table = CRC32_0x04C11DB7_table;
            break;
#endif
#ifdef CRC32_0x000000AF_LOOKUP_TABLE
        case 0x000000AFUL:
            p_table = CRC32_0x000000AF_table;
            break;
#endif
        default:
            return CRC_ERROR_LOOKUP_TABLE;
    }

#if defined(CRC32_USE_LOOKUP_TABLE) && (CRC32_USE_LOOKUP_TABLE == 1)
    if (p_table == NULL) {
        CRC_ERROR_SIMPLE("CRC32: Lookup table not found");
        return CRC_ERROR_LOOKUP_TABLE;
    }
#endif
#else
    CRC_WARN("CRC32: Using direct calculation method, lookup table not used");
#endif
    if (data_len > 0) {
        uint8_t *_buf = (uint8_t *)data;
        bool input_reflected = CRC_getInputReflected(crc_type);
        bool output_reflected = CRC_getOutputReflected(crc_type);
        CRC_DEBUG("CRC32: Input reflection: %s", input_reflected ? "yes" : "no");
        CRC_DEBUG("CRC32: Output reflection: %s", output_reflected ? "yes" : "no");
        CRC_DEBUG("CRC32: Processing %zu bytes", data_len);
        while (data_len-- != 0) {
            uint8_t b = *_buf++;
            CRC_TRACE("CRC32: Processing byte: 0x%02X", b);
            b = input_reflected ? bit_invert_Byte(b) : b;
    #if !defined(CRC32_USE_LOOKUP_TABLE) || (CRC32_USE_LOOKUP_TABLE != 1)
            crc = crc ^ (((uint32_t)(b)) << 24);
            CRC_TRACE("CRC32: After XOR with input: 0x%08lX", crc);
            for (uint8_t i = 0; i != 8; i++) {
                crc = (crc & 0x80000000UL) ? (crc << 1) ^ poly : (crc << 1);
                CRC_TRACE("CRC32: Bit %d: MSB=%d, CRC=0x%08lX", i, msb ? 1 : 0, crc);
            }
    #else
            uint32_t old_crc = crc;
            crc = (uint32_t)((crc << 8) ^ p_table[(uint8_t)((crc ^ ((uint32_t)(b) << 24)) >> 24)]);
            CRC_TRACE("CRC32: Lookup result: 0x%08lX -> 0x%08lX", old_crc, crc);
    #endif
        }

        if (output_reflected) {
            uint32_t pre_reflect = crc;
            crc = output_reflected ? bit_invert_Int32(crc) : crc;
            CRC_TRACE("CRC32: After output reflection: 0x%08lX -> 0x%08lX", pre_reflect, crc);
        }
    } else {
        CRC_WARN("Input data length is zero");
    }

    uint32_t final_xor = CRC32_getFinalXOR(crc_type);
    CRC_DEBUG("CRC32: Final XOR value: 0x%08lX", final_xor);

    *result = crc ^ final_xor;
    CRC_INFO("CRC32 result: 0x%08lX", *result);

    return CRC_SUCCESS;
}

uint32_t CRC32(const void *data, size_t data_len, crc_t crc_type) {
    uint32_t result = 0;
    if (CRC32_Calculate(data, data_len, crc_type, &result) != CRC_SUCCESS) {
        return 0; // Return 0 on error
    }
    return result;
}

crc_error_t CRC8_ValidateAppended(const void *data, size_t data_len, crc_t crc_type) {

    // Calculate CRC of data excluding the appended CRC byte
    uint8_t calculated_crc;
    crc_error_t retVal = CRC8_Calculate(data, data_len - sizeof(uint8_t), crc_type, &calculated_crc);
    if (retVal != CRC_SUCCESS) {
        return retVal;
    }

    // Compare with appended CRC
    const uint8_t *data_bytes = (const uint8_t *)data;
    return (calculated_crc == data_bytes[data_len - sizeof(uint8_t)])? CRC_SUCCESS : CRC_ERROR_CRC_MISMATCH;
}

crc_error_t CRC16_ValidateAppended(const void *data, size_t data_len, crc_t crc_type) {
\
    // Calculate CRC of data excluding the appended CRC bytes
    uint16_t calculated_crc;
    crc_error_t retVal = CRC16_Calculate(data, data_len - sizeof(uint16_t), crc_type, &calculated_crc);
    if (retVal != CRC_SUCCESS) {
        return retVal;
    }

    // Compare with appended CRC
    const uint8_t *data_bytes = (const uint8_t *)data;
    uint16_t appended_crc = ((uint16_t)data_bytes[data_len - sizeof(uint16_t)] << 8) |
                            data_bytes[data_len - (sizeof(uint16_t) - 1)];

    return (calculated_crc == appended_crc)? CRC_SUCCESS : CRC_ERROR_CRC_MISMATCH;
}

crc_error_t CRC32_ValidateAppended(const void *data, size_t data_len, crc_t crc_type) {

    // Calculate CRC of data excluding the appended CRC bytes
    uint32_t calculated_crc;
    crc_error_t retVal = CRC32_Calculate(data, data_len - sizeof(uint32_t), crc_type, &calculated_crc);
    if (retVal != CRC_SUCCESS) {
        return retVal;
    }

    // Compare with appended CRC
    const uint8_t *data_bytes = (const uint8_t *)data;
    uint32_t appended_crc = ((uint32_t)data_bytes[data_len - (sizeof(uint32_t))] << 24) |
                           ((uint32_t)data_bytes[data_len - (sizeof(uint32_t)-1)] << 16) |
                           ((uint32_t)data_bytes[data_len - (sizeof(uint32_t)-2)] << 8) |
                           data_bytes[data_len - (sizeof(uint32_t)-3)];

    return (calculated_crc == appended_crc)? CRC_SUCCESS : CRC_ERROR_CRC_MISMATCH;
}
