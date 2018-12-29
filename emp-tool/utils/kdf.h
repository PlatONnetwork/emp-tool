#ifndef KDF_H__
#define KDF_H__




#include <cstdint>
#include <cstddef>


#define SM3_DIGEST_SIZE 32
#define SM3_len 256


namespace emp
{

typedef struct sm3_state_st
{
	uint32_t length[2];
	uint32_t state[8];
	size_t curlen;
	unsigned char buf[64];
} SM3_STATE;


int SM3_Init(SM3_STATE *ctx);
int SM3_Update(SM3_STATE *ctx, const unsigned char *data, size_t len);
int SM3_Final(unsigned char *md, SM3_STATE *ctx);
unsigned char *SM3_256(unsigned char *md, const unsigned char *data, size_t len);
void SM3_KDF(unsigned char Z[], unsigned short zlen, unsigned short klen, unsigned char K[]);

}



#endif // KDF_H__

