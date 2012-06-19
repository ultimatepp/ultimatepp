#include "CtrlLib/CtrlLib.h"

using namespace Upp;

//#define   TFILE <Lang/test.t>
//#include  <Core/t.h>

void ThreadTest(int lang)
{
	RLOG("In thread0: " << LNGAsText(lang) << ' ' << t_("Invalid date/time:"));
	RLOG("In thread0: " << LNGAsText(lang) << ' ' << t_("Cancel"));
	RLOG("In thread0 S: " << LNGAsText(lang) << ' ' << GetLngString("Cancel"));		
	RLOG("In thread0: " << LNGAsText(lang) << ' ' << AsString(GetSysDate()));
	SetLanguage(lang);
	for(int i = 0; i < 10; i++) {
		Sleep(100);
		RLOG("In thread: " << LNGAsText(lang) << ' ' << t_("Invalid date/time:"));
		RLOG("In thread: " << LNGAsText(lang) << ' ' << t_("Cancel"));
		RLOG("In thread S: " << LNGAsText(lang) << ' ' << GetLngString("Cancel"));		
		RLOG("In thread: " << LNGAsText(lang) << ' ' << AsString(GetSysDate()));
	}
	
}

GUI_APP_MAIN
{
	RDUMP(LNGAsText(GetSystemLNG()));
	RDUMP(t_("Sunday"));
	Thread::Start(callback1(ThreadTest, LNG_('P','L','P','L')));
	Thread::Start(callback1(ThreadTest, LNG_('F','R','F','R')));
	Thread::Start(callback1(ThreadTest, LNG_ENGLISH));
	SetLanguage(LNG_('C','S','C','Z'));
	for(int i = 0; i < 10; i++) {
		Sleep(100);
		RLOG("Main: " << t_("Sunday"));
		RLOG("Main: " << t_("Cancel"));
		RLOG("Main S: " << GetLngString("Cancel"));		
		RLOG("Main: " << AsString(GetSysDate()));
	}
}
