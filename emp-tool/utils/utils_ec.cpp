#ifndef OT_NP_USE_MIRACL

#include "emp-tool/utils/utils_ec.h"
#include "emp-tool/utils/hash.h"
#include <mutex>


namespace emp {


static bool gs_initialized = false;
static std::mutex gs_relic_lock;


block KDF(eb_t in) 
{
	uint8_t tmp[100];
	int eb_size = eb_size_bin(in, ECC_PACK);
	eb_write_bin(tmp, eb_size, in, ECC_PACK);
	return Hash::hash_for_block((const void*)tmp, eb_size);
}

void bn_to_block(block * b, const bn_t bn) 
{
	bn_write_bin((uint8_t*)b, sizeof(block), bn);
}

void block_to_bn(bn_t bn, const block * b) 
{
	bn_read_bin(bn, (const uint8_t*)b, sizeof(block));
}

int initialize_relic() 
{
    std::lock_guard<std::mutex> lock(gs_relic_lock);
	if(gs_initialized) 
	    return 0;
	
	gs_initialized = true;
	if (core_init() != STS_OK) 
	{
		core_clean();
		return -1;
	}

	eb_param_set(EBACS_B251);
	return 0;
}

}//emp

#endif
