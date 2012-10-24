#include "Styler.h"

#define IMAGECLASS StylerCtrlsImg
#define IMAGEFILE <Styler/StylerCtrls.iml>
#include <Draw/iml_source.h>

Color Invert(const Color& c)
{
	return Color(255-c.GetR(), 255-c.GetG(),255-c.GetB());
}

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

void ChStylerSkin(const Color& c, int blnd, const Color& fg, const Color& bg)
{
	ChStdSkin();
	ColorizeIml(CtrlsImg::Iml(), StylerCtrlsImg::Iml(), c, 255);


		SColorFace_Write(Blend(c, fg, min(255, blnd))); //controls face color
		SColorShadow_Write(Blend(c, bg, min(255, blnd)));
		SColorLtFace_Write(Blend(c, fg, min(255, blnd+20)));
		SColorDkShadow_Write(Blend(c, bg, min(255, blnd+20)));

		SColorHighlight_Write(Blend(fg, c, min(255, blnd+20)));
		SColorDisabled_Write(Blend(bg, c, min(255, blnd+20)));

		SColorPaper_Write(Blend(c, fg, min(255, blnd+40))); //for lists, edits etc..
		SColorText_Write(Blend(c, bg, min(255, blnd+40)));

		SColorHighlightText_Write(Blend(c, fg, min(255, blnd+60)));

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

void XmlizeCol(XmlIO& xml, const String& tag, Color& c)
{
	if(xml.IsStoring())
	{
		if(!c.IsNullInstance()) xml(tag, c);
	}
	if(xml.IsLoading())
	{
		int i = xml->FindTag(tag);
		if(i>=0) { XmlIO _ = xml.At(i); ::Xmlize(_, c); }
	}
}

NAMESPACE_UPP
template<> Stream& operator%(Stream& s, PerChStyle& o)
{
	Vector<Color> vc;
	
	if(s.IsStoring())
	{
		vc 
		<< SColorFace()
		<< SColorShadow()
		<< SColorLtFace()
		<< SColorDkShadow()
		<< SColorHighlight()
		<< SColorDisabled()
		<< SColorPaper()
		<< SColorText()
		<< SColorHighlightText()
		<< SColorLight()
	
		<< SColorMenu()
		<< SColorMenuText()
		<< SColorMenuMark()
		<< SColorInfo()
		<< SColorInfoText()
		<< SColorMark()
		<< SColorLabel()
		<< LabelBoxTextColor()
		<< LabelBoxColor()
		;
	}
	s % vc;
	if(s.IsLoading())
	{
		int i = 0;
		SColorFace_Write(vc[i++]);
		SColorShadow_Write(vc[i++]);
		SColorLtFace_Write(vc[i++]);
		SColorDkShadow_Write(vc[i++]);
		SColorHighlight_Write(vc[i++]);
		SColorDisabled_Write(vc[i++]);
		SColorPaper_Write(vc[i++]);
		SColorText_Write(vc[i++]);
		SColorHighlightText_Write(vc[i++]);
		SColorLight_Write(vc[i++]);
	
		SColorMenu_Write(vc[i++]);
		SColorMenuText_Write(vc[i++]);
		SColorMenuMark_Write(vc[i++]);
		SColorInfo_Write(vc[i++]);
		SColorInfoText_Write(vc[i++]);
		SColorMark_Write(vc[i++]);
		SColorLabel_Write(vc[i++]);
		LabelBoxTextColor_Write(vc[i++]);
		LabelBoxColor_Write(vc[i++]);				
	}
	return s;
}

#define XMLIZECOL(TAG, SCOL) \
	if(xml.IsStoring()) \
		XmlizeCol(xml, TAG, c = SCOL()); \
	if(xml.IsLoading()) \
	{ \
		c = Null; \
		XmlizeCol(xml, TAG, c); if(!c.IsNullInstance()) COMBINE(SCOL, _Write)(c); \
	} \


template<> void Xmlize(XmlIO& xml, PerChStyle& o)
{
	Color c;

	XMLIZECOL("face", SColorFace)
	XMLIZECOL("shadow", SColorShadow)
	XMLIZECOL("ltface", SColorLtFace)
	XMLIZECOL("dkshadow", SColorDkShadow)
	XMLIZECOL("highlight", SColorHighlight)
	XMLIZECOL("disabled", SColorDisabled)
	XMLIZECOL("paper", SColorPaper)
	XMLIZECOL("text", SColorText)
	XMLIZECOL("highlighttext", SColorHighlightText)
	XMLIZECOL("light", SColorLight)
	
	XMLIZECOL("menu", SColorMenu)
	XMLIZECOL("menutext", SColorMenuText)
	XMLIZECOL("menumark", SColorMenuMark)
	XMLIZECOL("info", SColorInfo)
	XMLIZECOL("infotext", SColorInfoText)
	XMLIZECOL("mark", SColorMark)
	XMLIZECOL("label", SColorLabel)
	XMLIZECOL("labelboxtext", LabelBoxTextColor)
	XMLIZECOL("labelbox", LabelBoxColor)
}
END_UPP_NAMESPACE

bool SaveGlobalStyle(const String& filename)
{
	PerChStyle d;
	return StoreAsXMLFile(d, "style", filename);	
	//return StoreToFile(d, fs.Get());
}

bool LoadGlobalStyle(const String& filename)
{
	ChReset();

	PerChStyle d;
	bool b = LoadFromXMLFile(d, filename);
	//b = LoadFromFile(d, filename);

	if(!b) return b;

	RefreshGlobalStyles();
	RefreshLayoutDeepAll();

	return b;	
}

//ChamStyleCtrl

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
	clmenumark <<= SColorMenuMark();
	clinfo <<= SColorInfo();
	clinfotext <<= SColorInfoText();
	clmark <<= SColorMark();
	cllabel <<= SColorLabel();
	cllbtext <<= LabelBoxTextColor();
	cllb <<= LabelBoxColor();
}

void ChamStyleCtrl::OnColorAction()
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
	SColorMenuMark_Write(~clmenumark);
	SColorInfo_Write(~clinfo);
	SColorInfoText_Write(~clinfotext);
	SColorMark_Write(~clmark);
	SColorLabel_Write(~cllabel);
	LabelBoxTextColor_Write(~cllbtext);
	LabelBoxColor_Write(~cllb);

	RefreshGlobalStyles();
	WhenSColorsChange(); //let outside apply own changes after recalculation, before we apply them
	RefreshLayoutDeepAll();

	Action();
}

ChamStyleCtrl::ChamStyleCtrl()
{
	CtrlLayout(*this);

	clface <<= THISBACK(OnColorAction);
	clshadow <<= THISBACK(OnColorAction);
	clltface <<= THISBACK(OnColorAction);
	cldkshadow <<= THISBACK(OnColorAction);
	clhighlight <<= THISBACK(OnColorAction);
	cldisabled <<= THISBACK(OnColorAction);
	clpaper <<= THISBACK(OnColorAction);
	cltext <<= THISBACK(OnColorAction);
	clhighlighttext <<= THISBACK(OnColorAction);
	cllight <<= THISBACK(OnColorAction);

	clmenu <<= THISBACK(OnColorAction);
	clmenutext <<= THISBACK(OnColorAction);
	clmenumark <<= THISBACK(OnColorAction);
	clinfo <<= THISBACK(OnColorAction);
	clinfotext <<= THISBACK(OnColorAction);
	clmark <<= THISBACK(OnColorAction);
	cllabel <<= THISBACK(OnColorAction);
	cllbtext <<= THISBACK(OnColorAction);
	cllb <<= THISBACK(OnColorAction);

	load <<= THISBACK(DoLoad);
	save <<= THISBACK(DoSave);
}

void ChamStyleCtrl::DoSave()
{
	FileSel fs;
	if(!fs.ExecuteSaveAs("Save global colors")) return;
	SaveGlobalStyle(fs.Get());
}

void ChamStyleCtrl::DoLoad()
{
	FileSel fs;
	if(!fs.ExecuteOpen("Save global colors")) return;
	LoadGlobalStyle(fs.Get());
	Update();
}

//StylerCtrl

void StylerCtrl::SetColorizedSkin()
{
	Color c  = ~mcl;
	int blnd = ~bl;
	Color fg = ~fgcl;
	Color bg = ~bgcl;

	ChStylerSkin(c, blnd, fg, bg);
}

void StylerCtrl::OnSkinAction()
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
		case 2: SetColorizedSkin(); break;
		default:
		case 0: ChClassicSkin(); break;
	}

	RefreshGlobalStyles();
	WhenSkinChange(); //let outside apply own changes after recalculation, before we apply them
	RefreshLayoutDeepAll();

	UpdateAction();
}

void StylerCtrl::OnInvertAction()
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

	OnSkinAction();
}

void StylerCtrl::Updated()
{
	Color fg = ~fgcl;
	Color bg = ~bgcl;

	dk <<= bool(Grayscale(fg)<Grayscale(bg));
}

StylerCtrl::StylerCtrl()
{
	CtrlLayout(*this);

	dk.SetReadOnly();

	stsw <<= 2; //ColorizedSkin
	stsw <<= THISBACK(OnSkinAction);

	mcl <<= Gray();
	mcl <<= THISBACK(OnSkinAction);

	fgcl <<= White();
	fgcl <<= THISBACK(OnSkinAction);

	bgcl <<= Black();
	bgcl <<= THISBACK(OnSkinAction);
	

	bl.MinMax(0,255);
	bl <<= 192;
	bl <<= THISBACK(OnSkinAction);

	inv <<= THISBACK(OnInvertAction);

	//SkinAction(); //try our setup
}
