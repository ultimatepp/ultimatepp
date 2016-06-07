#include "WebInterface.h"
#include "Config.h"

using namespace Upp;

WebInterface::WebInterface()
{
	Ini::skylark_log = Config::WebInterfaceTrace;
	port = Config::WebInterfacePort;
	root = "";
	prefork = 0; //important!
	session.cookie = "__Skylark_Gui_session_cookie__";
	use_caching = false;
	thr.Run(THISBACK(Run));
}


WebInterface::~WebInterface()
{
	StopServer();
}

void WebInterface::WorkThread()
{
	/*
		Open database connection here
	*/
	RunThread();
}

void WebInterface::StopServer()
{
	Quit();
	thr.Wait();
}

