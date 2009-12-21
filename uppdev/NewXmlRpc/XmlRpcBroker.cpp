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
		return ErrorValue();
	}
	
	return param.in.GetCount() ? param.in[0] : Null;
}
