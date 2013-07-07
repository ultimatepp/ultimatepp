#include <signal.h>

//#include "Web.h"
#include "Scgi.h"

NAMESPACE_UPP

bool run = true;

void sighandler(int sig)
{
	run = false;
}

Scgi::Scgi(int port)
{
	this->port = port;
	
	signal(SIGABRT, sighandler);
	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
	
/*
	query.CaseSensitive();
	post.CaseSensitive();
*/
}

VectorMap<String, String> Scgi::ParseUri(String const &uri) const
{
	VectorMap<String, String> res;
	const char *p = ~uri;
	while(*p)
	{
		const char *last = p;
		while(*p && *p != '=' && *p != '&')
			p++;
//		String key = case_sensitive ? UrlDecode(last, p) : ToUpper(UrlDecode(last, p));
		String key = ToUpper(UrlDecode(last, p));
		if(*p == '=')
			p++;
		last = p;
		while(*p && *p != '&')
			p++;
		res.Add(key, UrlDecode(last, p));
		if(*p)
			p++;
	}
	return res;
}

void Scgi::Process()
{
	clientIP = clientSock.GetPeerAddr();
	OnAccepted();
	
	HttpHeader header;
	header.Read(clientSock);
	String method = header.GetMethod();
	int64 contentLen = header.GetContentLength();
	serverVars = header.fields;
	header.Clear();
	
	query.Clear();
	if(serverVars.Find("QUERY_STRING") >= 0)
		query = ParseUri(serverVars.Get("QUERY_STRING"));
	
	post.Clear();
	if(method == "POST")
	{
		if (contentLen > 0 && clientSock.IsOpen() && !clientSock.IsEof() && !clientSock.IsError())
		{
			String data = clientSock.Get(contentLen);
			post = ParseUri(data);
		}
	}
	
	OnRequest();

	clientSock.Close();
	OnClosed();
}

bool Scgi::Accept()
{
	return clientSock.Accept(serverSock);
}

void Scgi::Run(int listenCount)
{
	serverSock.Listen(port, listenCount);
	while(run)
		if(Accept())
			Process();
}

END_UPP_NAMESPACE
