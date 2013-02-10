#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>

using namespace Upp;

struct MyApp : TopWindow {
	RichTextCtrl ctrl;
	TopWindow win;

	void Do(int i);
	
	void Perform();
	
	typedef MyApp CLASSNAME;
	
	MyApp();
};

void MyApp::Do(int i)
{
	String qtf;
	qtf << "Here goes Image "
//	    << "@@iml:" << Random(100) * 10 + 100 << "*" << Random(150) * 10 + 100 << "`CtrlImg:exclamation` !!!"
	    << "&This was pass: " << i;
	if(i & 1) {
		RLOG("Lock: " << i);		
		GuiLock __;
		RLOG("Call: " << i);		
		ctrl <<= qtf;
		RLOG("Done: " << i);		
	}
	else {
		PdfDraw w;
		ParseQTF(qtf).Paint(w, 0, 0, 3000);
		SaveFile(GetHomeDirFile(AsString(i % 100)), w.Finish());
	}
}

void MyApp::Perform()
{
	Open();
	int i = 0;
	CoWork co;
	while(IsOpen()) {
		co & THISBACK1(Do, i++);
		Ctrl::ProcessEvents();
	}
	RLOG("EXITING!!!!!!!!!!!!!!");
	while(!co.IsFinished())
		Ctrl::ProcessEvents();
}

MyApp::MyApp()
{
	Add(ctrl.SizePos());
}

GUI_APP_MAIN
{
	MyApp().Perform();
}
