/**
 * @file string_custom.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "string_custom.h"

/**
 * @brief Función que localiza un caracter o cadena dentro de otra cadena. Ejemplo: 
 * from = "Hola mundo"
 * Posiciones: 
 * 'H' 'o' 'l' 'a' ' ' 'm' 'u' 'n' 'd' 'o' '\\0'
 *  0   1   2   3   4   5   6   7   8   9   10
 * search = "mundo"
 * Resultado de la función: 5
 * @param from: (const char*) cadena de caracteres en la cual se buscará 
 * @param search: (const char*) cadena de caracteres cuya ocurrencia se buscará en la cadena principal
 * @return posicion: (int16) Índice de ocurrencia de la cadena a buscar en la cadena principal. Si no hay ocurrencia se devuelve -1
*/
int16_t string_indexOf( const char *search, const char *from){
    char *p = strstr(from,search);
    if(p == NULL)
        return -1;
    else
        return (int16_t)(p - from);
}

/**
 * @brief Función para validar que una cadena de caracteres corresponde a una secuencia de dígitos
 * @param str 
 * @return true 
 * @return false 
 */
bool string_validate_int(const char *str){
    while (*str) {
        if(!isdigit(*str++)){
            return false;
        }
    }
    return true;
}

/**
 * @brief Función para validar que una cadena de caracteres corresponde a una secuencia de dígitos hexadecimales (0-9 y a-f)
 * 
 * @param str 
 * @return true 
 * @return false 
 */
bool string_validate_hex(const char *str){
    while (*str) {
        if(!isxdigit(*str++)){
            return false;
        }
    }
    return true;
}
