/**
 * @file checksum.h
 * @brief Librería de implementación de funciones de suma de verificación de 8 bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/
#ifndef CHECKSUM8_H
#define CHECKSUM8_H

#pragma region Useful macros
#define CHECKSUM8_LOG 1
#define CHECKSUM8_USE_IMPLEMENTATION_NAMES  1
#pragma endregion

#pragma region  Dependencies
#include <stdint.h>
#pragma endregion

#pragma region Custom types
typedef enum CHECKSUM8_t{
    CHECKSUM8_XOR, CHECKSUM8_modulo256, CHECKSUM8_2complement
} CHECKSUM8_t;
#pragma endregion

#pragma region Function prototypes
uint8_t checksum8(void *datos, uint16_t len, CHECKSUM8_t checksum_type); 
#if defined(CHECKSUM8_USE_IMPLEMENTATION_NAMES) && ( CHECKSUM8_USE_IMPLEMENTATION_NAMES == 1 )
static const char checksum8_implementation_0[] = "XOR"; 
static const char checksum8_implementation_1[] = "modulo 256";
static const char checksum8_implementation_2[] = "2's complement";
static const char * const checksum8_implementations[] = {
    checksum8_implementation_0, checksum8_implementation_1, checksum8_implementation_2
};
#endif
#pragma endregion

#endif /*CHECKSUM8_H*/