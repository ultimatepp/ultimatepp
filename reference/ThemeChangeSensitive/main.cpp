#include <CtrlLib/CtrlLib.h>

#ifndef _SkinSensitive_SkinSensitive_h
#define _SkinSensitive_SkinSensitive_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ThemeChangeSensitive/MyApp.lay>
#include <CtrlCore/lay.h>

struct MyApp : public WithMyAppLayout<TopWindow> {
	virtual void Skin();
	
	Image MakeImage();
	
	MyApp();
};

#endif

Image MyApp::MakeImage()
{ // make some image in a way that reflects dark/light mode
	Size sz = CtrlImg::HandCursor().GetSize();
	ImageDraw iw(sz.cx, sz.cy);
	iw.DrawRect(sz, SColorFace());
	iw.DrawImage(0, 0, CtrlImg::HandCursor()); // HandCursor is adjusted wrt dark/light
	iw.DrawRect(sz.cx / 2, sz.cy / 2, DPI(5), DPI(5), SColorHighlight()); // SColorHighlight is adjusted wrt dark/light
	return iw;
}

void MyApp::Skin()
{ // called on opening window and on theme change
	list4.Clear(); // just refill the list with new colors
	list4.Add(AttrText("Blend(SRed, SYellow)").NormalInk(Blend(SRed(), SYellow())).Bold());
	list4.Add(AttrText("SLtBlue").NormalInk(SBlue().Resolved()).Bold()); // Resolved converts SBlue to normal color, just as does Blend
	list4.Add(AttrText("Blend(SRed, SLtBlue)").NormalInk(Blend(SRed(), SLtBlue())).Bold());
	
	img3.SetImage(MakeImage());
}

MyApp::MyApp()
{
	CtrlLayout(*this, "How to dynamically react to theme changes");
	list1.AddColumn("Ignoring skin change");
	list1.NoCursor();
	list1.Add(AttrText("Blend(Red, Yellow)").NormalInk(Blend(SRed(), SYellow())).Bold());
	list1.Add(AttrText("SBlue").NormalInk(SBlue().Resolved()).Bold()); // Resolved converts SBlue to normal color, just as does Blend, for testing
	list1.Add(AttrText("Blend(Red, LtBlue)").NormalInk(Blend(SRed(), SLtBlue())).Bold());
	
	list2.AddColumn("Using AColor");
	list2.NoCursor();
	list2.Add(AttrText("Blend(Red, Yellow)").NormalInk(AColor(Blend(Red(), Yellow()))).Bold()); // Light theme color that gets adjusted if theme is dark
	list2.Add(AttrText("SLtBlue").NormalInk(AColor(Blue())).Bold());
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
	
	img1.SetImage(CtrlImg::HandCursor()); // iml image constants react to theme changes (even theirs copies)
	img2.SetImage(MakeImage()); // the image copy is stored just once and does not reflect theme changes
	
	std << [=] { Ctrl::SetSkin(ChStdSkin); };
	dark << [=] { Ctrl::SetSkin(ChDarkSkin); };
	host << [=] { Ctrl::SetSkin(ChHostSkin); };
	
	toggle << [=] { Ctrl::SwapDarkLight(); };
	
#ifndef _DEBUG
	info = "Predefined key for Toggle dark was set to Space";
#endif
}

GUI_APP_MAIN
{
#ifndef _DEBUG
	Ctrl::SwapDarkLightKey(K_SPACE); // just to demonstrate that toggle key can be activated in Release mode too and/or different
#endif

	Ctrl::SkinChangeSensitive(); // activate changes of skin (theme) when host theme changes
	MyApp().Run();
}
