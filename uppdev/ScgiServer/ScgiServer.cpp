#include <signal.h>

#include <Core/Core.h>
#include <Web/Web.h>

#include "ScgiServer.h"

using namespace Upp;

bool run = true;

void sighandler(int sig)
{
	run = false;
}

ScgiServer::ScgiServer(int port)
{
	this->port = port;
	
	signal(SIGABRT, sighandler);
	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);
}

void ScgiServer::Run()
{
	ServerSocket(serverSock, port);
	while (run) {
		if (serverSock.Accept(clientSock, &clientIP)) {
			WhenAccepted();
			
			String sLen = clientSock.ReadUntil(':');
			int len = atoi(sLen);
			String data;
			
			if (clientSock.IsOpen() && !clientSock.IsEof() && !clientSock.IsError()) {
				// len + 1 = data plus the trailing , as in SCGI spec
				data = clientSock.ReadCount(len+1, 3000);
			}
			
			String key;
			int spos = 0;			
			for (int i=0; i < data.GetCount(); i++) {
				if (data[i] == 0) {
					if (key.IsEmpty())
						key = data.Mid(spos, i-spos);
					else {
						String value = data.Mid(spos, i-spos);
						
						serverVars.Add(key, value);
						key.Clear();
					}
					
					spos = i + 1;
				}
			}
			
			query.SetURL("?" + serverVars.Get("QUERY_STRING"));
			
			hasPostData = false;
			if (serverVars.Get("REQUEST_METHOD") == "POST") {
				len = atoi(serverVars.Get("CONTENT_LENGTH"));
				if (len > 0 && clientSock.IsOpen() && !clientSock.IsEof() && 
				    !clientSock.IsError())
				{
					data = clientSock.ReadCount(len, 3000);
					post.SetURL("?" + data);
					hasPostData = true;
				}
			}
			
			WhenRequest();

			clientSock.Close();
			serverVars.Clear();
			query.Clear();
			
			WhenClosed();
		}
	}
}
