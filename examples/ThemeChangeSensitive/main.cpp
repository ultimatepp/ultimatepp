#include <CtrlLib/CtrlLib.h>

#ifndef _SkinSensitive_SkinSensitive_h
#define _SkinSensitive_SkinSensitive_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ThemeChangeSensitive/MyApp.lay>
#include <CtrlCore/lay.h>

struct MyApp : public WithMyAppLayout<TopWindow> {
	virtual void Skin();
	
	MyApp();
};

#endif

void MyApp::Skin()
{ // called on opening window or on skin change
	list4.Clear(); // just refill the list with new colors
	list4.Add(AttrText("Blend(SRed, SYellow)").NormalInk(Blend(SRed(), SYellow())).Bold());
	list4.Add(AttrText("SLtBlue").NormalInk(SBlue().Resolved()).Bold()); // Resolved converts SBlue to normal color, just as does Blend
	list4.Add(AttrText("Blend(SRed, SLtBlue)").NormalInk(Blend(SRed(), SLtBlue())).Bold());
}

MyApp::MyApp()
{
	CtrlLayout(*this, "Window title");
	list1.AddColumn("Ignoring skin change");
	list1.NoCursor();
	list1.Add(AttrText("Blend(Red, Yellow)").NormalInk(Blend(SRed(), SYellow())).Bold());
	list1.Add(AttrText("LtBlue").NormalInk(SBlue().Resolved()).Bold()); // Resolved converts SBlue to normal color, just as does Blend, for testing
	list1.Add(AttrText("Blend(Red, LtBlue)").NormalInk(Blend(SRed(), SLtBlue())).Bold());
	
	list2.AddColumn("Using AColor");
	list2.NoCursor();
	list2.Add(AttrText("Blend(Red, Yellow)").NormalInk(AColor(Blend(Red(), Yellow()))).Bold());
	list2.Add(AttrText("SLtBlue").NormalInk(AColor(Blue())).Bold()); // Resolved converts SBlue to normal color, just as does Blend
	list2.Add(AttrText("Blend(Red, LtBlue)").NormalInk(AColor(Blend(Red(), LtBlue()))).Bold());

	list3.AddColumn("Using SColor");
	list3.NoCursor();
	static SColor ry([] { return Blend(SRed(), SYellow()); }); // Gets reevaluated on skin change
	static SColor rb([] { return Blend(SRed(), SLtBlue()); });
	list3.Add(AttrText("Blend(Red, Yellow)").NormalInk(ry).Bold());
	list3.Add(AttrText("SLtBlue").NormalInk(SBlue()).Bold()); // SBlue is different based on skin
	list3.Add(AttrText("Blend(Red, LtBlue)").NormalInk(rb).Bold());
	
	list4.AddColumn("Reloaded with Skin");
	list4.NoCursor();
}

GUI_APP_MAIN
{
	Ctrl::SkinChangeSensitive(); // activate changes of skin (theme) when host theme changes
	MyApp().Run();
}
