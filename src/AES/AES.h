/**
 * @file AES.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef AES_H
#define AES_H

#pragma region Dependencies
#include <stdint.h>
#include <stdbool.h>
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Useful macros

#pragma region General constants
/**
 * @brief Macro de tamaño en bytes de bloque AES. NO debe modificarse.
 *
 */
#define AES_BLOCK_SIZE 16
#pragma endregion

#pragma region AES-128 specific constants
/**
 * @brief Macro de tamaño de clave fija en bytes para AES-128. NO debe modificarse.
 *
 */
#define AES128_FIXED_KEY_SIZE 16

/**
 * @brief Macro de tamaño de clave fija en words para AES-128. NO debe modificarse.
 *
 */
#define AES128_FIXED_KEY_SIZE_WORDS 4

/**
 * @brief
 *
 */
#define AES128_KEY_EXP_SIZE 176

#pragma endregion

#pragma region AES-192 specific constants
/**
 * @brief Macro de tamaño de clave fija en bytes para AES-192. NO debe modificarse.
 *
 */
#define AES192_FIXED_KEY_SIZE 24

/**
 * @brief Macro de tamaño de clave fija en words para AES-192. NO debe modificarse.
 *
 */
#define AES192_FIXED_KEY_SIZE_WORDS 6

/**
 * @brief
 *
 */
#define AES192_KEY_EXP_SIZE 208
#pragma endregion

#pragma region AES-256 specific constants
/**
 * @brief Macro de tamaño de clave fija en bytes para AES-256. NO debe modificarse.
 *
 */
#define AES256_FIXED_KEY_SIZE 32

/**
 * @brief Macro de tamaño de clave fija en words para AES-256. NO debe modificarse.
 *
 */
#define AES256_FIXED_KEY_SIZE_WORDS 8

/**
 * @brief
 *
 */
#define AES256_KEY_EXP_SIZE 240
#pragma endregion

#pragma endregion

#pragma region Custom types

    /**
     * @brief
     *
     */
    enum AES_blockMode
    {
        CBC = 0,
        ECB,
        CTR
    };

    /**
     * @brief
     *
     */
    typedef enum AES_standard
    {
        AES_128,
        AES_192,
        AES_256
    } AES_standard_t;

    /**
     * @brief
     *
     */
    typedef struct AES128_key
    {
        union
        {
            uint32_t key_words[AES128_FIXED_KEY_SIZE_WORDS];
            uint8_t key_bytes[AES128_FIXED_KEY_SIZE];
        };
    } AES128_key_t;

    /**
     * @brief
     *
     */
    typedef struct AES192_key
    {
        union
        {
            uint32_t key_words[AES192_FIXED_KEY_SIZE_WORDS];
            uint8_t key_bytes[AES192_FIXED_KEY_SIZE];
        };
    } AES192_key_t;

    /**
     * @brief
     *
     */
    typedef struct AES256_key
    {
        union
        {
            uint32_t key_words[AES256_FIXED_KEY_SIZE_WORDS];
            uint8_t key_bytes[AES256_FIXED_KEY_SIZE];
        };
    } AES256_key_t;

    /**
     * @brief Estructura de datos para vector de inicialización
     * 
     */
    typedef struct AES_iv {
        uint8_t iv_array[AES_BLOCK_SIZE];
    } AES_iv_t;

    /**
     * @brief 
     * 
     */
    typedef struct AES128_ctx
    {
        uint8_t roundKey[AES128_KEY_EXP_SIZE];
        uint8_t iv[AES_BLOCK_SIZE];                 // Initialization vector for CBC and CTR modes

    } AES128_ctx_t;

    /**
     * @brief 
     * 
     */
    typedef struct AES192_ctx
    {
        uint8_t roundKey[AES192_KEY_EXP_SIZE];
        uint8_t iv[AES_BLOCK_SIZE];                 // Initialization vector for CBC and CTR modes

    } AES192_ctx_t;

    /**
     * @brief 
     * 
     */
    typedef struct AES256_ctx
    {
        uint8_t roundKey[AES256_KEY_EXP_SIZE];
        uint8_t iv[AES_BLOCK_SIZE];                 // Initialization vector for CBC and CTR modes

    } AES256_ctx_t;

#pragma endregion

#pragma region Function prototypes
void AES128_init_ctx(AES128_ctx_t *ctx, const AES128_key_t *key, const AES_iv_t *iv);
void AES192_init_ctx(AES192_ctx_t *ctx, const AES192_key_t *key, const AES_iv_t *iv);
void AES256_init_ctx(AES256_ctx_t *ctx, const AES256_key_t *key, const AES_iv_t *iv);

static void AES128_set_fixedKey(AES128_ctx_t *ctx);
static void AES192_set_fixedKey(AES192_ctx_t *ctx);
static void AES256_set_fixedKey(AES256_ctx_t *ctx);

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*BASE64_H*/