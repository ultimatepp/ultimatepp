#include "Styler.h"

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
	ChFinish();
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

void StylerCtrl::SkinAction()
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
	UpdateAction();
}

Color Invert(const Color& c)
{
	return Color(255-c.GetR(), 255-c.GetG(),255-c.GetB());
}

void StylerCtrl::InvertAction()
{
	Color c  = ~mcl;
	Color fg = ~fgcl;
	Color bg = ~bgcl;

	fg = Invert(fg);
	bg = Invert(bg);
	c =  Invert(c);
	
	fgcl <<= fg;
	bgcl <<= bg;
	mcl  <<= c;

	UpdateAction();
}

void StylerCtrl::Updated()
{

}

StylerCtrl::StylerCtrl()
{
	CtrlLayout(*this);

	dk.SetReadOnly();

	stsw <<= 2;
	stsw <<= THISBACK(SkinAction);

	mcl <<= Gray();
	mcl <<= THISBACK(SkinAction);

	fgcl <<= White();
	fgcl <<= THISBACK(SkinAction);

	bgcl <<= Black();
	bgcl <<= THISBACK(SkinAction);
	

	bl.MinMax(0,255);
	bl <<= 192;
	bl <<= THISBACK(SkinAction);

	inv <<= THISBACK(InvertAction);

	SkinAction();
}

//

void ChamStyleCtrl::Updated()
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

void ChamStyleCtrl::ColorAction()
{
	ChReset();

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
	Action();
}

ChamStyleCtrl::ChamStyleCtrl()
{
	CtrlLayout(*this);
	
	clface <<= THISBACK(ColorAction);
	clshadow <<= THISBACK(ColorAction);
	clltface <<= THISBACK(ColorAction);
	cldkshadow <<= THISBACK(ColorAction);
	clhighlight <<= THISBACK(ColorAction);
	cldisabled <<= THISBACK(ColorAction);
	clpaper <<= THISBACK(ColorAction);
	cltext <<= THISBACK(ColorAction);
	clhighlighttext <<= THISBACK(ColorAction);
	cllight <<= THISBACK(ColorAction);

	clmenu <<= THISBACK(ColorAction);
	clmenutext <<= THISBACK(ColorAction);
	clinfo <<= THISBACK(ColorAction);
	clinfotext <<= THISBACK(ColorAction);
	clmark <<= THISBACK(ColorAction);
	cllabel <<= THISBACK(ColorAction);
	cllbtext <<= THISBACK(ColorAction);
	cllb <<= THISBACK(ColorAction);
	 
	Update();	
}