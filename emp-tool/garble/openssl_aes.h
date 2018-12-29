#ifdef _WIN32
#pragma once



#include "emp-tool/utils/block_utils.h"



/// copy from <openssl/aes.h>
/*
 * Because array size can't be a const in C, the following two are macros.
 * Both sizes are in bytes.
 */


#ifdef  __cplusplus
extern "C" {
#endif


#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16


#define AES_ENCRYPT     1
#define AES_DECRYPT     0


#ifdef AES_LONG
typedef unsigned long u32;
#else
typedef unsigned int u32;
#endif
typedef unsigned short u16;
typedef unsigned char u8;


///* This should be a hidden type, but EVP requires that the size be known */
//struct aes_key_st {
//# ifdef AES_LONG
//    unsigned long rd_key[4 * (AES_MAXNR + 1)];
//# else
//    unsigned int rd_key[4 * (AES_MAXNR + 1)];
//# endif
//    int rounds;
//};
//
//typedef struct aes_key_st AES_KEY;


#ifdef  __cplusplus
}
#endif


namespace emp
{

//using juzix::emp::block;
/* This should be a hidden type, but EVP requires that the size be known */
struct aes_key_st {
# ifdef AES_LONG
	unsigned long rd_key[4 * (AES_MAXNR + 1)];
# else
	unsigned int rd_key[4 * (AES_MAXNR + 1)];
# endif
	int rounds;
};

typedef struct aes_key_st AES_KEY;

void
AES_set_encrypt_key(block userkey, AES_KEY *key);

void
AES_ecb_encrypt_blks(block *blks, unsigned int nblks, const AES_KEY *key);


void
AES_set_decrypt_key(block userkey, AES_KEY *key);

void
AES_ecb_decrypt_blks(block *blks, unsigned nblks, const AES_KEY *key);

}

#endif//WIN32