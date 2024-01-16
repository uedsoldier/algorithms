/**
 * @file PKCS7.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "PKCS7.h"

PKCS7_error_t PKCS7_add_padding(PKCS7_padding_t *pkcs7, const void *data, const size_t data_len, const size_t block_size){
    if(block_size == 0 || block_size > PKCS7_MAX_BLOCK_SIZE_BYTES){
        return PKCS7_INVALID_BLOCK_SIZE;
    }
    memset(pkcs7->data_with_padding,0,PKCS7_MAX_BLOCK_SIZE_BYTES);
    uint8_t padding_bytes_amount = block_size - (data_len % block_size);
    pkcs7->padding_byte_value = padding_bytes_amount;
    pkcs7->data_len_with_padding = data_len + padding_bytes_amount;

    uint8_t *data_with_padding = pkcs7->data_with_padding;

    memcpy(data_with_padding, data, data_len);

    for(uint8_t i = 0; i != padding_bytes_amount; i++){
        data_with_padding[data_len + i ] = pkcs7->padding_byte_value;
    }

    memcpy(pkcs7->data_with_padding,data_with_padding,pkcs7->data_len_with_padding);

     #if defined(PKCS7_LOG) && PKCS7_LOG == 1
    printf("PKCS7 add padding\n[");
	for (size_t i = 0; i != pkcs7->data_len_with_padding; i++)
	{
		printf("%02X ",pkcs7->data_with_padding[i]);
	}
	printf("]\n");
    #endif

    return PKCS7_OK;
}

PKCS7_error_t PKCS7_remove_padding(PKCS7_unpadding_t *pkcs7, const void *data, const size_t data_len){
    memset(pkcs7->data_without_padding,0,PKCS7_MAX_BLOCK_SIZE_BYTES);

    uint8_t padding_bytes_amount = *((uint8_t *)data + data_len - 1);
    pkcs7->padding_byte_to_remove = padding_bytes_amount;
    pkcs7->data_len_without_padding = data_len - padding_bytes_amount;

    #if defined(PKCS7_LOG) && PKCS7_LOG == 1
    printf("Padding bytes amount: %u, Padding byte to remove: %02X\n",padding_bytes_amount,pkcs7->padding_byte_to_remove );
    #endif

    uint8_t *data_without_padding = pkcs7->data_without_padding;
    memcpy(data_without_padding, (uint8_t *)data, pkcs7->data_len_without_padding);
    memcpy(pkcs7->data_without_padding,data_without_padding,pkcs7->data_len_without_padding);

    #if defined(PKCS7_LOG) && PKCS7_LOG == 1
    printf("PKCS7 remove padding\n[");
	for (size_t i = 0; i != data_len; i++)
	{
		printf("%02X ",pkcs7->data_without_padding[i]);
	}
	printf("]\n");
    #endif


    return PKCS7_OK;
}