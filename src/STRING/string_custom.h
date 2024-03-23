/**
 * @file string_custom.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef STRING_CUSTOM_H
#define STRING_CUSTOM_H

#pragma region Dependencies
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#pragma endregion

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Function prototypes
#pragma endregion

int16_t string_indexOf( const char *search, const char *from);
bool string_validate_int(const char *str);
bool string_validate_hex(const char *str);


#ifdef __cplusplus
}
#endif

#endif /*STRING_CUSTOM_H*/