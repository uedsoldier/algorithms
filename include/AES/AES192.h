/**
 * @file AES192.h
 * @brief AES-192 encryption and decryption implementation with ECB and CBC
 * modes
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef AES192_H
#define AES192_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "AES_common.h"
#include "PKCS7.h"

/**
 * @brief Fixed key size in bytes for AES-192
 *
 * This value MUST NOT be modified as it's part of the AES-192 specification.
 */
#define AES192_FIXED_KEY_SIZE 24

/**
 * @brief Number of rounds in AES-192 cipher
 *
 * Standard number of rounds for AES-192 encryption/decryption operations.
 */
#define AES192_NUM_ROUNDS 12

/**
 * @brief Size of the expanded key used in AES-192
 *
 * This value represents the total size of the key schedule after expansion.
 */
#define AES192_KEY_EXP_SIZE 208

/**
 * @brief Maximum buffer size for operations
 *
 * Defines the maximum allowed buffer size for encryption/decryption operations.
 * This limit helps prevent buffer overflow in memory-constrained environments.
 */
#define AES192_MAX_BUFFER_SIZE 1024

/**
 * @brief AES-192 context structure containing key and state information
 *
 * This structure holds all necessary data for AES-192 operations including
 * the encryption key, initialization vector, and operation counters.
 */
typedef struct {
    union {
        uint8_t array[AES192_FIXED_KEY_SIZE];      /* Byte-level key access */
        uint32_t words[AES192_FIXED_KEY_SIZE / 4]; /* Word-level key access */
    } key;
    uint8_t iv[AES_BLOCK_LEN];   /* Initialization vector for CBC mode */
    size_t input_len_normalized; /* Normalized input length */
    uint32_t encrypted_chunks;   /* Count of encrypted blocks */
    uint32_t decrypted_chunks;   /* Count of decrypted blocks */
} AES192_ctx_t;

/**
 * @brief Initialize AES-192 context with key and optional IV
 *
 * @param ctx Pointer to AES-192 context structure
 * @param key 24-byte (192-bit) encryption key
 * @param iv 16-byte initialization vector (required for CBC mode, can be NULL
 * for ECB)
 */
void AES192_init_ctx(AES192_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);

/**
 * @brief Initialize AES-192 context for ECB mode (no IV needed)
 *
 * @param ctx Pointer to AES-192 context structure
 * @param key 24-byte (192-bit) encryption key
 */
void AES192_init_ctx_ecb(AES192_ctx_t *ctx, const uint8_t *key);

/**
 * @brief Encrypt data using AES-192 in ECB mode
 *
 * @param ctx AES-192 context with initialized key
 * @param in Input data buffer (plaintext)
 * @param out Output data buffer (must be at least as large as padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_encrypt(AES192_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

/**
 * @brief Decrypt data using AES-192 in ECB mode
 *
 * @param ctx AES-192 context with initialized key
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding removal, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_ECB_decrypt(AES192_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

/**
 * @brief Encrypt data using AES-192 in CBC mode
 *
 * @param ctx AES-192 context with initialized key and IV
 * @param in Input data buffer (plaintext)
 * @param out Output data buffer (must be at least as large as padded input)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_encrypt(AES192_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

/**
 * @brief Decrypt data using AES-192 in CBC mode
 *
 * @param ctx AES-192 context with initialized key and IV
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param usePKCS7 True for PKCS7 padding removal, false for zero padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES192_CBC_decrypt(AES192_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, size_t *output_len,
                                 bool usePKCS7);

#endif /* AES192_H */
