#ifndef _INTEGER_UTILS_H__
#define _INTEGER_UTILS_H__


#include "bit.h"

namespace emp
{


//https://github.com/samee/obliv-c/blob/obliv-c/src/ext/oblivc/obliv_bits.c#L1487
void add_full(Bit* dest, Bit * carryOut, const Bit * op1, const Bit * op2, const Bit * carryIn, int size);

void sub_full(Bit * dest, Bit * borrowOut, const Bit * op1, const Bit * op2, const Bit * borrowIn, int size);
void mul_full(Bit * dest, const Bit * op1, const Bit * op2, int size);

void ifThenElse(Bit * dest, const Bit * tsrc, const Bit * fsrc, int size, Bit cond);

void condNeg(Bit cond, Bit * dest, const Bit * src, int size);

void div_full(Bit * vquot, Bit * vrem, const Bit * op1, const Bit * op2, int size);

void init(Bit * bits, const bool* b, int length, int party = PUBLIC);
void init(Bit * bits, const bool* b, int length, int party);


}

#endif//_INTEGER_UTILS_H__
