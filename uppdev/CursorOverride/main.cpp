#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Testcase : public TopWindow{
	
public:
	typedef Testcase CLASSNAME;
	
	Testcase(){

	}

	virtual void ContextMenu(Bar &bar){
		bar.Add(t_("Lengthy process with OverrideCursor..."),THISBACK(LengthyProcess1));
		bar.Add(t_("Lengthy process with WaitCursor..."),THISBACK(LengthyProcess2));
	}

	virtual void LengthyProcess1(){
		// Not working on Linux:
		Image cursor=OverrideCursor(Image::Wait());
		Sleep(1000);
		OverrideCursor(cursor);
	}

	virtual void LengthyProcess2(){
		// Not working on Linux:
		WaitCursor waitcursor;
		Sleep(1000);
	}

	virtual void RightUp(Point p, dword keyflags){
		MenuBar::Execute(THISBACK(ContextMenu));
	}

	virtual void LeftUp(Point p, dword keyflags){
		// Working OK on Linux:
		Image cursor=OverrideCursor(Image::Wait());
		Sleep(1000);
		OverrideCursor(cursor);
	}

	virtual void MiddleUp(Point p, dword keyflags){
		// Not working on Linux:
		WaitCursor waitcursor;
		Sleep(1000);
	}

};

GUI_APP_MAIN
{
	Testcase().Run();
}
