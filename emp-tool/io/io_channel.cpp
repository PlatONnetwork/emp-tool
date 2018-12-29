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
//#include "platon_logger.h"
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

		// int IOChannel::send_data(const void * data, int nbyte)
		// {
		// 	//LOGE("send data size: %d start...", nbyte);
		// 	auto startTime = std::chrono::high_resolution_clock::now();
		// 	int ret = send_data_impl(data, nbyte);
		// 	std::chrono::duration<int64_t, std::micro> costTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
		// 	//LOGE("send data size: %d end, cost time: %d.", nbyte, costTime.count());
		// 	io_sent_cost_time += costTime.count();
		// 	io_sent_bytes += nbyte;
		// 	if (ret != nbyte)
		// 	{
		// 		LOGW("send data exception, network error or connection closed . ret=%d", ret);
		// 		io_state = ret;
		// 		return -1;
		// 	}

		// 	++io_sent_count;
		// 	return 0;
		// }

		// int IOChannel::recv_data(void * data, int nbyte)
		// {
		// 	//LOGE("recv data size: %d start...", nbyte);
		// 	auto startTime = std::chrono::high_resolution_clock::now();
		// 	int ret = recv_data_impl(data, nbyte);
		// 	std::chrono::duration<int64_t, std::micro> costTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
		// 	//LOGE("recv data size: %d end, cost time: %d", nbyte, costTime.count());
		// 	io_recv_cost_time += costTime.count();
		// 	io_recv_bytes += nbyte;
		// 	if (ret != nbyte)
		// 	{
		// 		LOGW("recv data exception, network error or connection closed . ret=%d", ret);
		// 		io_state = ret;
		// 		return -1;
		// 	}

		// 	++io_recv_count;
		// 	return 0;
		// }

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

		//int IOChannel::send_ep(epoint **ep, int num) 
		//{
		//	uint8_t buffer[64];
		//	int ep_size = EP_SIZES;
		//	
		//	int ret = -1;
		//	for(int i = 0; i < num; ++i)
		//	{
		//	    point_to_bytes(buffer, *(ep + i));
		//		if ((ret = send_data(&ep_size, sizeof(ep_size))) != 0)
		//			return -1;
		//
		//		if ((ret = send_data(buffer, ep_size * sizeof(uint8_t))) != 0)
		//			return -1;
		//	}
		//
		//	return 0;
		//}
		//
		//int IOChannel::recv_ep(epoint **ep, int num)
		//{
		//	int recvBytes = 0;
		//	uint8_t buffer[64];
		//    int recv_size = 0;
		//
		//	int ret = 0;
		//	//1. recv epoint count
		//	//2. recv a epoint: 2.1. recv size, 2.2. recv epoint bytes
		//	for(int i = 0; i < num; ++i)
		//	{
		//		ret = recv_data(&recv_size, sizeof(int));
		//		if (ret != 0)
		//			return -1;
		//
		//		ret = recv_data(buffer, recv_size * sizeof(uint8_t));
		//		if (ret != 0)
		//			return -1;
		//
		//		bytes_to_point(*(ep + i), buffer, recv_size);
		//	}
		//
		//	return 0;
		//}

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
		int IOChannel::send_ep(void **ep, int num)
		{
			int32_t toSendLen = 0;
			int32_t pos = 0;
			epoint ** ppep = (epoint **)ep;
			//int epointBytes = get_epoints_size(ep, num);
			int32_t epointBytes = get_ep_sizes() * num;
			int32_t totalEpointDataSize = epointBytes + sizeof(int32_t)*num;

			toSendLen = sizeof(int) * 2 + totalEpointDataSize;
			uint8_t *buffer = new uint8_t[toSendLen];

			//1. send epoint count
			int32_t epoint_num = num;
			memcpy(buffer + pos, &epoint_num, sizeof(int32_t));
			//std::string epoint_num_str = getBytes(buffer, sizeof(int32_t));
			//LOGD("#####  the send epoint_num: %s ####", epoint_num_str.data());

			pos += sizeof(int32_t);

			//2. send total epoint data size
			memcpy(buffer + pos, &totalEpointDataSize, sizeof(totalEpointDataSize));
			pos += sizeof(int32_t);

			//3. send all the data, coy to buffer
			int32_t epoint_len = 64;
			for (int i = 0; i < num; ++i)
			{
				//epoint_len = get_point_size(*(ep + i));
				//LOGD("the get_point_size of i=%d is  => %d \n", i, epoint_len);
				memcpy(buffer + pos, &epoint_len, sizeof(int32_t));
				pos += sizeof(int32_t);

				point_to_bytes(buffer + pos, *(ppep + i));
				pos += epoint_len;
			}

			//4. real sending data
			if (send_data(buffer, sizeof(int32_t)) != 0)
			{
				//LOGE("send epoint list num: %d failed", num);
				delete[]buffer;
				return -1;
			}
			if (send_data(buffer + sizeof(int32_t), sizeof(int32_t)) != 0)
			{
				//LOGE("send epoint data size: %d failed", totalEpointDataSize);
				delete[]buffer;
				return -1;
			}

			if (send_data(buffer + 2 * sizeof(int32_t), totalEpointDataSize) != 0)
			{
				//LOGE("send data, length: %d failed", totalEpointDataSize);
				delete[]buffer;
				return -1;
			}

			//LOGD("### sent epoint list => %d, bytes: %d, datasize: %d \n", num, toSendLen, totalEpointDataSize);
			delete[]buffer;
			return 0;
		}

		int IOChannel::recv_ep(void **ep, int num)
		{
			//1. recv epoint count
			//unsigned char epoint_num_bf[4];
			//memset(epoint_num_bf, 0, 4);
			int32_t epoint_num = 0;
			epoint ** ppep = (epoint **)ep;
			if (recv_data(&epoint_num, sizeof(int32_t)) != 0)
			{
				//LOGW("recv epoint num failed !");
				return -1;
			}

			//std::string epoint_num_str = getBytes(epoint_num_bf, sizeof(int32_t));
			//LOGD("#####  the recv epoint_num: %s ####", epoint_num_str.data());
			//memcpy(&epoint_num, epoint_num_bf, sizeof(int32_t));

			if (epoint_num != num)
			{
				//LOGE("epoint list recv number:  %d, expect %d, should be equal !", epoint_num, num);
				return -1;
			}

			//2. recv total epoint data size
			int32_t totalEpointDataSize = 0;//contain epoint size
			if (recv_data(&totalEpointDataSize, sizeof(int32_t)) != 0)
			{
				//LOGW("recv total epoint data size failed !");
				return -1;
			}
			uint8_t* buffer = new uint8_t[totalEpointDataSize];

			//3. recv all the epoint data
			if (recv_data(buffer, totalEpointDataSize) != 0)
			{
				//LOGE("recv batch epoint list num: %d failed", num);
				delete[]buffer;
				return -1;
			}
			//LOGD("###  recv epoint list => %d, %d bytes  ", num, totalEpointDataSize + 2 * sizeof(int32_t));

			int32_t epoint_len = 0;
			int32_t pos = 0;
			//4. parse to epoint
			for (int i = 0; i < num; ++i)
			{
				memcpy(&epoint_len, buffer + pos, sizeof(int32_t));
				pos += sizeof(int32_t);

				bytes_to_point(*(ppep + i), buffer + pos, epoint_len);
				pos += epoint_len;
			}

			delete[]buffer;
			return 0;
		}

		int IOChannel::send_bn(const void* bn, int num)
		{
			const big* pbn = (const big*)bn;
			int totalBigDataLen = get_bigs_size(pbn, num) + sizeof(int) * num;
			int pos = 0;
			int toSendLen = sizeof(int) + totalBigDataLen;
			uint8_t *buffer = new uint8_t[toSendLen];

			//1. copy big num
			memcpy(buffer + pos, &num, sizeof(int));
			pos += sizeof(int);

			//2. copy big data size
			memcpy(buffer + pos, &totalBigDataLen, sizeof(int));
			pos += sizeof(int);

			//3. copy big data list
			int big_len = 0;
			for (int i = 0; i < num; ++i)
			{
				big_len = get_byte_size(pbn[i]);
				memcpy(buffer + pos, &big_len, sizeof(int));
				pos += sizeof(int);

				big_to_bytes(big_len, pbn[i], (char*)buffer + pos, 0);
				pos += big_len;
			}

			if (send_data(buffer, toSendLen) != 0)
			{
				//LOGE("send batch big list num: %d failed", num);
				delete[]buffer;
				return -1;
			}

			delete[]buffer;
			return 0;
		}

		int IOChannel::recv_bn(void* bn, int num)
		{
			big* pbn = (big*)bn;
			int bigNum = 0;
			int bigDataSize = 0;

			//1. recv big num
			if (recv_data(&bigNum, sizeof(int)) != 0)
				return -1;
			if (num != bigNum)
			{
				//LOGE("the big num is not equal, recv num: %d, expect num: %d", bigNum, num);
				return -1;
			}

			//2. recv big data size
			if (recv_data(&bigDataSize, sizeof(int)) != 0)
				return -1;
			uint8_t *buffer = new uint8_t[bigDataSize];

			if (recv_data(buffer, bigDataSize) != 0)
			{
				//LOGE("recv batch big list num: %d failed", num);
				delete[]buffer;
				return -1;
			}

			int bigSize = 0;
			int pos = 0;
			for (int i = 0; i < num; ++i)
			{
				memcpy(&bigSize, buffer + pos, sizeof(int));
				pos += sizeof(int);

				bytes_to_big(bigSize, (const char*)buffer + pos, pbn[i]);
				pos += bigSize;
			}

			delete[]buffer;
			return 0;
		}

#endif//OT_NP_USE_MIRACL

}//emp



