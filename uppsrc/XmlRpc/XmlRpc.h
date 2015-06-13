#ifndef _XmlRpc_XmlRpc2_h
#define _XmlRpc_XmlRpc2_h

#include <Web/Web.h>
#include <Core/Rpc/Rpc.h>

NAMESPACE_UPP

bool   XmlRpcPerform(Socket& http, const char *group);
bool   XmlRpcServer(int port = 80, const char *group = NULL);

class XmlRpcCall {
	bool       shorted;
	HttpClient server;
	RpcData    data;
	String     method;
	String     error;
	String     faultString;
	int        faultCode;
	int        timeout;
	bool       shouldExecute;

public:
	XmlRpcCall& Method(const char *name);

	template <class T>
	XmlRpcCall& operator<<(const T& x)                { data << x; return *this; }
	template <class T>
	void        Set(int i, const T& x)                { data.Set(i, x); }

	Value       Execute();
	Value       Retry();

	template <class T>
	bool operator>>(T& x)                             { if(IsError(Execute())) return false;
	                                                    try { data >> x; } catch(ValueTypeMismatch) { return false; } return true; }
	XmlRpcCall& operator()(const char *method)        { Method(method); return *this; }

//$-template<class T1, ...> XmlRpcCall& operator()(const char *method, T1 p1, ...)
#define E__Templ(I)  class COMBINE(T, I)
#define E__Decl(I)   const COMBINE(T, I)& COMBINE(p, I)
#define E__Param(I)  *this << COMBINE(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	XmlRpcCall& operator()(const char *method, __List##I(E__Decl)) { \
		Method(method); \
		__List##I(E__Param); \
		return *this; \
	}

	__Expand20(E__Body)
#undef E__Templ
#undef E__Decl
#undef E__Param
#undef E__Body
//$+

	String GetFaultString() const                               { return faultString; }
	int    GetFaultCode() const                                 { return faultCode; }
	String GetError() const                                     { return error; }
	String GetMethod() const                                    { return method; }
	void   ClearError();
	
	XmlRpcCall& Proxy(const String& host, int port)             { server.Proxy(host, port); return *this; }
	XmlRpcCall& Proxy(const char *url)                          { server.Proxy(url); return *this; }
	XmlRpcCall& ProxyAuth(const String& usr, const String& pwd) { server.ProxyAuth(usr, pwd); return *this; }

	XmlRpcCall& MaxRequestSize(int maxsize)                     { server.MaxContentSize(maxsize); return *this; }

	XmlRpcCall& TimeOut(int msec)                               { timeout = msec; return *this; }
	XmlRpcCall& URL(const char *url);
	
	XmlRpcCall(const char *url);
	XmlRpcCall();
};

void LogXmlRpcCalls(bool b = true);

int CharFilterNoCr(int c);

END_UPP_NAMESPACE

#endif
