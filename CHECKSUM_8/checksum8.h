/**
 * @file checksum.h
 * @brief Librería de implementación de funciones de suma de verificación de 8 bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/
#ifndef CHECKSUM8_H
#define CHECKSUM8_H

#define CHECKSUM8_USE_IMPLEMENTATION_NAMES  0

// Dependencias
#include <stdint.h>

uint8_t checksum8_XOR(void *datos, uint16_t len);			// Función para cálculo de checksum de 8 bits mediante OR exclusiva (XOR)
uint8_t checksum8_modulo256(void *datos, uint16_t len);		// Función para cálculo de checksum de 8 bits mediante módulo 256
uint8_t checksum8_2complement(void *datos, uint16_t len);	// Función para cálculo de checksum de 8 bits mediante complemento a 2

#if defined(CHECKSUM8_USE_IMPLEMENTATION_NAMES) && ( CHECKSUM8_USE_IMPLEMENTATION_NAMES == 1 )
static const char checksum8_implementation_0[] = "XOR"; 
static const char checksum8_implementation_1[] = "modulo 256";
static const char checksum8_implementation_2[] = "2's complement";
static const char * const checksum8_implementations[] = {
    checksum8_implementation_0, checksum8_implementation_1, checksum8_implementation_2
}
#endif

#endif /*CHECKSUM8_H*/