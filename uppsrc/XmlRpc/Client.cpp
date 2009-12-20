#include "XmlRpc.h"

#define LLOG(x)  LOG(x)

Value XmlRpcCall::Execute()
{
	String request = XmlHeader();
	request << XmlTag("methodCall")(XmlTag("methodName")(method) + FormatXmlRpcParams(data.out));
	String response = server.Post(request).TimeoutMsecs(timeout).ExecuteRedirect();
	LLOG("response: " << response);
	if(IsNull(response)) {
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
		LOG(s);
		error = s;
		return ErrorValue(s);
	}
	return data.in.GetCount() ? data.in[0] : Null;
}
