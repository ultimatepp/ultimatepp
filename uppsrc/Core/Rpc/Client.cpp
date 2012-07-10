#include "XmlRpc.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

static bool sLogRpcCalls;

void LogRpcRequests(bool b)
{
	sLogRpcCalls = b;
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
	json = false;
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

Value RpcRequest::Retry()
{
	ClearError();
	shouldExecute = true;
	return Execute();
}

Value JsonRpcData(const Value& v)
{
	if(IsDateTime(v))
		return FormatIso8601(v);
	return v;
}

Value RpcRequest::Execute()
{
	if(!shouldExecute)
		return Value();
	shouldExecute = false;
	String request;
	if(json) {
		static Atomic id;
		Json json;
		json("jsonrpc", "2.0")
		    ("method", method);
		if(data.out.GetCount()) {
			JsonArray a;
			for(int i = 0; i < data.out.GetCount(); i++)
				a << JsonRpcData(data.out[i]);
			json("params", a);
		}
		else
		if(data.out_map.GetCount()) {
			Json m;
			for(int i = 0; i < data.out_map.GetCount(); i++)
				m((String)data.out_map.GetKey(i), JsonRpcData(data.out_map.GetValue(i)));
			json("params", m);
		}
		json("id", id);
		AtomicInc(id);
		request = ~json;
	}
	else {
		request = XmlHeader();
		request << XmlTag("methodCall")(XmlTag("methodName")(method) + FormatXmlRpcParams(data.out));
	}
	if(sLogRpcCalls)
		RLOG("XmlRpc call request:\n" << request);
	String response;
	New();
	data.out.Clear();
	data.out_map.Clear();
	if(shorted)	
		response = RpcExecute(request, "", "127.0.0.1");
	else
		response = Post(request).Execute();
	if(sLogRpcCalls)
		RLOG("XmlRpc call response:\n" << response);
	if(IsNull(response)) {
		faultCode = XMLRPC_CLIENT_HTTP_ERROR;
		faultString = GetErrorDesc();
		error = "Http request failed: " + faultString;
		LLOG(error);
		return ErrorValue(error);
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
					return result;
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
						return ErrorValue(error);
					}
				}
			}
			String s;
			faultString = "Invalid response";
			faultCode = XMLRPC_CLIENT_RESPONSE_ERROR;
			error = faultString;
			LLOG(error);
			return ErrorValue(error);
		}
		catch(CParser::Error e) {
			String s;
			faultString = e;
			faultCode = XMLRPC_CLIENT_JSON_ERROR;
			error.Clear();
			error << "JSON Error: " << faultString;
			LLOG(error);
			return ErrorValue(error);
		}
	}
	else {
		XmlParser p(response);
		try {	
			p.ReadPI();
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
					return ErrorValue(error);
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
			faultCode = XMLRPC_CLIENT_XML_ERROR;
			error.Clear();
			error << "XML Error: " << faultString;
			LLOG(error << ": " << p.GetPtr());
			return ErrorValue(error);
		}
		return data.in.GetCount() ? data.in[0] : Null;
	}
}

void RpcRequest::ClearError()
{
	faultCode = 0;
	faultString.Clear();
	error.Clear();
}

END_UPP_NAMESPACE
