#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/miracl_hash.h"
#include "emp-tool/utils/sse.h"
#include <fstream>



namespace emp
{


Hash::Hash() 
{
	SM3_Init(&hash);
}

Hash::~Hash() 
{
}

void Hash::put(const void * data, int nbyte) 
{
    if(nbyte > HASH_BUFFER_SIZE)
        SM3_Update(&hash, (unsigned char*)data, nbyte);  // TODO
    else if(size + nbyte < HASH_BUFFER_SIZE) {
        memcpy(buffer + size, data, nbyte);
        size += nbyte;
    } else {
        SM3_Update(&hash, (unsigned char*)buffer, size);   // TODO
        memcpy(buffer, data, nbyte);
        size = nbyte;
    }

}
void Hash::put_block(const block* block, int nblock /*=1*/)
{
	put(block, sizeof(block)*nblock);
}

void Hash::digest(char * a)
{
    if(size > 0) 
    {
        SM3_Update(&hash, (unsigned char*)buffer, size);
        size = 0;
    }
    SM3_Final((unsigned char *)a, &hash);
}

void Hash::reset() 
{
    SM3_Init(&hash);
    size = 0;
}

void Hash::hash_once(void * digest, const void * data, int nbyte) 
{
    (void)SM3_256((unsigned char *)digest, (unsigned char *)data, nbyte);
}

string Hash::hash_once_hex(const void * data, int nbytes)
{
	string inputHashHex;
	unsigned char szHash[64];
	Hash::hash_once(szHash, data, nbytes);
	char szHex[4];
	for (int i = 0; i < 32; ++i)
	{
		snprintf(szHex, 4, "%02X", szHash[i]);
		inputHashHex.append(szHex, 2);
	}

	return inputHashHex;
}

unsigned char* Hash::hash_file(unsigned char* output, const char* file)
{
	std::ifstream  fin(file, std::fstream::binary);
	if (!fin.good())
	{
		return NULL;
	}

	SM3_STATE ctx;
	if (!SM3_Init(&ctx))
	{
		fin.close();
		return NULL;
	}
	
	char sz32Bytes[32] = {0};
	int count = 0;
	int read = 0;
	while (!fin.eof()) {
		fin.read(sz32Bytes, 32);
		read = fin.gcount();
		count += read;
		if (!SM3_Update(&ctx, (unsigned char*)sz32Bytes, read))
		{
			fin.close();
			return NULL;
		}
	}
	
	if (!SM3_Final(output, &ctx))
	{
		fin.close();
		return NULL;
	}

	fin.close();
	return output;
}

block Hash::hash_for_block(const void * data, int nbyte) 
{
	char digest[256] = {0};//20 -> 256
	memset(digest, 0, sizeof(digest));
	
	hash_once(digest, data, nbyte);
	return juzix_mm_load_si128((block*)&digest[0]);
}


block Hash::hash_for_epoint(epoint *in) 
{
	uint8_t buffer[256];
	memset(buffer, 0, sizeof(buffer));
	
	int ep_size = get_point_size(in);
	point_to_bytes(buffer, in);
	return hash_for_block(buffer, ep_size);
}

void Hash::put_epoint(epoint **ep, int length) 
{
	uint8_t buffer[256];
	memset(buffer, 0, sizeof(buffer));
	
	int len_x = 0;
	int len_y = 0;
	for(int i = 0; i < length; ++i) {
		point_to_bytes(buffer, ep[i]);
		put(buffer, len_x + len_y);
	}
}

}//emp

#endif//OT_NP_USE_MIRACL
