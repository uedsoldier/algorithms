/**
 * @file checksum.c
 * @brief Librería de implementación de funciones de suma de verificación de 8 bits
 * @author Ing. José Roberto Parra Trewartha
*/

#include <stdint.h>
#include "checksum.h"

/**
 * @brief Función para cálculo de checksum de 8 bits mediante OR exclusiva (XOR)
 * @param datos (void*) Apuntador a dato o estructura de datos para determinar su checksum XOR
 * @param len (uint16_t) Cantidad de datos para realizar checksum
 * @return Checksum de 8 bits mediante XOR
*/
uint8_t checksum8_XOR(void *datos, uint16_t len)
{
	uint8_t retVal = 0;						//Valor de retorno
	uint8_t *_buf = (uint8_t*)datos;		//Asignacion de datos con apuntador a uint8_t
	for(uint16_t i=0 ; i != len; i++)		//Ciclo iterativo para cálculo de checksum
	{
		retVal ^= *_buf++;
	}
	return retVal;
}


/**
 * @brief Función para cálculo de checksum de 8 bits mediante mediante módulo 256
 * @param datos (void*) Apuntador a dato o estructura de datos para determinar su checksum módulo 256
 * @param len (uint16_t) Cantidad de datos para realizar checksum
 * @return Checksum de 8 bits mediante módulo 256
*/
uint8_t checksum8_modulo256(void *datos, uint16_t len)
{
	uint8_t retVal = 0;						//Valor de retorno
	uint8_t *_buf = (uint8_t*)datos;		//Asignacion de datos con apuntador a uint8_t
	for(uint16_t i=0 ; i != len; i++)		//Ciclo iterativo para suma de bytes
	{
		retVal += *_buf++;
	}
	return retVal;
}

/**
 * @brief Función para cálculo de checksum de 8 bits mediante mediante complemento a 2
 * @param datos (void*) Apuntador a dato o estructura de datos para determinar su checksum complemento a 2
 * @param len (uint16_t) Cantidad de datos para realizar checksum
 * @return Checksum de 8 bits mediante complemento a 2
*/
uint8_t checksum8_2complement(void *datos, uint16_t len)
{
	uint8_t retVal = 0;						//Valor de retorno
	uint8_t *_buf = (uint8_t*)datos;		//Asignacion de datos con apuntador a uint8_t
	for(uint16_t i=0 ; i != len; i++)		//Ciclo iterativo para suma de bytes
	{
		retVal += *_buf++;
	}
	return (uint8_t)(0x0100-(uint8_t)(retVal));
}
