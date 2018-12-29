#ifndef OT_NP_USE_MIRACL
#ifndef UTILS_EC_H__
#define UTILS_EC_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "relic/relic.h"
#include "relic/relic_types.h"

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include "emp-tool/utils/block_utils.h"
#include "emp-tool/utils/hash.h"
#include <gmp.h>



namespace emp {

#define ECC_PACK false
#define BIT_LEN 128
#define EB_SIZE 65


//printf("##** call in XXXl function func##l(H p) function=%s\n", __FUNCTION__);

#define __batch(func)\
    template <typename H> void func##l(H p) {\
            func(p);\
        }\
	template <typename H, typename V, typename... T> void func##l(H p, V v, T... t) {\
		func(p);\
		func##l(p, t...);\
	}\
	
	

#define __batch2(func)\
template <typename H1, typename H2, typename H3> void func##_norm(H1 h1, H2 h2, H3 h3) {\
	func(h1, h2, h3);\
	eb_norm(h1, h1);\
}\

#define __batch3(func)\
template <typename H1, typename H2, typename H3, typename H4> void func##_mod(H1 h1, H2 h2, H3 h3, H4 h4) {\
	func(h1, h2, h3);\
	bn_mod(h1, h1, h4);\
}\

typedef eb_t eb_tpl[2];
typedef bn_t bn_tpl[2];



__batch(eb_new);
__batch(eb_free);
__batch(bn_new);
__batch(bn_free);



__batch2(eb_mul);
__batch2(eb_mul_fix);
__batch2(eb_sub);
__batch2(eb_add);



__batch3(bn_mul);
__batch3(bn_sub);
__batch3(bn_add);



// template<typename T, typename ... L>
// void random_bn(T t, L... l) {
// 	random_bn(l...);
// 	random_bn(t);
// }

// void random_bn(bn_t a, int sign = BN_POS, int bits = BIT_LEN) {
// 	int digits;
// 	SPLIT(bits, digits, bits, DIG_LOG);
// 	digits += (bits > 0 ? 1 : 0);
// 	bn_grow(a, digits);
// 	PRG prg;
// 	prg.random_data((void*)a->dp, digits * sizeof(dig_t));
// 	a->used = digits;
// 	a->sign = sign;
// 	if (bits > 0) {
// 		dig_t mask = ((dig_t)1 << (dig_t)bits) - 1;
// 		a->dp[a->used - 1] &= mask;
// 	}
// 	bn_trim(a);
// }

// void random_bn(bn_t *a, int length=1, int sign = BN_POS, int bits = BIT_LEN) {
// 	for(int i = 0; i < length; ++i)
// 		random_bn(a[i]);
// }

// template<typename T, typename ... L>
// 	void random_eb(T t, L... l) {
// 		random_eb(l...);
// 		random_eb(t);
// 	}

// void random_eb(eb_t p) {
// 	bn_t n, k;
// 	bn_new(k);
// 	bn_new(n);
// 	eb_curve_get_ord(n);
// 	random_bn(k, BN_POS, bn_bits(n));
// 	bn_mod(k, k, n);
// 	eb_mul_gen(p, k);
// }

// void random_eb(eb_t *p, int length=1) {
// 	bn_t n, k;
// 	bn_new(k);
// 	bn_new(n);
// 	eb_curve_get_ord(n);
// 	for(int i = 0; i < length; ++i) {
// 		random_bn(k, BN_POS, bn_bits(n));
// 		bn_mod(k, k, n);
// 		eb_mul_gen(p[i], k);
// 	}
// }

// void random_mpz(mpz_t out, int nbits) {
// 	int nbytes = (nbits+7)/8;
// 	uint8_t * data = (uint8_t *)new block[(nbytes+15)/16];
// 	PRG prg;
// 	prg.random_data((void*)data, nbytes);
// 	if (nbits % 8 != 0)
// 		data[0] %= (1 << (nbits % 8));
// 	mpz_import(out, nbytes, 1, 1, 0, 0, data);
// 	delete [] data;
// }

// void random_mpz(mpz_t rop, const mpz_t n) {
// 	auto size = mpz_sizeinbase(n, 2);
// 	while (1) {
// 		random_mpz(rop, (int)size);
// 		if (mpz_cmp(rop, n) < 0) {
// 			break;
// 		}
// 	}
// }



//static bool initialized = false;


//TODO: fix the following.
block KDF(eb_t in);

void bn_to_block(block * b, const bn_t bn);

void block_to_bn(bn_t bn, const block * b);


int initialize_relic();

}//emp

#endif // UTILS_EC_H__

#endif//OT_NP_USE_MIRACL
