#include "BlueBar.h"

namespace Upp {

#define IMAGECLASS BlueImg
#define IMAGEFILE <art/BlueBar/BlueSkin.iml>
#include <Draw/iml.h>

void InstallBlueToolBar()
{
	ToolBar::Style& s = ToolBar::StyleDefault().Write();
	s = ToolBar::StyleDefault();
	s.buttonstyle.look[CTRL_NORMAL] = Null;
	s.buttonstyle.look[CTRL_HOT] = BlueImg::BH();
	s.buttonstyle.look[CTRL_PRESSED] = BlueImg::BP();
	s.buttonstyle.look[CTRL_DISABLED] = Null;
	s.buttonstyle.look[CTRL_CHECKED] = BlueImg::B1();
	s.buttonstyle.look[CTRL_HOTCHECKED] = BlueImg::BH();

	for(int i = 0; i < 6; i++) {
		s.buttonstyle.light[i] = false;
		s.buttonstyle.contrast[i] = -40;
	}
	s.buttonstyle.contrast[CTRL_HOT] = s.buttonstyle.contrast[CTRL_HOTCHECKED] = 0;
	s.arealook = Null;
	s.look = BlueImg::LK();
}

void InstallBlueMenuBar()
{
	MenuBar::Style& s = MenuBar::StyleDefault().Write();
	s = MenuBar::StyleDefault();
	s.arealook = Null;
	s.look = BlueImg::LK();
	s.topitem[1] = s.item = BlueImg::BH();
	s.itemtext = s.topitemtext[0] = s.topitemtext[1] = s.topitemtext[2] = SColorText();
	s.leftgap = Zx(24);
	s.lsepm = s.leftgap + s.textgap;
	s.popupiconbar = BlueImg::LR();
	ImageBuffer ib(7, 7);
	Fill(ib, SColorShadow(), 7 * 7);
	for(int i = 0; i < 5; i++)
		Fill(ib[i + 1] + 1, SColorMenu(), 5);
	ib.SetHotSpot(Point(2, 2));
	ib.Set2ndSpot(Point(4, 4));
	s.popupframe = Image(ib);
	s.topitem[2] = BlueImg::TK();
}

void InstallBlueBar()
{
	InstallBlueMenuBar();
	InstallBlueToolBar();
}

}
