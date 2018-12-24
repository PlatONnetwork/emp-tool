#ifndef BIT_H__
#define BIT_H__

#include <string>
#include "block.h"
#include "swappable.h"
#include "constants.h"

using std::string;

namespace emp {
class Bit : public Swappable<Bit>{ public:
	block bit;

	Bit(bool _b = false, int party = PUBLIC);
	Bit(const block& a);

	bool reveal(int party = PUBLIC) const;

	string reveal_string(int party = PUBLIC) const;

	Bit operator!=(const Bit& rhs) const; 
	Bit operator==(const Bit& rhs) const;
	Bit operator &(const Bit& rhs) const;  
	Bit operator |(const Bit& rhs) const;
	Bit operator !() const; 

	//swappable
	Bit select(const Bit & select, const Bit & new_v)const ;
	Bit operator ^(const Bit& rhs) const;

	//batcher
	template<typename... Args>
	static size_t bool_size(Args&&... args) {
		return 1;
	}

	static void bool_data(bool *b, bool data) {
		b[0] = data;
	}

	Bit(size_t size, const block* a);
};
//#include "emp-tool/circuits/bit.hpp"
}
#endif
