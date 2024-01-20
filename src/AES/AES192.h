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
#ifndef AES192_LOG
#define AES192_LOG 1
#endif

#if defined(AES192_LOG) && AES192_LOG == 1
static const char *AES192_TAG = "AES192";
#endif

#pragma endregion

#pragma region Memory macros
/**
 * @brief Macro para utilización de asignación dinámica de memoria en las funciones AES.
 * Usar esta característica permite ahorrar memoria RAM, pero no todos los dispositivos ni todos
 * los compiladores soportan tales funcionalidades (malloc(), calloc(), realloc(), etc.)
 */
#ifndef AES192_DYNAMIC_MEMORY
#define AES192_DYNAMIC_MEMORY 0
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria. Por ejemplo: microcontroladores.
 *
 */
#define AES192_MAX_BUFFER_SIZE 64

/**
 * @brief Use PKCS7 standard syntax for storing signed and/or encrypted data
 * 
 */
#define AES192_USE_PKCS7   1

#if defined(AES192_USE_PKCS7) && AES192_USE_PKCS7 == 1
#include "../PKCS7/PKCS7.h"    // Modify path to convenience 
#endif

#pragma endregion

#pragma region General constants

/**
 * @brief Macro de tamaño de clave fija en bytes para AES-128. NO debe modificarse.
 *
 */
#define AES192_FIXED_KEY_SIZE 24

/**
 * @brief
 *
 */
#define AES192_KEY_EXP_SIZE 208

/**
 * @brief The number of rounds in AES-128 Cipher
 * 
*/
#define AES192_NUM_ROUNDS 12 

#pragma region Custom types

    /**
     * @brief
     *
     */
    typedef struct AES192_key
    {
        uint8_t array[AES192_FIXED_KEY_SIZE];
    } AES192_key_t;


    /**
     * @brief
     *
     */
    typedef struct AES192_ctx
    {
        AES192_key_t key;    // Key
        uint8_t iv[AES_BLOCK_LEN];          // Initialization vector for CBC and CTR modes
        size_t encrypted_chunks;
        size_t decrypted_chunks;
        size_t input_len_normalized;

    } AES192_ctx_t;

#pragma endregion


    static void AES192_encrypt_chunk(AES192_ctx_t *ctx, uint8_t *in,uint8_t *out);
    static void AES192_decrypt_chunk(AES192_ctx_t *ctx, uint8_t *in,uint8_t *out);

    void AES192_init_ctx(AES192_ctx_t *ctx, const uint8_t *key, const uint8_t *iv);
    
    AES_errcode_t AES192_ECB_encrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES192_ECB_decrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

    AES_errcode_t AES192_CBC_encrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES192_CBC_decrypt(AES192_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

#pragma endregion

#pragma region Auxiliary functions
static void KeyExpansion_AES192(uint8_t *inputKey, uint8_t *expandedKeys);

#pragma endregion

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*AES192_H*/