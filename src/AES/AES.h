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
#include <stddef.h>
#include <string.h>
#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Useful macros

#define AES128 1
#define AES192 0
#define AES256 0

/**
 * @brief Macro para depuración mediante la función printf(). Se recomienda usar únicamente
 * para fines de desarrollo y pruebas.
 */
#ifndef AES_LOG
#define AES_LOG 1
#endif

#if defined(AES_LOG) && AES_LOG == 1
static const char *AES_TAG = "AES";
#endif

/**
 * Define the macros below to 1/0 to enable/disable the mode of operation.

 */
#pragma region Operation modes
/**
 * @brief ECB enables the basic ECB 16-byte block algorithm. All can be enabled simultaneously.
 */
#ifndef AES_ECB
#define AES_ECB 1
#endif

/**
 * @brief CBC enables AES encryption in CBC-mode of operation.
 */
#ifndef AES_CBC
#define AES_CBC 1
#endif

/**
 * @brief CTR enables encryption in counter-mode.
 */
#ifndef AES_CTR
#define AES_CTR 0
#endif

#define MULTIPLY_AS_A_FUNCTION 1

#pragma endregion

#pragma region Memory macros
/**
 * @brief Macro para utilización de asignación dinámica de memoria en las funciones AES.
 * Usar esta característica permite ahorrar memoria RAM, pero no todos los dispositivos ni todos
 * los compiladores soportan tales funcionalidades (malloc(), calloc(), realloc(), etc.)
 */
#ifndef AES_DYNAMIC_MEMORY
#define AES_DYNAMIC_MEMORY 0
#endif

/**
 * @brief Macro para utilizacion de buffers fijos auxiliares para entrada de datos, de tal forma
 * que el buffer de entrada de datos no se modifica. Para utilizarse, la macro AES_DYNAMIC_MEMORY
 * debe estar indefinida o con valor igual a 0.
 *
 */
#if !defined(AES_DYNAMIC_MEMORY) || (AES_DYNAMIC_MEMORY == 0)
#ifndef AES_USE_BUFFERS
#define AES_USE_BUFFERS 0
#endif
#endif

/**
 * @brief Macro para definición de máximo tamaño de buffer, para los casos en los que no se
 * requiere asignación dinámica de memoria
 *
 */
#ifdef AES_USE_BUFFERS
#define AES_MAX_BUFFER_SIZE 128
#endif
#pragma endregion

#pragma region General constants
/**
 * @brief Macro de tamaño en bytes de bloque AES. NO debe modificarse.
 *
 */
#define AES_BLOCK_LEN 16

/**
 * @brief The number of columns comprising a state in AES. This is a constant in AES. Value=4
 *
 */
#define Nb 4

#if defined(AES256) && (AES256 == 1)
#define Nk 8  // The number of 32 bit words in a key.
#define Nr 14 // The number of rounds in AES Cipher.
#elif defined(AES192) && (AES192 == 1)
#define Nk 6  // The number of 32 bit words in a key.
#define Nr 12 // The number of rounds in AES Cipher.
#else
#define Nk 4  // The number of 32 bit words in a key.
#define Nr 10 // The number of rounds in AES Cipher.
#endif

/**
 * @brief Macro para encontrar el siguiente múltiplo de 16 más cernano a un número. NO debe modificarse.
 *
 */
#define AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(N) (((N + 15) >> 4) << 4)

    static const uint8_t sbox[256] = {
        // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0X63, 0X7C, 0X77, 0X7B, 0XF2, 0X6B, 0X6F, 0XC5, 0X30, 0X01, 0X67, 0X2B, 0XFE, 0XD7, 0XAB, 0X76,
        0XCA, 0X82, 0XC9, 0X7D, 0XFA, 0X59, 0X47, 0XF0, 0XAD, 0XD4, 0XA2, 0XAF, 0X9C, 0XA4, 0X72, 0XC0,
        0XB7, 0XFD, 0X93, 0X26, 0X36, 0X3F, 0XF7, 0XCC, 0X34, 0XA5, 0XE5, 0XF1, 0X71, 0XD8, 0X31, 0X15,
        0X04, 0XC7, 0X23, 0XC3, 0X18, 0X96, 0X05, 0X9A, 0X07, 0X12, 0X80, 0XE2, 0XEB, 0X27, 0XB2, 0X75,
        0X09, 0X83, 0X2C, 0X1A, 0X1B, 0X6E, 0X5A, 0XA0, 0X52, 0X3B, 0XD6, 0XB3, 0X29, 0XE3, 0X2F, 0X84,
        0X53, 0XD1, 0X00, 0XED, 0X20, 0XFC, 0XB1, 0X5B, 0X6A, 0XCB, 0XBE, 0X39, 0X4A, 0X4C, 0X58, 0XCF,
        0XD0, 0XEF, 0XAA, 0XFB, 0X43, 0X4D, 0X33, 0X85, 0X45, 0XF9, 0X02, 0X7F, 0X50, 0X3C, 0X9F, 0XA8,
        0X51, 0XA3, 0X40, 0X8F, 0X92, 0X9D, 0X38, 0XF5, 0XBC, 0XB6, 0XDA, 0X21, 0X10, 0XFF, 0XF3, 0XD2,
        0XCD, 0X0C, 0X13, 0XEC, 0X5F, 0X97, 0X44, 0X17, 0XC4, 0XA7, 0X7E, 0X3D, 0X64, 0X5D, 0X19, 0X73,
        0X60, 0X81, 0X4F, 0XDC, 0X22, 0X2A, 0X90, 0X88, 0X46, 0XEE, 0XB8, 0X14, 0XDE, 0X5E, 0X0B, 0XDB,
        0XE0, 0X32, 0X3A, 0X0A, 0X49, 0X06, 0X24, 0X5C, 0XC2, 0XD3, 0XAC, 0X62, 0X91, 0X95, 0XE4, 0X79,
        0XE7, 0XC8, 0X37, 0X6D, 0X8D, 0XD5, 0X4E, 0XA9, 0X6C, 0X56, 0XF4, 0XEA, 0X65, 0X7A, 0XAE, 0X08,
        0XBA, 0X78, 0X25, 0X2E, 0X1C, 0XA6, 0XB4, 0XC6, 0XE8, 0XDD, 0X74, 0X1F, 0X4B, 0XBD, 0X8B, 0X8A,
        0X70, 0X3E, 0XB5, 0X66, 0X48, 0X03, 0XF6, 0X0E, 0X61, 0X35, 0X57, 0XB9, 0X86, 0XC1, 0X1D, 0X9E,
        0XE1, 0XF8, 0X98, 0X11, 0X69, 0XD9, 0X8E, 0X94, 0X9B, 0X1E, 0X87, 0XE9, 0XCE, 0X55, 0X28, 0XDF,
        0X8C, 0XA1, 0X89, 0X0D, 0XBF, 0XE6, 0X42, 0X68, 0X41, 0X99, 0X2D, 0X0F, 0XB0, 0X54, 0XBB, 0X16};

#if (defined(AES_CBC) && AES_CBC == 1) || (defined(AES_ECB) && AES_ECB == 1)
    static const uint8_t rsbox[256] = {
        0X52, 0X09, 0X6A, 0XD5, 0X30, 0X36, 0XA5, 0X38, 0XBF, 0X40, 0XA3, 0X9E, 0X81, 0XF3, 0XD7, 0XFB,
        0X7C, 0XE3, 0X39, 0X82, 0X9B, 0X2F, 0XFF, 0X87, 0X34, 0X8E, 0X43, 0X44, 0XC4, 0XDE, 0XE9, 0XCB,
        0X54, 0X7B, 0X94, 0X32, 0XA6, 0XC2, 0X23, 0X3D, 0XEE, 0X4C, 0X95, 0X0B, 0X42, 0XFA, 0XC3, 0X4E,
        0X08, 0X2E, 0XA1, 0X66, 0X28, 0XD9, 0X24, 0XB2, 0X76, 0X5B, 0XA2, 0X49, 0X6D, 0X8B, 0XD1, 0X25,
        0X72, 0XF8, 0XF6, 0X64, 0X86, 0X68, 0X98, 0X16, 0XD4, 0XA4, 0X5C, 0XCC, 0X5D, 0X65, 0XB6, 0X92,
        0X6C, 0X70, 0X48, 0X50, 0XFD, 0XED, 0XB9, 0XDA, 0X5E, 0X15, 0X46, 0X57, 0XA7, 0X8D, 0X9D, 0X84,
        0X90, 0XD8, 0XAB, 0X00, 0X8C, 0XBC, 0XD3, 0X0A, 0XF7, 0XE4, 0X58, 0X05, 0XB8, 0XB3, 0X45, 0X06,
        0XD0, 0X2C, 0X1E, 0X8F, 0XCA, 0X3F, 0X0F, 0X02, 0XC1, 0XAF, 0XBD, 0X03, 0X01, 0X13, 0X8A, 0X6B,
        0X3A, 0X91, 0X11, 0X41, 0X4F, 0X67, 0XDC, 0XEA, 0X97, 0XF2, 0XCF, 0XCE, 0XF0, 0XB4, 0XE6, 0X73,
        0X96, 0XAC, 0X74, 0X22, 0XE7, 0XAD, 0X35, 0X85, 0XE2, 0XF9, 0X37, 0XE8, 0X1C, 0X75, 0XDF, 0X6E,
        0X47, 0XF1, 0X1A, 0X71, 0X1D, 0X29, 0XC5, 0X89, 0X6F, 0XB7, 0X62, 0X0E, 0XAA, 0X18, 0XBE, 0X1B,
        0XFC, 0X56, 0X3E, 0X4B, 0XC6, 0XD2, 0X79, 0X20, 0X9A, 0XDB, 0XC0, 0XFE, 0X78, 0XCD, 0X5A, 0XF4,
        0X1F, 0XDD, 0XA8, 0X33, 0X88, 0X07, 0XC7, 0X31, 0XB1, 0X12, 0X10, 0X59, 0X27, 0X80, 0XEC, 0X5F,
        0X60, 0X51, 0X7F, 0XA9, 0X19, 0XB5, 0X4A, 0X0D, 0X2D, 0XE5, 0X7A, 0X9F, 0X93, 0XC9, 0X9C, 0XEF,
        0XA0, 0XE0, 0X3B, 0X4D, 0XAE, 0X2A, 0XF5, 0XB0, 0XC8, 0XEB, 0XBB, 0X3C, 0X83, 0X53, 0X99, 0X61,
        0X17, 0X2B, 0X04, 0X7E, 0XBA, 0X77, 0XD6, 0X26, 0XE1, 0X69, 0X14, 0X63, 0X55, 0X21, 0X0C, 0X7D};
#endif

    /**
     * @brief The round constant word array, Rcon[i], contains the values given by x to the power (i-1)
     * being powers of x (x is denoted as {02}) in the field GF(2^8)
     */
    static const uint8_t Rcon[11] = {
        0X8D, 0X01, 0X02, 0X04, 0X08, 0X10, 0X20, 0X40, 0X80, 0X1B, 0X36};

/**
 * @brief
 *
 */
#define getSBoxValue(num) (sbox[(num)])

#if (defined(AES_CBC) && AES_CBC == 1) || (defined(AES_ECB) && AES_ECB == 1)
/**
 * @brief
 *
 */
#define getSBoxInvert(num) (rsbox[(num)])
#endif

#pragma endregion

#pragma region AES-128 specific constants
#if defined(AES128) && AES128 > 0

/**
 * @brief Macro de tamaño de clave fija en bytes para AES-128. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE 16

/**
 * @brief Macro de tamaño de clave fija en words para AES-128. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE_WORDS 4

/**
 * @brief
 *
 */
#define AES_KEY_EXP_SIZE 176

#endif
#pragma endregion

#pragma region AES-192 specific constants
#if defined(AES192) && AES192 > 0
/**
 * @brief Macro de tamaño de clave fija en bytes para AES-192. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE 24

/**
 * @brief Macro de tamaño de clave fija en words para AES-192. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE_WORDS 6

/**
 * @brief
 *
 */
#define AES_KEY_EXP_SIZE 208
#endif
#pragma endregion

#pragma region AES-256 specific constants
#if defined(AES256) && AES256 > 0
/**
 * @brief Macro de tamaño de clave fija en bytes para AES-256. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE 32

/**
 * @brief Macro de tamaño de clave fija en words para AES-256. NO debe modificarse.
 *
 */
#define AES_FIXED_KEY_SIZE_WORDS 8

/**
 * @brief
 *
 */
#define AES_KEY_EXP_SIZE 240
#endif
#pragma endregion

#pragma endregion

#pragma region Custom types

    /**
     * @brief Enumeración de códigos de error para funciones AES
     *
     */
    typedef enum AES_errcode
    {
        AES_CODE_OK,
        AES_CODE_EMPTY_INPUT_BUFFER,
        AES_CODE_INCORRECT_BUFFER_SIZE,
        AES_CODE_NULL_MALLOC
    } AES_errcode_t;

    /**
     * @brief
     *
     */

    /**
     * @brief
     *
     */
    typedef struct AES_key
    {
        union
        {
            uint32_t key_words[AES_FIXED_KEY_SIZE_WORDS];
            uint8_t key_bytes[AES_FIXED_KEY_SIZE];
        };
    } AES_key_t;

    /**
     * @brief Estructura de datos para vector de inicialización
     *
     */
    typedef struct AES_iv
    {
        uint8_t iv_array[AES_BLOCK_LEN];
    } AES_iv_t;

    /**
     * @brief
     *
     */
    typedef struct AES_ctx
    {
        uint8_t roundKey[AES_KEY_EXP_SIZE]; // Key
        #if (defined(AES_CBC) && (AES_CBC == 1)) || (defined(AES_CTR) && (AES_CTR == 1))
        uint8_t iv[AES_BLOCK_LEN]; // Initialization vector for CBC and CTR modes
        #endif
        uint32_t encrypted_chunks;
        uint32_t decrypted_chunks;
        size_t input_len_normalized;

    } AES_ctx_t;

#pragma endregion

#pragma region variables
    /**
     * @brief Array holding the intermediate results during decryption.
     *
     */
    typedef uint8_t AES_state_t[4][4];
#pragma endregion

#pragma region Function prototypes

    static void KeyExpansion(uint8_t *roundKey, const uint8_t *key);
    static void AddRoundKey(uint8_t round, AES_state_t *state, const uint8_t *roundKey);
    static void SubBytes(AES_state_t *state);
    static void ShiftRows(AES_state_t *state);
    static uint8_t Xtime(uint8_t x);
    static void MixColumns(AES_state_t *state);

#if defined(MULTIPLY_AS_A_FUNCTION) && (MULTIPLY_AS_A_FUNCTION == 1)
    static uint8_t Multiply(uint8_t x, uint8_t y);
#else
#define Multiply(x, y)                         \
    (((y & 1) * x) ^                           \
     ((y >> 1 & 1) * xtime(x)) ^               \
     ((y >> 2 & 1) * xtime(xtime(x))) ^        \
     ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^ \
     ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))))
#endif
    static void InvMixColumns(AES_state_t *state);
    static void InvSubBytes(AES_state_t *state);
    static void InvShiftRows(AES_state_t *state);

#if defined(AES_CBC) && (AES_CBC == 1)
    static void XorWithIv(uint8_t *buf, const AES_iv_t *iv);
#endif

#if (defined(AES_CBC) && AES_CBC == 1) || (defined(AES_ECB) && AES_ECB == 1)
    static void AES_encrypt_chunk(AES_ctx_t *ctx, AES_state_t *in_state,AES_state_t *out_state);
    static void AES_decrypt_chunk(AES_ctx_t *ctx, AES_state_t *in_state,AES_state_t *out_state);
#endif

    void AES_init_ctx(AES_ctx_t *ctx, const AES_key_t *key);
#if (defined(AES_CBC) && (AES_CBC == 1)) || (defined(AES_CTR) && (AES_CTR == 1))
    void AES_init_ctx_iv(AES_ctx_t *ctx, const AES_key_t *key, const AES_iv_t *iv);
    void AES_ctx_set_iv(AES_ctx_t *ctx, const AES_iv_t *iv);
    static void AES_set_fixedKey(AES_ctx_t *ctx);
#endif

#if defined(AES_ECB) && (AES_ECB > 0)
    AES_errcode_t AES_ECB_encrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES_ECB_decrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);

#endif

#if defined(AES_CBC) && (AES_CBC > 0)
    AES_errcode_t AES_CBC_encrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
    AES_errcode_t AES_CBC_decrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
#endif

#if defined(AES_CTR) && (AES_CTR > 0)
    AES_errcode_t AES_CTR_xcrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len);
#endif

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /*BASE64_H*/