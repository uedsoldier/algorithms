/**
 * @file AES192.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef AES192_H
#define AES192_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "AES_common.h"
#include "PKCS7.h"

#define AES192_FIXED_KEY_SIZE 24
#define AES192_NUM_ROUNDS 12
#define AES192_KEY_EXP_SIZE 208
#define AES192_MAX_BUFFER_SIZE 1024

typedef struct
{
    union
    {
        uint8_t array[AES192_FIXED_KEY_SIZE];
        uint32_t words[AES192_FIXED_KEY_SIZE / 4];
    } key;
    uint8_t iv[AES_BLOCK_LEN];
    size_t input_len_normalized;
    uint32_t encrypted_chunks;
    uint32_t decrypted_chunks;
} AES192_ctx_t;

/**
 * @brief Initialize AES-192 context with key and optional IV
 *
 * @param ctx AES192 context structure
 * @param key 24-byte encryption key
 * @param iv 16-byte initialization vector (can be NULL for ECB mode)
 */
void AES192_init_ctx(AES192_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);

/**
 * @brief Initialize AES-192 context for ECB mode (no IV needed)
 *
 * @param ctx AES192 context structure
 * @param key 24-byte encryption key
 */
void AES192_init_ctx_ecb(AES192_ctx_t *ctx, const uint8_t *key);

/**
 * @brief Encrypt data using AES-192 in ECB mode
 *
 * @param ctx AES192 context with initialized key
 * @param in Input data buffer
 * @param out Output data buffer (must be at least as large as the padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_encrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
                                 size_t input_len, size_t *output_len, bool usePKCS7);

/**
 * @brief Decrypt data using AES-192 in ECB mode
 *
 * @param ctx AES192 context with initialized key
 * @param in Input data buffer (encrypted)
 * @param out Output data buffer (decrypted)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_decrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
                                 size_t input_len, size_t *output_len, bool usePKCS7);

/**
 * @brief Encrypt data using AES-192 in CBC mode
 *
 * @param ctx AES192 context with initialized key and IV
 * @param in Input data buffer
 * @param out Output data buffer (must be at least as large as the padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_encrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
                                 size_t input_len, size_t *output_len, bool usePKCS7);

/**
 * @brief Decrypt data using AES-192 in CBC mode
 *
 * @param ctx AES192 context with initialized key and IV
 * @param in Input data buffer (encrypted)
 * @param out Output data buffer (decrypted)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 Whether to use PKCS7 padding (true) or zero padding (false)
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_decrypt(AES192_ctx_t *ctx, const uint8_t *in, uint8_t *out,
                                 size_t input_len, size_t *output_len, bool usePKCS7);


#endif /* AES192_H */