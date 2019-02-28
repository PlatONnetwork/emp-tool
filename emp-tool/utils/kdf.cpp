#include "emp-tool/utils/kdf.h"
#include <cstring>



//rotate left(circular left shift) operation
#define ROTL(value, bits)	(((value) << (bits)) | ((value) >> (32-bits)))
//rotate right(circular right shift) operation
#define ROTR(value, bits)	(((value) >> (bits)) | ((value) << (32-bits)))
//right left operation
#define SHL(value, bits)	((value) << (bits))
//right shift operation
#define SHR(value, bits)	((value) >> (bits))

#define P0(x) ((x) ^ ROTL(x,  9) ^ ROTL(x, 17))
#define P1(x) ((x) ^ ROTL(x, 15) ^ ROTL(x, 23))

#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#define MIN(a, b) ((a) < (b)) ? (a) : (b)



namespace emp
{

static void uint32_to_uint8(uint8_t *output, const uint32_t *input, const size_t len)
{
	for(size_t i = 0, j = 0; j < len; i++, j += 4) 
		for (int k = 0; k < 4; k++)
			output[j + k] = (uint8_t)(input[i] >> (24 - 8 * k) & 0xFF);
}

static void uint8_to_uint32(uint32_t *output, const uint8_t *input, const size_t len)
{
	for(size_t i = 0, j = 0; j < len; i++, j += 4)
	{ 
		output[i] = 0;
		for (int k = 0; k < 4; k++)
			output[i] |= ((uint32_t)input[j + k]) << (24 - 8 * k);
	}
}



static uint32_t FF(uint32_t x, uint32_t y, uint32_t z, int j)
{
	if(j < 16)
		return x ^ y ^ z;
	else
		return (x & y) | (x & z) | (y & z);
}

static uint32_t GG(uint32_t x, uint32_t y, uint32_t z, int j)
{
	if(j < 16)
		return x ^ y ^ z;
	else
		return (x & y) | (~x & z);
}

static void SM3_compress(SM3_STATE *ctx)
{
	uint32_t A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2, T, W[68], W_[64];	
	uint32_t t1,t2,t3;
	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

	uint8_to_uint32(W, ctx->buf, 64);
	for(int j = 0; j < 68; j++)
	{
		if(j < 16)
			W[j] = W[j];
		else
			W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTL(W[j - 3], 15)) ^ ROTL(W[j - 13], 7) ^ W[j - 6];
	}
	for(int j = 0; j < 64; j++)
		W_[j] = W[j] ^ W[j + 4];

	for(int j = 0; j < 64; j++)
	{
		if(j < 16)
			T = 0x79cc4519;
		else
			T = 0x7a879d8a;
		SS1 = ROTL(ROTL(A, 12) + E + ROTL(T, j), 7);
		SS2 = SS1 ^ ROTL(A, 12);
		TT1 = FF(A, B, C, j) + D + SS2 + W_[j];
		t1 = GG(E, F, G, j);
		t2 = H + SS1;
		TT2 = t1 + t2 + W[j];
		D = C;
		C = ROTL(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F, 19);
		F = E;
		E = P0(TT2);


	}

	ctx->state[0] ^= A;
	ctx->state[1] ^= B;
	ctx->state[2] ^= C;
	ctx->state[3] ^= D;
	ctx->state[4] ^= E;
	ctx->state[5] ^= F;
	ctx->state[6] ^= G;
	ctx->state[7] ^= H;
	ctx->curlen	   = 0;
}

int SM3_Init(SM3_STATE *ctx)
{
	ctx->state[0] = 0x7380166f;
	ctx->state[1] = 0x4914b2b9;
	ctx->state[2] = 0x172442d7;
	ctx->state[3] = 0xda8a0600;
	ctx->state[4] = 0xa96f30bc;
	ctx->state[5] = 0x163138aa;
	ctx->state[6] = 0xe38dee4d;
	ctx->state[7] = 0xb0fb0e4e;
	ctx->length[0] = 0;
	ctx->length[1] = 0;
	ctx->curlen = 0;
	memset(ctx->buf, 0, 64);
	return 1;
}

int SM3_Update(SM3_STATE *ctx, const unsigned char *data, size_t len)
{
	size_t n;
	ctx->length[0] += len >> 29;
	ctx->length[1] += len << 3;	
	if (ctx->length[1] < (len << 3))
		ctx->length[0]++;

	while (len > 0)
	{
		n = MIN(len, (64 - ctx->curlen));
		memcpy(ctx->buf + ctx->curlen, data, n);
		ctx->curlen += n;
		data += n;
		len -= n;

		if (ctx->curlen == 64)
			SM3_compress(ctx);
	}
	return 1;
}

int SM3_Final(unsigned char *md, SM3_STATE *ctx)
{
	uint8_t length[8];
	const uint8_t PAD[64] = { 0x80 };
	uint32_to_uint8(length, ctx->length, 8);
	memcpy(ctx->buf + ctx->curlen, PAD, 64 - ctx->curlen);
	if (ctx->curlen >= 56)
	{		
		SM3_compress(ctx);
		memset(ctx->buf, 0, 56);
	}	
	memcpy(ctx->buf + 56, length, 8);
	SM3_compress(ctx);
	uint32_to_uint8(md, ctx->state, SM3_DIGEST_SIZE);
	return 1;
}

unsigned char *SM3_256(unsigned char *md, const unsigned char *data, size_t len)
{
	SM3_STATE ctx;
	if (!SM3_Init(&ctx))
		return NULL;
	if (!SM3_Update(&ctx, data, len))
		return NULL;
	if (!SM3_Final(md, &ctx))
		return NULL;
	return md;
}
void SM3_KDF(unsigned char Z[], unsigned short zlen, unsigned short klen, unsigned char K[])
{
	unsigned short i, j, t;
	unsigned int bitklen;
	unsigned char Ha[SM3_len / 8];
	unsigned char ct[4] = { 0,0,0,1 };
	SM3_STATE md;
	bitklen = klen * 8;
	if (bitklen%SM3_len)
		t = bitklen / SM3_len + 1;
	else
		t = bitklen / SM3_len;
	//s4: K=Ha1||Ha2||...
	for (i = 1; i<t; i++)
	{
		//s2: Hai=Hv(Z||ct)
		SM3_Init(&md);
		SM3_Update(&md, Z, zlen);
		SM3_Update(&md, ct, 4);
		SM3_Final(Ha, &md);
		memcpy((K + (SM3_len / 8)*(i - 1)), Ha, SM3_len / 8);
		if (ct[3] == 0xff)
		{
			ct[3] = 0;
			if (ct[2] == 0xff)
			{
				ct[2] = 0;
				if (ct[1] == 0xff)
				{
					ct[1] = 0;
					ct[0]++;
				}
				else ct[1]++;
			}
			else ct[2]++;
		}
		else ct[3]++;
	}
	//s3: klen/v �������Ĵ���
	SM3_Init(&md);
	SM3_Update(&md, Z, zlen);
	SM3_Update(&md, ct, 4);
	SM3_Final(Ha, &md);
	if (bitklen%SM3_len)
	{
		i = (SM3_len - bitklen + SM3_len*(bitklen / SM3_len)) / 8;
		j = (bitklen - SM3_len*(bitklen / SM3_len)) / 8;
		memcpy((K + (SM3_len / 8)*(t - 1)), Ha, j);
	}
	else
	{
		memcpy((K + (SM3_len / 8)*(t - 1)), Ha, SM3_len / 8);
	}
}

}//emp