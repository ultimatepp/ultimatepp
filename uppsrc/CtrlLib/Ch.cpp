#include "CtrlLib.h"

NAMESPACE_UPP

#define LLOG(x)  // RLOG(x)

#define IMAGECLASS CtrlsImg
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

#define IMAGECLASS ClassicCtrlsImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml_source.h>

void ChSysInit();

void ChStdSkin()
{
	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void SbWc(Value *look)
{
	Color wc = Blend(SColorFace(), SColorPaper(), 170);
	look[CTRL_NORMAL] = wc;
	look[CTRL_HOT] = wc;
	look[CTRL_PRESSED] = SColorText();
	look[CTRL_DISABLED] = wc;
}

void ChClassicSkin()
{
	LLOG("ChInitWinClassic");

	ChSysInit();
	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	
	ColoredOverride(CtrlsImg::Iml(), ClassicCtrlsImg::Iml());
	for(int q = 0; q < 4; q++)
		CtrlsImg::Set(CtrlsImg::I_HTB + q, AdjustColors(CtrlsImg::Get(ClassicCtrlsImg::I_B + q)));

	{
		Button::Style& s = Button::StyleNormal().Write();
		s.monocolor[0] = s.monocolor[1] = s.monocolor[2] = s.monocolor[3] = SColorText();
		s.pressoffset.x = s.pressoffset.y = 1;
		s.transparent = false;
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		SbWc(s.hupper);
		SbWc(s.hlower);
		SbWc(s.vupper);
		SbWc(s.vlower);
		for(int i = 0; i < 4; i++)
			s.vthumb[i] = s.hthumb[i] = Button::StyleNormal().look[i];
	}

	{
		MultiButton::Style& s = MultiButton::StyleDefault().Write();
		s.border = s.trivialborder = 2;
	}

	{
		SeparatorCtrl::Style& s = SeparatorCtrl::StyleDefault().Write();
		s.l1 = SColorShadow();
		s.l2 = SColorLight();
	}

	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.popupbody = SColorFace();
	}

	LabelBoxTextColor_Write(SColorText());
}

#ifdef PLATFORM_X11

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

#endif

END_UPP_NAMESPACE
