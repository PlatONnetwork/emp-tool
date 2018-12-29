#pragma once


#include "block.h"
#include <cstdint>

namespace emp
{
		class IOChannel 
		{
		public:
			IOChannel();
			virtual ~IOChannel();

			virtual int send_data(const void * data, int nbyte) = 0;
			virtual int recv_data(void * data, int nbyte) = 0;

			virtual void sync() {};
			virtual void setNoDelay(bool flag) {};

			virtual void flush() {};

			// int send_data(const void * data, int nbyte);
			// int recv_data(void* data, int nbytes);

			int send_block(const block* data, int nblock);
			int recv_block(block* data, int nblock);

			
			void set_key(const block* b);
			int send_data_enc(const void * data, int len);

			int send_block_enc(const block* data, int len);

			int recv_data_enc(void * data, int len);

			int recv_block_enc(block* data, int len);

#ifndef OT_NP_USE_MIRACL
			int send_eb(const void * eb, size_t num);

			int recv_eb(void* eb, size_t num);

			int send_bn(const void * bn, size_t num);

			int recv_bn(void* bn, size_t num);
#else//miracl
			int send_ep(void **ep, int num);

			int recv_ep(void **ep, int num);

			int send_bn(const void* bn, int num);

			int recv_bn(void* bn, int num);
#endif//JUZIX_OT_USE_RELIC


			// int get_io_count() const { return io_recv_count + io_sent_count; };

			// int get_io_recv_count() const { return io_recv_count; };

			// int get_io_sent_count() const { return io_sent_count; };

			// int get_io_state() const { return io_state; };

			// int64_t get_io_time() const { return io_recv_cost_time + io_sent_cost_time; };

			// int64_t get_io_sent_time() const { return io_sent_cost_time; };

			// int64_t get_io_recv_time() const { return io_recv_cost_time; };

			// int64_t get_io_sentbytes() const { return io_sent_bytes; };

			// int64_t get_io_recvbytes() const { return io_recv_bytes; };

			// void set_io_state(int state) { io_state = state; };

		protected:
			void *prg = nullptr;//Prg
			// int	io_sent_count = 0;
			// int io_recv_count = 0;
			// int64_t io_sent_cost_time = 0;
			// int64_t io_recv_cost_time = 0;
			// int64_t io_sent_bytes = 0;
			// int64_t io_recv_bytes = 0;
			int io_state = 0;//0: good, -2: timeout, -1: error
		};

}//emp
