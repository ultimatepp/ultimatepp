#ifndef _ScgiServer_ScgiServer_h
#define _ScgiServer_ScgiServer_h

#include <Web/Web.h>

using namespace Upp;

class ScgiServer {
public:
	HttpQuery query;
	HttpQuery post;
	
	ScgiServer(int port = 7800);
	
	void Run();	
	
	dword ClientIP()    { return clientIP; }
	Socket ClientSock() { return clientSock; }
	
	bool HasPostData()  { return hasPostData; }
	
	virtual void OnAccepted() {}
	virtual void OnRequest() {}
	virtual void OnClosed() {}
	
protected:
	int port;
	Socket serverSock, clientSock;
	VectorMap<String,String> serverVars;
	dword clientIP;
	bool hasPostData;
};

#endif
