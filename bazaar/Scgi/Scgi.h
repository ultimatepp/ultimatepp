#ifndef _ScgiServer_ScgiServer_h
#define _ScgiServer_ScgiServer_h

#include <Core/Core.h>

NAMESPACE_UPP

class Scgi
{
	public:
		VectorMap<String, String> const &ServerVars(void) const	{ return serverVars; }
		VectorMap<String, String> const &Query(void) const		{ return query; }
		VectorMap<String, String> const &Post(void) const		{ return post; }
		
		Scgi(int port = 7800);
		
		void Process();
		bool Accept();
		void Run(int listenCount = 10);
		
		String  ClientIP()		{ return clientIP; }
		TcpSocket &ClientSock()	{ return clientSock; }
		
//		void   Write(const String& text) { clientSock.Write(text); }
		
		virtual void OnAccepted() {}
		virtual void OnRequest() {}
		virtual void OnClosed() {}
		
	private:
		int port;
		TcpSocket serverSock, clientSock;
		String clientIP;
		
		VectorMap<String, String> ParseUri(String const &uri) const;

		VectorMap<String, String> serverVars;
		VectorMap<String, String> query;
		VectorMap<String, String> post;
};

END_UPP_NAMESPACE

#endif
