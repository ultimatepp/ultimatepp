#ifndef _SkinTest_SkinTest_h
#define _SkinTest_SkinTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SkinTest/SkinTest.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS Img
#define IMAGEFILE <SkinTest/Skin.iml>
#include <Draw/iml.h>

Button::Style BtnStyle1()
{
	Button::Style s = Button::StyleNormal();
	for(int i = 0; i < 4; i++)
		s.look[i] = Img::VGrad32();
	s.pressoffset = Point(1, -1);
	return s;
}

Button::Style BtnStyle2()
{
	Button::Style s = Button::StyleNormal();
	for(int i = 0; i < 4; i++)
		s.look[i] = Img::Btn32();
	s.pressoffset = Point(1, -1);
	return s;
}

MenuBar::Style MenuStyle()
{
	MenuBar::Style s = MenuBar::StyleDefault();
	s.item = Img::YellowBox(); //Img::HGrad32();
	s.topitem = Img::YellowBox();
	return s;
}

class SkinTest : public WithSkinTestLayout<TopWindow> {
public:
	typedef SkinTest CLASSNAME;
	Button::Style btnstyle1, btnstyle2;
	MenuBar::Style menustyle;
	MenuBar menu;

	SkinTest() {
		CtrlLayout(*this, "Window title");
		Sizeable();

		btnstyle1 = BtnStyle1();
		btnstyle2 = BtnStyle2();
		menustyle = MenuStyle();

		btn1.SetStyle(btnstyle1);
		btn1.SetImage(Img::MailIcon());

		btn2.SetStyle(btnstyle2);
		btn2.SetImage(Img::MailIcon());

		menu.SetStyle(menustyle);
		AddFrame(menu);
		menu.Set(THISBACK(MainMenu));
	}

	void MainMenu(Bar &bar) {
		bar.Add("Menu1", THISBACK(Menu1));
	}

	void Menu1(Bar &bar) {
		bar.Add("Menu Item 1", Img::MailIconSmall(), THISBACK(MenuItem));
		bar.Add("Menu Item 2", THISBACK(MenuItem));
		bar.Add("Menu Item 3", THISBACK(MenuItem));
	}

	void MenuItem() {

	}
};

#endif
