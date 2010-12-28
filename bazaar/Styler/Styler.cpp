#include "Styler.h"

#define ADDSTYLE 1

#define IMAGECLASS StylerCtrlsImg
#define IMAGEFILE <Styler/StylerCtrls.iml>
#include <Draw/iml_source.h>

void ColorizeIml(Iml& target, Iml& source, const Color& c, int alpha)
{
	for(int i = 0; i < target.GetCount(); i++) {
		int q = source.Find(target.GetId(i));
		if(q >= 0) {
			Image m = Colorize(source.Get(q), c, alpha);
			target.Set(i, m);
		}
	}
}

void RefreshGlobalStyles()
{
	{
		MenuBar::Style& st = MenuBar::StyleDefault().Write();
		st = st.Standard();
	}
	{
		ToolBar::Style& st = ToolBar::StyleDefault().Write();
		st = st.Standard();
	}
	{
		StatusBar::Style& st = StatusBar::StyleDefault().Write();
		st = st.Standard();
	}
	{
		ScrollBar::Style& st = ScrollBar::StyleDefault().Write();
		st = st.Standard();
	}
	{
		Splitter::Style& st = Splitter::StyleDefault().Write();
		st = st.Standard();
	}
	{
		Button::Style& st = Button::StyleNormal().Write();
		st = st.Standard();
	}
	{
		ButtonOption::Style& st = ButtonOption::StyleDefault().Write();
		st = st.Standard();
	}
	{
		SpinButtons::Style& st = SpinButtons::StyleDefault().Write();
		st = st.Standard();
	}
	{
		EditField::Style& st = EditField::StyleDefault().Write();
		st = st.Standard();
	}
	{
		HeaderCtrl::Style& st = HeaderCtrl::StyleDefault().Write();
		st = st.Standard();
	}
	{
		TabCtrl::Style& st = TabCtrl::StyleDefault().Write();
		st = st.Standard();
	}
	{
		DropList::Style& st = DropList::StyleDefault().Write();
		st = st.Standard();
	}
#ifdef ADDSTYLE
	{
		ArrayCtrl::Style& st = ArrayCtrl::StyleDefault().Write();
		st = st.Standard();
	}
	{
		TextCtrl::Style& st = TextCtrl::StyleDefault().Write();
		st = st.Standard();
	}
#endif
}

void RefreshLayoutDeepAll()
{
	Vector<Ctrl*> vc = Ctrl::GetTopWindows();
	for(int i = 0; i < vc.GetCount(); i++)
		vc[i]->RefreshLayoutDeep();
}

void StylerCtrl::ColorizedSkin()
{
	Color c  = ~mcl;
	int blnd = ~bl;
	Color fg = ~fgcl;
	Color bg = ~bgcl;
	
	ChStdSkin();
	ColorizeIml(CtrlsImg::Iml(), StylerCtrlsImg::Iml(), c, 255);

	if(Grayscale(fg)<Grayscale(bg))
	{
		dk <<= true;
	}
	else
	{
		dk <<= false;
	}

		SColorFace_Write(Blend(c, fg, blnd)); //controls face color
		SColorShadow_Write(Blend(c, bg, blnd));
		SColorLtFace_Write(Blend(c, fg, blnd+20));
		SColorDkShadow_Write(Blend(c, bg, blnd+20));

		SColorHighlight_Write(Blend(fg, c, blnd+20));
		SColorDisabled_Write(Blend(bg, c, blnd+20));

		SColorPaper_Write(Blend(c, fg, blnd+40)); //for lists, edits etc..
		SColorText_Write(Blend(c, bg, blnd+40));

		SColorHighlightText_Write(Blend(c, fg, blnd+60));


	SColorLight_Write(SColorHighlight());

	SColorMenu_Write(SColorPaper());
	SColorMenuText_Write(SColorText());
	SColorInfo_Write(SColorPaper());
	SColorInfoText_Write(SColorText());
	SColorMark_Write(SColorText());
	
	SColorLabel_Write(SColorText());
	LabelBoxTextColor_Write(SColorText());
	LabelBoxColor_Write(SColorHighlight());
}

void StylerCtrl::Updated()
{
	if((int)stsw.GetData() != 2)
	{
		mcl.Disable();
		fgcl.Disable();
		bgcl.Disable();
		bl.Disable();
		inv.Disable();
	}
	else
	{
		mcl.Enable();
		fgcl.Enable();
		bgcl.Enable();
		bl.Enable();
		inv.Enable();
	}

	//reset
	switch((int)stsw.GetData())
	{
		case 1: ChStdSkin(); break;
		case 2: ColorizedSkin(); break;
		default:
		case 0: ChClassicSkin(); break;
	}

	RefreshGlobalStyles();
	RefreshLayoutDeepAll();
}

void StylerCtrl::InvertColors()
{
	Color c  = ~mcl;
	Color fg = ~fgcl;
	Color bg = ~bgcl;

	fg = Color(255-fg.GetR(), 255-fg.GetG(),255-fg.GetB());
	bg = Color(255-bg.GetR(), 255-bg.GetG(),255-bg.GetB());
	c =  Color(255- c.GetR(), 255- c.GetG(),255- c.GetB());
	
	fgcl <<= fg;
	bgcl <<= bg;
	mcl  <<= c;

	UpdateAction();
}

StylerCtrl::StylerCtrl()
{
	CtrlLayout(*this);

	dk.SetReadOnly();

	stsw <<= 2;
	stsw <<= THISBACK(UpdateAction);

	mcl <<= Gray();
	mcl <<= THISBACK(UpdateAction);

	fgcl <<= White();
	mcl <<= THISBACK(UpdateAction);

	bgcl <<= Black();
	bgcl <<= THISBACK(UpdateAction);
	

	bl.MinMax(0,255);
	bl <<= 192;
	bl <<= THISBACK(UpdateAction);

	inv <<= THISBACK(InvertColors);

	UpdateAction();
}

//

void ChamStyleCtrl::Reload()
{
	clface <<= SColorFace();
	clshadow <<= SColorShadow();
	clltface <<= SColorLtFace();
	cldkshadow <<= SColorDkShadow();
	clhighlight <<= SColorHighlight();
	cldisabled <<= SColorDisabled();
	clpaper <<= SColorPaper();
	cltext <<= SColorText();
	clhighlighttext <<= SColorHighlightText();
	cllight <<= SColorLight();

	clmenu <<= SColorMenu();
	clmenutext <<= SColorMenuText();
	clinfo <<= SColorInfo();
	clinfotext <<= SColorInfoText();
	clmark <<= SColorMark();
	cllabel <<= SColorLabel();
	cllbtext <<= LabelBoxTextColor();
	cllb <<= LabelBoxColor();
}

void ChamStyleCtrl::Updated()
{
	SColorFace_Write(~clface);
	SColorShadow_Write(~clshadow);
	SColorLtFace_Write(~clltface);
	SColorDkShadow_Write(~cldkshadow);
	SColorHighlight_Write(~clhighlight);
	SColorDisabled_Write(~cldisabled);
	SColorPaper_Write(~clpaper);
	SColorText_Write(~cltext);
	SColorHighlightText_Write(~clhighlighttext);
	SColorLight_Write(~cllight);

	SColorMenu_Write(~clmenu);
	SColorMenuText_Write(~clmenutext);
	SColorInfo_Write(~clinfo);
	SColorInfoText_Write(~clinfotext);
	SColorMark_Write(~clmark);
	SColorLabel_Write(~cllabel);
	LabelBoxTextColor_Write(~cllbtext);
	LabelBoxColor_Write(~cllb);

	RefreshGlobalStyles();
	RefreshLayoutDeepAll();
}

ChamStyleCtrl::ChamStyleCtrl()
{
	CtrlLayout(*this);
	
	clface <<= THISBACK(UpdateAction);
	clshadow <<= THISBACK(UpdateAction);
	clltface <<= THISBACK(UpdateAction);
	cldkshadow <<= THISBACK(UpdateAction);
	clhighlight <<= THISBACK(UpdateAction);
	cldisabled <<= THISBACK(UpdateAction);
	clpaper <<= THISBACK(UpdateAction);
	cltext <<= THISBACK(UpdateAction);
	clhighlighttext <<= THISBACK(UpdateAction);
	cllight <<= THISBACK(UpdateAction);

	clmenu <<= THISBACK(UpdateAction);
	clmenutext <<= THISBACK(UpdateAction);
	clinfo <<= THISBACK(UpdateAction);
	clinfotext <<= THISBACK(UpdateAction);
	clmark <<= THISBACK(UpdateAction);
	cllabel <<= THISBACK(UpdateAction);
	cllbtext <<= THISBACK(UpdateAction);
	cllb <<= THISBACK(UpdateAction);
	 
	Reload();	
}