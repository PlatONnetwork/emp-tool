#ifdef OT_NP_USE_MIRACL
#include "emp-tool/io/io_channel.h"

namespace emp{

void otnp_init(void** epoint_C);
void otnp_uninit(void** epoint_C);

int otnp_send(IOChannel* io, void** epointC, const block* data0, const block* data1, int length);
int otnp_recv(IOChannel* io, void** epointC, block* data, const bool* b, int length);

}

#endif//