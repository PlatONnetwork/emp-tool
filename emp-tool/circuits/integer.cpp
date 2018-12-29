#include "integer.h"
#include "emp-tool/circuits/integer_utils.h"
#include "emp-tool/execution/protocol_execution.h"
#include <cstring>
#include <cassert>
#include <algorithm>
#include <math.h>
#include "emp-tool/utils/utils.h"


using std::string;
using std::min;

namespace emp
{

void Integer::bool_data(bool* data, size_t len, string str) {
	string bin = dec_to_bin(str);
	std::reverse(bin.begin(), bin.end());
//		cout << "convert " <<str<<" "<<bin<<endl;
	int l = (bin.size() > (size_t)len ? len : bin.size());
	for(int i = 0; i < l; ++i)
		data[i] = (bin[i] == '1');
	for (size_t i = l; i < len; ++i)
		data[i] = data[l-1];
}

void Integer::bool_data(bool* data, size_t len, long long num) {
	bool_data(data, len, std::to_string(num));
}
	

	/*
Integer::Integer(const bool * b, int length, int party) {
  bits = new Bit[length];
  init(bits,b,length, party);
  }*/


Integer::Integer(Integer&& in) : length(in.length) {
	bits = in.bits;
	in.bits = nullptr;
}

Integer::Integer(const Integer& in): length(in.length) {
	bits = new Bit[length];
	memcpy(bits, in.bits, sizeof(Bit)*length);
}

Integer& Integer::operator= (Integer rhs){
	length = rhs.length;
	std::swap(bits, rhs.bits);
	return *this;
}

Integer::Integer(int len, const void * b) : length(len) {
	bits = new Bit[len];
	memcpy(bits, b, sizeof(Bit)*len);
}

Integer::~Integer() {
	if (bits!=nullptr) delete[] bits;
}

Integer::Integer(const int8_t& value, int party) : Integer(8 * sizeof(value), std::to_string(value), party) {
}

Integer::Integer(const int16_t& value, int party): Integer(8 * sizeof(value), std::to_string(value), party) {
}

Integer::Integer(const int32_t& value, int party): Integer(8 * sizeof(value), std::to_string(value), party) {
}

Integer::Integer(const int64_t& value, int party) : Integer(8 * sizeof(value), std::to_string(value), party) {
}

Integer::Integer(int len, const string& str, int party) : length(len) {
	bool* b = new bool[len];
	bool_data(b, len, str);
	bits = new Bit[length];
	init(bits,b,length, party);
	delete[] b;
}


Integer::Integer(int len, long long input, int party)
	: Integer(len, std::to_string(input), party) {
	}


Integer Integer::select(const Bit & select, const Integer & a) const{
	Integer res(*this);
	for(int i = 0; i < size(); ++i)
		res[i] = bits[i].select(select, a[i]);
	return res;
}


Bit& Integer::operator[](int index) {
	return bits[min(index, size()-1)];
}


const Bit &Integer::operator[](int index) const {
	return bits[min(index, size()-1)];
}


string Integer::reveal_string(int party) const {
	bool * b = new bool[length];
	ProtocolExecutionProxy::prot_exec.reveal(b, party, (block *)bits,  length);
	string bin="";
	for(int i = length-1; i >= 0; --i)
		bin += (b[i]? '1':'0');
	delete [] b;
	return bin_to_dec(bin);
}


int64_t Integer::reveal(int party) const {
	string s = reveal_string(party);
	return stoll(s);
}

uint64_t Integer::reveal_uint(int party) const {
	Integer tmp = *this;
	tmp.resize(tmp.size()+1, false);
	string s = tmp.reveal_string(party);
	return stoll(s);
}


int Integer::size() const {
	return length;
}

//circuits

Integer Integer::abs() const {
	Integer res(*this);
	for(int i = 0; i < size(); ++i)
		res[i] = bits[size()-1];
	return ( (*this) + res) ^ res;
}


Integer& Integer::resize(int len, bool signed_extend) {
	Bit * old = bits;
	bits = new Bit[len];
	memcpy(bits, old, min(len, size())*sizeof(Bit));
	Bit extended_bit = old[length-1] & signed_extend;
	for(int i = min(len, size()); i < len; ++i)
		bits[i] = extended_bit;
	this->length = len;
	delete[] old;
	return *this;
}

//Logical operations

Integer Integer::operator^(const Integer& rhs) const {
	Integer res(*this);
	for(int i = 0; i < size(); ++i)
		res.bits[i] = res.bits[i] ^ rhs.bits[i];
	return res;
}


Integer Integer::operator|(const Integer& rhs) const {
	Integer res(*this);
	for(int i = 0; i < size(); ++i)
		res.bits[i] = res.bits[i] | rhs.bits[i];
	return res;
}


Integer Integer::operator&(const Integer& rhs) const {
	Integer res(*this);
	for(int i = 0; i < size(); ++i)
		res.bits[i] = res.bits[i] & rhs.bits[i];
	return res;
}


Integer Integer::operator<<(int shamt) const {
	Integer res(*this);
	if(shamt > size()) {
		for(int i = 0; i < size(); ++i)
			res.bits[i] = false;
	}
	else {
		for(int i = size()-1; i >= shamt; --i)
			res.bits[i] = bits[i-shamt];
		for(int i = shamt-1; i>=0; --i)
			res.bits[i] = false;
	}
	return res;
}


Integer Integer::operator>>(int shamt) const {
	Integer res(*this);
	if(shamt >size()) {
		for(int i = 0; i < size(); ++i)
			res.bits[i] = false;
	}
	else {
		for(int i = shamt; i < size(); ++i)
			res.bits[i-shamt] = bits[i];
		for(int i = size()-shamt; i < size(); ++i)
			res.bits[i] = false;
	}
	return res;
}


Integer Integer::operator<<(const Integer& shamt) const {
	Integer res(*this);
	for(int i = 0; i < min(int(ceil(log2(size()))) , shamt.size()-1); ++i)
		res = res.select(shamt[i], res<<(1<<i));
	return res;
}


Integer Integer::operator>>(const Integer& shamt) const{
	Integer res(*this);
	for(int i = 0; i <min(int(ceil(log2(size()))) , shamt.size()-1); ++i)
		res = res.select(shamt[i], res>>(1<<i));
	return res;
}

//Comparisons

Bit Integer::geq (const Integer& rhs) const {
	assert(size() == rhs.size());
/*	Bit res(false);
	for(int i = 0; i < size(); ++i) {
		res = ((bits[i]^res)&(rhs[i]^res))^bits[i];
	} 
	return res; 
*/
	Integer tmp = (*this) - rhs;
	return !tmp[tmp.size()-1];
}


Bit Integer::equal(const Integer& rhs) const {
	assert(size() == rhs.size());
	Bit res(true);
	for(int i = 0; i < size(); ++i)
		res = res & (bits[i] == rhs[i]);
	return res;
}

/* Arithmethics
 */

Integer Integer::operator+(const Integer & rhs) const {
	assert(size() == rhs.size());
	Integer res(*this);
	add_full(res.bits, nullptr, bits, rhs.bits, nullptr, size());
	return res;
}


Integer Integer::operator-(const Integer& rhs) const {
	assert(size() == rhs.size());
	Integer res(*this);
	sub_full(res.bits, nullptr, bits, rhs.bits, nullptr, size());
	return res;
}



Integer Integer::operator*(const Integer& rhs) const {
	assert(size() == rhs.size());
	Integer res(*this);
	mul_full(res.bits, bits, rhs.bits, size());
	return res;
}


Integer Integer::operator/(const Integer& rhs) const {
	assert(size() == rhs.size());
	Integer res(*this);
	Integer i1 = abs();
	Integer i2 = rhs.abs();
	Bit sign = bits[size()-1] ^ rhs[size()-1];
	div_full(res.bits, nullptr, i1.bits, i2.bits, size());
	condNeg(sign, res.bits, res.bits, size());
	return res;
}

Integer Integer::operator%(const Integer& rhs) const {
	assert(size() == rhs.size());
	Integer res(*this);
	Integer i1 = abs();
	Integer i2 = rhs.abs();
	Bit sign = bits[size()-1];
	div_full(nullptr, res.bits, i1.bits, i2.bits, size());
	condNeg(sign, res.bits, res.bits, size());
	return res;
}


Integer Integer::operator-() const {
	return Integer(size(), 0, PUBLIC)-(*this);
}

//Others

Integer Integer::leading_zeros() const {
	Integer res = *this;
	for(int i = size() - 2; i>=0; --i)
		res[i] = res[i+1] | res[i];

	for(int i = 0; i < res.size(); ++i)
		res[i] = !res[i];
	return res.hamming_weight();
}


Integer Integer::hamming_weight() const {
	vector<Integer> vec;
	for(int i = 0; i < size(); i++) {
		Integer tmp(2, 0, PUBLIC);
		tmp[0] = bits[i];
		vec.push_back(tmp);
	}

	while(vec.size() > 1) {
		int j = 0;
		for(size_t i = 0; i < vec.size()-1; i+=2) {
			vec[j++] = vec[i]+vec[i+1];
		}
		if(vec.size()%2 == 1) {
			vec[j++] = vec[vec.size()-1];
		}
		for(int i = 0; i < j; ++i)
			vec[i].resize(vec[i].size()+1, false);
		int vec_size = vec.size();
		for(int i = j; i < vec_size; ++i)
			vec.pop_back();
	}
	return vec[0];
}

Integer Integer::modExp(Integer p, Integer q) {
	// the value of q should be less than half of the MAX_INT
	Integer base = *this;
	Integer res(1, size());
	for(int i = 0; i < p.size(); ++i) {
		Integer tmp = (res * base) % q;
		res.select(p[i], tmp);
		base = (base*base) % q; 
	}
	return res;
}

}


