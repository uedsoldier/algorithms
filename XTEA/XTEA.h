/**
 * @file XTEA.h
 * @brief Librería de implementación de funciones para encriptación de datos mediante el algoritmo XTEA (Extended Tiny Encryption Algorithm)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#ifndef XTEA_H
#define XTEA_H

// Dependencias
#include <stdbool.h>
#include <stdint.h>
#include "../../utils/utils.h"

/**
 * @brief  Número de interaciones para la ejecución del algoritmo. 32 es bastante, 16 son suficientes, y alrededor de 8 serviría para muchas aplicaciones.
*/
#define XTEA_ITERATIONS 8


/**
 * @brief Constante δ derivada de la razón áurea
*/
#define XTEA_DELTA 0x9E3779B9UL

/**
 * @brief Suma inicial para desencriptado
 * 0x8DDE6E40 para 64
 * 0xC6EF3720 para 32
 * 0xE3779B90 para 16
 * 0xF1BBCDC8 para 8
 * 
*/
uint32_t XTEA_DEC_SUM = XTEA_DELTA * XTEA_ITERATIONS;

/**
 * Prototipos de funciones
*/
void XTEA_encipher(uint32_t *in, uint32_t *out,uint32_t *key );      // Función de cifrado
void XTEA_decipher (uint32_t * in,uint32_t * out,uint32_t * key);    // Función de descrifrado

#endif /*XTEA_H*/
