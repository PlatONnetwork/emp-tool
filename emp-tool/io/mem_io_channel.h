#include <string>
#include "emp-tool/io/io_channel.h"
#include "emp-tool/io/file_io_channel.h"

#ifndef MEM_IO_CHANNEL
#define MEM_IO_CHANNEL

/** @addtogroup IO
  @{
 */
namespace emp {
class MemIO: public IOChannel { public:
	char * buffer = nullptr;
	int64_t size = 0;
	int64_t read_pos = 0;
	int64_t cap;

	MemIO(int64_t _cap=1024*1024) {
		this->cap = _cap;
		buffer = new char[cap];
		size = 0;
	}
	~MemIO() {
		if(buffer!=nullptr)
			delete[] buffer;
	}
	void load_from_file(FileIO * fio, int64_t size) {
		delete[] buffer;
		buffer = new char[size];
		this->cap = size;
		this->read_pos = 0;
		this->size = size;
		fio->recv_data(buffer, size);
	}
	void clear() {
		size = 0;
	}
	
	int send_data(const void * data, int len) {
		if(size + len >= cap){
			char * new_buffer = new char[2*(cap+len)];
			memcpy(new_buffer, buffer, size);
			delete[] buffer;
			buffer = new_buffer;
			cap = 2*(cap + len);
		}
		memcpy(buffer + size, data, len);
		size += len;

		return len;
	}

	int recv_data(void  * data, int len) {
		if(read_pos + len <= size) {
			memcpy(data, buffer + read_pos, len);
			read_pos += len;
		} else {
			fprintf(stderr,"error: mem_recv_data\n");
			return -1;
		}

		return len;
	}
};
/**@}*/
}
#endif//MEM_IO_CHANNEL_H__
