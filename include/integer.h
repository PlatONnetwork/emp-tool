#ifndef INTEGER_H__
#define INTEGER_H__

#include "constants.h"
#include "bit.h"
#include "number.h"
#include "comparable.h"
#include "swappable.h"
#include <vector>
#include <cstdint>

using std::vector;

namespace emp {
class Integer : public Swappable<Integer>, public Comparable<Integer> { public:
	int length = 0;
	Bit* bits = nullptr;

	Integer(Integer&& in);
	Integer(const Integer& in);
	Integer& operator= (Integer rhs);

	Integer(int len, const void * b);
	~Integer();

	Integer(const int8_t& value, int party = PUBLIC);
	Integer(const int16_t& value, int party = PUBLIC);
	Integer(const int32_t& value, int party = PUBLIC);
	Integer(const int64_t& value, int party = PUBLIC);

	Integer(int length, const string& str, int party = PUBLIC);
	Integer(int length, long long input, int party = PUBLIC);
	Integer() :length(0),bits(nullptr){}

//Comparable
	Bit geq(const Integer & rhs) const;
	Bit equal(const Integer & rhs) const;

//Swappable
	Integer select(const Bit & sel, const Integer & rhs) const;
	Integer operator^(const Integer& rhs) const;

	int size() const;
	// template<typename O>
	// O reveal(int party=PUBLIC) const;
	string reveal_string(int party = PUBLIC) const;
	int64_t reveal(int party = PUBLIC) const;
	uint64_t reveal_uint(int party = PUBLIC) const;

	Integer abs() const;
	Integer& resize(int length, bool signed_extend = true);
	Integer modExp(Integer p, Integer q);
	Integer leading_zeros() const;
	Integer hamming_weight() const;

	Integer operator<<(int shamt)const;
	Integer operator>>(int shamt)const;
	Integer operator<<(const Integer& shamt)const;
	Integer operator>>(const Integer& shamt)const;

	Integer operator+(const Integer& rhs)const;
	Integer operator-(const Integer& rhs)const;
	Integer operator-()const;
	Integer operator*(const Integer& rhs)const;
	Integer operator/(const Integer& rhs)const;
	Integer operator%(const Integer& rhs)const;
	Integer operator&(const Integer& rhs)const;
	Integer operator|(const Integer& rhs)const;

	Bit& operator[](int index);
	const Bit & operator[](int index) const;
	
//batcher
	template<typename... Args>
	static size_t bool_size(size_t size, Args... args) {
		return size;
	}

	static void bool_data(bool* data, size_t len, string str);
	static void bool_data(bool* data, size_t len, long long num);
};


//#include "integer.hpp"
}
#endif// INTEGER_H__
