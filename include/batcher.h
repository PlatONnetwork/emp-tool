#ifndef BATCHER_H__
#define BATCHER_H__

#include <vector>
#include <cstddef>
#include "block.h"
using std::vector;

namespace emp {
class ProtocolExecution;
class Batcher { public:
	vector<bool> values;
	vector<int> lens;
	int len_ptr = 0;
	block * labels = nullptr;
	block * label_ptr = nullptr;

	void add(bool * b, int length);

	~Batcher();

	template<typename T, typename... Args>
	void add(Args&&... args) {
		int length = T::bool_size(std::forward<Args>(args)...);
		bool * tmp = new bool[length];
		T::bool_data(tmp, std::forward<Args>(args)...);
		add(tmp, length);
		delete []tmp;
	}

	void to_bool(bool * b) {
		for(size_t i = 0; i < values.size(); ++i){
			b[i] = values[i];
		}
	}
	
	void make_semi_honest(int party, ProtocolExecution * be = nullptr);

	void set_blocks(block * b);

	template<typename T>
	T next() {
		T res = T(lens[len_ptr], label_ptr);
		label_ptr +=lens[len_ptr];
		len_ptr ++;
		return res;
	}

	int size() {
		return values.size();
	}
};
}
#endif