#include "emp-tool/io/io_channel.h"
#include <cstddef>
#include <stdint.h>
#include <string>

#ifndef OT_NP_USE_MIRACL
#include "emp-tool/utils/utils_ec.h"
#else
#include "emp-tool/utils/miracl_utils.h"
#endif

#include "emp-tool/utils/prg.h"
#include "emp-tool/utils/block_utils.h"
#include "emp-tool/utils/prg.h"
#include <chrono>



namespace emp
{


		IOChannel::IOChannel() : prg(nullptr)
		{}

		IOChannel::~IOChannel()
		{
			if (prg != nullptr)
				delete (PRG*)prg;
		}


		int IOChannel::send_block(const block* data, int nblock)
		{
			return send_data(data, nblock * sizeof(block));
		}

		int IOChannel::recv_block(block* data, int nblock)
		{
			return recv_data(data, nblock * sizeof(block));
		}

		void IOChannel::set_key(const block* b)
		{
			if (b == nullptr)
			{
				if (prg)
				{
					delete (PRG*)prg;
					prg = nullptr;
				}
				return;
			}

			if (prg != nullptr)
				delete (PRG*)prg;

			prg = new PRG(b);
		}

		int IOChannel::send_data_enc(const void * data, int len)
		{
			char * tmp = new char[len];
			PRG* ptrPrg = (PRG*)prg;
			ptrPrg->random_data(tmp, len);
			for (int i = 0; i < len; ++i)
				tmp[i] ^= ((char *)data)[i];
			int sendBytes = send_data(tmp, len);
			delete[] tmp;

			return sendBytes;
		}

		int IOChannel::send_block_enc(const block* data, int len)
		{
			block * tmp = new block[len];
			PRG* ptrPrg = (PRG*)prg;
			ptrPrg->random_block(tmp, len);
			for (int i = 0; i < len; ++i)
				tmp[i] = xorBlocks(data[i], tmp[i]);

			int sentBytes = send_block(tmp, len);
			delete[] tmp;
			return sentBytes;
		}

		int IOChannel::recv_data_enc(void * data, int len)
		{
			int recvBytes = recv_data(data, len);
			if (prg == nullptr)
				return -1;
			char * tmp = new char[len];
			PRG* ptrPrg = (PRG*)prg;
			ptrPrg->random_data(tmp, len);
			for (int i = 0; i < len; ++i)
				((char *)data)[i] ^= tmp[i];

			delete[] tmp;
			return recvBytes;
		}

		int IOChannel::recv_block_enc(block* data, int len)
		{
			int ret = recv_block(data, len);
			if (ret != 0)
				return -1;

			if (prg == nullptr)
				return -1;

			block * tmp = new block[len];
			PRG* ptrPrg = (PRG*)prg;
			ptrPrg->random_block(tmp, len);
			for (int i = 0; i < len; ++i)
				data[i] = xorBlocks(data[i], tmp[i]);
			delete[] tmp;

			return 0;
		}

#ifndef OT_NP_USE_MIRACL
		int IOChannel::send_eb(const void * eb, size_t num)
		{
			uint8_t buffer[EB_SIZE];
			//	printf("*****   send_eb: ");
			//	printData(eb, num);
			const eb_t* peb = (const eb_t*)eb;
			for (size_t i = 0; i < num; ++i)
			{
				int eb_size = eb_size_bin(peb[i], ECC_PACK);
				eb_write_bin(buffer, eb_size, peb[i], ECC_PACK);
				send_data(&eb_size, sizeof(int));
				send_data(buffer, eb_size * sizeof(uint8_t));
			}

			return 0;
		}

		int IOChannel::recv_eb(void* eb, size_t num)
		{
			uint8_t buffer[EB_SIZE];
			int eb_size;
			eb_t* peb = (eb_t*)eb;
			for (size_t i = 0; i < num; ++i) {
				recv_data(&eb_size, sizeof(int));
				recv_data(buffer, eb_size * sizeof(uint8_t));
				eb_read_bin(peb[i], buffer, eb_size);
			}

			//	printf("*****   recv_eb: ");
			//	printData(eb, num);

			return 0;
		}

		int IOChannel::send_bn(const void * bn, size_t num)
		{
			dig_t buffer[4];
			int bn_size;
			const bn_t* pbn = (const bn_t*)bn;
			for (size_t i = 0; i < num; ++i)
			{
				bn_size = bn_size_raw(pbn[i]);
				bn_write_raw(buffer, bn_size, pbn[i]);
				send_data(&bn_size, sizeof(int));
				send_data(buffer, bn_size * sizeof(uint64_t));
			}

			return 0;
		}

		int IOChannel::recv_bn(void* bn, size_t num)
		{
			dig_t buffer[4];
			int bn_size;
			bn_t* pbn = (bn_t*)bn;
			for (size_t i = 0; i < num; ++i)
			{
				recv_data(&bn_size, sizeof(int));
				recv_data(buffer, bn_size * sizeof(uint64_t));
				bn_read_raw(pbn[i], buffer, bn_size);
			}

			return 0;
		}

#else

void IOChannel::send_ep(void *ep, size_t num) 
{
	uint8_t buffer[64];
	int ep_size = 64;
	epoint* ptr = (epoint*)ep;
	for(size_t i = 0; i < num; ++i) 
	{
	    point_to_bytes(buffer, ptr + i);
		send_data(&ep_size, sizeof(ep_size));
		send_data(buffer, ep_size * sizeof(uint8_t));
	}
}

void IOChannel::recv_ep(void *ep, size_t num) 
{
	uint8_t buffer[64];
    int recv_size = 0;
    epoint* ptr = (epoint*)ep;
	for(size_t i = 0; i < num; ++i) 
	{
		recv_data(&recv_size, sizeof(int));
		recv_data(buffer, recv_size * sizeof(uint8_t));
		
		bytes_to_point(ptr + i, buffer, recv_size);
	}

}

void IOChannel::send_bn(const void* bn, size_t num) 
{
	uint8_t buffer[32];
	int bn_size;
	big* ptr = (big*)bn;
	for(size_t i = 0; i < num; ++i) 
	{
		bn_size = get_byte_size(*(ptr+i));
		big_to_bytes(bn_size, *(ptr+i), (char*)buffer, 1);
		send_data(&bn_size, sizeof(int));
		send_data(buffer, bn_size * sizeof(uint8_t));
	}
}

void IOChannel::recv_bn(void* bn, size_t num) 
{
	uint8_t buffer[32];
	int bn_size;
	big* ptr = (big*)bn;
	for(size_t i = 0; i < num; ++i) 
	{
		recv_data(&bn_size, sizeof(int));
		recv_data(buffer, bn_size * sizeof(uint8_t));
		bytes_to_big(bn_size, (const char*)buffer, *(ptr+i));
	}
}

#endif//OT_NP_USE_MIRACL

}//emp


