/**
 * @file AES128.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef AES128_H
#define AES128_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "AES_common.h"
#include "PKCS7.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#define AES128_MAX_BUFFER_SIZE 256

/**
 * @brief Macro de tamaño de clave fija en bytes para AES-128. NO debe modificarse.
 *
 */
#define AES128_FIXED_KEY_SIZE 16

/**
 * @brief
 *
 */
#define AES128_KEY_EXP_SIZE 176

/**
 * @brief The number of rounds in AES-128 Cipher
 *
 */
#define AES128_NUM_ROUNDS 10

    /**
     * @brief
     *
     */
    /**
     * @brief AES-128 context structure
     *
     */
    typedef struct AES128_ctx
    {
        union
        {
            uint8_t array[AES128_FIXED_KEY_SIZE];
            uint32_t words[AES128_FIXED_KEY_SIZE / 4];
        } key;                     // Key with both byte and word access
        uint8_t iv[AES_BLOCK_LEN]; // Initialization vector for CBC and CTR modes
        size_t input_len_normalized;
        uint32_t encrypted_chunks;
        uint32_t decrypted_chunks;
    } AES128_ctx_t;


    void AES128_init_ctx(AES128_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);

    /**
     * @brief AES-128 ECB mode encryption with optional PKCS7 padding
     * 
     * @param ctx AES context containing key and state information
     * @param in Input data buffer
     * @param out Output data buffer
     * @param input_len Length of input data in bytes
     * @param output_len Pointer to store the length of output data
     * @param use_padding Boolean flag to enable/disable PKCS7 padding
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t AES128_ECB_encrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len, bool use_padding);

    /**
     * @brief AES-128 ECB mode decryption with optional PKCS7 padding removal
     * 
     * @param ctx AES context containing key and state information
     * @param in Input data buffer (ciphertext)
     * @param out Output data buffer (plaintext)
     * @param input_len Length of input data in bytes
     * @param output_len Pointer to store the length of output data
     * @param use_padding Boolean flag to enable/disable PKCS7 padding removal
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t AES128_ECB_decrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len, bool use_padding);

    /**
     * @brief AES-128 CBC mode encryption with optional PKCS7 padding
     * 
     * @param ctx AES context containing key, IV and state information
     * @param in Input data buffer
     * @param out Output data buffer
     * @param input_len Length of input data in bytes
     * @param output_len Pointer to store the length of output data
     * @param use_padding Boolean flag to enable/disable PKCS7 padding
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t AES128_CBC_encrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len, bool use_padding);

    /**
     * @brief AES-128 CBC mode decryption with optional PKCS7 padding removal
     * 
     * @param ctx AES context containing key, IV and state information
     * @param in Input data buffer (ciphertext)
     * @param out Output data buffer (plaintext)
     * @param input_len Length of input data in bytes
     * @param output_len Pointer to store the length of output data
     * @param use_padding Boolean flag to enable/disable PKCS7 padding removal
     * @return AES_errcode_t Error code (AES_CODE_OK on success)
     */
    AES_errcode_t AES128_CBC_decrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len, bool use_padding);

#ifdef __cplusplus
}
#endif

#endif /*AES128_H*/
