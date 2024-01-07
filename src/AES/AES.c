/**
 * @file AES.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AES.h"

/**
 * @brief
 *
 * @param ctx
 * @param key
 */
void AES_init_ctx(AES_ctx_t *ctx, const AES_key_t *key)
{
    KeyExpansion(ctx->roundKey, key->key_bytes);
}

#if (defined(AES_CBC) && (AES_CBC == 1)) || (defined(AES_CTR) && (AES_CTR == 1))

/**
 * @brief
 *
 * @param ctx
 * @param key
 * @param iv
 */
void AES_init_ctx_iv(AES_ctx_t *ctx, const AES_key_t *key, const AES_iv_t *iv)
{
    AES_init_ctx(ctx->roundKey, key->key_bytes);
    AES_ctx_set_iv(ctx, iv);
}

/**
 * @brief
 *
 * @param ctx
 * @param iv
 */
void AES_ctx_set_iv(AES_ctx_t *ctx, const AES_iv_t *iv)
{
    memcpy(ctx->iv, iv, AES_BLOCK_LEN);
}

/**
 * @brief
 * TODO necessary??
 * @param ctx
 */
static void AES_set_fixedKey(AES_ctx_t *ctx)
{
    uint8_t *fixed_key = ctx->roundKey;
    size_t i;
    for (i = 0; i < AES_KEY_EXP_SIZE && fixed_key[i] != 0; ++i)
        ;
    memset(fixed_key + i, 0, AES_KEY_EXP_SIZE - i);
}
#endif

/**
 * @brief This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
 * @param roundKey
 * @param key
 */
static void KeyExpansion(uint8_t *roundKey, const uint8_t *key)
{
    uint8_t i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk; ++i)
    {
        roundKey[(i * 4) + 0] = key[(i * 4) + 0];
        roundKey[(i * 4) + 1] = key[(i * 4) + 1];
        roundKey[(i * 4) + 2] = key[(i * 4) + 2];
        roundKey[(i * 4) + 3] = key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk; i < Nb * (Nr + 1); ++i)
    {
        {
            k = (i - 1) * 4;
            tempa[0] = roundKey[k + 0];
            tempa[1] = roundKey[k + 1];
            tempa[2] = roundKey[k + 2];
            tempa[3] = roundKey[k + 3];
        }

        if (i % Nk == 0)
        {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }

            tempa[0] = tempa[0] ^ Rcon[i / Nk];
        }
#if defined(AES256) && (AES256 == 1)
        if (i % Nk == 4)
        {
            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }
        }
#endif
        j = i * 4;
        k = (i - Nk) * 4;
        roundKey[j + 0] = roundKey[k + 0] ^ tempa[0];
        roundKey[j + 1] = roundKey[k + 1] ^ tempa[1];
        roundKey[j + 2] = roundKey[k + 2] ^ tempa[2];
        roundKey[j + 3] = roundKey[k + 3] ^ tempa[3];
    }
}

/**
 * @brief This function adds the round key to state.
 * the round key is added to the state by an XOR function.
 * @param round
 * @param state
 * @param roundKey
 */
static void AddRoundKey(uint8_t round, AES_state_t *state, const uint8_t *roundKey)
{
    uint8_t i, j;
    for (i = 0; i != 4; ++i)
    {
        for (j = 0; j != 4; ++j)
        {
            (*state)[i][j] ^= roundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}

/**
 * @brief The SubBytes Function Substitutes the values in the state matrix with values in an S-box.
 *
 * @param state
 */
static void SubBytes(AES_state_t *state)
{
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

static void ShiftRows(AES_state_t *state)
{
}

static uint8_t Xtime(uint8_t x)
{
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1B));
}

/**
 * @brief MixColumns function mixes the columns of the state matrix
 * @param state
 */
static void MixColumns(AES_state_t *state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i)
    {
        t = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
        Tm = (*state)[i][0] ^ (*state)[i][1];
        Tm = Xtime(Tm);
        (*state)[i][0] ^= Tm ^ Tmp;
        Tm = (*state)[i][1] ^ (*state)[i][2];
        Tm = Xtime(Tm);
        (*state)[i][1] ^= Tm ^ Tmp;
        Tm = (*state)[i][2] ^ (*state)[i][3];
        Tm = Xtime(Tm);
        (*state)[i][2] ^= Tm ^ Tmp;
        Tm = (*state)[i][3] ^ t;
        Tm = Xtime(Tm);
        (*state)[i][3] ^= Tm ^ Tmp;
    }
}

#if defined(MULTIPLY_AS_A_FUNCTION) && (MULTIPLY_AS_A_FUNCTION == 1)
/**
 * @brief Multiply is used to multiply numbers in the field GF(2^8)
 *
 * @param x
 * @param y
 * @return uint8_t
 */
static uint8_t Multiply(uint8_t x, uint8_t y)
{
    return (((y & 1) * x) ^ ((y >> 1 & 1) * Xtime(x)) ^ ((y >> 2 & 1) * Xtime(Xtime(x))) ^
            ((y >> 3 & 1) * Xtime(Xtime(Xtime(x)))) ^
            ((y >> 4 & 1) * Xtime(Xtime(Xtime(Xtime(x))))));
}
#endif

#if (defined(AES_CBC) && AES_CBC == 1) || (defined(AES_ECB) && AES_ECB == 1)
/**
 * @brief MixColumns function mixes the columns of the state matrix.
 *
 * @param state
 */
static void InvMixColumns(AES_state_t *state)
{
    uint8_t i, a, b, c, d;
    for (i = 0; i != 4; ++i)
    {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

/**
 * @brief The SubBytes Function Substitutes the values in the state matrix with values in an S-box.
 *
 * @param state
 */
static void InvSubBytes(AES_state_t *state)
{
    uint8_t i, j;
    for (i = 0; i != 4; ++i)
    {
        for (j = 0; j != 4; ++j)
        {
            (*state)[j][i] = getSBoxInvert((*state)[j][i]);
        }
    }
}

static void InvShiftRows(AES_state_t *state)
{
    uint8_t temp;

    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}
#endif

/**
 * @brief Function that encrypts the PlainText.
 * 
 * @param ctx 
 * @param in_state 
 * @param out_state 
 */
static void AES_encrypt_chunk(AES_ctx_t *ctx, AES_state_t *in_state,AES_state_t *out_state)
{
    uint8_t round = 0;
    uint8_t *_in = (uint8_t *)in_state;
	uint8_t *_out = (uint8_t *)out_state;

    #if defined(AES_LOG) && AES_LOG == 1
	uint32_t index;
	printf("IN: ");
	for( index=0 ; index!=AES_BLOCK_LEN; index++){
		uint8_t c = _in[index];
		printf("%02x ",c);
	}	
	printf("\t");
    #endif

    memcpy(out_state,in_state,sizeof(AES_state_t));
    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0, out_state, ctx->roundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1;; ++round)
    {
        SubBytes(out_state);
        ShiftRows(out_state);
        if (round == Nr)
        {
            break;
        }
        MixColumns(out_state);
        AddRoundKey(round, out_state, ctx->roundKey);
    }
    // Add round key to last round
    AddRoundKey(Nr, out_state, ctx->roundKey);

    #if defined(AES_LOG) && AES_LOG == 1
	printf("OUT: ");
	for( index =0 ; index != AES_BLOCK_LEN; index++){
		uint8_t c = _out[index];
		printf("%02x ",c);
	}	
	printf("\n");
	#endif
}

#if defined(AES_CBC) && (AES_CBC == 1)
static void XorWithIv(uint8_t *buf, const AES_iv_t *iv)
{
    uint8_t i;
    for (i = 0; i != AES_BLOCK_LEN; ++i) // The block in AES is always 128bit no matter the key size
    {
        buf[i] ^= iv->iv_array[i];
    }
}

#endif

#if (defined(AES_CBC) && AES_CBC == 1) || (defined(AES_ECB) && AES_ECB == 1)

/**
 * @brief 
 * 
 * @param ctx 
 * @param in_state 
 * @param out_state 
 */
static void AES_decrypt_chunk(AES_ctx_t *ctx, AES_state_t *in_state,AES_state_t *out_state)
{
    uint8_t round = 0;
    memcpy(out_state,in_state,sizeof(AES_state_t));
    // Add the First round key to the state before starting the rounds.
    AddRoundKey(Nr, out_state, ctx->roundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (Nr - 1);; --round)
    {
        InvShiftRows(out_state);
        InvSubBytes(out_state);
        AddRoundKey(round, out_state, ctx->roundKey);
        if (round == 0)
        {
            break;
        }
        InvMixColumns(out_state);
    }
}
#endif

#if defined(AES_ECB) && (AES_ECB > 0)
/**
 * @brief 
 * 
 * @param AES 
 * @param in 
 * @param out 
 * @param input_len 
 * @param output_len 
 * @return AES_errcode_t 
 */
AES_errcode_t AES_ECB_encrypt(AES_ctx_t *ctx, void *in, void *out, size_t input_len, uint32_t *output_len){
    #if defined(AES_LOG) && AES_LOG == 1
    printf("AES ECB encrypt\n");
    #endif
    // Input buffer length verification
    if(input_len<= 0){
        #if defined(AES_LOG) && AES_LOG == 1
        printf("\tEmpty input buffer\n");
        #endif
        
        return AES_CODE_EMPTY_INPUT_BUFFER;
    }
    // Normalized length calculation
    ctx->encrypted_chunks = 0;
    ctx->input_len_normalized = (size_t)(AES_ROUNDUP_TO_NEAREST_MULTIPLE_OF_16(input_len));

    // Input buffer allocation, backup and initialization 
	#if defined (AES_DYNAMIC_MEMORY) && (AES_DYNAMIC_MEMORY == 1)
	uint8_t *aux_in = (uint8_t *) malloc(ctx->input_len_normalized);
	uint8_t *_in = (uint8_t *)aux_in;
	if(aux_in == NULL){
		#if defined(AES_LOG) && AES_LOG == 1
		printf("\tDynamic memory assignment error\n");
		#endif
		return AES_CODE_NULL_MALLOC;
	}
	memset(aux_in,0,ctx->input_len_normalized);
	memcpy(_in, in, ctx->input_len_normalized);
	#else
		#if defined(AES_USE_BUFFERS) && (AES_USE_BUFFERS == 1)
	if( ctx->input_len_normalized > AES_MAX_BUFFER_SIZE){
		#if defined(AES_LOG) && AES_LOG == 1
		printf("ERROR: Normalized input length exceeds max buffer size\n");
		#endif
		return AES_CODE_INCORRECT_BUFFER_SIZE;
	}
	uint8_t aux_in[AES_MAX_BUFFER_SIZE];
	uint8_t *_in = (uint8_t *)aux_in;
	memset(_in, 0 , AES_MAX_BUFFER_SIZE);
	memcpy(_in, in, ctx->input_len_normalized);
		#else
	uint8_t *_in = (uint8_t *)in;
		#endif
	#endif

    // Initialization vector and output buffer allocation
	uint8_t *_out = (uint8_t *)out; 
	memset(_out, 0 , ctx->input_len_normalized);
	#if defined(AES_LOG) && AES_LOG == 1
	uint32_t i;
	printf("\tInput buffer:\t");
	for( i=0 ; i!=ctx->input_len_normalized; i++){
		uint8_t c = _in[i];
		printf("%02x ",c);
	}
	printf("\n");

	printf("\tOutput buffer:\t");
	for( i=0 ; i!=ctx->input_len_normalized; i++){
		uint8_t c = _out[i];
		printf("%02x ",c);
	}
	printf("\n");
	#endif

    // Chunk encryption
	while(ctx->input_len_normalized> 0) {
		#if defined(AES_LOG) && AES_LOG == 1
		printf("\tChunk %u: ",ctx->encrypted_chunks);
		#endif

			AES_encrypt_chunk(ctx,_in,_out);

		_in += AES_BLOCK_LEN; _out += AES_BLOCK_LEN; ctx->input_len_normalized-= AES_BLOCK_LEN; ctx->encrypted_chunks++;
		
	}
	*output_len = ctx->encrypted_chunks*AES_BLOCK_LEN;
	#if defined (XTEA_DYNAMIC_MEMORY) && (XTEA_DYNAMIC_MEMORY == 1)
	free(aux_in);
	#endif
	return AES_CODE_OK;


}

    
    AES_errcode_t AES_ECB_decrypt(AES_ctx_t *AES, void *in, void *out, size_t input_len, uint32_t *output_len){
        
    }

#endif

#if defined(AES_CBC) && (AES_CBC > 0)
    
    AES_errcode_t AES_CBC_encrypt(AES_ctx_t *AES, void *in, void *out, size_t input_len, uint32_t *output_len){
        
    }

    
    AES_errcode_t AES_CBC_decrypt(AES_ctx_t *AES, void *in, void *out, size_t input_len, uint32_t *output_len){
        
    }

#endif

#if defined(AES_CTR) && (AES_CTR > 0)
    
    AES_errcode_t AES_CTR_xcrypt(AES_ctx_t *AES, void *in, void *out, size_t input_len, uint32_t *output_len){
        
    }

#endif