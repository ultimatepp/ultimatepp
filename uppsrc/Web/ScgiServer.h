#ifndef _ScgiServer_ScgiServer_h
#define _ScgiServer_ScgiServer_h

class ScgiServer {
public:
	HttpQuery query;
	HttpQuery post;
	
	ScgiServer(int port = 7800);
	
	void Process();
	bool Accept();
	void Run(int listenCount = 10);
	
	dword  ClientIP()    { return clientIP; }
	Socket ClientSock()  { return clientSock; }
	
	void   Write(const String& text) { clientSock.Write(text); }
	
	bool HasPostData()   { return hasPostData; }
	
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
