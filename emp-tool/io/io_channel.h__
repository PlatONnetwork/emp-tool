#ifndef IO_CHANNEL_H__
#define IO_CHANNEL_H__

#include "block.h"

#ifndef OT_NP_USE_MIRACL //default use relic
#include "emp-tool/utils/utils_ec.h"
#else
#include "emp-tool/utils/miracl_utils.h"
#endif//

#include "emp-tool/utils/prg.h"

/** @addtogroup IO
  @{
 */

namespace emp {

//Note: we change base IO template class to virtual class with interface send_data, recv_data
class IOChannel {
public:
	PRG *prg = nullptr;
	virtual int send_data(const void * data, int nbyte) = 0;
	virtual int recv_data(void * data, int nbyte) = 0;

	virtual void flush(){};
	virtual void sync(){};

	void send_block(const block* data, int nblock);
	void recv_block(block* data, int nblock);

	IOChannel();
	virtual ~IOChannel();

	void set_key(const block* b);

	void send_data_enc(const void * data, int len);
	void send_block_enc(const block* data, int len);

	void recv_data_enc(void * data, int len);
	void recv_block_enc(block* data, int len);

#ifndef OT_NP_USE_MIRACL //default use relic
	void send_bn_enc(const bn_t * bn, size_t num);
	void send_eb_enc(const eb_t * eb, size_t num);

	void recv_eb_enc(eb_t* eb, size_t num);
	void recv_bn_enc(bn_t* bn, size_t num);

	void send_eb(const eb_t * eb, size_t num);
	void recv_eb(eb_t* eb, size_t num);

	void send_bn(const bn_t * bn, size_t num);
	void recv_bn(bn_t* bn, size_t num);
#else//use miracl
	int send_ep(epoint *ep, size_t num);
	int recv_ep(epoint *ep, size_t num);

	int send_bn(const big* bn, size_t num);
	int recv_bn(big* bn, size_t num);
#endif//OT_NP_USE_MIRACL

};
/**@}*/
}
#endif// IO_CHANNEL_H__