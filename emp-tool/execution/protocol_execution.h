#ifndef PROTOCOL_EXECUTION_H__
#define PROTOCOL_EXECUTION_H__
#include "block.h"
#include "constants.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace emp
{

	class ProtocolExecution {
	public:
		int cur_party;
		//ProtocolExecution* gc_ = nullptr;


		ProtocolExecution(int party = PUBLIC) : cur_party(party) {}
		virtual ~ProtocolExecution() {}
		virtual void feed(block * lbls, int party, const bool* b, int nel) = 0;
		virtual void reveal(bool*out, int party, const block *lbls, int nel) = 0;
		virtual void finalize() {}

		//virtual void setup(IOChannel* io) = 0;

	};

	class EMP_MPC_DLL_API ProtocolExecutionProxy
	{
	private:
		#ifdef _WIN32
		    static DWORD prot_proxy_key_;
		#else
		    static pthread_key_t prot_proxy_key_;
		#endif//

		static ProtocolExecutionProxy prot_exec_;

		static void prot_global_init()
		{
#ifdef _WIN32
			prot_proxy_key_ = TlsAlloc();
#else
			pthread_key_create(&prot_proxy_key_, (void(*)(void *))NULL);
#endif
		}

		static void prot_global_uninit()
		{
#ifdef _WIN32
			TlsFree(prot_proxy_key_);
#else
			pthread_key_delete(prot_proxy_key_);
#endif
		}

	public:
		static ProtocolExecution* getProtocolExecution()
		{
#ifdef _WIN32
			LPVOID lret = TlsGetValue(prot_proxy_key_);
#else
			void* lret = pthread_getspecific(prot_proxy_key_);
#endif
			if (lret != 0)
				return (ProtocolExecution*)lret;
			else
			{
				printf("nullptr for prot_exec !\n");
				return nullptr;
			}
		}

		ProtocolExecutionProxy() {
			prot_global_init();
		}
		~ProtocolExecutionProxy() {
			prot_global_uninit();
		}

		static void setup(ProtocolExecution* _prot_exec)
		{
#ifdef _WIN32
			TlsSetValue(prot_proxy_key_, _prot_exec);
#else
			pthread_setspecific(prot_proxy_key_, _prot_exec);
#endif
		}

		static void finalize()
		{
#ifdef _WIN32
			LPVOID lret = TlsGetValue(prot_proxy_key_);
			if (lret != 0)
			{
				delete (ProtocolExecution*)lret;
				TlsSetValue(prot_proxy_key_, nullptr);
			}
#else
			void* lret = pthread_getspecific(prot_proxy_key_);
			if (lret != NULL)
			{
				delete (ProtocolExecution*)lret;
				pthread_setspecific(prot_proxy_key_, NULL);
			}
#endif
		}

		static void feed(block * lbls, int party, const bool* b, int nel)
		{
			ProtocolExecution* ptr = getProtocolExecution();
			if (ptr != nullptr)
				return ptr->feed(lbls, party, b, nel);
		}

		static void reveal(bool*out, int party, const block *lbls, int nel)
		{
			ProtocolExecution* ptr = getProtocolExecution();
			if (ptr != nullptr)
				return ptr->reveal(out, party, lbls, nel);
		}
	};
}//emp
#endif