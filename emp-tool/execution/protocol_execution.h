#ifndef PROTOCOL_EXECUTION_H__
#define PROTOCOL_EXECUTION_H__
#include "block.h"
#include "constants.h"

#ifdef _WIN32
#include <windows.h>
#include <memory>
#endif

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

		class EMP_MPC_DLL_API ProtocolExecutionProxy
		{
		public:
#ifndef _WIN32
            static thread_local ProtocolExecutionProxy prot_exec;

			ProtocolExecution* prot_exec_ = nullptr;

			ProtocolExecution* getProtocolExecution()
		    {
		        return prot_exec_;
		    }

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
#else
		public:
			static DWORD prot_proxy_key;
            static ProtocolExecutionProxy prot_exec;

			static void prot_global_init()
		    {
		        prot_proxy_key = TlsAlloc();
		    }

		    static void prot_global_uninit()
		    {
		        TlsFree(prot_proxy_key);
		    }

		   	static ProtocolExecution* getProtocolExecution()
		    {
		        LPVOID lret = TlsGetValue(prot_proxy_key);
				if (lret != 0)
					return (ProtocolExecution*)lret;
				else
				{
					printf("nullptr for circ_exec !\n");
					return nullptr;
				}
		    }

			static void setup(ProtocolExecution* prot_exec)
			{
				LPVOID lret = TlsGetValue(prot_proxy_key);
				if ((lret == 0 && GetLastError() == ERROR_SUCCESS) || lret != 0)
				{
					TlsSetValue(prot_proxy_key,  prot_exec);
				}
				else
					printf("error in setup CircuitExecution, %d\n", GetLastError());
			}

			static void finalize()
			{
				LPVOID lret = TlsGetValue(prot_proxy_key);
				if (lret != 0)
				{
					TlsSetValue(prot_proxy_key, nullptr);
				}
			}

		public:
			ProtocolExecutionProxy() {
            	prot_global_init();
            }
			~ProtocolExecutionProxy() {
				prot_global_uninit();
			}

			void feed(block * lbls, int party, const bool* b, int nel)
			{
				return getProtocolExecution()->feed(lbls, party, b, nel);
			}

			void reveal(bool*out, int party, const block *lbls, int nel)
			{
				return getProtocolExecution()->reveal(out, party, lbls, nel);
			}
#endif

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
