#include "SysExecTest.h"
#include <SysExec/SysExec.h>

void SysExecTest::btnCb(int n)
{
#ifdef PLATFORM_POSIX
	String term = "gedit";
#else
	String term = "cmd.exe";
#endif
	bool res;
	
	status.Set("RUNNING.......");
	Ctrl::ProcessEvents();
	switch(n)
	{
		case 1:
			res = SysExec(term, "");
			break;
			
		case 2:
			res = SysStart(term, "");
			break;
			
		case 3:
			res = SysExecAdmin(pass, term, "");
			break;
			
		case 4:
			res = SysStartAdmin(pass, term, "");
			break;
			
		case 5:
			res = SysExecUser(user, pass, term, "");
			break;
			
		case 6:
			res = SysStartUser(user, pass, term, "");
			break;
			
		default:
			NEVER();
	}

	String resStr;
	resStr = res ? "TRUE" : "FALSE";
	status.Set("Last command returned '" + resStr + "' status");
}

SysExecTest::SysExecTest()
{
	CtrlLayout(*this, "Test SysExec package");
	AddFrame(status);
	pass.Password();
	
	btn1 <<= THISBACK1(btnCb, 1);
	btn2 <<= THISBACK1(btnCb, 2);
	btn3 <<= THISBACK1(btnCb, 3);
	btn4 <<= THISBACK1(btnCb, 4);
	btn5 <<= THISBACK1(btnCb, 5);
	btn6 <<= THISBACK1(btnCb, 6);
}

GUI_APP_MAIN
{
	SysExecTest().Run();
}

