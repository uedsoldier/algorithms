/**
 * @file checksum.h
 * @brief Librería de implementación de funciones de suma de verificación de 8 bits
 * @author Ing. José Roberto Parra Trewartha
*/
#ifndef CHECKSUM_H

#define CHECKSUM_H

uint8_t checksum8_XOR(void *datos, uint16_t len);			// Función para cálculo de checksum de 8 bits mediante OR exclusiva (XOR)
uint8_t checksum8_modulo256(void *datos, uint16_t len);		// Función para cálculo de checksum de 8 bits mediante módulo 256
uint8_t checksum8_2complement(void *datos, uint16_t len);	// Función para cálculo de checksum de 8 bits mediante complemento a 2

#endif /*CHECKSUM_H*/