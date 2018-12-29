#ifdef OT_NP_USE_MIRACL

#ifndef MIRACL_HASH_H__
#define MIRACL_HASH_H__


#include <stdio.h>
#include <string>

#include "block.h"
#include "constants.h"

#include "emp-tool/utils/kdf.h"
#include "emp-tool/utils/miracl_utils.h"

using std::string;

namespace emp
{


		class Hash
		{
		public:
			SM3_STATE hash;
			char buffer[HASH_BUFFER_SIZE];
			int size = 0;
			static const int DIGEST_SIZE = SM3_DIGEST_SIZE;//20;


			Hash();

			~Hash();

			void put(const void * data, int nbyte);

			void put_block(const block* block, int nblock = 1);

			void digest(char * a);

			void reset();

			static void hash_once(void * digest, const void * data, int nbyte);
			static string hash_once_hex(const void* data, int nbytes);

			static unsigned char* hash_file(unsigned char* output, const char* file);

			static block hash_for_block(const void * data, int nbyte);

			static block hash_for_epoint(epoint *in);

			void put_epoint(epoint **ep, int length);
		};


}//emp

#endif // MIRACL_HASH_H__

#endif//OT_NP_USE_MIRACL
