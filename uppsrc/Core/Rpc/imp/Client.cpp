#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>

namespace Upp {

#define LLOG(x)  // LOG(x)

static bool sLogRpcCalls;
static bool sLogRpcCallsCompress = true;

void LogRpcRequests(bool b)
{
	sLogRpcCalls = b;
}

void LogRpcRequestsCompress(bool b)
{
	sLogRpcCallsCompress = b;
}

RpcRequest& RpcRequest::Url(const char *url)
{
	shorted = true;
	if(url && *url) {
		HttpRequest::Url(url);
		shorted = false;
	}
	shouldExecute = true;
	return *this;
}

RpcRequest& RpcRequest::Method(const char *name)
{
	shouldExecute = true;
	method = name;
	data.Reset();
	error.Clear();
	return *this;
}

void RpcRequest::Init()
{
	ContentType("text/xml");
	RequestTimeout(30000);
	MaxRetries(0);
	json = false;
	supports_i8 = false;
}

RpcRequest::RpcRequest(const char *url)
{
	Url(url);
	Init();
}

RpcRequest::RpcRequest()
{
	Url(NULL);
	Init();
}

String XmlRpcExecute(const String& request, const char *group, const char *peeraddr, bool& json);
String XmlRpcExecute(const String& request, const char *group, const char *peeraddr);

RpcGet RpcRequest::Retry()
{
	ClearError();
	shouldExecute = true;
	return Execute();
}

Value JsonRpcData(const Value& v)
{
	if(IsDateTime(v) && !IsNull(v))
		return FormatIso8601(v);
	return v;
}

String RpcExecuteShorted(const String& request_);

RpcGet RpcRequest::Execute()
{
	if(!shouldExecute)
		return RpcGet();
	shouldExecute = false;
	String request;
	if(json) {
		ContentType("application/json");
		static Atomic id;
		Json json;
		json("jsonrpc", "2.0")
		    ("method", method);
		if(data.out.GetCount()) {
			JsonArray a;
			for(int i = 0; i < data.out.GetCount(); i++) {
				const Value& v = data.out[i];
				if(v.Is<RawJsonText>())
					a.CatRaw(v.To<RawJsonText>().json);
				else
					a << JsonRpcData(v);
			}
			json("params", a);
		}
		else
		if(data.out_map.GetCount()) {
			Json m;
			for(int i = 0; i < data.out_map.GetCount(); i++) {
				const Value& v = data.out_map.GetValue(i);
				String key = (String)data.out_map.GetKey(i);
				if(v.Is<RawJsonText>())
					m.CatRaw(key, v.To<RawJsonText>().json);
				else
					m(key, JsonRpcData(v));
			}
			json("params", m);
		}
		json("id", id);
		AtomicInc(id);
		request = ~json;
	}
	else {
		ContentType("text/xml");
		request = XmlHeader();
		if(protocol_version.GetCount())
			request << "<!--protocolVersion=\"" << protocol_version << "\"-->\r\n";
		request << XmlTag("methodCall")(XmlTag("methodName")(method) + FormatXmlRpcParams(data.out, supports_i8));
	}
	if(sLogRpcCalls) {
		if(sLogRpcCallsCompress)
			RLOG("=== XmlRpc call request:\n" << CompressLog(request));
		else
			RLOG("=== XmlRpc call request:\n" << request);
	}
	String response;
	New();
	if(shorted)
		response = RpcExecuteShorted(request);
	else
		response = Post(request).Execute();
	if(sLogRpcCalls) {
		if(sLogRpcCallsCompress)
			RLOG("=== XmlRpc call response:\n" << CompressLog(response));
		else
			RLOG("=== XmlRpc call response:\n" << response);
	}
	RpcGet h;
	if(IsNull(response)) {
		faultCode = RPC_CLIENT_HTTP_ERROR;
		faultString = GetErrorDesc();
		error = "Http request failed: " + faultString;
		LLOG(error);
		h.v = ErrorValue(error);
		return h;
	}
	if(json) {
		try {
			Value r = ParseJSON(response);
			if(IsValueMap(r)) {
				ValueMap m = r;
				Value result = m["result"];
				if(!result.IsVoid()) {
					data.in.Clear();
					data.in.Add(result);
					data.ii = 0;
					h.v = result;
					return h;
				}
				Value e = m["error"];
				if(IsValueMap(e)) {
					Value c = e["code"];
					Value m = e["message"];
					if(IsNumber(c) && IsString(m)) {
						faultCode = e["code"];
						faultString = e["message"];
						error.Clear();
						error << "Failed '" << faultString << "' (" << faultCode << ')';
						LLOG(s);
						h.v = ErrorValue(error);
						return h;
					}
				}
			}
			String s;
			faultString = "Invalid response";
			faultCode = RPC_CLIENT_RESPONSE_ERROR;
			error = faultString;
			LLOG(error);
			h.v = ErrorValue(error);
			return h;
		}
		catch(CParser::Error e) {
			String s;
			faultString = e;
			faultCode = RPC_CLIENT_JSON_ERROR;
			error.Clear();
			error << "JSON Error: " << faultString;
			LLOG(error);
			h.v = ErrorValue(error);
			return h;
		}
	}
	else {
		XmlParser p(response);
		try {
			p.ReadPI();
			while(p.IsComment())
				p.ReadComment();
			p.PassTag("methodResponse");
			if(p.Tag("fault")) {
				Value m = ParseXmlRpcValue(p);
				if(IsValueMap(m)) {
					ValueMap mm = m;
					faultString = mm["faultString"];
					faultCode = mm["faultCode"];
					error.Clear();
					error << "Failed '" << faultString << "' (" << faultCode << ')';
					LLOG(s);
					h.v = ErrorValue(error);
					return h;
				}
			}
			else {
				data.in = ParseXmlRpcParams(p);
				data.ii = 0;
				p.PassEnd();
			}
		}
		catch(XmlError e) {
			String s;
			faultString = e;
			faultCode = RPC_CLIENT_XML_ERROR;
			error.Clear();
			error << "XML Error: " << faultString;
			LLOG(error << ": " << p.GetPtr());
			h.v = ErrorValue(error);
			return h;
		}
		h.v = data.in.GetCount() ? data.in[0] : Null;
		return h;
	}
}

void RpcRequest::ClearError()
{
	faultCode = 0;
	faultString.Clear();
	error.Clear();
}

}
