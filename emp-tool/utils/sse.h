#pragma once

#include "block.h"
#include <cstddef>
#include <cstdint>

////////////////////////////////////////

using emp::block;

#ifndef __USE_MM_FUNC__

#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
//#include <wmmintrin.h>

#else

#ifdef __ARM_NEON__
#include "arm_neon.h"

// ******************************************
// type-safe casting between types
// ******************************************

#define vreinterpretq_m128_f16(x) \
    vreinterpretq_f32_f16(x)

#define vreinterpretq_m128_f32(x) \
    (x)

#define vreinterpretq_m128_f64(x) \
    vreinterpretq_f32_f64(x)


#define vreinterpretq_m128_u8(x) \
    vreinterpretq_f32_u8(x)

#define vreinterpretq_m128_u16(x) \
    vreinterpretq_f32_u16(x)

#define vreinterpretq_m128_u32(x) \
    vreinterpretq_f32_u32(x)

#define vreinterpretq_m128_u64(x) \
    vreinterpretq_f32_u64(x)


#define vreinterpretq_m128_s8(x) \
    vreinterpretq_f32_s8(x)

#define vreinterpretq_m128_s16(x) \
    vreinterpretq_f32_s16(x)

#define vreinterpretq_m128_s32(x) \
    vreinterpretq_f32_s32(x)

#define vreinterpretq_m128_s64(x) \
    vreinterpretq_f32_s64(x)


#define vreinterpretq_f16_m128(x) \
    vreinterpretq_f16_f32(x)

#define vreinterpretq_f32_m128(x) \
    (x)

#define vreinterpretq_f64_m128(x) \
    vreinterpretq_f64_f32(x)


#define vreinterpretq_u8_m128(x) \
    vreinterpretq_u8_f32(x)

#define vreinterpretq_u16_m128(x) \
    vreinterpretq_u16_f32(x)

#define vreinterpretq_u32_m128(x) \
    vreinterpretq_u32_f32(x)

#define vreinterpretq_u64_m128(x) \
    vreinterpretq_u64_f32(x)


#define vreinterpretq_s8_m128(x) \
    vreinterpretq_s8_f32(x)

#define vreinterpretq_s16_m128(x) \
    vreinterpretq_s16_f32(x)

#define vreinterpretq_s32_m128(x) \
    vreinterpretq_s32_f32(x)

#define vreinterpretq_s64_m128(x) \
    vreinterpretq_s64_f32(x)


#define vreinterpretq_m128i_s8(x) \
    vreinterpretq_s32_s8(x)

#define vreinterpretq_m128i_s16(x) \
    vreinterpretq_s32_s16(x)

#define vreinterpretq_m128i_s32(x) \
    (x)

#define vreinterpretq_m128i_s64(x) \
    vreinterpretq_s32_s64(x)


#define vreinterpretq_m128i_u8(x) \
    vreinterpretq_s32_u8(x)

#define vreinterpretq_m128i_u16(x) \
    vreinterpretq_s32_u16(x)

#define vreinterpretq_m128i_u32(x) \
    vreinterpretq_s32_u32(x)

#define vreinterpretq_m128i_u64(x) \
    vreinterpretq_s32_u64(x)


#define vreinterpretq_s8_m128i(x) \
    vreinterpretq_s8_s32(x)

#define vreinterpretq_s16_m128i(x) \
    vreinterpretq_s16_s32(x)

#define vreinterpretq_s32_m128i(x) \
    (x)

#define vreinterpretq_s64_m128i(x) \
    vreinterpretq_s64_s32(x)


#define vreinterpretq_u8_m128i(x) \
    vreinterpretq_u8_s32(x)

#define vreinterpretq_u16_m128i(x) \
    vreinterpretq_u16_s32(x)

#define vreinterpretq_u32_m128i(x) \
    vreinterpretq_u32_s32(x)

#define vreinterpretq_u64_m128i(x) \
    vreinterpretq_u64_s32(x)

#endif //__ARM_NEON__

#endif//__USE_MM_FUNC__



/////////  function declaration /////////////
//_mm_slli_epi64(B,1)
block juzix_func_mm_slli_epi64(block b, int count);

//_mm_slli_epi32
block juzix_func_mm_slli_epi32(block b, int count);

//_mm_setzero_si128()
block juzix_func_mm_setzero_si128();

//_mm_loadu_si128
block juzix_func_mm_loadu_si128(block* b);

//_mm_loadl_epi64
block juzix_func_mm_loadl_epi64(block* id);

//_mm_xor_si128
block juzix_func_mm_xor_si128(block x, block y);

//_mm_and_si128
block juzix_func_mm_and_si128(block x, block y);
//_mm_or_si128
block juzix_func_mm_or_si128(block x, block y);

//_mm_set_epi8(ch17,ch16..., ch0)
block juzix_func_mm_set_epi8(char b15, char b14, char b13, char b12, char b11, char b10, char b9, char b8, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0);


//_mm_set_epi16(b7,b6,...,b0)
block juzix_func_mm_set_epi16(short b7, short b6, short b5, short b4, short b3, short b2, short b1, short b0);

//_mm_set_epi32
block juzix_func_mm_set_epi32(int i3, int i2, int i1, int i0);

//_mm_set_epi64((__m64)(X), (__m64)(Y))
block juzix_func_mm_set_epi64(uint64_t i1, uint64_t i0);

//_mm_movemask_epi8
int juzix_func_mm_movemask_epi8(block b);

//_mm_srai_epi32
block juzix_func_mm_srai_epi32(block a, int count);


///////     exception functions //////////////////
block juzix_func_mm_and_si128(block x, block y);

// _mm_testz_si128(__m128i s1, __m128i s2);
/*
int _mm_testz_si128(__m128i s1, __m128i s2);

Returns '1' if the bitwise AND operation on s1 and s2 results in all zeros, else returns '0'. That is,

_mm_testz_si128 := ( (s1 & s2) == 0 ? 1 : 0 )

**/
int juzix_func_mm_testz_si128(block x, block y);//exception in redhat or centos linux

//_mm_set_epi64x((__m64)(X), (__m64)(Y))
block juzix_func_mm_set_epi64x(uint64_t i1, uint64_t i0);

//_mm_load_si128
block juzix_func_mm_load_si128(block* X);
///////     exception functions //////////////////




////////////////////////////////////////
////sample example of the mm_xxx macro and functions
#ifndef __USE_MM_FUNC__//使用intel内置指令
/////////////////// mm_ins //////////
/*
int _mm_testz_si128(__m128i s1, __m128i s2);

Returns '1' if the bitwise AND operation on s1 and s2 results in all zeros, else returns '0'. That is,

_mm_testz_si128 := ( (s1 & s2) == 0 ? 1 : 0 )

**/
#define juzix_mm_testz_si128(x,y) juzix_func_mm_testz_si128(x,y)

//_mm_slli_epi64
#define juzix_mm_slli_epi64(X,N) _mm_slli_epi64(X,N)
//_mm_slli_epi32
#define juzix_mm_slli_epi32(X,N) _mm_slli_epi32(X,N)
//_mm_setzero_si128
#define juzix_mm_setzero_si128() _mm_setzero_si128()
//_mm_load_si128
#define juzix_mm_load_si128(x) _mm_load_si128((block*)x)
//_mm_loadu_si128
#define juzix_mm_loadu_si128(x) _mm_loadu_si128((block*)x)

//_mm_loadl_epi64
#define juzix_mm_loadl_epi64(x) _mm_loadl_epi64((block*)x)

//_mm_xor_si128
#define juzix_mm_xor_si128(x,y) _mm_xor_si128(x,y)

//_mm_and_si128
#define juzix_mm_and_si128(x,y) _mm_and_si128(x,y)

//_mm_or_si128
#define juzix_mm_or_si128(x,y) _mm_or_si128(x,y)

//_mm_srai_epi32
#define juzix_mm_srai_epi32(x, n) _mm_srai_epi32(x, n)

//_mm_set_epi8(ch17,ch16..., ch0)
#define juzix_mm_set_epi8(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) _mm_set_epi8(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)

//_mm_set_epi16(b7,b6,...,b0)
#define juzix_mm_set_epi16(b7,b6,b5,b4,b3,b2,b1,b0) _mm_set_epi16(b7,b6,b5,b4,b3,b2,b1,b0)

//_mm_set_epi32
#define juzix_mm_set_epi32(b3,b2,b1,b0) _mm_set_epi32(b3,b2,b1,b0)

//_mm_set_epi64
#define juzix_mm_set_epi64(X,Y)	juzix_func_mm_set_epi64(X,Y)

//_mm_set_epi64x
#define juzix_mm_set_epi64x(X,Y) juzix_func_mm_set_epi64(X, Y)

//_mm_movemask_epi8
#define juzix_mm_movemask_epi8(x) _mm_movemask_epi8(x)

/////////////////// mm_ins //////////

#else
////////  implement with funciton //////////////

//_mm_slli_epi64
#define juzix_mm_slli_epi64(X,N) juzix_func_mm_slli_epi64(X,N)

//_mm_slli_epi32
#define juzix_mm_slli_epi32(X,N) juzix_func_mm_slli_epi32(X,N)

//_mm_setzero_si128
#define juzix_mm_setzero_si128() juzix_func_mm_setzero_si128()

//_mm_load_si128
#define juzix_mm_load_si128(x) juzix_func_mm_load_si128((block*)x)

//_mm_loadu_si128
#define juzix_mm_loadu_si128(x) juzix_func_mm_loadu_si128((block*)x)

//_mm_loadl_epi64
#define juzix_mm_loadl_epi64(x) juzix_func_mm_loadl_epi64((block*)x)

//_mm_xor_si128
#define juzix_mm_xor_si128(x,y) juzix_func_mm_xor_si128(x, y)

//_mm_and_si128
#define juzix_mm_and_si128(x,y) juzix_func_mm_and_si128(x,y)

//_mm_or_si128
#define juzix_mm_or_si128(x,y) juzix_func_mm_or_si128(x,y)

//_mm_srai_epi32
#define juzix_mm_srai_epi32(x, n) juzix_func_mm_srai_epi32(x, n)

//_mm_set_epi8(ch17,ch16..., ch0)
#define juzix_mm_set_epi8(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) juzix_func_mm_set_epi8(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0)

//_mm_set_epi16(b7,b6,...,b0)
#define juzix_mm_set_epi16(b7, b6, b5, b4, b3, b2, b1, b0) juzix_func_mm_set_epi16(b7, b6, b5, b4, b3, b2, b1, b0)

//_mm_set_epi32
#define juzix_mm_set_epi32(b3,b2,b1,b0) juzix_func_mm_set_epi32(b3,b2,b1,b0)

//_mm_set_epi64
#define juzix_mm_set_epi64(X,Y) juzix_func_mm_set_epi64((uint64_t)X, (uint64_t)Y)

//_mm_set_epi64x
#define juzix_mm_set_epi64x(X,Y) juzix_func_mm_set_epi64x(X, Y)

//_mm_movemask_epi8
#define juzix_mm_movemask_epi8(x) juzix_func_mm_movemask_epi8(x)



/*
int _mm_testz_si128(__m128i s1, __m128i s2);

Returns '1' if the bitwise AND operation on s1 and s2 results in all zeros, else returns '0'. That is,

_mm_testz_si128 := ( (s1 & s2) == 0 ? 1 : 0 )

**/
#define juzix_mm_testz_si128(x,y) juzix_func_mm_testz_si128(x,y)

#endif//






