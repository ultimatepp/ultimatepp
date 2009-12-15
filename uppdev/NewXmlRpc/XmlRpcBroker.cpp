#include "XmlRpc.h"

#define LLOG(x) LOG(x)

VectorMap<String, Callback1<XmlRpcData&> > xmlrpcmap;

void Register(const char *name, Callback1<XmlRpcData&> cb)
{
	xmlrpcmap.Add(name, cb);
}

String XmlRpcExecute(const String& request)
{
	XmlParser p(request);
	try {
		String r = XmlHeader();
		r << "<methodResponse>\r\n";
		p.ReadPI();
		p.PassTag("methodCall");
		p.PassTag("methodName");
		String methodname = p.ReadText();
		LLOG("method name: " << methodname);
		p.PassEnd();
		XmlRpcData data;
		data.in = ParseXmlRpcParams(p);
		int q = xmlrpcmap.Find(methodname);
		if(q >= 0) {
			xmlrpcmap[q](data);
			if(IsValueArray(data.out))
				r << FormatXmlRpcParams(data.out);
			r << "\r\n</methodResponse>\r\n";
		}
		p.PassEnd();
		return r;
	}
	catch(XmlError e) { // Add error handling!
		LOG("XmlError " << e << ": " << p.GetPtr());
	}
	return Null;
}

Value XmlRpcCall(const char *name, XmlRpcData& param)
{
	String request = XmlHeader();
	request << XmlTag("methodCall")(XmlTag("methodName")(name) + FormatXmlRpcParams(param.out));

	DDUMP(request);

	String response = XmlRpcExecute(request); //!@!
	
	DDUMP(response);

	XmlParser p(response);
	try {	
		p.ReadPI();
		p.PassTag("methodResponse");
		param.in = ParseXmlRpcParams(p);
		param.ii = 0;
		p.PassEnd();
	}
	catch(XmlError e) {
		LOG("XmlError " << e << ": " << p.GetPtr());
	}
	
	return param.in.GetCount() ? param.in[0] : Null;
}
