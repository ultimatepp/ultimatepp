#ifndef _MyEcho_MyEcho_h
#define _MyEcho_MyEcho_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;
#define MAX_CACHE_LINE	200


class MyDocEdit : public DocEdit
{
	Thread timerthread;
	String inputString;
	static String tipString;
	String dir;
public:
	virtual bool  Key(dword key, int count);
	MyDocEdit();
	void ShowTip();
	int RunCommand(String user_input);
	void InsertData();
};

class MyApp : public TopWindow
{
public:
	MyDocEdit text;
	typedef MyApp CLASSNAME;
	MyApp()
	{
		SetRect(0, 0, 640, 480);
		Add(text.LeftPosZ(10, 500).TopPosZ(10, 350));
	}
};

#endif

