#ifndef CIRCUIT_EXECUTION_H__
#define CIRCUIT_EXECUTION_H__
#include "block.h"
#include "constants.h"

#ifdef _WIN32
#include <windows.h>
#include <memory>
#endif

namespace emp {

#ifndef _WIN32
		#define _EMP_CIR_PD_  CircuitExecution *circ
#else	
		#define _EMP_CIR_PD_
#endif


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


		class EMP_MPC_DLL_API CircuitExecutionProxy
		{
		public:
			//Note: we only support thread_local variables at present, together with proxy CircuitExecution object in heap, so object could auto released in desctruction
#ifndef _WIN32
            static thread_local CircuitExecutionProxy circ_exec;

			CircuitExecution* circ_exec_ = nullptr;//only reference, not manage the object

			CircuitExecution* getCircuitExecution()
		    {
		        return circ_exec_;
		    }

			CircuitExecutionProxy() {}
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
#else//windows
			
		private:
		    //static bool is_loaded;
		    //static std::mutex lck;
		    static DWORD cir_proxy_key;

		   // bool is_setup = false;
		    static void cir_global_init()
		    {
		        cir_proxy_key = TlsAlloc();
		    }

		    static void cir_global_uninit()
		    {
		        TlsFree(cir_proxy_key);
		    }

		public:
			static CircuitExecutionProxy circ_exec;
		    CircuitExecutionProxy() {
		    	cir_global_init();
		    }

		    ~CircuitExecutionProxy() {
				cir_global_uninit();
			}


		    static CircuitExecution* getCircuitExecution()
		    {
		        LPVOID lret = TlsGetValue(cir_proxy_key);
				if (lret != 0)
					return (CircuitExecution*)lret;
				else
				{
					printf("nullptr for circ_exec !\n");
					return nullptr;
				}
		    }
            
			//CircuitExecution* circ_exec_ = nullptr;//only reference, not manage the object

			static void setup(CircuitExecution* cuicuit_exec)
			{
				LPVOID lret = TlsGetValue(cir_proxy_key);
				if ((lret == 0 && GetLastError() == ERROR_SUCCESS) || lret != 0)
				{
					TlsSetValue(cir_proxy_key, cuicuit_exec);
				}
				else
					printf("error in setup CircuitExecution, %d\n", GetLastError());
			}

			static void finalize()
			{
				LPVOID lret = TlsGetValue(cir_proxy_key);
				if (lret != 0)
				{
					TlsSetValue(cir_proxy_key, nullptr);
				}
			}

			block and_gate(const block& in1, const block& in2)
			{
				return getCircuitExecution()->and_gate(in1, in2);
			}

			block xor_gate(const block&in1, const block&in2)
			{
				return getCircuitExecution()->xor_gate(in1, in2);
			}

			block not_gate(const block& in1)
			{
				return getCircuitExecution()->not_gate(in1);
			}

			block public_label(bool b)
			{
				return getCircuitExecution()->public_label(b);
			}
#endif//


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