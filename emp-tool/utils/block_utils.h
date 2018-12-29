#ifndef BLOCK_UTILS_H
#define BLOCK_UTILS_H

//#include "platon_core_conf.h"
#include "block.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>


namespace emp {

		extern const char fix_key[];

		bool getLSB(const block & x);

		block makeBlock(int64_t x, int64_t y);

		block zero_block();

		block one_block();


		block xorBlocks(block x, block y);

		block andBlocks(block x, block y);

		block make_delta(const block & a);

		void xorBlocks_arr(block* res, const block* x, const block* y, int nblocks);

		void xorBlocks_arr(block* res, const block* x, block y, int nblocks);

		bool cmpBlock(const block * x, const block * y, int nblocks);

		//deprecate soon
		bool block_cmp(const block * x, const block * y, int nblocks);

		bool isZero(const block * b);

		bool isOne(const block * b);

		std::string toHex(const std::vector<unsigned char> input);

		void printBlock(const block* b, int num);
		void printBools(const bool* b, int num);

		template<class T>
		void printData(T* b)
		{
			for (int i = 0; i < sizeof(*b); ++i)
			{
				printf("%02X", *(((unsigned char*)b) + i) & 0xff);

			}
			printf(",");
		}

		template<class T>
		void printData(T* b, int num)
		{
			for (int i = 0; i < num; ++i)
			{
				printData(b + i);
			}
			printf("\n");
		}


		std::string getBytes(void* b, int num);

		inline const uint8_t* input_column(const uint8_t *input, int x, int y, int ncols)
		{
			return (const uint8_t*)&input[(x)* ncols / 8 + (y) / 8];
		}

		inline uint8_t* output_row(uint8_t *output, int x, int y, int nrows)
		{
			return (uint8_t*)&output[(y)* nrows / 8 + (x) / 8];
		}


		//Modified from
		//https://mischasan.wordpress.com/2011/10/03/the-full-sse2-bit-matrix-transpose-routine/
		// with inner most loops changed to _mm_set_epi8 and _mm_set_epi16
		void sse_trans(uint8_t *out, uint8_t const *inp, int nrows, int ncols);

		block double_block(block bl);

		void mul128(__m128i a, __m128i b, __m128i *res1, __m128i *res2);

}//emp

#endif // BLOCK_UTILS_H