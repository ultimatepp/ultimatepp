#include "Local.h"

#include "Turtle.brc"

#ifdef PLATFORM_POSIX
#include <sys/wait.h>
#endif

#ifdef GUI_TURTLE

NAMESPACE_UPP

#define LLOG(x)     // DLOG(x)
#define LDUMP(x)    // DDUMP(x)
#define LTIMING(x)

int         Ctrl::main_pid;
Vector<int> Ctrl::pid;
bool        Ctrl::quit;

void Ctrl::Broadcast(int signal) {
#ifdef PLATFORM_POSIX
	if(getpid() == main_pid)
		for(int i = 0; i < pid.GetCount(); i++)
			kill(pid[i], signal);	
#endif
}

void Ctrl::Signal(int signal)
{
#ifdef PLATFORM_POSIX
	int i = 0;
	while(i < pid.GetCount())
		if(pid[i] && waitpid(pid[i], 0, WNOHANG | WUNTRACED) > 0)
			pid.Remove(i);
		else
			i++;
	switch(signal) {
	case SIGTERM:
	case SIGHUP:
		quit = true;
		Broadcast(signal);
		break;
	case SIGINT:
		Broadcast(signal);
		exit(0);
		break;
	case SIGALRM:
		if(getpid() != main_pid) {
			// "Timeout - session stoped"
			exit(0);
		}
		break;
	}
#endif
}
	
String    Ctrl::host = "localhost";
int       Ctrl::port = 8088;
bool      Ctrl::debugmode;
String    Ctrl::ip = "0.0.0.0";
TcpSocket Ctrl::socket;
WebSocket Ctrl::websocket;

bool Ctrl::StartSession()
{
	LLOG("Connect");

#ifdef PLATFORM_POSIX
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = Signal;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGALRM, &sa, NULL);
	sigaction(SIGCHLD, &sa, NULL);
	
	main_pid = getpid();
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
		RLOG("Cannot open server socket for listening!");
		Exit(1);
	}
#endif

	RLOG("Starting to listen on 8088");
	socket.Timeout(2000); // TODO: Not quite ideal way to make quit work..
	for(;;) {
		if(quit)
			return false;
		if(server.IsError())
			server.ClearError();
		if(socket.Accept(server)) {
			HttpHeader  http;
			if(http.Read(socket)) {
				RLOG("Accepted, header read");
				if(websocket.WebAccept(socket, http)) { // TODO: Connection limit, info
#ifdef PLATFORM_POSIX
					int newpid = fork();
					if(debugmode || newpid == 0)
						break;
					else {
						pid.Add(newpid);
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
	RLOG("Connection established with " << socket.GetPeerAddr());
	server.Close();
	if(socket.IsError())
		RLOG("CONNECT ERROR: " << socket.GetErrorDesc());

	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();

#ifdef PLATFORM_POSIX
	SetStdFont(ScreenSans(12)); //FIXME general handling
#endif
	ChStdSkin();

	extern Size DesktopSize;
	extern StaticRect& DesktopRect();

	DesktopRect().Color(Cyan());
	DesktopRect().SetRect(0, 0, DesktopSize.cx, DesktopSize.cy);
	SetDesktop(Desktop());

	return true;
}

Callback Ctrl::WhenDisconnect;

END_UPP_NAMESPACE

#endif
