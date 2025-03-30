/**
 * @file AES128.h
 * @brief AES-128 encryption and decryption interface
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef AES128_H
#define AES128_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AES_common.h"
#include "PKCS7.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Macro for defining the maximum buffer size, for cases where dynamic
 * memory allocation is not required. For example: microcontrollers.
 */
#define AES128_MAX_BUFFER_SIZE 256

/**
 * @brief Macro for fixed key size in bytes for AES-128. MUST NOT be modified.
 */
#define AES128_FIXED_KEY_SIZE 16

/**
 * @brief Macro for expanded key size for AES-128.
 */
#define AES128_KEY_EXP_SIZE 176

/**
 * @brief The number of rounds in AES-128 Cipher.
 */
#define AES128_NUM_ROUNDS 10

/**
 * @brief AES-128 context structure.
 */
typedef struct AES128_ctx {
    union {
        uint8_t array[AES128_FIXED_KEY_SIZE];
        uint32_t words[AES128_FIXED_KEY_SIZE / 4];
    } key;                      // Key with both byte and word access
    uint8_t iv[AES_BLOCK_LEN];  // Initialization vector for CBC and CTR modes
    size_t input_len_normalized;
    uint32_t encrypted_chunks;
    uint32_t decrypted_chunks;
} AES128_ctx_t;

/**
 * @brief Initialize AES-128 context with key and IV.
 *
 * @param ctx AES context to initialize
 * @param key Pointer to the key (16 bytes)
 * @param iv Pointer to the initialization vector (16 bytes)
 */
void AES128_init_ctx(AES128_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);

/**
 * @brief AES-128 ECB mode encryption with optional PKCS7 padding.
 *
 * @param ctx AES context containing key and state information
 * @param in Input data buffer
 * @param out Output data buffer
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param use_padding Boolean flag to enable/disable PKCS7 padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES128_ECB_encrypt(AES128_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding);

/**
 * @brief AES-128 ECB mode decryption with optional PKCS7 padding removal.
 *
 * @param ctx AES context containing key and state information
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param use_padding Boolean flag to enable/disable PKCS7 padding removal
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES128_ECB_decrypt(AES128_ctx_t *ctx, void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding);

/**
 * @brief AES-128 CBC mode encryption with optional PKCS7 padding.
 *
 * @param ctx AES context containing key, IV, and state information
 * @param in Input data buffer
 * @param out Output data buffer
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param use_padding Boolean flag to enable/disable PKCS7 padding
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES128_CBC_encrypt(AES128_ctx_t *ctx, const void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding);

/**
 * @brief AES-128 CBC mode decryption with optional PKCS7 padding removal.
 *
 * @param ctx AES context containing key, IV, and state information
 * @param in Input data buffer (ciphertext)
 * @param out Output data buffer (plaintext)
 * @param input_len Length of input data in bytes
 * @param output_len Pointer to store the length of output data
 * @param use_padding Boolean flag to enable/disable PKCS7 padding removal
 * @return AES_errcode_t Error code (AES_CODE_OK on success)
 */
AES_errcode_t AES128_CBC_decrypt(AES128_ctx_t *ctx, void *in, void *out,
                                 size_t input_len, uint32_t *output_len,
                                 bool use_padding);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * @brief AES128 class for embedded systems.
 */
class AES128 {
   public:
    /**
     * @brief Constructor to initialize the AES128 context with a key and IV.
     * @param key Pointer to the key (16 bytes)
     * @param iv Pointer to the initialization vector (16 bytes)
     */
    AES128(const uint8_t *key, const uint8_t *iv);

    /**
     * @brief Encrypt data using AES-128 in ECB mode.
     * @param input Pointer to input data buffer
     * @param input_len Length of input data in bytes
     * @param output Pointer to output data buffer
     * @param output_len Pointer to store the length of output data
     * @param use_padding Enable or disable PKCS7 padding
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t encryptECB(const uint8_t *input, size_t input_len,
                             uint8_t *output, uint32_t *output_len,
                             bool use_padding = true);

    /**
     * @brief Decrypt data using AES-128 in ECB mode.
     * @param input Pointer to input data buffer
     * @param input_len Length of input data in bytes
     * @param output Pointer to output data buffer
     * @param output_len Pointer to store the length of output data
     * @param use_padding Enable or disable PKCS7 padding removal
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t decryptECB(const uint8_t *input, size_t input_len,
                             uint8_t *output, uint32_t *output_len,
                             bool use_padding = true);

    /**
     * @brief Get the last error code.
     * @return AES_errcode_t Last error code.
     */
    AES_errcode_t encryptCBC(const uint8_t *input, size_t input_len,
                             uint8_t *output, uint32_t *output_len,
                             bool use_padding);
    AES_errcode_t decryptCBC(const uint8_t *input, size_t input_len,
                             uint8_t *output, uint32_t *output_len,
                             bool use_padding);
    AES_errcode_t getErrorCode() const;

   private:
    AES128_ctx_t ctx_;          // Underlying AES-128 context
    AES_errcode_t error_code_;  // Last error code
};

#endif /* __cplusplus */

#endif /* AES128_H */
