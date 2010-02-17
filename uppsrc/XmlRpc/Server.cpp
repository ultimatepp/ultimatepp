#include "XmlRpc.h"
#include "XmlRpc.h"

#define LLOG(x) //  LOG(x)

VectorMap< String, void (*)(XmlRpcData&) >& XmlRpcMap(const char *group)
{
	static VectorMap<String, VectorMap< String, void (*)(XmlRpcData&) > > mm;
	return mm.GetAdd(group);
}

void Register(const char *name, void (*method)(XmlRpcData&), const char *group)
{
	XmlRpcMap(group).Add(name, method);
}

String XmlRpcExecute(const String& request, const char *group, const char *peeraddr)
{
	VectorMap< String, void (*)(XmlRpcData&) >& xmlrpcmap = XmlRpcMap(group);
	XmlParser p(request);
	XmlRpcData data;
	String methodname;
	try {
		String r = XmlHeader();
		r << "<methodResponse>\r\n";
		p.ReadPI();
		p.PassTag("methodCall");
		p.PassTag("methodName");
		methodname = p.ReadText();
		LLOG("method name: " << methodname);
		p.PassEnd();
		data.peeraddr = peeraddr;
		data.in = ParseXmlRpcParams(p);
		int q = xmlrpcmap.Find(methodname);
		if(q < 0)
			return FormatXmlRpcError(4, methodname + " method is unknown");
		else {
			(*xmlrpcmap[q])(data);
			if(IsValueArray(data.out)) {
				if(IsError(data.out[0])) {
					LLOG("ProcessingError");
					return FormatXmlRpcError(3, methodname + " Processing error: " + GetErrorText(data.out[0]));
				}
				r << FormatXmlRpcParams(data.out);
			}
			r << "\r\n</methodResponse>\r\n";
		}
		p.PassEnd();
		return r;
	}
	catch(XmlError e) {
		LLOG("XmlError " << e << ": " << p.GetPtr());
		return FormatXmlRpcError(1, methodname + " XML Error: " + e);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		return FormatXmlRpcError(2, methodname + " Value type mismatch at parameter " + AsString(data.ii));
	}
	return Null;
}

int CharFilterNoCr(int c)
{
	return c == '\r' ? 0 : c;
}

String ReadLine(Socket& s)
{
	return Filter(s.ReadUntil('\n'), CharFilterNoCr);
}

bool XmlRpcServer(int port, const char *group)
{
	Socket rpc;
	if(!ServerSocket(rpc, 1234, true, 5))
		return false;
	for(;;) {
		Socket http;
		if(rpc.Accept(http)) {
			LLOG("=== Accepted connection ===================================================");
			String request = ReadLine(http);
			if(http.IsError()) continue;
			VectorMap<String, String> hdr;
			for(;;) {
				String s = ReadLine(http);
				if(s.IsEmpty()) break;
				LLOG(s);
				int q = s.Find(':');
				if(q >= 0)
					hdr.GetAdd(s.Mid(0, q)) = TrimLeft(s.Mid(q + 1));
			}
			if(http.IsError()) continue;
			String r = XmlRpcExecute(http.ReadCount(atoi(hdr.Get("Content-Length", "")), 90000),
			                         group, http.GetPeerAddr());
			LLOG("--------- Server response:\n" << r << "=============");
			String response;
			response <<
				"HTTP/1.1 200 OK\r\n"
				"Date: Mon, 23 May 2005 22:38:34 GMT\r\n"
				"Server: U++\r\n"
				"Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\r\n"
				"Accept-Ranges: bytes\r\n"
				"Content-Length: " << r.GetCount() << "\r\n"
				"Connection: close\r\n"
				"Content-Type: application/soap+xml; charset=utf-8\r\n\r\n" << r;
			http.Write(response);
		}
	}
}
