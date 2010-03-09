#include "XmlRpc.h"

#define LLOG(x)  // LOG(x)

XmlRpcCall::XmlRpcCall(const char *url)
{
	shorted = true;
	if(url && *url) {
		server.URL(url);
		shorted = false;
	}
	server.ContentType("text/xml");
	timeout = 30000;
}

String XmlRpcExecute(const String& request, const char *group, const char *peeraddr);

Value XmlRpcCall::Execute()
{
	String request = XmlHeader();
	request << XmlTag("methodCall")(XmlTag("methodName")(method) + FormatXmlRpcParams(data.out));
	String response;
	if(shorted)	
		response = XmlRpcExecute(request, "", "127.0.0.1");
	else
		response = server.Post(request).TimeoutMsecs(timeout).ExecuteRedirect();
	LLOG("response: " << response);
	if(IsNull(response)) {
		LLOG("ERROR: " << server.GetError());
		return ErrorValue("Http request failed: " + server.GetError());
	}
	XmlParser p(response);
	try {	
		p.ReadPI();
		p.PassTag("methodResponse");
		if(p.Tag("fault")) {
			Value m = ParseXmlRpcValue(p);
			if(IsValueMap(m)) {
				ValueMap mm = m;
				String s;
				s << "Failed '" << mm["faultString"] << "' (" << mm["faultCode"] << ')';
				error = s;
				LLOG(s);
				return ErrorValue(s);
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
		s << "XmlError " << e << ": " << p.GetPtr();
		LLOG(s);
		error = s;
		return ErrorValue(s);
	}
	return data.in.GetCount() ? data.in[0] : Null;
}
