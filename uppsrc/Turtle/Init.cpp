#include "Turtle.h"

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {
String      TurtleServer::host              = "localhost";
int         TurtleServer::port              = 8888;
String      TurtleServer::ip                = "0.0.0.0";
int         TurtleServer::connection_limit  = 100;
bool        TurtleServer::debugmode;
WebSocket   TurtleServer::websocket;
int         TurtleServer::mainpid;
bool        TurtleServer::quit;
dword       TurtleServer::mousebuttons      = 0;
dword       TurtleServer::modifierkeys      = 0;
Size        TurtleServer::desktopsize       = Size(1024, 1024);
int64       TurtleServer::update_serial     = 0;
int64       TurtleServer::recieved_update_serial;
int         TurtleServer::serial_time0      = Null;
int64       TurtleServer::serial_0          = 0;
Time        TurtleServer::stat_started;
int64       TurtleServer::stat_data_send;
int         TurtleServer::stat_putrect;
int         TurtleServer::stat_putimage;
int         TurtleServer::stat_setimage;
int64       TurtleServer::stat_setimage_len;
int         TurtleServer::stat_roundtrip_ms;
int         TurtleServer::stat_client_ms;

Event<int, String> TurtleServer::WhenConnect;
Event<int>         TurtleServer::WhenTerminate;
Event<>            TurtleServer::WhenDisconnect;

void RunTurtleGui(TurtleServer& gui, Event<> app_main)
{
	if(TurtleServer::StartSession())
		RunVirtualGui((VirtualGui &) gui, app_main);
}

}
