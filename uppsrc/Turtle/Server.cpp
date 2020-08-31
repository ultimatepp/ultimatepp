#include "Turtle.h"
#include "Turtle.brc"

#ifdef PLATFORM_POSIX
#include <wait.h>
#endif

#define LLOG(x)     // RLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {

static Vector<int> sProcessIds;

bool TurtleServer::StartSession()
{
	LLOG("Connect");

#ifdef PLATFORM_POSIX
	mainpid = getpid();
#endif

	IpAddrInfo ipinfo;
	ipinfo.Execute(ip, port, IpAddrInfo::FAMILY_IPV4);

	TcpSocket server;

#ifdef _DEBUG
	int qq = 0;
	for(;;) {
		if(server.Listen(ipinfo, port, 5, false, true))
			break;
		Cout() << "Trying to start listening (other process using the same port?) " << ++qq << "\n";
		Sleep(1000);
	}
#else
	if(!server.Listen(ipinfo, port, 5, false, true)) {
		LLOG("Cannot open server socket for listening!");
		Exit(1);
	}
#endif

	LLOG("Starting to listen on " << port << ", pid: " << getpid());
	socket.Timeout(0); // TODO: Not quite ideal way to make quit work..
	for(;;) {
		SocketWaitEvent we;
		we.Add(socket, WAIT_READ);
		we.Wait(250);
#ifdef PLATFORM_POSIX
		int i = 0;
		while(i < sProcessIds.GetCount())
			if(sProcessIds[i] && waitpid(sProcessIds[i], 0, WNOHANG | WUNTRACED) > 0) {
				WhenTerminate(sProcessIds[i]);
				sProcessIds.Remove(i);
			}
			else
				i++;
#endif
		if(server.IsError())
			server.ClearError();
		if(socket.Accept(server)) {
			HttpHeader  http;
			if(http.Read(socket)) {
				RLOG("Accepted, header read");
				if(websocket.WebAccept(socket, http)) { // TODO: Connection limit, info
					if(sProcessIds.GetCount() >= connection_limit) {
						socket.Close();
						continue;
					}
#ifdef PLATFORM_POSIX
					if(debugmode)
						break;
					int newpid = fork();
					if(newpid == 0)
						break;
					else {
						sProcessIds.Add(newpid);
						WhenConnect(newpid, socket.GetPeerAddr());
						socket.Close();
						continue;
					}
#else
					break;
#endif
				}
				RLOG("Sending HTML");
				String html = String(turtle_html, turtle_html_length);
				html.Replace("%%host%%", "ws://" + Nvl(GetIniKey("turtle_host"), Nvl(host, "localhost")) + ":" + AsString(port));
				HttpResponse(socket, http.scgi, 200, "OK", "text/html", html);
			}
			socket.Close();
		}
	}
	RLOG("Connection established with " << socket.GetPeerAddr() << ", pid: " << getpid());
	server.Close();
	if(socket.IsError())
		LLOG("CONNECT ERROR: " << socket.GetErrorDesc());
	stat_started = GetSysTime();
	return true;
}

void TurtleServer::Broadcast(int signal)
{
#ifdef PLATFORM_POSIX
	if(getpid() == mainpid)
		for(int i = 0; i < sProcessIds.GetCount(); i++)
			kill(sProcessIds[i], signal);
#endif
}
}