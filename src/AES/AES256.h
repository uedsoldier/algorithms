/**
 * @file AES256.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef AES256_H
#define AES256_H

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
#ifndef AES256_LOG
#define AES256_LOG 1
#endif

#if defined(AES256_LOG) && AES256_LOG == 1
static const char *AES256_TAG = "AES256";
#endif

#pragma endregion

#pragma region Memory macros
/**
 * @brief Macro para utilización de asignación dinámica de memoria en las funciones AES.
 * Usar esta característica permite ahorrar memoria RAM, pero no todos los dispositivos ni todos
 * los compiladores soportan tales funcionalidades (malloc(), calloc(), realloc(), etc.)
 */
#ifndef AES256_DYNAMIC_MEMORY
#define AES256_DYNAMIC_MEMORY 0
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#define AES256_MAX_BUFFER_SIZE 64

/**
 * @brief Use PKCS7 standard syntax for storing signed and/or encrypted data
 * 
 */
#define AES256_USE_PKCS7   1

#if defined(AES256_USE_PKCS7) && AES256_USE_PKCS7 == 1
#include "../PKCS7/PKCS7.h"    // Modify path to convenience 
#endif

#pragma endregion

#pragma region General constants

/**
 * @brief Macro de tamaño de clave fija en bytes para AES-128. NO debe modificarse.
 *
 */
#define AES256_FIXED_KEY_SIZE 32

/**
 * @brief
 *
 */
#define AES256_KEY_EXP_SIZE 240

/**
 * @brief The number of rounds in AES-128 Cipher
 * 
*/
#define AES256_NUM_ROUNDS 14 

#pragma region Custom types

    /**
     * @brief
     *
     */
    typedef struct AES256_key
    {
        uint8_t array[AES256_FIXED_KEY_SIZE];
    } AES256_key_t;


    /**
     * @brief
     *
     */
    typedef struct AES256_ctx
    {
        AES256_key_t key;    // Key
        uint8_t iv[AES_BLOCK_LEN];          // Initialization vector for CBC and CTR modes
        size_t encrypted_chunks;
        size_t decrypted_chunks;
        size_t input_len_normalized;

    } AES256_ctx_t;

#pragma endregion


    static void AES256_encrypt_chunk(AES256_ctx_t *ctx, uint8_t *in,uint8_t *out);
    static void AES256_decrypt_chunk(AES256_ctx_t *ctx, uint8_t *in,uint8_t *out);

    void AES256_init_ctx(AES256_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);
    
    AES_errcode_t AES256_ECB_encrypt(AES256_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES256_ECB_decrypt(AES256_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

    AES_errcode_t AES256_CBC_encrypt(AES256_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES256_CBC_decrypt(AES256_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

#pragma endregion

#pragma region Auxiliary functions
static void KeyExpansion_AES256(uint8_t *inputKey, uint8_t *expandedKeys);

#pragma endregion

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*AES256_H*/