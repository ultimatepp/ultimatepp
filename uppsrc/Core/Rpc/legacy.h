#define XMLRPC_METHOD(x)  RPC_METHOD(x)

#define XMLRPC_GMETHOD(x, group) RPC_GMETHOD(x, group)

typedef RpcError XmlRpcError;

inline
void   SetXmlRpcMethodFilter(String (*filter)(const String& methodname))
{
	SetRpcMethodFilter(filter);
}

inline
String XmlRpcExecute(const String& request, const char *group, const char *peeraddr)
{
	RpcExecute(request, group, peeraddr);
}

inline
bool   XmlRpcPerform(TcpSocket& http, const char *group)
{
	RpcPerform(http, group);
}

inline
bool   XmlRpcServerLoop(int port, const char *group = NULL)
{
	RpcServerLoop(port, group);
}

inline
void   ThrowXmlRpcError(int code, const char *s)
{
	ThrowXmlRpcError(code, s);
}

inline
void   ThrowXmlRpcError(const char *s)
{
	ThrowXmlRpcError(s);
}

inline
void LogXmlRpcRequests(bool b = true)
{
	LogRpcRequests(b);
}

inline
void SetXmlRpcServerTrace(Stream& s, int level = 1)
{
	SetRpcServerTrace(s, level);
}
