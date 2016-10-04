#include <Core/Core.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace Upp;

int GetMicroCount()
{
	struct timeval tv[1];
	gettimeofday(tv, NULL);
	return (dword)tv->tv_sec * 1000000 + tv->tv_usec;
}

CONSOLE_APP_MAIN {
	int skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in srvadr;
	srvadr.sin_family = AF_INET;
	srvadr.sin_port = htons(2345);
	srvadr.sin_addr.s_addr = inet_addr("10.0.0.20");
	srvadr.sin_addr.s_addr = inet_addr("127.0.0.1");

	for(int i = 0; i < 100000; i++) {
		char h[512];
		int tm = GetMicroCount();
		char buffer[1000];
		fd_set file_set;
		sendto(skt, &h, 512, 0, (sockaddr *) &srvadr, sizeof(srvadr));
		timeval time_out;
		time_out.tv_sec = 1;                        /* set to 1 sec */
		time_out.tv_usec = 0;
		FD_ZERO(&file_set);
		FD_SET(skt, &file_set);
		if(select(skt + 1, &file_set, NULL, NULL, &time_out) > 0) {
			recvfrom(skt, buffer, 1000, 0, NULL, NULL);
			Cout() << "PING " << GetMicroCount() - tm << "us\n";
		}
		else
			Cout() << "TIMEOUT\n";
		Sleep(1000);
	}
}
