#ifndef BLOCK_H__
#define BLOCK_H__

////////////////////////////////////////

#ifndef __USE_MM_FUNC__ // default is use sse instruction
#include <emmintrin.h>
#endif//

namespace emp {

#ifndef __USE_MM_FUNC__ // default is use sse instruction

typedef __m128i block;
typedef __m128i block_tpl[2];

#else
#ifndef __ARM_NEON__

//block definition
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
	struct block
	{
		unsigned char data[16];//or 16*8 bits, unsigned char[16];
	};
#else
	#include <emmintrin.h>
	typedef __m128i block;
#endif


#ifdef __cplusplus
}
#endif

typedef block block_tpl[2];

#else
#include "arm_neon.h"

typedef float32x4_t __m128;
typedef int32x4_t __m128i;
typedef int32x4_t block;
typedef int32x4_t block_tpl[2];
#endif//__ARM_NEON__

#endif//__USE_MM_FUNC__

}//emp

#ifdef _WIN32
#define not !
#define and &&
#define or ||
#define xor ^
#endif


#endif // BLOCK_H__