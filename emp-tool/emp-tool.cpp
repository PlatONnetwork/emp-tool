#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"

#ifndef _WIN32
emp::ProtocolExecutionProxy emp::ProtocolExecutionProxy::prot_exec_;
emp::CircuitExecutionProxy  emp::CircuitExecutionProxy::circ_exec_;
pthread_key_t emp::CircuitExecutionProxy::cir_proxy_key_;
pthread_key_t emp::ProtocolExecutionProxy::prot_proxy_key_;
#else                                    
emp::ProtocolExecutionProxy emp::ProtocolExecutionProxy::prot_exec_;
emp::CircuitExecutionProxy  emp::CircuitExecutionProxy::circ_exec_;
DWORD emp::CircuitExecutionProxy::cir_proxy_key_;
DWORD emp::ProtocolExecutionProxy::prot_proxy_key_;
#endif