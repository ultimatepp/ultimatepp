#include "Urr.h"

NAMESPACE_UPP

#define LLOG(x)    // LOG(GetTickCount() << ": " << x)
#define LTIMING(x) // RTIMING(x)

void UrrServer::Reset()
{
	srvskt = -1;
	lingertime = 10000;
	senttime = 200;
	acktimeout = 50;
	blksize = 63000;
	retries = 10;
}

bool UrrServer::Create(int port, const char *addr)
{
	LLOG("Creating URR server " << port);
#ifdef PLATFORM_WIN32
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(0x101, &wsadata);
	}
#endif
	srvskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(srvskt < 0) return false;
	sockaddr_in srvadr;
	srvadr.sin_family = AF_INET;
	srvadr.sin_port = htons(port);
	srvadr.sin_addr.s_addr = addr ? inet_addr(addr) : INADDR_ANY;
	if(bind(srvskt, (sockaddr *) &srvadr, sizeof(srvadr)) != 0)
		return false;
	return true;
}

void UrrServer::Close()
{
	if(srvskt >= 0) {
		closesocket(srvskt);
		srvskt = -1;
	}
}

void UrrServer::SendTo(SOCKET skt, char *h, const void *data, int sz, UrrRequest& r)
{
//	LTIMING("SendTo");
	memcpy(h + 20, data, sz);
	LLOG("sendto " << r.id << " part " << Peek32le(h) << ", " << sz << " bytes: " << String(h + 20, min(sz, 64)));
	sendto(skt, h, 20 + sz, 0, (sockaddr *)r.adr, r.adrsz);
}

void UrrServer::SendBack(UrrRequest& r, String data, bool linger)
{
//	LTIMING("SendBack");
	LLOG("Send back " << r.id);
	if(r.flags & URR_COMPRESS)
		data = ZCompress(data);
	int tc = GetTickCount();
	SOCKET skt;
	{
		Mutex::Lock __(mutex);
		LLOG("Adding sent/linger");
		if(linger) {
			TimeI& t = done.AddTail();
			t.time = tc;
			t.ii = donemap.Put(r.id, data);
		}
		if(r.flags & URR_ONEWAY) {
			LLOG("One way request " << r.id);
			processing.UnlinkKey(r.id);
			return;
		}
		TimeI& t = sent.AddTail();
		t.time = tc;
		t.ii = sentindex.Put(r.id);
		if(skts.GetCount())
			skt = skts.Pop();
		else
			skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	Buffer<char> buffer(20 + r.blksize);
	char *b = buffer;
	memcpy(b + 4, &r.id, 16);
	if(data.GetCount() <= r.blksize) {
		LLOG("Single response");
		Poke32le(b, URR_SINGLE);
		SendTo(skt, b, ~data, data.GetCount(), r);
	}
	else {
		int part = 0;
		int pos = 0;
		for(;;) {
			int pos = part * r.blksize;
			int l;
			if(pos + r.blksize >= data.GetCount()) {
				part = URR_LAST;
				l = data.GetCount() - pos;
			}
			else
				l = r.blksize;
			int retry = 0;
			for(;;) {
				Poke32le(b, part);
				SendTo(skt, b, ~data + pos, l, r);
				char ack[20];
				LLOG("Waiting for ACK retries: " << retry);
				if(Rcv(skt, ack, 20, acktimeout) == 20) {
					LLOG("ACK " << r.id << " part " << Peek32le(ack));
					if(memcmp(ack + 4, &r.id, 16) == 0 && Peek32le(ack) == part) {
						LLOG("Part OK");
						break;
					}
					LLOG("Wrong ACK recieved");
					loss++;
				}
				if(++retry > retries) {
					LLOG("Failed");
					loss++;
					goto end;
				}
				LLOG("RESEND");	
			}
			if(part == URR_LAST)
				break;
			part++;
		}
		Poke32le(b, URR_FIN);
		SendTo(skt, b, b, 0, r);
	}

end:
	r.adrsz = 0;
	{
		Mutex::Lock __(mutex);
		skts.Add(skt);
		processing.UnlinkKey(r.id);
	}
	return;
}

bool UrrServer::Accept(UrrRequest& r)
{
	Buffer<char> buffer(blksize);
	for(;;) {
		r.server = this;
		r.adrsz = 256;
		int q = Rcv(srvskt, ~buffer, blksize, Null, r.adr, &r.adrsz);
		LLOG("Recieved " << q);
		LTIMING("Recieved");
		if(q >= 20) {
			mutex.Enter();
			int tc = GetTickCount();
			while(done.GetCount() && done.Head().time + lingertime < tc) {
				int ii = done.Head().ii;
				donemap[ii].Clear();
				donemap.Unlink(ii);
				done.DropHead();
			}
			while(sent.GetCount() && sent.Head().time + senttime < tc) {
				sentindex.Unlink(sent.Head().ii);
				sent.DropHead();
			}
			char *b = buffer;
			memcpy(&r.id, b + 4, 16);
			r.flags = b[1];
			r.blksize = max(min(Peek16le(b + 2), blksize), 512) - 20;
			if(processing.Find(r.id) >= 0) {
				LLOG("Repeated request while processing " << r.id);
				SOCKET skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				char h[20];
				memcpy(h + 4, &r.id, 16);
				Poke32le(h, URR_PROCESSING);
				sendto(skt, h, 20, 0, (sockaddr *)r.adr, r.adrsz);
				closesocket(skt);
				mutex.Leave();
				loss++;
				continue;
			}
			if(sentindex.Find(r.id) >= 0) {
				LLOG("Reply was already sent for " << r.id);
				mutex.Leave();
				loss++;
				continue;
			}
			int i = donemap.Find(r.id);
			if(i >= 0) {
				LLOG("Old Duplicate request " << r.id);
				mutex.Leave();
				SendBack(r, donemap[i], true);
				loss++;
				continue;
			}
			processing.Put(r.id);
			r.request = String(~buffer + 20, q - 20);
			LLOG("Accepted " << r.id << ": " << r.request);
			mutex.Leave();
			return true;
		}
	}
	return false;
}

void UrrServer::Return(UrrRequest& r, const String& data, bool linger)
{
	LLOG("UrrServer::Return");
	SendBack(r, data, linger);
}

void UrrRequest::Return(const String& s, bool linger)
{
	LLOG("UrrRequest::Return");
	if(server)
		server->Return(*this, s, linger);
}

END_UPP_NAMESPACE
