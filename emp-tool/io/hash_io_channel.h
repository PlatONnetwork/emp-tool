#ifndef HASH_IO_CHANNEL_H__
#define HASH_IO_CHANNEL_H__

#include "emp-tool/io/net_io_channel.h"

#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/miracl_hash.h"
#else
#include "emp-tool/utils/hash.h"
#endif

/** @addtogroup IO
    @{
  */
namespace emp {  
class HashIO: public IOChannel { public:
	Hash h;
	NetIO * netio;
	HashIO(NetIO * _netio ) {
		this->netio = _netio;
	}
	int send_data(const void * data, int len) {
		h.put(data, len);
		return len;
	}
	int recv_data(void  * data, int len) {
		netio->recv_data(data, len);
		h.put(data, len);
		return len;
	}
	void get_digest(char * dgst){
		h.digest(dgst);
	}
};
/**@}*/
}
#endif//HASH_IO_CHANNEL_H__
