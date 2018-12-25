
#ifndef ABANDON_IO_CHANNEL
#define ABANDON_IO_CHANNEL
#include "emp-tool/io/io_channel.h"

/** @addtogroup IO
  @{
 */

namespace emp {
// Essentially drop all communication
class AbandonIO: public IOChannel { public:
	int size = 0;
	int send_data(const void * data, int len) {
		size += len;
		return 0;
	}

	int recv_data(void  * data, int len) {
		return 0;
	}
};
}
/**@}*/
#endif
