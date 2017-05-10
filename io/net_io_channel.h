#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <netinet/tcp.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/Endpoint.h>
#include <cryptoTools/Network/IOService.h>
#include <sys/types.h>

#include <string>
#include "io_channel.h"
using namespace std;

#ifndef NETWORK_IO_CHANNEL
#define NETWORK_IO_CHANNEL
/** @addtogroup IO
	@{
  */

extern osuCrypto::IOService gIoSerive;
extern std::list<osuCrypto::Endpoint> gEndpoints;

class NetIO : public IOChannel<NetIO> {
public:
	bool is_server;
	osuCrypto::Channel chan;
	//int mysocket = -1;
	//int consocket = -1;
	//FILE * stream = nullptr;
	//char * buffer = nullptr;
	bool has_sent = false;
	string addr;
	int port;
#ifdef COUNT_IO
	uint64_t counter = 0;
#endif
	NetIO(const char * address, int port, bool quiet = false) {

		this->port = port;
		is_server = (address == nullptr);

		if (is_server) {
			// wildcard address
			addr = "0.0.0.0";
		}

		for (auto& ep : gEndpoints)
		{
			if (ep.IP() == addr && ep.port() == port)
			{
				chan = ep.addChannel("c");
				chan.waitForConnection();

				if (!quiet)
					cout << "connected" << endl;
				return;
			}
		}

		gEndpoints.emplace_back(gIoSerive, addr, port, is_server ? osuCrypto::EpMode::Server : osuCrypto::EpMode::Client, "c");
		auto& ep = gEndpoints.back();
		chan = ep.addChannel("c");
		chan.waitForConnection();

		if (!quiet)
			cout << "connected" << endl;
		//return;

		//struct sockaddr_in dest;
		//struct sockaddr_in serv;
		//socklen_t socksize = sizeof(struct sockaddr_in);
		//memset(&serv, 0, sizeof(serv));
		//serv.sin_family = AF_INET;
		//serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
		//serv.sin_port = htons(port);           /* set the server port number */    
		//mysocket = socket(AF_INET, SOCK_STREAM, 0);
		//int reuse = 1;
		//setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
		//bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
		//listen(mysocket, 1);
		//consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
		//}
		//else {


		//	//addr = string(address);
		//	//struct sockaddr_in dest; 
		//	//consocket = socket(AF_INET, SOCK_STREAM, 0);
		//	//memset(&dest, 0, sizeof(dest));
		//	//dest.sin_family = AF_INET;
		//	//dest.sin_addr.s_addr = inet_addr(address);
		//	//dest.sin_port = htons(port);
		//	//while(connect(consocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
		//	//	usleep(1000);
		//	//}
		//}
//		set_nodelay();
		//if(!quiet)
		//	cout << "connected"<<endl;
		//stream = fdopen(consocket, "wb+");
		//buffer = new char[NETWORK_BUFFER_SIZE];
		//memset(buffer, 0, NETWORK_BUFFER_SIZE);
		//setvbuf(stream, buffer, _IOFBF, NETWORK_BUFFER_SIZE);
	}

	~NetIO() {
		//fflush(stream);
		//close(consocket);
		//delete[] buffer;
	}
	void sync() {
		int tmp = 0;
		if (is_server) {
			send_data(&tmp, 1);
			recv_data(&tmp, 1);
		}
		else {
			recv_data(&tmp, 1);
			send_data(&tmp, 1);
			flush();
		}
	}
	void set_nodelay() {
		//const int one=1;
		//setsockopt(consocket,IPPROTO_TCP,TCP_NODELAY,&one,sizeof(one));
	}

	void set_delay() {
		//const int zero = 0;
		//setsockopt(consocket,IPPROTO_TCP,TCP_NODELAY,&zero,sizeof(zero));
	}

	void flush() {
		//fflush(stream);
	}

	void send_data_impl(const void * data, int len) {
#ifdef COUNT_IO
		counter += len;
#endif
		chan.send(data, len);

		//int sent = 0;
		//while (sent < len) {
		//	int res = fwrite(sent + (char*)data, 1, len - sent, stream);
		//	if (res >= 0)
		//		sent += res;
		//	else
		//		fprintf(stderr, "error: net_send_data %d\n", res);
		//}
		has_sent = true;
	}

	void recv_data_impl(void  * data, int len) {

		chan.recv(data, len);
		//if (has_sent)
		//	fflush(stream);
		//has_sent = false;
		//int sent = 0;
		//while (sent < len) {
		//	int res = fread(sent + (char*)data, 1, len - sent, stream);
		//	if (res >= 0)
		//		sent += res;
		//	else
		//		fprintf(stderr, "error: net_send_data %d\n", res);
		//}
	}
};
/**@}*/
#endif//NETWORK_IO_CHANNEL
