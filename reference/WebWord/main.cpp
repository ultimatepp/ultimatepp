#include "WebWord.h"

using namespace Upp;

String FormatSize(int64 n)
{
	if(n < 10000)
		return Format("%d B", n);
	else
	if(n < 10000 * 1024)
		return Format("%d.%d KB", n >> 10, (n & 1023) / 103);
	else
	if(n < I64(10000000) * 1024)
		return Format("%d.%d MB", n >> 20, (n & 1023) / 103);
	else
		return Format("%d.%d GB", n >> 30, (n & 1023) / 103);
}

void WebWord::ShowInfo()
{
	String s;
	s << "Mem " << MemoryUsedKb() << " KB";
	static int sent_prev;
#ifdef flagTURTLEGUI
	int secs = GetSysTime() - TurtleServer::stat_started;
	Time tm = Time(1, 1, 1, 0, 0, 0) + secs;
	s << ", uptime " << Format("%d:%0d:%02d:%02d", tm - Date(1, 1, 1), tm.hour, tm.minute, tm.second);
	s << ", data sent " << FormatSize(TurtleServer::stat_data_send);
	if(secs)
		s << ", average bandwidth " << FormatSize(TurtleServer::stat_data_send / secs) << "/s";
	s << ", actual bandwidth " << FormatSize(TurtleServer::stat_data_send - sent_prev);
	s << ", putimage " << TurtleServer::stat_putimage;
	s << ", putrect " << TurtleServer::stat_putrect;
	s << ", setimage " << TurtleServer::stat_setimage << " len " << FormatSize(TurtleServer::stat_setimage_len);
	s << ", roundtrip " << TurtleServer::stat_roundtrip_ms << " ms";
	s << ", client " << TurtleServer::stat_client_ms << " ms";
#ifdef PLATFORM_POSIX
	s << ", cpu time " << int((double) clock() / CLOCKS_PER_SEC * 1000) << " ms";
#endif
	sent_prev = TurtleServer::stat_data_send;
	statusbar.Set(s);
#endif
}

WebWord::WebWord()
{
	AddFrame(statusbar);
	Add(editor.SizePos());
	Sizeable().Zoomable();
//	FrameLess();
	Maximize();
	Title("WebWord");
	Icon(CtrlImg::File());
	editor.ClearModify();
	ActiveFocus(editor);
	SetTimeCallback(-1000, THISBACK(ShowInfo));
}

void AppMainLoop()
{
	// "Main" stuff should go in here...

	WebWord().Run();
}

#ifdef flagTURTLEGUI

CONSOLE_APP_MAIN
{

#ifdef _DEBUG
	TurtleServer::DebugMode();
#endif

	// MemoryLimitKb(100000000); // Can aid preventing DDoS attacks.

	TurtleServer guiserver;
	guiserver.Host("localhost");
	guiserver.Port(8888);
	guiserver.MaxConnections(100);
	RunTurtleGui(guiserver, AppMainLoop);
}

#else

GUI_APP_MAIN
{
	AppMainLoop();
}

#endif

