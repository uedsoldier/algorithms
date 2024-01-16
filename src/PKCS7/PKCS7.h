/**
 * @file PKCS7.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PKCS7_H
#define PKCS7_H

#pragma region Dependencies
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 
 * 
 */
#define PKCS7_MAX_BLOCK_SIZE_BYTES  128

/**
 * @brief 
 * 
 */
#define PKCS7_LOG   0


#pragma region Custom types

typedef struct PKCS7_padding{
    uint8_t data_with_padding[PKCS7_MAX_BLOCK_SIZE_BYTES];
    size_t data_len_with_padding;
    uint8_t padding_byte_value;
} PKCS7_padding_t;

typedef struct  PKCS7_unpadding{
    uint8_t data_without_padding[PKCS7_MAX_BLOCK_SIZE_BYTES];
    size_t data_len_without_padding;
    uint8_t padding_byte_to_remove;
} PKCS7_unpadding_t;

typedef enum {
    PKCS7_BLOCK_SIZE_128_BIT = 128/8,     // 16-bytes
    PKCS7_BLOCK_SIZE_192_BIT = 192/8,     // 24-bytes
    PKCS7_BLOCK_SIZE_256_BIT = 256/8,     // 32-bytes
    
} PKCS7_paddingBlockSize;

typedef enum {
    PKCS7_OK = 0,
    PKCS7_INVALID_BLOCK_SIZE,

} PKCS7_error_t;

#pragma endregion


#pragma region Function prototypes
PKCS7_error_t PKCS7_add_padding(PKCS7_padding_t *pkcs7, const void *data, const size_t data_len, const size_t block_size);
PKCS7_error_t PKCS7_remove_padding(PKCS7_unpadding_t *pkcs7, const void *data, const size_t data_len);
#pragma endregion




#ifdef __cplusplus
}
#endif

#endif /*PKCS7_H*/