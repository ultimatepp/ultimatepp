#include "Turtle.h"
#include "Turtle.brc"

#ifdef PLATFORM_POSIX
#include <sys/wait.h>
#endif

#define LLOG(x)     // RLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {

static Vector<int> sChildPids;

static bool sSendTurtleHtml(TcpSocket& s, const String& host, int port)
{
	HttpHeader h;
	if(!h.Read(s))
		return false;
	LLOG("Sending Turtle HTML");
	String html = String(turtle_html, turtle_html_length);
	html.Replace("%%host%%", Format("ws://%s:%d", host, port));
	return HttpResponse(s, h.scgi, 200, "OK", "text/html", html);
}

static void sUpdateChildList()
{
#ifdef PLATFORM_POSIX
	int i = 0;
	while(i < sChildPids.GetCount()) {
		if(sChildPids[i] && waitpid(sChildPids[i], 0, WNOHANG | WUNTRACED) > 0) {
			TurtleServer::WhenTerminate(sChildPids[i]);
			sChildPids.Remove(i);
		}
		else ++i;
	}
#endif
}

void TurtleServer::Broadcast(int signal)
{
#ifdef PLATFORM_POSIX
	if(getpid() == mainpid)
		for(int i = 0; i < sChildPids.GetCount(); i++)
			kill(sChildPids[i], signal);
#endif
}

bool TurtleServer::StartSession()
{
	// TODO: See if we can add secure websocket (wss://) support.
	
	LLOG("Connect");

#ifdef PLATFORM_POSIX
	mainpid = getpid();
#endif

	IpAddrInfo ipinfo;
	ipinfo.Execute(ip, port, IpAddrInfo::FAMILY_IPV4);

	TcpSocket server;

#ifdef _DEBUG
	int cnt = 0;
	while(!server.Listen(ipinfo, port, 5, false, true)) {
		LLOG("Trying to start listening (other process using the same port?) " << ++cnt);
		Sleep(1000);
	}
#else
	if(!server.Listen(ipinfo, port, 5, false, true)) {
		LLOG("Cannot open server socket for listening!");
		Exit(1);
	}
#endif

	LLOG("Starting to listen on " << port << ", pid: " << getpid());

	for(;;) {
		sUpdateChildList();
		if(server.IsError())
			server.ClearError();
		TcpSocket socket;
		if(!socket.Accept(server))
			continue;
		if(!sSendTurtleHtml(socket, host, port))
			continue;
		websocket.NonBlocking();
		while(!websocket.Accept(server))
			Sleep(20);
		LLOG("Websocket connection accepted. IP: " << websocket.GetPeerAddr());
#ifdef PLATFORM_POSIX
		if(sChildPids.GetCount() >= connection_limit)
			continue;
		if(debugmode)
			break;
		int newpid = fork();
		if(!newpid)
			break;
		else {
			LLOG("Process forked. Pid: " << newpid);
			sChildPids.Add(newpid);
			WhenConnect(newpid, websocket.GetPeerAddr());
			continue;
		}
#else
		break;
#endif
	}

	server.Close();
	stat_started = GetSysTime();
	return true;
}
}
