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
	
String    Ctrl::host = "localhost";
int       Ctrl::port = 8088;
bool      Ctrl::debugmode;
int       Ctrl::connection_limit = INT_MAX;
String    Ctrl::ip = "0.0.0.0";
TcpSocket Ctrl::socket;
WebSocket Ctrl::websocket;
int64     Ctrl::update_serial;
int64     Ctrl::recieved_update_serial;

Callback2<int, String> Ctrl::WhenConnect;
Callback1<int>         Ctrl::WhenTerminate;

StaticRect& DesktopRect();

bool Ctrl::StartSession()
{
	LLOG("Connect");

#ifdef PLATFORM_POSIX
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
		LLOG("Cannot open server socket for listening!");
		Exit(1);
	}
#endif

	LLOG("Starting to listen on " << port << ", pid: " << getpid());
	socket.Timeout(2000); // TODO: Not quite ideal way to make quit work..
	for(;;) {
		if(quit)
			return false;
#ifdef PLATFORM_POSIX
		int i = 0;
		while(i < pid.GetCount())
			if(pid[i] && waitpid(pid[i], 0, WNOHANG | WUNTRACED) > 0) {
				WhenTerminate(pid[i]);
				pid.Remove(i);
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
					if(pid.GetCount() >= connection_limit) {
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
						pid.Add(newpid);
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
	LLOG("Connection established with " << socket.GetPeerAddr() << ", pid: " << getpid());
	server.Close();
	if(socket.IsError())
		LLOG("CONNECT ERROR: " << socket.GetErrorDesc());

	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();

#ifdef PLATFORM_POSIX
	SetStdFont(ScreenSans(12)); //FIXME general handling
#endif
	ChStdSkin();

	DesktopRect().Color(Cyan());
	DesktopRect().SetRect(0, 0, DesktopSize.cx, DesktopSize.cy);
	SetDesktop(Desktop());
	
	stat_started = GetSysTime();
	
	while(!IsWaitingEvent())
		GuiSleep(10);
	
	ProcessEvents();

	return true;
}

Callback Ctrl::WhenDisconnect;

END_UPP_NAMESPACE

#endif
