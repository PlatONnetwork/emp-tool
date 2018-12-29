#include "emp-tool/utils/sse.h"
#include <cstring>
#include <cstdint>
#include <cstddef>



#ifndef __ARM_NEON__

//_mm_slli_epi64(B,1)
block juzix_func_mm_slli_epi64(block b, int count) 
{
    //left move
    //printf("-------------  juzix_func_mm_slli_epi64 \n");
    block blk;
    uint64_t* pt = (uint64_t*)&blk;
    uint64_t* ptx = (uint64_t*)&b;
    pt[1] = ptx[1] << count;
    pt[0] = ptx[0] << count; 
    return blk;
}

//_mm_slli_epi32
block juzix_func_mm_slli_epi32(block b, int count)
{
    //left move
    //printf("-------------  juzix_func_mm_slli_epi32 \n");
    block blk;
    uint32_t* pt = (uint32_t*)&blk;
    uint32_t* ptx = (uint32_t*)&b;
    
    pt[3] = ptx[3] << count;
    pt[2] = ptx[2] << count;
    pt[1] = ptx[1] << count;
    pt[0] = ptx[0] << count;
    return blk;
}


//_mm_setzero_si128()
block juzix_func_mm_setzero_si128()
{
    //printf("-------------  juzix_func_mm_setzero_si128 \n");
    block blk;
    memset(&blk, 0, sizeof(blk));
    return blk;
}


//_mm_loadu_si128
block juzix_func_mm_loadu_si128(block* b) 
{
    //Loads 128-bit value. Address p not need be 16-byte aligned.
    //printf("-------------  juzix_func_mm_loadu_si128 \n");
    return *b;
}

//_mm_loadl_epi64
block juzix_func_mm_loadl_epi64(block* id)
{
    //Load the lower 64 bits of the value pointed to by p into the lower 64 bits of the result, zeroing the upper 64 bits of the result.
    //k_128 = _mm_loadl_epi64((__m128i const *) (&id));
    //printf("-------------  juzix_func_mm_loadl_epi64 \n");
    block blk;
    uint64_t* ptr = (uint64_t*)id;
    uint64_t* ret = (uint64_t*)&blk;
    ret[1] = 0;
    ret[0] = ptr[0];//get low bits
    return blk;
}


//_mm_xor_si128
block juzix_func_mm_xor_si128(block x, block y)
{
    block blk;
    uint64_t* pt = (uint64_t*)&blk;
    uint64_t* ptx = (uint64_t*)&x;
	uint64_t* pty = (uint64_t*)&y;
	pt[0] = ptx[0]^pty[0]; 
    pt[1] = ptx[1]^pty[1];
    
	return blk;
}

//_mm_or_si128
block juzix_func_mm_or_si128(block x, block y)
{
    //printf("-------------  juzix_func_mm_or_si128 \n");
    block blk;
    uint64_t* pt = (uint64_t*)&blk;
    uint64_t* ptx = (uint64_t*)&x;
	uint64_t* pty = (uint64_t*)&y;
	pt[0] = ptx[0] | pty[0]; 
    pt[1] = ptx[1] && pty[1];
    
	return blk;
}


//_mm_set_epi8(ch17,ch16..., ch0)
block juzix_func_mm_set_epi8(char b15, char b14, char b13, char b12, char b11, char b10, char b9, char b8, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0)
{
    block blk;
    
    //do some conversation
    char* ptr = (char*)&blk;
    ptr[15] = b15;
    ptr[14] = b14;
    ptr[13] = b13;
    ptr[12] = b12;
    ptr[11] = b11;
    ptr[10] = b10;
    ptr[9] = b9;
    ptr[8] = b8;
    ptr[7] = b7;
    ptr[6] = b6;
    ptr[5] = b5;
    ptr[4] = b4;
    ptr[3] = b3;
    ptr[2] = b2;
    ptr[1] = b1;
    ptr[0] = b0;
    //convertBlock(&blk, &ret);
    return blk;
    //return _mm_set_epi8(c1,c2,c3,c4,c5,c6,c7,c8)
}


//_mm_set_epi16(b7,b6,...,b0)
block juzix_func_mm_set_epi16(short b7, short b6, short b5, short b4, short b3, short b2, short b1, short b0)
{
   block ret;
   //do some conversation
   uint16_t* ptr = (uint16_t*)&ret;
   ptr[7] = b7;
   ptr[6] = b6;
   ptr[5] = b5;
   ptr[4] = b4;
   ptr[3] = b3;
   ptr[2] = b2;
   ptr[1] = b1;
   ptr[0] = b0;

   return ret;
   //return _mm_set_epi16(c1,c2,c3,c4,c5,c6,c7,c8)
}

//_mm_set_epi32
block juzix_func_mm_set_epi32(int i3, int i2, int i1, int i0)
{
    //_mm_set_epi32(a,b,c,d)
    block ret;
    
    uint32_t* ptr = (uint32_t*)&ret;
    ptr[3] = i3;
    ptr[2] = i2;
    ptr[1] = i1;
    ptr[0] = i0;

    return ret;
}

//_mm_movemask_epi8
int juzix_func_mm_movemask_epi8(block b)
{
    //todo: 需要自己实现， 以下可能理解不准确！！！！
    int value = 0;//32bits
    //从b的16个8bits整数中提取最高位，然后创建16bits的整数，并且返回32bits整数（高位补0）
    unsigned char* ptr = (unsigned char*)&b;
    
    for (int i = 0; i < 16; ++i)
    {
        value = ((ptr[i] >> 7 & 0x00000001) << i) | value;
    }

    return value;
}


block juzix_func_mm_srai_epi32(block a, int count)
{
    //Shifts the four signed 32-bit integers in a right by count bits while shifting in the sign bit.
    block blk;
    uint32_t* ptr = (uint32_t*)&blk;
    uint32_t* src = (uint32_t*)&a;

    ptr[0] = src[0] >> count & 0xffffffff;
    ptr[1] = src[1] >> count & 0xffffffff;
    ptr[2] = src[2] >> count & 0xffffffff;
    ptr[3] = src[3] >> count & 0xffffffff;
    return blk;
}

static block juzix_func_mm_clmulepi64_si128 (block a, block b, int imm8)
{
    // ToDo
    block blk;
    return blk;
}

///////     exception functions //////////////////

//juzix_func_mm_testz_si128 is a exeptional function
//_mm_and_si128
block juzix_func_mm_and_si128(block x, block y)
{
    //printf("-------------  juzix_func_mm_and_si128 \n");
    block blk;
    uint64_t* pt = (uint64_t*)&blk;
    uint64_t* ptx = (uint64_t*)&x;
    uint64_t* pty = (uint64_t*)&y;
    pt[0] = ptx[0] & pty[0];
    pt[1] = ptx[1] & pty[1];

    return blk;
}

/*
int _mm_testz_si128(__m128i s1, __m128i s2);

Returns '1' if the bitwise AND operation on s1 and s2 results in all zeros, else returns '0'. That is,

_mm_testz_si128 := ( (s1 & s2) == 0 ? 1 : 0 )

**/
int juzix_func_mm_testz_si128(block x, block y)
{
    block ret = juzix_func_mm_and_si128(x, y);
    uint64_t* ptr = (uint64_t*)&ret;
    return ptr[0] == 0 && ptr[1] == 0 ? 1 : 0;
}

//_mm_set_epi64((__m64)(X), (__m64)(Y))
block juzix_func_mm_set_epi64(uint64_t i1, uint64_t i0)
{
    //_mm_set_epi32(a,b,c,d)
    block ret;

    uint64_t* ptr = (uint64_t*)&ret;
    ptr[1] = i1;
    ptr[0] = i0;

    return ret;
}

//_mm_set_epi64x(X, Y)
block juzix_func_mm_set_epi64x(uint64_t i1, uint64_t i0)
{
    return juzix_func_mm_set_epi64(i1, i0);
}

//_mm_load_si128
block juzix_func_mm_load_si128(block* X)
{
    //Loads 128-bit value. Address p must be 16-byte aligned.
    //printf("-------------  juzix_func_mm_load_si128 \n");
    return *X;
}

///////     exception functions //////////////////


#else// __ARM_NEON__

block juzix_func_mm_slli_si128(block a, int count)
{ 
	block ret; 
	if (count <= 0) { 
		ret = a; 
	} 
	else if (count > 15) { 
		ret = juzix_func_mm_setzero_si128(); 
	} 
	else { 
		ret = vreinterpretq_m128i_s8(vextq_s8(vreinterpretq_s8_m128i(a), vdupq_n_s8(0), count)); 
	} 
	ret; 
}

block juzix_func_mm_slli_si128(block a, int count)
{ 
	block ret; 
	if (count <= 0) { 
		ret = a; 
	} 
	else if (count > 15) { 
		ret = juzix_func_mm_setzero_si128(); 
	} 
	else { 
		ret = vreinterpretq_m128i_s8(vextq_s8(vdupq_n_s8(0), vreinterpretq_s8_m128i(a), 16 - count)); \
	} 
	return ret; 
}


block juzix_func_mm_slli_epi64(block b, int count)
{
	block ret;
	if (count <= 0) {
		ret = b;
	}
	else if (count > 63) {
		ret = juzix_func_mm_setzero_si128();
	}
	else {
		ret = vreinterpretq_m128i_s64(vshlq_n_s64(vreinterpretq_s64_m128i(b), count));
	}
	return ret;
}

block juzix_func_mm_slli_epi32(block b, int count)
{
    block ret; 
    if (count <= 0) {
        ret = b; 
    } 
    else if (count > 31) { 
        ret = juzix_func_mm_setzero_si128(); 
    } 
    else { 
        ret = vreinterpretq_m128i_s32(vshlq_n_s32(vreinterpretq_s32_m128i(b), (count))); 
    } 
    return ret; 
}

block juzix_func_mm_setzero_si128()
{
    return vreinterpretq_m128i_s32(vdupq_n_s32(0));
}


block juzix_func_mm_load_si128(block* p)
{
    return vreinterpretq_m128i_s32(vld1q_s32((int32_t *)p));
}

block juzix_func_mm_loadu_si128(block* p);
{
    return vreinterpretq_m128i_s32(vld1q_u32((uint32_t *)p));
}

block juzix_func_mm_loadl_epi64(block* p)
{
    uint64x1_t lo = vget_low_u64(vreinterpretq_u64_m128i(*p));
    return vreinterpretq_m128i_u64(vcombine_u64(lo, vcreate_u64(0x0)));
}

block juzix_func_mm_xor_si128(block x, block y)
{
    return vreinterpretq_m128i_s32( veorq_s32(vreinterpretq_s32_m128i(x), vreinterpretq_s32_m128i(y)) );
}

block juzix_func_mm_and_si128(block x, block y)
{
    return vreinterpretq_m128i_s32( vandq_s32(vreinterpretq_s32_m128i(x), vreinterpretq_s32_m128i(y)) );
}

block juzix_func_mm_or_si128(block x, block y)
{
    return vreinterpretq_m128i_s32( vorrq_s32(vreinterpretq_s32_m128i(x), vreinterpretq_s32_m128i(y)) );
}

block juzix_func_mm_set_epi8(char b15, char b14, char b13, char b12, char b11, char b10, char b9, char b8, char b7, char b6, char b5, char b4, char b3, char b2, char b1, char b0)
{
	int8_t __attribute__((aligned(16))) data[16] = { b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};
	return vreinterpretq_m128i_s8(vld1q_s8(data));
}

block juzix_func_mm_set_epi16(short w7, short w6, short w5, short w4, short w3, short w2, short w1, short w0)
{
	int16_t __attribute__((aligned(16))) data[8] = { w0, w1, w2, w3, w4, w5, w6, w7};
	return vreinterpretq_m128i_s16(vld1q_s16(data));
}

block juzix_func_mm_set_epi32(int i3, int i2, int i1, int i0)
{
	int32_t __attribute__((aligned(16))) data[4] = { i0, i1, i2, i3 };
	return vreinterpretq_m128i_s32(vld1q_s32(data));
}

block juzix_func_mm_set_epi64(uint64_t q1, uint64_t q0)
{
	uint64_t __attribute__((aligned(16))) data[2] = { q0, q1 };
	return vreinterpretq_m128i_s64(vld1q_u64(data));
}

block juzix_func_mm_set_epi64x(int64_t q1, int64_t q0)
{
	int64_t __attribute__((aligned(16))) data[2] = { q0, q1 };
	return vreinterpretq_m128i_s64(vld1q_s64(data));
}

int juzix_func_mm_movemask_epi8(block b)
{
	uint8x16_t input = vreinterpretq_u8_m128i(b);
	static const int8_t __attribute__((aligned(16))) xr[8] = { -7, -6, -5, -4, -3, -2, -1, 0 };
	uint8x8_t mask_and = vdup_n_u8(0x80);
	int8x8_t mask_shift = vld1_s8(xr);

	uint8x8_t lo = vget_low_u8(input);
	uint8x8_t hi = vget_high_u8(input);

	lo = vand_u8(lo, mask_and);
	lo = vshl_u8(lo, mask_shift);

	hi = vand_u8(hi, mask_and);
	hi = vshl_u8(hi, mask_shift);

	lo = vpadd_u8(lo, lo);
	lo = vpadd_u8(lo, lo);
	lo = vpadd_u8(lo, lo);

	hi = vpadd_u8(hi, hi);
	hi = vpadd_u8(hi, hi);
	hi = vpadd_u8(hi, hi);

	return ((hi[0] << 8) | (lo[0] & 0xFF));
}

int juzix_func_mm_testz_si128(block x, block y)
{
    uint32x4_t r = vtstq_u32(vreinterpretq_u32_m128i(x), vreinterpretq_u32_m128i(y));
    return (vgetq_lane_u32(r, 0) != 0) ? 0 : 1;
}

block juzix_func_mm_srai_epi32(block a, int count)
{
	block ret;
	if (count <= 0) {
		ret = a; 
	} 
	else if (count > 31) { 
		ret = vreinterpretq_m128i_s32(vshrq_n_s32(vreinterpretq_s32_m128i(a), 16)); 
		ret = vreinterpretq_m128i_s32(vshrq_n_s32(vreinterpretq_s32_m128i(ret), 16)); 
	} 
	else { 
		ret = vreinterpretq_m128i_s32(vshrq_n_s32(vreinterpretq_s32_m128i(a), count)); 
	} 
	return ret; 

}

block juzix_func_mm_clmulepi64_si128 ( block a, block b, int imm8)
{
    switch(imm8)
    {
        case 0x00:
            return vreinterpretq_s32_p16(vmull_p8(vget_low_p8(vreinterpretq_p8_s32(a)), vget_low_p8(vreinterpretq_p8_s32(b))));
        case 0x01:
            return vreinterpretq_s32_p16(vmull_p8(vget_high_p8(vreinterpretq_p8_s32(a)), vget_low_p8(vreinterpretq_p8_s32(b))));
        case 0x10:
            return vreinterpretq_s32_p16(vmull_p8(vget_low_p8(vreinterpretq_p8_s32(a)), vget_high_p8(vreinterpretq_p8_s32(b))));
        case 0x11:
            return vreinterpretq_s32_p16(vmull_p8(vget_high_p8(vreinterpretq_p8_s32(a)), vget_high_p8(vreinterpretq_p8_s32(b))));
        default:
            return vreinterpretq_m128i_u64(vcombine_u64(vcreate_u64(0x0), vcreate_u64(0x0)));
    }
}


#endif//




