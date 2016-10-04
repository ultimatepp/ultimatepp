#include <Core/Core.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
using namespace Upp;

CONSOLE_APP_MAIN
{
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2,2), &wsaData);

	int srvskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in srvadr;
	srvadr.sin_family = AF_INET;
	srvadr.sin_port = htons(2345);
	srvadr.sin_addr.s_addr = INADDR_ANY;
	bind(srvskt, (sockaddr *) &srvadr, sizeof(srvadr));
	int replyskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	for(;;) {
		fd_set file_set;
		timeval time_out;
		FD_ZERO(&file_set);
		FD_SET(srvskt, &file_set);
		time_out.tv_sec = 1;                        /* set to 1 sec */
		time_out.tv_usec = 0;
		char h[512];
		char cladr[256];
		socklen_t cladrsz = 256;
		if(select(srvskt + 1, &file_set, NULL, NULL, &time_out) > 0) {
			if(recvfrom(srvskt, &h, 512, 0, (sockaddr *)cladr, &cladrsz) > 0) {
				sendto(replyskt, &h, 512, 0, (sockaddr *)cladr, cladrsz);
	//			Cout() << "Recieved PING\n";
			}
		}
	}
//	WSACleanup();
}
