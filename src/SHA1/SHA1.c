/**
 * @file SHA1.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "SHA1.h"

/**
 * @brief 
 * 
 * @param ctx 
 */
static void sha1_init(SHA1_ctx_t *ctx){
	ctx->count[0] = ctx->count[1] = 0;
    ctx->hash[0] = 0x67452301;
    ctx->hash[1] = 0xEFCDAB89;
    ctx->hash[2] = 0x98BADCFE;
    ctx->hash[3] = 0x10325476;
    ctx->hash[4] = 0xC3D2E1F0;
}

/**
 * @brief 
 * 
 * @param ctx 
 */
static void sha1_compile(SHA1_ctx_t *ctx){
	uint32_t *w = ctx->wbuf;

	uint32_t v[5];
    memcpy(v, ctx->hash, SHA1_DIGEST_SIZE_BYTES);

	#define hf(i)   w[i]

    five_cycle(v, ch, 0x5A827999,  0);
    five_cycle(v, ch, 0x5A827999,  5);
    five_cycle(v, ch, 0x5A827999, 10);
    one_cycle(v,0,1,2,3,4, ch, 0x5A827999, hf(15));
	#undef  hf
	#define hf(i) (w[(i) & 15] = rotl32(                    \
                w[((i) + 13) & 15] ^ w[((i) + 8) & 15] \
                ^ w[((i) +  2) & 15] ^ w[(i) & 15], 1))

	one_cycle(v,4,0,1,2,3, ch, 0x5a827999, hf(16));
    one_cycle(v,3,4,0,1,2, ch, 0x5a827999, hf(17));
    one_cycle(v,2,3,4,0,1, ch, 0x5a827999, hf(18));
    one_cycle(v,1,2,3,4,0, ch, 0x5a827999, hf(19));

    five_cycle(v, parity, 0x6ed9eba1,  20);
    five_cycle(v, parity, 0x6ed9eba1,  25);
    five_cycle(v, parity, 0x6ed9eba1,  30);
    five_cycle(v, parity, 0x6ed9eba1,  35);

    five_cycle(v, maj, 0x8f1bbcdc,  40);
    five_cycle(v, maj, 0x8f1bbcdc,  45);
    five_cycle(v, maj, 0x8f1bbcdc,  50);
    five_cycle(v, maj, 0x8f1bbcdc,  55);

    five_cycle(v, parity, 0xca62c1d6,  60);
    five_cycle(v, parity, 0xca62c1d6,  65);
    five_cycle(v, parity, 0xca62c1d6,  70);
    five_cycle(v, parity, 0xca62c1d6,  75);

	ctx->hash[0] += v[0]; ctx->hash[1] += v[1];
    ctx->hash[2] += v[2]; ctx->hash[3] += v[3];
    ctx->hash[4] += v[4];
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param data 
 * @param len 
 */
static void sha1_hash(SHA1_ctx_t *ctx, const uint8_t *data, size_t len){
	uint32_t pos = (uint32_t)(ctx->count[0] & SHA1_MASK);
    uint32_t space = SHA1_BLOCK_SIZE_BYTES - pos;
    const unsigned char *sp = data;

    if((ctx->count[0] += len) < len)
        ++(ctx->count[1]);

    while(len >= space)     /* tranfer whole blocks if possible  */
    {
        memcpy(((uint8_t*)ctx->wbuf) + pos, sp, space);
        sp += space; len -= space; space = SHA1_BLOCK_SIZE_BYTES; pos = 0;
        bsw_32(ctx->wbuf, SHA1_BLOCK_SIZE_BYTES >> 2);
        sha1_compile(ctx);
    }

    memcpy(((unsigned char*)ctx->wbuf) + pos, sp, len);
}

/**
 * @brief 
 * 
 * @param ctx 
 * @param hval 
 */
static void sha1_end(SHA1_ctx_t *ctx, uint8_t *hval){
    uint32_t    i = (uint32_t)(ctx->count[0] & SHA1_MASK);
    /* put bytes in the buffer in an order in which references to   */
    /* 32-bit words will put bytes with lower addresses into the    */
    /* top of 32 bit words on BOTH big and little endian machines   */
    bsw_32(ctx->wbuf, (i + 3) >> 2);

    /* we now need to mask valid bytes and add the padding which is */
    /* a single 1 bit and as many zero bits as necessary. Note that */
    /* we can always add the first padding byte here because the    */
    /* buffer always has at least one empty slot                    */
    ctx->wbuf[i >> 2] &= 0xFFFFFF80 << 8 * (~i & 3);
    ctx->wbuf[i >> 2] |= 0x00000080 << 8 * (~i & 3);

    /* we need 9 or more empty positions, one for the padding byte  */
    /* (above) and eight for the length count. If there is not      */
    /* enough space, pad and empty the buffer                       */
    if(i > SHA1_BLOCK_SIZE_BYTES - 9)
    {
        if(i < 60) ctx->wbuf[15] = 0;
        sha1_compile(ctx);
        i = 0;
    }
    else    /* compute a word index for the empty buffer positions  */
        i = (i >> 2) + 1;

    while(i < 14) /* and zero pad all but last two positions        */
        ctx->wbuf[i++] = 0;
    
    /* the following 32-bit length fields are assembled in the      */
    /* wrong byte order on little endian machines but this is       */
    /* corrected later since they are only ever used as 32-bit      */
    /* word values.                                                 */
    ctx->wbuf[14] = (ctx->count[1] << 3) | (ctx->count[0] >> 29);
    ctx->wbuf[15] = ctx->count[0] << 3;
    sha1_compile(ctx);

    /* extract the hash value as bytes in case the hash buffer is   */
    /* misaligned for 32-bit words                                  */
    for(i = 0; i < SHA1_DIGEST_SIZE_BYTES; ++i)
        hval[i] = (unsigned char)(ctx->hash[i >> 2] >> (8 * (~i & 3)));
}

/**
 * @brief 
 * 
 * @param in 
 * @param out 
 * @param len 
 */
void sha1(const void *in, const void *out, size_t len){
    SHA1_ctx_t ctx;
    sha1_init(&ctx); sha1_hash(&ctx, (uint8_t *)in, len); sha1_end(&ctx, (uint8_t *)out);
}
