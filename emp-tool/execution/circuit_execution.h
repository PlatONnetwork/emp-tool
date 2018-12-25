#ifndef CIRCUIT_EXECUTION_H__
#define CIRCUIT_EXECUTION_H__
#include "block.h"
#include "constants.h"
namespace emp {


	enum RTCktOpt { on, off };


		class CircuitExecution {
		public:
			virtual block and_gate(const block& in1, const block& in2) = 0;
			virtual block xor_gate(const block&in1, const block&in2) = 0;
			virtual block not_gate(const block& in1) = 0;
			virtual block public_label(bool b) = 0;
			//virtual block delta() = 0;
			virtual ~CircuitExecution() { }
		};


		class CircuitExecutionProxy
		{
		public:
			//Note: we only support thread_local variables at present, together with proxy CircuitExecution object in heap, so object could auto released in desctruction
			thread_local static CircuitExecutionProxy circ_exec;

			CircuitExecution* circ_exec_ = nullptr;//only reference, not manage the object

			~CircuitExecutionProxy() {
				finalize();
			}
			
			void setup(CircuitExecution* cuicuit_exec)
			{
				if (circ_exec_)//in case of memory leak
				{
					delete circ_exec_;
					circ_exec_ = nullptr;
				}

				circ_exec_ = cuicuit_exec;
			}

			void finalize()
			{
				if (circ_exec_)
				{
					delete circ_exec_;
					circ_exec_ = nullptr;
				}
			}

			block and_gate(const block& in1, const block& in2)
			{
				return circ_exec_->and_gate(in1, in2);
			}

			block xor_gate(const block&in1, const block&in2)
			{
				return circ_exec_->xor_gate(in1, in2);
			}

			block not_gate(const block& in1)
			{
				return circ_exec_->not_gate(in1);
			}

			block public_label(bool b)
			{
				return circ_exec_->public_label(b);
			}

			// block delta()
			// {
			// 	return circ_exec_->delta();
			// }
				
		};

// class CircuitExecution { 
// public:
// #ifndef THREADING
// 	static CircuitExecution * circ_exec;//NOte: if we use non-thread_local varia
// #else
// 	static __thread CircuitExecution * circ_exec;
// #endif
// 	virtual block and_gate(const block& in1, const block& in2) = 0;
// 	virtual block xor_gate(const block&in1, const block&in2) = 0;
// 	virtual block not_gate(const block& in1) = 0;
// 	virtual block public_label(bool b) = 0;
// 	virtual ~CircuitExecution (){ }
// };
// enum RTCktOpt{on, off};
}
#endif