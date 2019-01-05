#ifndef UTILS_H__
#define UTILS_H__

#include <string>
#include "block.h"
#include <sstream>
#include <cstddef>//https://gcc.gnu.org/gcc-4.9/porting_to.html


#include "emp-tool/utils/prg.h"
#include <chrono>
#define macro_xstr(a) macro_str(a)
#define macro_str(a) #a

using std::string;
using std::chrono::time_point;
using std::chrono::high_resolution_clock;

namespace emp {
template<typename T>
void inline delete_array_null(T * ptr){
	if(ptr != nullptr) {
		delete[] ptr;
		ptr = nullptr;
	}
}

inline 
void error(const char * s, int line = 0, const char * file = nullptr) {
	fprintf(stderr, s, "\n");
	if(file != nullptr) {
		fprintf(stderr, "at %d, %s\n", line, file);
	}
	exit(1);
}

template<class... Ts>
void run_function(void *function, const Ts&... args) {	
	reinterpret_cast<void(*)(Ts...)>(function)(args...);
}

inline void parse_party_and_port(char ** arg, int argc, int * party, int * port) {
	if (argc == 1)
		error("ERROR: argc = 1, need two argsm party ID {1,2} and port.");
	*party = atoi(arg[1]);
	*port = atoi(arg[2]);
}

void inline parse_party_and_port(char ** arg, int * party, int * port) {
	parse_party_and_port(arg, 2, party, port);
}



inline std::string Party(int p) {
	if (p == ALICE)	
		return "ALICE";
	else if (p == BOB)
		return "BOB";
	else return "PUBLIC";
}

// Timing related
inline time_point<high_resolution_clock> clock_start() { 
	return high_resolution_clock::now();
}

inline double time_from(const time_point<high_resolution_clock>& s) {
	return std::chrono::duration_cast<std::chrono::microseconds>(high_resolution_clock::now() - s).count();
}

//block conversions
template <typename T>
std::string m128i_to_string(const __m128i var) {
	std::stringstream sstr;
	const T* values = (const T*) &var;
	for (unsigned int i = 0; i < sizeof(__m128i) / sizeof(T); i++) {
		sstr <<"0x"<<std::hex<< values[i] << " ";
	}
	return sstr.str();
}

block bool_to128(const bool * data);
void int64_to_bool(bool * data, uint64_t input, int length);

//Other conversions
template<typename t>
t bool_to_int(const bool * data, size_t len) {
	if (len != 0) len = (len > sizeof(t)*8 ? sizeof(t)*8 : len);
	else len = sizeof(t)*8;
	t res = 0;
	for(size_t i = 0; i < len-1; ++i) {
		if(data[i])
			res |= (1LL<<i);
	}
	if(data[len-1]) return -1*res;
	else return res;
}

std::string hex_to_binary(std::string hex);
string change_base(string str, int old_base, int new_base);
string dec_to_bin(const string& dec);
string bin_to_dec(const string& bin2);
const char* hex_char_to_bin(char c);

//deprecate soon






}
#endif// UTILS_H__
