#include "bit.h"
#include <cstring>
#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"


namespace emp {

Bit::Bit(bool b, int party) {
	if (party == PUBLIC)
		bit = CircuitExecutionProxy::circ_exec.public_label(b);
	else ProtocolExecutionProxy::prot_exec.feed(&bit, party, &b, 1); 
}

Bit::Bit(const block& a) {
		memcpy(&bit, &a, sizeof(block));
}

Bit::Bit(size_t size, const block* a) {
		memcpy(&bit, a, sizeof(block));
}

Bit Bit::select(const Bit & select, const Bit & new_v) const{
	Bit tmp = *this;
	tmp = tmp ^ new_v;
	tmp = tmp & select;
	return *this ^ tmp;
}

bool Bit::reveal(int party) const {
	bool res;
	ProtocolExecutionProxy::prot_exec.reveal(&res, party, &bit, 1);
	return res;
}

string Bit::reveal_string(int party) const {
	bool res;
	ProtocolExecutionProxy::prot_exec.reveal(&res, party, &bit, 1);
	return res ? "true" : "false";
}

Bit Bit::operator==(const Bit& rhs) const {
	return !(*this ^ rhs);
}

Bit Bit::operator!=(const Bit& rhs) const {
	return (*this) ^ rhs;
}

Bit Bit::operator &(const Bit& rhs) const{
	Bit res;
	res.bit = CircuitExecutionProxy::circ_exec.and_gate(bit, rhs.bit);
	return res;
}
Bit Bit::operator ^(const Bit& rhs) const{
	Bit res;
	res.bit = CircuitExecutionProxy::circ_exec.xor_gate(bit, rhs.bit);
	return res;
}

Bit Bit::operator |(const Bit& rhs) const{
	return (*this ^ rhs) ^ (*this & rhs);
}

Bit Bit::operator!() const {
	return CircuitExecutionProxy::circ_exec.not_gate(bit);
}

}
