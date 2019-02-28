#ifndef CIRCUIT_EXECUTION_H__
#define CIRCUIT_EXECUTION_H__
#include "block.h"
#include "emp-tool/utils/block_utils.h"
#include "constants.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

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


		class EMP_MPC_DLL_API CircuitExecutionProxy
		{
		//Note: we only support thread_local variables at present, together with proxy CircuitExecution object in heap, so object could auto released in desctruction
		private://
			static CircuitExecutionProxy circ_exec_;

		#ifdef _WIN32
		    static DWORD cir_proxy_key_;
		#else
		    static pthread_key_t cir_proxy_key_;
		#endif//

		    static void cir_global_init()
		    {
		  	#ifdef _WIN32
		        cir_proxy_key_ = TlsAlloc();
		    #else
		        pthread_key_create(&cir_proxy_key_,(void(*)(void *))NULL);
		    #endif
		    }

		    static void cir_global_uninit()
		    {
		    #ifdef _WIN32
		        TlsFree(cir_proxy_key_);
		    #else
		        pthread_key_delete(cir_proxy_key_);
		    #endif
		    }

		public:
		    CircuitExecutionProxy() {
		    	cir_global_init();
		    }

		    ~CircuitExecutionProxy() {
				cir_global_uninit();
			}


		    static CircuitExecution* getCircuitExecution()
		    {
		     #ifdef _WIN32
		        LPVOID lret = TlsGetValue(cir_proxy_key_);
		     #else
		        void* lret = pthread_getspecific(cir_proxy_key_);
		     #endif
				if (lret != 0)
					return (CircuitExecution*)lret;
				else
				{
					printf("nullptr for circ_exec_ !\n");
					return nullptr;
				}
		    }

			static void setup(CircuitExecution* circuit_exe)
			{
			#ifdef _WIN32
				TlsSetValue(cir_proxy_key_, circuit_exe);
			#else
				pthread_setspecific(cir_proxy_key_, circuit_exe);
			#endif
			}

			static void finalize()
			{
			#ifdef _WIN32
				LPVOID lret = TlsGetValue(cir_proxy_key_);
				if (lret != 0)
				{
					delete (CircuitExecution*)lret;
					TlsSetValue(cir_proxy_key_, nullptr);
				}
			#else
				void* lret = pthread_getspecific(cir_proxy_key_);
				if (lret != NULL)
				{
					delete (CircuitExecution*)lret;
					pthread_setspecific(cir_proxy_key_, NULL);
				}
			#endif
			}

			static block and_gate(const block& in1, const block& in2)
			{
				CircuitExecution* ptr = getCircuitExecution();
				if (ptr != nullptr)
				{
					return getCircuitExecution()->and_gate(in1, in2);
				}	
				else
				{
					printf("error, not setup protocol_execution !\n");
					return zero_block();
				}
			}

			static block xor_gate(const block& in1, const block& in2)
			{
				CircuitExecution* ptr = getCircuitExecution();
				if (ptr != nullptr)
				{
					return getCircuitExecution()->xor_gate(in1, in2);
				}
				else
				{
					printf("error, not setup protocol_execution !\n");
					return zero_block();
				}
			}

			static block not_gate(const block& in1)
			{
				CircuitExecution* ptr = getCircuitExecution();
				if (ptr != nullptr)
				{
					return ptr->not_gate(in1);
				}
				else
				{
					printf("error, not setup protocol_execution !\n");
					return zero_block();
				}
			}

			static block public_label(bool b)
			{
				CircuitExecution* ptr = getCircuitExecution();
				if (ptr != nullptr)
				{
					return ptr->public_label(b);
				}
				else
				{
					printf("error, not setup protocol_execution !\n");
					return zero_block();
				}
			}

			// block delta()
			// {
			// 	return circ_exec_->delta();
			// }		
};

}//emp
#endif