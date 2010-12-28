#ifndef _ValueCtrlTest_ValueCtrlTest_h
#define _ValueCtrlTest_ValueCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <ValueCtrl/ValueCtrl.h>

#define LAYOUTFILE <ValueCtrlTest/ValueCtrlTest.lay>
#include <CtrlCore/lay.h>



class ValueCtrlTest : public WithValueCtrlTestLayout<TopWindow> {
public:
	typedef ValueCtrlTest CLASSNAME;
	ValueCtrlTest();

	void ToInfo(const String& s)
	{
		info.Insert(info.GetLength(), s);
		info.Insert(info.GetLength(), "\n");
		info.SetCursor(info.GetLength());
	}
	void Clear() { info.Clear(); }
	void ActionCB();
	
	ValueCtrl vc;
};

#endif

