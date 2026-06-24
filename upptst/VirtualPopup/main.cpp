#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList dl, dl2;
	ArrayCtrl popup;
	MenuBar  menu;
	
	void Dump() {
		DUMP(popup.IsOpen());
		DUMP(popup.IsVisible());
		DUMP(Ctrl::Name(popup.GetTopCtrl()));
		DUMP(Ctrl::Name(popup.GetTopWindow()));
		DUMP(Ctrl::Name(popup.GetOwner()));
		DUMP(Ctrl::Name(popup.GetOwnerCtrl()));
	}

	void RightDown(Point p, dword keyflags) override {
		if(popup.IsOpen()) {
			popup.Close();
			LOG("========= CLOSE");
		}
		else {
			Rect sr = GetScreenRect();
			sr.Offset(100, 100);
			sr.SetSize(300, 300);
			popup.SetRect(sr);
			popup.VirtualPopUp(this, true);
			LOG("========= OPEN");
		}
		Dump();
	}

	MyApp() {
		*this << dl.LeftPosZ(10, 1000).TopPos(10)
		      << dl2.LeftPosZ(10, 1000).TopPos(150);
		for(int i = 0; i < 100; i++) {
			dl.Add(i);
			dl2.Add(i);
		}
		
		popup.SetFrame(BlackFrame());
		popup.AddColumn("Test");
		popup.AddColumn("Test");
		popup.MouseMoveCursor();
		popup.WhenLeftDouble = [=] {
			String s = "\1";
			for(Ctrl *c : GetTopCtrls())
				s << Name(c) << "\n";
			PromptOK(s);
		};
		popup.WhenSel = [=] {
			DUMP(popup.IsPopUp());
		};
		for(int i = 0; i < 100; i++)
			popup.Add(i);
//		lbl = "This is virtual popup";
		AddFrame(menu);
		menu.Sub("Menu", [](Bar& bar) {
			for(int i = 0; i < 10; i++)
				bar.Add("Open", [] { SelectFileOpen("*\n*.txt"); });
			bar.Sub("Submenu", [](Bar& bar) {
				bar.Add("SubTest", [] { PromptOK("SubTest"); });
			});
		});
	}
};

GUI_APP_MAIN
{
	Ctrl::use_virtual_popups = true;

	MyApp().Run();
}
