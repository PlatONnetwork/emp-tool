#ifndef PROTOCOL_EXECUTION_H__
#define PROTOCOL_EXECUTION_H__
#include "block.h"
#include "constants.h"

namespace emp {

	class ProtocolExecution {
		public:
			int cur_party;
			//CircuitExecution* gc_ = nullptr;


			ProtocolExecution(int party = PUBLIC) : cur_party(party) {}
			virtual ~ProtocolExecution() {}
			virtual void feed(block * lbls, int party, const bool* b, int nel) = 0;
			virtual void reveal(bool*out, int party, const block *lbls, int nel) = 0;
			virtual void finalize() {}

			//virtual void setup(IOChannel* io) = 0;

		};

		class ProtocolExecutionProxy
		{
		public:
			static thread_local ProtocolExecutionProxy prot_exec;

			ProtocolExecution* prot_exec_ = nullptr;

			~ProtocolExecutionProxy() {
				finalize();
			}

			void setup(ProtocolExecution* prot)
			{
				if (prot_exec_)
				{
					prot_exec_->finalize();
					delete prot_exec_;
					prot_exec_ = nullptr;
				}

				prot_exec_ = prot;
			}

			void finalize()
			{
				if (prot_exec_)
				{
					prot_exec_->finalize();
					delete prot_exec_;
					prot_exec_ = nullptr;
				}
			}

			void feed(block * lbls, int party, const bool* b, int nel)
			{
				return prot_exec_->feed(lbls, party, b, nel);
			}

			void reveal(bool*out, int party, const block *lbls, int nel)
			{
				return prot_exec_->reveal(out, party, lbls, nel);
			}

		};

// class ProtocolExecution { 
// public:
// 	int cur_party;
// #ifndef THREADING
// 	static ProtocolExecution * prot_exec;
// #else
// 	static __thread ProtocolExecution * prot_exec;
// #endif

// 	ProtocolExecution(int party = PUBLIC) : cur_party (party) {}
// 	virtual ~ProtocolExecution() {}
// 	virtual void feed(block * lbls, int party, const bool* b, int nel) = 0;
// 	virtual void reveal(bool*out, int party, const block *lbls, int nel) = 0;
// 	virtual void finalize() {}
// };
}
#endif
