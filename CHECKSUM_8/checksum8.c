/**
 * @file checksum8.c
 * @brief Librería de implementación de funciones de suma de verificación de 8 bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
*/

#include "checksum8.h"

/**
 * @brief Función para cálculo de checksum de 8 bits mediante algún método implementado
 * @param datos (void*) Apuntador a dato o estructura de datos para determinar su checksum
 * @param len (uint16_t) Cantidad de datos para realizar checksum
 * @param tipo (CHECKSUM8_t) Tipo de CRC8
 * @return Checksum de 8 bits mediante XOR
*/

uint8_t checksum8(void *datos, uint16_t len, CHECKSUM8_t tipo) {
	uint8_t retval = 0;						//Valor de retorno
	uint8_t *_buf = (uint8_t*)datos;		//Asignacion de datos con apuntador a uint8_t
	for(uint16_t i=0 ; i != len; i++) {		//Ciclo iterativo para cálculo de checksum
		switch(tipo){
			case CHECKSUM8_XOR:
				retval ^= *_buf++; break;
			case CHECKSUM8_modulo256:
				retval += *_buf++; break;
			case CHECKSUM8_2complement:
				retval += *_buf++;
				break;
		}
	}
	return (tipo == CHECKSUM8_2complement)? (uint8_t)(0x0100-(uint8_t)(retval)) : retval;
}