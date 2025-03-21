/**
 * @file AES256.h
 * @brief AES-256 encryption and decryption implementation with ECB and CBC
 * modes
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef AES256_H
#define AES256_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "AES_common.h"
#include "PKCS7.h"

/**
 * @brief Fixed key size in bytes for AES-256
 *
 * This value MUST NOT be modified as it's part of the AES-256 specification.
 * AES-256 provides the highest security level among AES variants.
 */
#define AES256_FIXED_KEY_SIZE 32

/**
 * @brief Number of rounds in AES-256 cipher
 *
 * Standard number of rounds for AES-256 encryption/decryption operations.
 * Higher than AES-128 and AES-192 for increased security.
 */
#define AES256_NUM_ROUNDS 14

/**
 * @brief Size of the expanded key used in AES-256
 *
 * This value represents the total size of the key schedule after expansion.
 */
#define AES256_KEY_EXP_SIZE 240

/**
 * @brief Maximum buffer size for operations
 *
 * Defines the maximum allowed buffer size for encryption/decryption operations.
 * This limit helps prevent buffer overflow in memory-constrained environments.
 */
#define AES256_MAX_BUFFER_SIZE 1024

/**
 * @brief AES-256 context structure containing key and state information
 *
 * This structure holds all necessary data for AES-256 operations including
 * the encryption key, initialization vector, and operation counters.
 */
typedef struct {
    union {
        uint8_t array[AES256_FIXED_KEY_SIZE];      /* Byte-level key access */
        uint32_t words[AES256_FIXED_KEY_SIZE / 4]; /* Word-level key access */
    } key;
    uint8_t iv[AES_BLOCK_LEN];   /* Initialization vector for CBC mode */
    size_t input_len_normalized; /* Normalized input length */
    uint32_t encrypted_chunks;   /* Count of encrypted blocks */
    uint32_t decrypted_chunks;   /* Count of decrypted blocks */
} AES256_ctx_t;

/**
 * @brief Initialize AES-256 context with key and optional IV
 *
 * @param ctx Pointer to AES-256 context structure
 * @param key 32-byte (256-bit) encryption key
 * @param iv 16-byte initialization vector (required for CBC mode, can be NULL
 * for ECB)
 */
void AES256_init_ctx(AES256_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);

/**
 * @brief Initialize AES-256 context for ECB mode (no IV needed)
 *
 * @param ctx Pointer to AES-256 context structure
 * @param key 32-byte (256-bit) encryption key
 */
void AES256_init_ctx_ecb(AES256_ctx_t *ctx, const uint8_t *key);

/**
 * @brief Encrypt data using AES-256 in ECB mode
 *
 * Note: ECB mode is not recommended for most use cases as it doesn't provide
 * sufficient security for multiple blocks.
 *
 * @param ctx AES-256 context with initialized key
 * @param in Input data buffer (plaintext)
 * @param out Output data buffer (must be at least as large as padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES256_ECB_encrypt(AES256_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

/**
 * @brief Decrypt data using AES-256 in ECB mode
 *
 * @param ctx AES-256 context with initialized key
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding removal, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES256_ECB_decrypt(AES256_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

/**
 * @brief Encrypt data using AES-256 in CBC mode
 *
 * CBC mode provides better security than ECB by chaining blocks together.
 * A unique IV should be used for each encryption operation.
 *
 * @param ctx AES-256 context with initialized key and IV
 * @param in Input data buffer (plaintext)
 * @param out Output data buffer (must be at least as large as padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES256_CBC_encrypt(AES256_ctx_t *ctx, const uint8_t *in,
                                 uint8_t *out, size_t input_len,
                                 size_t *output_len, bool usePKCS7);

/**
 * @brief Decrypt data using AES-256 in CBC mode
 *
 * @param ctx AES-256 context with initialized key and IV
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding removal, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES256_CBC_decrypt(AES256_ctx_t *ctx, const uint8_t *in,
                                 uint8_t *out, size_t input_len,
                                 size_t *output_len, bool usePKCS7);

#endif /* AES256_H */
