#include "XmlRpc.h"
#include "XmlRpc.h"

#define LLOG(x)   // DLOG(x)

typedef void (*XmlRpcFnPtr)(XmlRpcData&);

static StaticMutex XmlRpcMapMutex;

VectorMap<String, XmlRpcFnPtr>& XmlRpcMap(const char *group)
{
	static VectorMap<String, VectorMap< String, void (*)(XmlRpcData&) > > mm;
	return mm.GetAdd(group);
}

void Register(const char *name, void (*method)(XmlRpcData&), const char *group)
{
	Mutex::Lock __(XmlRpcMapMutex);
	XmlRpcMap(group).Add(name, method);
}

XmlRpcFnPtr XmlRpcMapGet(const char *group, const char *name)
{
	Mutex::Lock __(XmlRpcMapMutex);
	return XmlRpcMap(group).Get(name, NULL);
}

String (*sXmlRpcMethodFilter)(const String& methodname);

void SetXmlRpcMethodFilter(String (*filter)(const String& methodname))
{
	sXmlRpcMethodFilter = filter;
}

struct XmlRpcError {
	int    code;
	String text;
};

void ThrowXmlRpcError(int code, const char *s)
{
	XmlRpcError e;
	e.code = code;
	e.text = s;
	throw e;
}

void ThrowXmlRpcError(const char *s)
{
	ThrowXmlRpcError(-1, s);
}

static Stream *xmlrpc_trace;

void SetXmlRpcServerTrace(Stream& s)
{
	xmlrpc_trace = &s;
}

String XmlRpcExecute(const String& request, const char *group, const char *peeraddr)
{
	if(xmlrpc_trace)
		*xmlrpc_trace << "XmlRpcRequest:\n" << request << '\n';
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
		XmlRpcMapMutex.Enter();
		if(sXmlRpcMethodFilter)
			methodname = (*sXmlRpcMethodFilter)(methodname);
		void (*fn)(XmlRpcData&) = XmlRpcMapGet(group, methodname);
		if(fn) {
			(*fn)(data);
			if(IsValueArray(data.out)) {
				ValueArray va = data.out;
				if(va.GetCount() && IsError(va[0])) {
					LLOG("ProcessingError");
					return FormatXmlRpcError(XMLRPC_SERVER_PROCESSING_ERROR, "Processing error: " + GetErrorText(data.out[0]));
				}
				r << FormatXmlRpcParams(data.out);
			}
			r << "\r\n</methodResponse>\r\n";
		}
		else {
			XmlRpcMapMutex.Leave();
			return FormatXmlRpcError(XMLRPC_UNKNOWN_METHOD_ERROR, "\'" + methodname + "\' method is unknown");
		}
		p.PassEnd();
		if(xmlrpc_trace)
			*xmlrpc_trace << "Server response:\n" << r << '\n';
		return r;
	}
	catch(XmlRpcError e) {
		LLOG("Client error: " << e.text);
		if(xmlrpc_trace)
			*xmlrpc_trace << "Client error: " << e.text << '\n';
		return FormatXmlRpcError(e.code, e.text);
	}
	catch(XmlError e) {
		LLOG("XmlError " << e << ": " << p.GetPtr());
		if(xmlrpc_trace)
			*xmlrpc_trace << "XmlError: " << e << '\n';
		return FormatXmlRpcError(XMLRPC_SERVER_XML_ERROR, "XML Error: " + e);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		if(xmlrpc_trace)
			*xmlrpc_trace << "ValueTypeMismatch at parameter " << data.ii << '\n';
		return FormatXmlRpcError(XMLRPC_SERVER_PARAM_ERROR, "Parameter mismatch at parameter " + AsString(data.ii));
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
					"Server: U++\r\n"
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
	if(!ServerSocket(rpc, 1234, true, 5))
		return false;
	for(;;) {
		Socket http;
		if(rpc.Accept(http))
			XmlRpcPerform(http, group);
	}
}
