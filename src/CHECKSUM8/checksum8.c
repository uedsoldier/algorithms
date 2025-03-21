/**
 * @file checksum8.c
 * @brief Librería de implementación de funciones de suma de verificación de 8
 * bits
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
 */

#include "checksum8.h"

uint8_t checksum8(void *datos, uint16_t len, CHECKSUM8_t checksum_type) {
    uint8_t retval = 0;  // Valor de retorno
    uint8_t *_buf =
        (uint8_t *)datos;  // Asignacion de datos con apuntador a uint8_t
    for (uint16_t i = 0; i != len;
         i++) {  // Ciclo iterativo para cálculo de checksum
        switch (checksum_type) {
            case CHECKSUM8_XOR:
                retval ^= *_buf++;
                break;
            case CHECKSUM8_modulo256:
                retval += *_buf++;
                break;
            case CHECKSUM8_2complement:
                retval += *_buf++;
                break;
        }
    }
    return (checksum_type == CHECKSUM8_2complement)
               ? (uint8_t)(0x0100 - (uint8_t)(retval))
               : retval;
}
