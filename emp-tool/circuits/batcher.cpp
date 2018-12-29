#include "batcher.h"
//#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"

namespace emp {
	void Batcher::add(bool * b, int length) {
		lens.push_back(length);
		for(int i = 0; i < length; ++i)
			values.push_back(b[i]);
	}

	Batcher::~Batcher(){
		if(labels != nullptr)
			delete[] labels;
	}

	void Batcher::make_semi_honest(int party, ProtocolExecution * be /* = nullptr */) {
		if (be == nullptr && ProtocolExecutionProxy::prot_exec.prot_exec_ == nullptr)
			return;

		if (be != nullptr)
			ProtocolExecutionProxy::prot_exec.setup(be);
		
		bool * bools = new bool[size()];
		to_bool(bools);
		label_ptr = labels = new block[size()];
		ProtocolExecutionProxy::prot_exec.feed(labels, party, bools, size());
		len_ptr = 0;
		delete[] bools;
	}

	void Batcher::set_blocks(block * b) {
		this->labels = b;
		this->label_ptr = b;
		len_ptr = 0;
	}
}