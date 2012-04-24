#include "XmlRpc.h"
#include "XmlRpc.h"

#define LLOG(x)   // DLOG(x)

NAMESPACE_UPP

int CharFilterNoCr(int c)
{
	return c == '\r' ? 0 : c;
}

String ReadLine(Socket& s)
{
	return Filter(s.ReadUntil('\n'), CharFilterNoCr);
}

bool XmlRpcPerform(Socket& http, const char *group)
{
	LLOG("=== Accepted connection ===================================================");
	String request = ToUpper(ReadLine(http));
	LLOG(request);
	if(http.IsError() || request.GetCount() == 0)
		return false;
	if(request.Find("POST") >= 0 || request.Find("HTTP") >= 0) {
		VectorMap<String, String> hdr;
		for(;;) {
			String s = ReadLine(http);
			if(s.IsEmpty()) break;
			LLOG(s);
			int q = s.Find(':');
			if(q >= 0)
				hdr.GetAdd(s.Mid(0, q)) = TrimLeft(s.Mid(q + 1));
		}
		if(!http.IsError()) {
			int len = atoi(hdr.Get("Content-Length", ""));
			String r;
			if(len > 0 && len < 1024 * 1024 * 1024) {
				r = XmlRpcExecute(http.ReadCount(len, 90000),
			                      group, http.GetPeerAddr());
				LLOG("--------- Server response:\n" << r << "=============");
				String response;
				String ts = WwwFormat(GetUtcTime());
				response <<
					"HTTP/1.0 200 OK\r\n"
					"Date: " << ts << "\r\n"
					"Server: U++ XMLRPC server\r\n"
					"Content-Length: " << r.GetCount() << "\r\n"
					"Connection: close\r\n"
					"Content-Type: text/xml\r\n\r\n"
					<< r;
				LLOG(response);
				http.Write(response);
				return true;
			}
		}
	}
	http.Write("HTTP/1.0 400 Bad request\r\n"
	           "Server: U++\r\n\r\n");
	return false;
}

bool XmlRpcServer(int port, const char *group)
{
	Socket rpc;
	if(!ServerSocket(rpc, port, true, 5))
		return false;
	for(;;) {
		Socket http;
		if(rpc.Accept(http))
			XmlRpcPerform(http, group);
	}
}

END_UPP_NAMESPACE
