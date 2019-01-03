#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

#define LLOG(x)  //  DLOG(x)

namespace Upp {

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
	ThrowRpcError(RPC_SERVER_PROCESSING_ERROR, s);
}

static Stream *rpc_trace, *suppressed_rpc_trace;
static int rpc_trace_level;
static bool rpc_trace_compress = true;

void SetRpcServerTrace(Stream& s, int level)
{
	rpc_trace = &s;
	rpc_trace_level = level;
}

void SetRpcServerTraceCompress(bool compress)
{
	rpc_trace_compress = compress;
}

void StopRpcServerTrace()
{
	rpc_trace = NULL;
}

void SuppressRpcServerTraceForMethodCall()
{
	suppressed_rpc_trace = rpc_trace;
	rpc_trace = NULL;
}

bool CallRpcMethod(RpcData& data, const char *group, String methodname, const String& request)
{
	LLOG("method name: " << methodname);
	if(sRpcMethodFilter)
		methodname = (*sRpcMethodFilter)(methodname);
	if(rpc_trace) {
		*rpc_trace << "RPC Request:\n";
		if(rpc_trace_compress)
			*rpc_trace << CompressLog(request);
		else
			*rpc_trace << request;
		*rpc_trace << '\n';
	}
	void (*fn)(RpcData&) = RpcMapGet(group, methodname);
	if(!fn)
		return false;
	(*fn)(data);
	return true;
}

struct XmlRpcDo {
	TcpSocket& http;
	RpcData    data;
	String     request;
	String     group;
	String     methodname;
	Value      id;
	bool       json;
	bool       shorted;

	String XmlResult();
	String DoXmlRpc();
	String JsonRpcError(int code, const char *text, const Value& id);
	String JsonResult();
	String ProcessJsonRpc(const Value& v);
	String DoJsonRpc();
	String RpcExecute();
	void   RpcResponse(const String& r);
	void   EndRpc();
	bool   Perform();
	
	XmlRpcDo(TcpSocket& http, const char *group);
};

XmlRpcDo::XmlRpcDo(TcpSocket& http, const char *group)
:	http(http), group(group)
{
	shorted = false;
}

String XmlRpcDo::XmlResult()
{
	String r = XmlHeader();
	r << "<methodResponse>\r\n";
	if(IsValueArray(data.out)) {
		ValueArray va = data.out;
		if(va.GetCount() && IsError(va[0])) {
			LLOG("ProcessingError");
			String e = GetErrorText(data.out[0]);
			if(rpc_trace)
				*rpc_trace << "Processing error: " << e << '\n';
			return FormatXmlRpcError(RPC_SERVER_PROCESSING_ERROR, "Processing error: " + e);
		}
		r << FormatXmlRpcParams(data.out, false);
	}
	r << "\r\n</methodResponse>\r\n";
	return r;
}

String XmlRpcDo::DoXmlRpc()
{
	XmlParser p(request);
	try {
		p.ReadPI();
		p.PassTag("methodCall");
		p.PassTag("methodName");
		methodname = p.ReadText();
		p.PassEnd();
		data.in = ParseXmlRpcParams(p);
		if(!CallRpcMethod(data, group, methodname, request))
			return FormatXmlRpcError(RPC_UNKNOWN_METHOD_ERROR, "\'" + methodname + "\' method is unknown");
		if(!data.rpc && !shorted)
			return Null;
		return XmlResult();
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
		return FormatXmlRpcError(RPC_SERVER_XML_ERROR, "XML Error: " + e);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << data.ii << '\n';
		return FormatXmlRpcError(RPC_SERVER_PARAM_ERROR, "Parameter mismatch at parameter " + AsString(data.ii));
	}
	return Null;
}

String XmlRpcDo::JsonRpcError(int code, const char *text, const Value& id)
{
	Json m;
	m("jsonrpc", "2.0");
	ValueMap err;
	err.Add("code", code);
	err.Add("message", text);
	m("error", err);
	m("id", id);
	return m;
}

String XmlRpcDo::JsonResult()
{
	if(IsValueArray(data.out)) {
		ValueArray va = data.out;
		Value result = Null;
		if(va.GetCount()) {
			if(IsError(va[0])) {
				LLOG("ProcessingError");
				String e = GetErrorText(data.out[0]);
				if(rpc_trace)
					*rpc_trace << "Processing error: " << e << '\n';
				return JsonRpcError(RPC_SERVER_PROCESSING_ERROR, "Processing error: " + e, id);
			}
			result = JsonRpcData(va[0]);
		}
		Json json;
		json("jsonrpc", "2.0");
		if(result.Is<RawJsonText>())
			json.CatRaw("result", result.To<RawJsonText>().json);
		else
			json("result", result);
		json("id", id);
		return json;
	}
	return JsonRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", id);
}

String XmlRpcDo::ProcessJsonRpc(const Value& v)
{
	LLOG("Parsed JSON request: " << v);
	id = v["id"];
	methodname = AsString(v["method"]);
	Value param = v["params"];
	if(param.Is<ValueMap>())
		data.in_map = param;
	else
		data.in = param;
	try {
		if(CallRpcMethod(data, group, methodname, request)) {
			if(!data.rpc && !shorted)
				return Null;
			return JsonResult();
		}
		return JsonRpcError(RPC_UNKNOWN_METHOD_ERROR, "Method not found", id);
	}
	catch(RpcError e) {
		LLOG("Processing error: " << e.text);
		if(rpc_trace)
			*rpc_trace << "Processing error: " << e.text << '\n';
		return JsonRpcError(e.code, e.text, id);
	}
	catch(ValueTypeMismatch) {
		LLOG("ValueTypeMismatch at parameter " << data.ii);
		if(rpc_trace)
			*rpc_trace << "ValueTypeMismatch at parameter " << data.ii << '\n';
		return JsonRpcError(RPC_SERVER_PARAM_ERROR, "Invalid params", id);
	}
}

String XmlRpcDo::DoJsonRpc()
{
	try {
		Value v = ParseJSON(request);
		if(v.Is<ValueMap>())
			return ProcessJsonRpc(v);
		if(v.Is<ValueArray>()) {
			JsonArray a;
			for(int i = 0; i < v.GetCount(); i++)
				a.CatRaw(ProcessJsonRpc(v[i]));
			return v.GetCount() ? ~a : String();
		}
	}
	catch(CParser::Error e) {}	
	return AsJSON(JsonRpcError(RPC_SERVER_JSON_ERROR, "Parse error", Null));
}

String XmlRpcDo::RpcExecute()
{
	json = false;
	try {
		CParser p(request);
		json = p.Char('{') || p.Char('[');
	}
	catch(CParser::Error) {}

	String r;
	String mn;
	TimeStop tm;
	if(json)
		r = DoJsonRpc();
	else
	    r = DoXmlRpc();

	if(rpc_trace) {
		mn << " (" << tm.Elapsed() << " ms)";
		if(rpc_trace_level == 0)
			*rpc_trace << "Rpc " << mn << " finished OK \n";
		else {
			if(rpc_trace_compress)
				*rpc_trace << "Rpc " << mn << " response:\n" << CompressLog(r) << '\n';
			else
				*rpc_trace << "Rpc " << mn << " response:\n" << r << '\n';
		}
	}
	if(suppressed_rpc_trace) {
		if(!rpc_trace)
			rpc_trace = suppressed_rpc_trace;
		suppressed_rpc_trace = NULL;
	}
	return r;
}

void XmlRpcDo::RpcResponse(const String& r)
{
	LLOG("--------- Server response:\n" << r << "=============");
	String response;
	String ts = WwwFormat(GetUtcTime());
	response <<
		"HTTP/1.0 200 OK\r\n"
		"Date: " << ts << "\r\n"
		"Server: U++ RPC server\r\n"
		"Content-Length: " << r.GetCount() << "\r\n"
		"Connection: close\r\n"
		"Content-Type: application/" << (json ? "json" : "xml") << "\r\n\r\n"
		<< r;
	LLOG(response);
	if(r.GetCount())
		http.Put(response);
}

void XmlRpcDo::EndRpc()
{
	RpcResponse(json ? JsonResult() : XmlResult());
}

void RpcData::EndRpc()
{
	if(rpc) {
		rpc->EndRpc();
		rpc = NULL;
	}
}

bool XmlRpcDo::Perform()
{
	LLOG("=== Accepted connection ===================================================");
	HttpHeader hdr;
	if(hdr.Read(http) && hdr.GetMethod() == "POST") {
		int len = atoi(hdr["content-length"]);
		if(len > 0 && len < 1024 * 1024 * 1024) {
			request = http.GetAll(len);
			data.peeraddr = http.GetPeerAddr();
			data.rpc = this;
			String r = RpcExecute();
			if(data.rpc)
				RpcResponse(r);
			return true;
		}
	}
	http.Put("HTTP/1.0 400 Bad request\r\n"
	         "Server: U++\r\n\r\n");
	return false;
}

bool RpcPerform(TcpSocket& http, const char *group)
{
	return XmlRpcDo(http, group).Perform();
}

String RpcExecuteShorted(const String& request_)
{
	HttpRequest dummy;
	XmlRpcDo h(dummy, "");
	h.request = request_;
	h.shorted = true;
	h.data.peeraddr = "127.0.0.1";
	return h.RpcExecute();
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

}
