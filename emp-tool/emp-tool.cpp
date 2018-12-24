#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"

// #ifndef THREADING
// emp::ProtocolExecution* emp::ProtocolExecutionProxy::prot_exec = nullptr;
// emp::CircuitExecution* emp::CircuitExecutionProxy::circ_exec = nullptr;
// #else
// __thread emp::ProtocolExecution* emp::ProtocolExecutionProxy::prot_exec = nullptr;
// __thread emp::CircuitExecution* emp::CircuitExecutionProxy::circ_exec = nullptr;
// #endif

thread_local emp::ProtocolExecutionProxy emp::ProtocolExecutionProxy::prot_exec;
thread_local emp::CircuitExecutionProxy emp::CircuitExecutionProxy::circ_exec;
