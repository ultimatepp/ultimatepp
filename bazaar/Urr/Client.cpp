#include "Urr.h"

#define LLOG(x)    // LOG(GetTickCount() << ": " << x)
#define LTIMING(x) // RTIMING(x)

NAMESPACE_UPP

void UrrClient::Init()
{
#ifdef PLATFORM_WIN32
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(0x101, &wsadata);
	}
#endif
	retries = 10;
	timeout = 10;
	qtimeout = 1000;
	loss = 0;
	blksize = 8000;
	flags = 0;
	skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UrrClient::~UrrClient()
{
	closesocket(skt);
}

UrrClient& UrrClient::SetServer(const char *host, int port)
{
	INTERLOCKED {
		hostent *he = gethostbyname(host);
		Zero(srvadr);
		if(he) {
			srvadr.sin_family = AF_INET;
			srvadr.sin_port = htons(port);
			srvadr.sin_addr = *(in_addr *)(he -> h_addr_list[0]);
		}
	}
	return *this;
}

String UrrClient::Call(const String& request)
{
	ASSERT(request.GetCount() < 8192);
	Uuid id;
	{
		LTIMING("Uuid::Create");
		id = Uuid::Create();
	}
	if(blksize < 512)
		blksize = 512;
	Buffer<char> buffer(blksize);
	char *b = ~buffer;
	int rqsz = min(blksize - 20, request.GetCount());
	int retry = 0;
	int sz;
	char adr[256];
	socklen_t len;
	int part;
	for(;;) {
		LLOG("SEND REQUEST " << id << ": " << request);
		b[0] = 0; // version
		b[1] = flags;
		Poke16le(b + 2, blksize);
		memcpy(b + 4, &id, 16);
		memcpy(b + 20, ~request, rqsz);
		sendto(skt, b, rqsz + 20, 0, (sockaddr *) &srvadr, sizeof(srvadr));
		if(flags & URR_ONEWAY)
			return Null;
	processing:
		sz = Rcv(skt, b, blksize, qtimeout, adr, &len);
		LLOG("Rcv " << sz);
		if(sz >= 20 && memcmp(b + 4, &id, 16) == 0) {
			part = Peek32le(b);
			if(part == URR_SINGLE) {
				LLOG("SINGLE response");
				String r = String(b + 20, sz - 20);
				return flags & URR_COMPRESS ? ZDecompress(r) : r;
			}
			if(part == URR_LAST || part == 0)
				break;
			if(part == URR_PROCESSING)
				goto processing;
			LLOG("Wrong part " << part << " after request");
		}
		loss++;
		if(++retry > retries) {
			LLOG("REQUEST TIMEOUT");
			return String::GetVoid();
		}
		LLOG("LOSS: RETRY REQUEST " << retry);
	}
	LLOG("MULTI response");
	String r;
	char ack[20];
	int  ackpart;
	for(;;) {
		LLOG("RECEIVED PART " << part);
		r.Cat(b + 20, sz - 20);
		ackpart = part;
		memcpy(ack, b, 20);
		if(part == URR_LAST)
			break;
		retry = 0;
		for(;;) {
			LLOG("Sending ACK");
			sendto(skt, ack, 20, 0, (sockaddr *)adr, len);
			sz = Rcv(skt, b, blksize, timeout, adr, &len);
			if(sz >= 20 && memcmp(b + 4, &id, 16) == 0) {
				part = Peek32le(b);
				if(part == ackpart + 1 || part == URR_LAST)
					break;
				LLOG("Wrong part " << Peek32le(b) << " after ACK");
			}
			loss++;
			if(++retry > retries) {
				LLOG("PART TIMEOUT");
				return String::GetVoid();
			}
			LLOG("LOSS: RETRY ACK " << retry);
		}
	}
	retry = 0;
	for(;;) {
		LLOG("Sending EOF ACK");
		sendto(skt, ack, 20, 0, (sockaddr *)adr, len);
		LLOG("Waiting for FIN");
		if(Rcv(skt, b, blksize, timeout, adr, &len) == 20 && Peek32le(b) == URR_FIN || ++retry > retries) {
			LLOG("FIN arrived");
			break;
		}
		LLOG("LOSS: RETRY FIN " << retry);
		loss++;
	}
	return flags & URR_COMPRESS ? ZDecompress(r) : r;
}

END_UPP_NAMESPACE
