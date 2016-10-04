#include <CtrlLib/CtrlLib.h>
using namespace Upp;
#define IMAGECLASS Imgs
#define IMAGEFILE <CtrlLib/Ctrl.iml>
#include <Draw/iml.h>

struct Dlg : TopWindow{
	Button b;

	typedef Dlg CLASSNAME;

	Dlg() {
		Title("Dialog").Sizeable();
		SetRect(0,0,300,200);
		Add(b.TopPos(10,20).LeftPos(10,100));
		b.SetLabel("OK");
		b<<=THISBACK(DoStuffAndExit);
	}

	void DoStuffAndExit(){
		Hide();
		Progress p;
		p.SetText("Pretending  work...");
		for(int i=0;i<100;i++){
			Sleep(25);
			p.Step();
		}
		Close();
	}
};

struct App : TopWindow {
	ToolBar tool;
	DropList test;

	typedef App CLASSNAME;

	App() {
		Title("My application with bars").Sizeable();
		AddFrame(tool);
		tool.Set(THISBACK(TBar));
		Add(test.LeftPos(0, 200).TopPos(50, 20));
		for(int i = 0; i < 100; i++)
			test.Add(i);
	}

	void MenuFn() {
		Dlg().Execute();
	}
	void TBar(Bar& bar) {
		bar.Add("Function", Imgs::open(), THISBACK(MenuFn));
		bar.Add("Function", Imgs::open(), THISBACK(MenuFn));
	}
};

GUI_APP_MAIN {
	GUI_PopUpEffect_Write(GUIEFFECT_NONE);
	App().Run();
}
