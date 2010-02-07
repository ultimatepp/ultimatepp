#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Switch/switch.lay>
#include <CtrlCore/lay.h>

struct MyApp1 : public TopWindow {
	typedef MyApp1 CLASSNAME;

	Switch s;

	void Change(){
		PromptOK("Switched to value " + AsString(~s));
	}

	MyApp1() {
		Title("Switch example");
		Add(s);
		s.SizePos();
		s.Add("Case 1")
		 .Add(12,"Case 2")
		 .Add(13.5,"Case 3");
		s.Add("Something", "Another case");
		s.DisableValue(12);
		s <<= 13.5;
		s <<= THISBACK(Change);
	}
};

GUI_APP_MAIN
{
	MyApp1().Run();
	
	WithSwitchLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Switch example - with layout");
	dlg.s1 <<= 0;
	dlg.s2 <<= 2;
	if(dlg.Execute() != IDOK)
		return;
	PromptOK(String().Cat() << "First: " << ~dlg.s1 << ", second: " << ~dlg.s2);
}
