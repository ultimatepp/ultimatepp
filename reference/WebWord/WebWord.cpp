#include "WebWord.h"

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
#ifdef PLATFORM_TURTLE
	int secs = GetSysTime() - Ctrl::stat_started;
	Time tm = Time(1, 1, 1, 0, 0, 0) + secs;
	s << ", uptime " << Format("%d:%0d:%02d:%02d", tm - Date(1, 1, 1), tm.hour, tm.minute, tm.second);
	s << ", data sent " << FormatSize(Ctrl::stat_data_send);
	if(secs)
		s << ", average bandwidth " << FormatSize(Ctrl::stat_data_send / secs) << "/s";
	s << ", actual bandwidth " << FormatSize(Ctrl::stat_data_send - sent_prev);
	s << ", putimage " << Ctrl::stat_putimage;
	s << ", putrect " << Ctrl::stat_putrect;
	s << ", setimage " << Ctrl::stat_setimage << " len " << FormatSize(Ctrl::stat_setimage_len);
	s << ", roundtrip " << Ctrl::stat_roundtrip_ms << " ms";
	s << ", client " << Ctrl::stat_client_ms << " ms";
#ifdef PLATFORM_POSIX
	s << ", cpu time " << int((double) clock() / CLOCKS_PER_SEC * 1000) << " ms";
#endif
	sent_prev = Ctrl::stat_data_send;
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

void Main()
{
	WebWord w;
	w.Run();
}

#ifdef flagTURTLE
CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	MemoryLimitKb(100000000); // Perhaps a good idea to set a limit to prevent DDoS
	Ctrl::connection_limit = 50; // Maximum number of concurrent users (preventing DDoS)

#ifdef _DEBUG
	Ctrl::debugmode = true; // Only single session in debug (no forking)
#endif

//	Ctrl::WhenDisconnect = callback(FinishApp); // Use this to gracefully exit (save data?)

#ifndef _DEBUG
	Ctrl::host = "myserverhost.org";
#endif

	if(Ctrl::StartSession()) {
		Main();
		Ctrl::EndSession();
	}

	LOG("Session Finished");
}
#else
GUI_APP_MAIN
{
	Main();
}
#endif
