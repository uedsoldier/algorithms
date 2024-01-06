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
extern "C" {
#endif

#pragma region Useful macros


/**
 * @brief Macro de tamaño en bytes de bloque AES. NO debe modificarse.
 * 
 */
#define AES_BLOCK_SIZE 16

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
 * @brief Macro de tamaño de clave fija en bytes para AES-256. NO debe modificarse.
 * 
 */
#define AES256_FIXED_KEY_SIZE 32

/**
 * @brief Macro de tamaño de clave fija en words para AES-256. NO debe modificarse.
 * 
 */
#define AES256_FIXED_KEY_SIZE_WORDS 8

#pragma endregion

#pragma region Custom types


typedef enum AES_standard {
    AES_128, AES_192, AES_256
} AES_standard_t;

typedef struct AES128_key
{
    union {
        uint32_t key_words[AES128_FIXED_KEY_SIZE_WORDS];
        uint8_t key_bytes[AES128_FIXED_KEY_SIZE];
    };
} AES128_key_t;

typedef struct AES192_key
{
    union {
        uint32_t key_words[AES192_FIXED_KEY_SIZE_WORDS];
        uint8_t key_bytes[AES192_FIXED_KEY_SIZE];
    };
} AES192_key_t;

typedef struct AES256_key
{
    union {
        uint32_t key_words[AES256_FIXED_KEY_SIZE_WORDS];
        uint8_t key_bytes[AES256_FIXED_KEY_SIZE];
    };
} AES256_key_t;


#pragma endregion

#pragma region Function prototypes
#pragma endregion



#ifdef __cplusplus
}
#endif

#endif /*BASE64_H*/