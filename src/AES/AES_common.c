/**
 * @file AES_common.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "AES_common.h"

 void print_buffer(uint8_t *buffer, size_t len, const char *msg){

    size_t i;
	printf("%s:\n",msg);
	for( i=0 ; i!=len; i++){
        uint8_t c = buffer[i];
        printf("%02x ",c);
	}
    printf("\n");	
}

/**
 * @brief 
 * 
 * @param rhs 
 * @param lhs 
 * @return uint8_t 
 */
 uint8_t gmul(uint8_t rhs, uint8_t lhs){
    uint8_t peasant = 0;
	uint16_t irreducible = 0x11b;
	while (lhs) {
		if (lhs & 1) {
			peasant = peasant ^ rhs;
		}
		if (rhs & 0x80) {
			rhs = (rhs << 1) ^ irreducible;
		}
		else {
			rhs = rhs << 1;
		}
		lhs = lhs >> 1;
	}
	return peasant;
}


/**
 * @brief 
 * 
 * @param plain_text 
 * @param roundKey 
 */
 void AddRoundKey(uint8_t* plain_text, uint8_t * roundKey){
    for (size_t i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = plain_text[i] ^ roundKey[i];
	}
}

/**
 * @brief 
 * 
 * @param plain_text 
 */
 void SubBytes(uint8_t * plain_text){
    for (size_t i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = s_box[plain_text[i]];
	}
}

/**
 * @brief 
 * 
 * @param plain_text 
 */
 void ShiftRows(uint8_t * plain_text){
    uint8_t temp_block[AES_BLOCK_LEN];

	for (size_t i = 0; i != AES_BLOCK_LEN; i += 4) {
		//incrementing by 5 causes the diagonal shift effect
		temp_block[i] = plain_text[i];
		temp_block[i + 1] = plain_text[(i + 5) % AES_BLOCK_LEN];
		temp_block[i + 2] = plain_text[(i + 10) % AES_BLOCK_LEN];
		temp_block[i + 3] = plain_text[(i + 15) % AES_BLOCK_LEN];
	}

	for (int i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = temp_block[i];
	}
}

/**
 * @brief 
 * 
 * @param plain_text 
 */
 void MixColumns(uint8_t *plain_text){
    uint8_t temp_block[AES_BLOCK_LEN];

	for (size_t i = 0; i != AES_BLOCK_LEN; i += 4) {

		temp_block[i] = gmul(plain_text[i], (uint8_t)2) ^ gmul(plain_text[i + 1], (uint8_t)3) ^ plain_text[i + 2] ^ plain_text[i + 3];
		temp_block[i + 1] = plain_text[i] ^ gmul(plain_text[i + 1], (uint8_t) 2) ^ gmul(plain_text[i + 2], (uint8_t) 3) ^ plain_text[i + 3];
		temp_block[i + 2] = plain_text[i] ^ plain_text[i + 1] ^ gmul(plain_text[i + 2], (uint8_t) 2) ^ gmul(plain_text[i + 3], (uint8_t) 3);
		temp_block[i + 3] = gmul(plain_text[i], (uint8_t) 3) ^ plain_text[i + 1] ^ plain_text[i + 2] ^ gmul(plain_text[i + 3], (uint8_t) 2);
	}

	for (int i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = temp_block[i];
	}
}

/**
 * @brief 
 * 
 * @param state 
 */
 void ReverseSubBytes(uint8_t * state){
    for (size_t i = 0; i != AES_BLOCK_LEN; i++) {
		state[i] = inv_s[state[i]];
	}
}

/**
 * @brief 
 * 
 * @param plain_text 
 */
 void ReverseShiftRows(uint8_t * plain_text){
    uint8_t temp_block[AES_BLOCK_LEN];
    size_t i;
	for ( i = 0; i != AES_BLOCK_LEN; i += 4) {
		//incrementing by 5 causes the diagonal shift effect
		temp_block[i] = plain_text[i];
		temp_block[(i + 5) % AES_BLOCK_LEN] = plain_text[i+1];
		temp_block[(i + 10) % AES_BLOCK_LEN] = plain_text[i+2];
		temp_block[(i + 15) % AES_BLOCK_LEN] = plain_text[i+3];
	}

	for ( i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = temp_block[i];
	}
}

/**
 * @brief 
 * 
 * @param plain_text 
 */
 void ReverseMixColumns(uint8_t *plain_text){
    uint8_t temp_block[AES_BLOCK_LEN];
    size_t i;
	for ( i = 0; i != AES_BLOCK_LEN; i += 4) { 
	//inverse multiplication > 9,11,13,14
		temp_block[i] = gmul(plain_text[i], (uint8_t) 14) ^ gmul(plain_text[i + 1], (uint8_t)11) ^ 
			gmul(plain_text[i + 2], (uint8_t)13) ^ gmul(plain_text[i + 3], (uint8_t)9);

		temp_block[i + 1] = gmul(plain_text[i], (uint8_t) 9) ^ gmul(plain_text[i + 1], (uint8_t)14) ^ 
			gmul(plain_text[i + 2], (uint8_t)11) ^ gmul(plain_text[i + 3], (uint8_t)13);

		temp_block[i + 2] = gmul(plain_text[i], (uint8_t)13) ^ gmul(plain_text[i + 1], (uint8_t)9) ^ 
			gmul(plain_text[i + 2], (uint8_t)14) ^ gmul(plain_text[i + 3], (uint8_t)11);

		temp_block[i + 3] = gmul(plain_text[i], (uint8_t)11) ^ gmul(plain_text[i + 1], (uint8_t)13) ^ 
			gmul(plain_text[i + 2], (uint8_t)9) ^ gmul(plain_text[i + 3], (uint8_t)14);
	}

    for ( i = 0; i != AES_BLOCK_LEN; i++) {
		plain_text[i] = temp_block[i];
	}
}