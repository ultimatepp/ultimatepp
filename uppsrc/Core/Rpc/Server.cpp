#include "Rpc.h"

#define LLOG(x)  //  DLOG(x)

NAMESPACE_UPP

typedef void (*RpcFnPtr)(RpcData&);

static StaticMutex RpcMapMutex;

VectorMap<String, RpcFnPtr>& RpcMap(const char *group)
{
	static VectorMap<String, VectorMap< String, void (*)(RpcData&) > > mm;
	return mm.GetAdd(group);
}

void Register(const char *name, void (*method)(RpcData&), const char *group)
{
	Mutex::Lock __(RpcMapMutex);
	RpcMap(group).Add(name, method);
}

RpcFnPtr RpcMapGet(const char *group, const char *name)
{
	Mutex::Lock __(RpcMapMutex);
	return RpcMap(group).Get(name, NULL);
}

String (*sRpcMethodFilter)(const String& methodname);

void SetRpcMethodFilter(String (*filter)(const String& methodname))
{
	sRpcMethodFilter = filter;
}

void ThrowRpcError(int code, const char *s)
{
	RpcError e;
	e.code = code;
	e.text = s;
	throw e;
}

void ThrowRpcError(const char *s)
{
	ThrowRpcError(XMLRPC_SERVER_PROCESSING_ERROR, s);
}

static Stream *rpc_trace;
static int rpc_trace_level;

void SetRpcServerTrace(Stream& s, int level)
{
	rpc_trace = &s;
	rpc_trace_level = level;
}

bool CallRpcMethod(RpcData& data, const char *group, String methodname)
{
	LLOG("method name: " << methodname);
	if(rpc_trace && rpc_trace_level == 0)
		*rpc_trace << "RpcRequest method:\n" << methodname << '\n';
	if(sRpcMethodFilter)
		methodname = (*sRpcMethodFilter)(methodname);
	void (*fn)(RpcData&) = RpcMapGet(group, methodname);
	if(!fn)
		return false;
	(*fn)(data);
	return true;
}

String DoXmlRpc(const String& request, const char *group, const char *peeraddr)
{
	XmlParser p(request);
	RpcData data;
	try {
		String methodname;
		String r = XmlHeader();
		r << "<methodResponse>\r\n";
		p.ReadPI();
		p.PassTag("methodCall");
		p.PassTag("methodName");
		methodname = p.ReadText();
		p.PassEnd();
		data.peeraddr = peeraddr;
		data.in = ParseXmlRpcParams(p);
		if(CallRpcMethod(data, group, methodname)) {
			if(IsValueArray(data.out)) {
				ValueArray va = data.out;
				if(va.GetCount() && IsError(va[0])) {
					LLOG("ProcessingError");
					String e = GetErrorText(data.out[0]);
					if(rpc_trace)
						*rpc_trace << "Processing error: " << e << '\n';
					return FormatXmlRpcError(XMLRPC_SERVER_PROCESSING_ERROR, "Processing error: " + e);
				}
				r << FormatXmlRpcParams(data.out);
			}
			r << "\r\n</methodResponse>\r\n";
		}
		else
			return FormatXmlRpcError(XMLRPC_UNKNOWN_METHOD_ERROR, "\'" + methodname + "\' method is unknown");
		p.PassEnd();
		return r;
	}
	catch(RpcError e) {
		LLOG("Processing error: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing error: " << e.text << '\n';
		return FormatXmlRpcError(e.code, e.text);
	}
	catch(XmlError e) {
		LLOG("XmlError " << e << ": " << p.GetPtr());
		if(rpc_trace)
			*rpc_trace << "XmlError: " << e << '\n';
		return FormatXmlRpcError(XMLRPC_SERVER_XML_ERROR, "XML Error: " + e);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << data.ii << '\n';
		return FormatXmlRpcError(XMLRPC_SERVER_PARAM_ERROR, "Parameter mismatch at parameter " + AsString(data.ii));
	}
	return Null;
}

Value JsonRpcError(int code, const char *text, const Value& id)
{
	ValueMap m;
	m.Add("jsonrpc", "2.0");
	ValueMap err;
	err.Add("code", code);
	err.Add("message", text);
	m.Add("error", err);
	m.Add("id", id);
	return m;
}

Value ProcessJsonRpc(const Value& v, const char *group, const char *peeraddr)
{
	LLOG("Parsed JSON request: " << v);
	Value id = v["id"];
	Value methodname = v["method"];
	Value param = v["params"];
	RpcData data;
	data.peeraddr = peeraddr;
	if(param.Is<ValueMap>())
		data.in_map = param;
	else
		data.in = param;
	try {
		if(CallRpcMethod(data, group, methodname)) {
			if(IsValueArray(data.out)) {
				ValueArray va = data.out;
				Value result = Null;
				if(va.GetCount()) {
					if(IsError(va[0])) {
						LLOG("ProcessingError");
						String e = GetErrorText(data.out[0]);
						if(rpc_trace)
							*rpc_trace << "Processing error: " << e << '\n';
						return JsonRpcError(XMLRPC_SERVER_PROCESSING_ERROR, "Processing error: " + e, id);
					}
					result = JsonRpcData(va[0]);
				}
				ValueMap m;
				m.Add("jsonrpc", "2.0");
				m.Add("result", result);
				m.Add("id", id);
				return m;
			}
		}
		return JsonRpcError(XMLRPC_UNKNOWN_METHOD_ERROR, "Method not found", id);
	}
	catch(RpcError e) {
		LLOG("Processing error: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing error: " << e.text << '\n';
		return AsJSON(JsonRpcError(e.code, e.text, id));
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << data.ii << '\n';
		return AsJSON(JsonRpcError(XMLRPC_SERVER_PARAM_ERROR, "Invalid params", id));
	}
}

String DoJsonRpc(const String& request, const char *group, const char *peeraddr)
{
	try {
		Value v = ParseJSON(request);
		if(v.Is<ValueMap>()) {
			Value r = ProcessJsonRpc(v, group, peeraddr);
			return r.IsVoid() ? String() : AsJSON(r);
		}
		if(v.Is<ValueArray>()) {
			JsonArray a;
			bool r = false;
			for(int i = 0; i < v.GetCount(); i++) {
				r = true;
				a << ProcessJsonRpc(v[i], group, peeraddr);
			}
			return r ? ~a : String();
		}
	}
	catch(CParser::Error e) {}	
	return AsJSON(JsonRpcError(XMLRPC_SERVER_JSON_ERROR, "Parse error", Null));
}

String RpcExecute(const String& request, const char *group, const char *peeraddr, bool& json)
{
	if(rpc_trace && rpc_trace_level == 1)
		*rpc_trace << "RPC Request:\n" << request << '\n';
	CParser p(request);
	String r;
	if(p.Char('{') || p.Char('[')) {
		json = true;
		r = DoJsonRpc(request, group, peeraddr);
	}
	else {
		json = false;
	    r = DoXmlRpc(request, group, peeraddr);
	}
	if(rpc_trace)
		if(rpc_trace_level == 0)
			*rpc_trace << "Rpc finished OK\n";
		else
			*rpc_trace << "Server response:\n" << r << '\n';
	return r;
}

String RpcExecute(const String& request, const char *group, const char *peeraddr)
{
	bool dummy;
	return RpcExecute(request, group, peeraddr, dummy);
}

bool RpcPerform(TcpSocket& http, const char *group)
{
	LLOG("=== Accepted connection ===================================================");
	HttpHeader hdr;
	if(hdr.Read(http) && hdr.GetMethod() == "POST") {
		int len = atoi(hdr["content-length"]);
		if(len > 0 && len < 1024 * 1024 * 1024) {
			bool json;
			String r = RpcExecute(http.GetAll(len), group, http.GetPeerAddr(), json);
			LLOG("--------- Server response:\n" << r << "=============");
			String response;
			String ts = WwwFormat(GetUtcTime());
			response <<
				"HTTP/1.0 200 OK\r\n"
				"Date: " << ts << "\r\n"
				"Server: U++ XMLRPC server\r\n"
				"Content-Length: " << r.GetCount() << "\r\n"
				"Connection: close\r\n"
				"Content-Type: text/" << (json ? "json" : "xml") << "\r\n\r\n"
				<< r;
			LLOG(response);
			if(r.GetCount())
				http.Put(response);
			return true;
		}
	}
	http.Put("HTTP/1.0 400 Bad request\r\n"
	         "Server: U++\r\n\r\n");
	return false;
}

bool RpcServerLoop(int port, const char *group)
{
	TcpSocket rpc;
	if(!rpc.Listen(port, 5))
		return false;
	for(;;) {
		TcpSocket http;
		if(http.Accept(rpc))
			RpcPerform(http, group);
	}
}

END_UPP_NAMESPACE
