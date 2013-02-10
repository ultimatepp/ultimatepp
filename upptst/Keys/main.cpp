#include <CtrlLib/CtrlLib.h>

using namespace Upp;

String GetKeyDescEx(int key)
{
	String desc = GetKeyDesc(key & ~K_KEYUP);
	if(key & K_KEYUP)
		desc << " UP";
	return desc;
}

struct App : TopWindow {
	ArrayCtrl  log;

	void Log(const String& s)
	{
		log.Add(s);
		log.GoEnd();
	}

	virtual void   Activate()
	{
		Log("Activate");
	}

	virtual void   Deactivate()
	{
		Log("Deactivate");
	}

	virtual bool   Key(dword key, int count)
	{
		Log(Format("Key(%x, %d) ", (int)key, count) + GetKeyDescEx(key));
		return false;
	}

	virtual void   GotFocus()
	{
		Log("GotFocus");
	}

	virtual void   LostFocus()
	{
		Log("LostFocus");
	}

	App()
	{
		log.NoHeader().AddColumn("");
		Add(log.SizePos());
	}
};

GUI_APP_MAIN
{
	App().Run();
}
