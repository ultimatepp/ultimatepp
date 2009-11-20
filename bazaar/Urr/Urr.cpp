#include "Urr.h"

#define LTIMING(x) // RTIMING(x)

NAMESPACE_UPP

int Rcv(SOCKET skt, void *buffer, int bsize, int timeout, void *adr, socklen_t *adrlen)
{
	LTIMING("Rcv");
	fd_set file_set;
	timeval time_out;
	FD_ZERO(&file_set);
	FD_SET(skt, &file_set);
	time_out.tv_sec = timeout / 1000;
	time_out.tv_usec = timeout % 1000 * 1000;
	if(adrlen)
		*adrlen = 256;
	if(IsNull(timeout) || select(skt + 1, &file_set, NULL, NULL, &time_out) > 0)
		return recvfrom(skt, (char *)buffer, bsize, 0, (sockaddr *)adr, adrlen);
	return -1;
}

END_UPP_NAMESPACE
