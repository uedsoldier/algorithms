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

#pragma region Dependencies
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "AES_common.h"
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Useful macros

/**
 * @brief Macro para depuración mediante la función printf(). Se recomienda usar únicamente
 * para fines de desarrollo y pruebas.
 */
#ifndef AES128_LOG
#define AES128_LOG 1
#endif

#if defined(AES128_LOG) && AES128_LOG == 1
static const char *AES128_TAG = "AES128";
#endif

#pragma endregion

#pragma region Memory macros

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#define AES128_MAX_BUFFER_SIZE 64

/**
 * @brief Use PKCS7 standard syntax for storing signed and/or encrypted data
 * 
 */
#define AES128_USE_PKCS7   1

#if defined(AES128_USE_PKCS7) && AES128_USE_PKCS7 == 1
#include "../PKCS7/PKCS7.h"    // Modify path to convenience 
#endif

#pragma endregion

#pragma region General constants

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

#pragma region Custom types

    /**
     * @brief
     *
     */
    typedef struct AES128_key
    {
        uint8_t array[AES128_FIXED_KEY_SIZE];
    } AES128_key_t;


    /**
     * @brief
     *
     */
    typedef struct AES128_ctx
    {
        AES128_key_t key;    // Key
        uint8_t iv[AES_BLOCK_LEN];          // Initialization vector for CBC and CTR modes
        size_t encrypted_chunks;
        size_t decrypted_chunks;
        size_t input_len_normalized;

    } AES128_ctx_t;

#pragma endregion


    static void AES128_encrypt_chunk(AES128_ctx_t *ctx, uint8_t *in,uint8_t *out);
    static void AES128_decrypt_chunk(AES128_ctx_t *ctx, uint8_t *in,uint8_t *out);

    void AES128_init_ctx(AES128_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);
    
    AES_errcode_t AES128_ECB_encrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES128_ECB_decrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

    AES_errcode_t AES128_CBC_encrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES128_CBC_decrypt(AES128_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

#pragma endregion

#pragma region Auxiliary functions
static void KeyExpansion_AES128(uint8_t *inputKey, uint8_t *expandedKeys);
#pragma endregion

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*AES128_H*/