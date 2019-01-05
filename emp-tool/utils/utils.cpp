#include "emp-tool/utils/utils.h"
#ifdef OT_NP_USE_MIRACL
////Note: we take miracl functions instead of gmp !
#include "emp-tool/utils/miracl_utils.h"
#endif

#ifndef _WIN32
#include <gmp.h>
#endif//

namespace emp
{
string bin_to_dec(const string& bin2) {
	if(bin2[0] == '0')
		return change_base(bin2, 2, 10);
	string bin = bin2;
	bin[0] = '0';
	bool flip = false;
	for(int i = bin.size()-1; i>=1; --i) {
		if(flip)
			bin[i] = (bin[i] == '1' ? '0': '1');
		if(bin[i] == '1')
			flip = true;
	}
	return "-"+change_base(bin, 2, 10);
}

string dec_to_bin(const string& dec) {
	string bin = change_base(dec, 10, 2);
	if(dec[0] != '-')
		return '0' + bin;
	bin[0] = '1';
	bool flip = false;
	for(int i = bin.size()-1; i>=1; --i) {
		if(flip)
			bin[i] = (bin[i] == '1' ? '0': '1');
		if(bin[i] == '1')
			flip = true;
	}
	return bin;
}


string change_base(string str, int old_base, int new_base) {
#if defined(_WIN32) && defined(OT_NP_USE_MIRACL)
	//use miracl
	//fprintf(stderr, "change_base: %s, old: %d, new: %d \n", str.data(), old_base, new_base);
	return transfer_base(str, old_base, new_base);
#else
	mpz_t tmp;
	mpz_init_set_str (tmp, str.c_str(), old_base);
	char * b = new char[mpz_sizeinbase(tmp, new_base) + 2];
	mpz_get_str(b, new_base, tmp);
	mpz_clear(tmp);
	string res(b);
	delete[]b;
	return res;
#endif//
}

const char* hex_char_to_bin(char c) {
	switch(toupper(c)) {
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default: return "0";
	}
}

std::string hex_to_binary(std::string hex) {
	std::string bin;
	for(unsigned i = 0; i != hex.length(); ++i)
		bin += hex_char_to_bin(hex[i]);
	return bin;
}

static uint64_t bool_to64(const bool * data) {
	uint64_t res = 0;
	for(int i = 0; i < 64; ++i) {
		if(data[i])
			res |= (1ULL<<i);
	}
	return res;
}

block bool_to128(const bool * data) {
	return makeBlock(bool_to64(data+64), bool_to64(data));
}

void int64_to_bool(bool * data, uint64_t input, int length) {
	for (int i = 0; i < length; ++i) {
		data[i] = (input & 1)==1;
		input >>= 1;
	}
}

}//emp
