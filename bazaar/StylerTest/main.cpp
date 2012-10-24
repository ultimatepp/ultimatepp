#include "StylerTest.h"

#define IMAGECLASS StylerTestImg
#define IMAGEFILE <StylerTest/StylerTest.iml>
#include <Draw/iml_source.h>

static Color SColorEditPaper = LtRed;
static Color SColorEditText = Yellow;
static Color SColorEditHighlight = Yellow;
static Color SColorEditHighlightText = Blue;

static Color SColorButtonFace = LtRed;
static Color SColorButtonText = Blue;

static Color SColorSpinButtonMonoColor = LtRed;
static Color SColorSpinButtonMonoColorh = White;
static Color SColorSpinButtonMonoColorp = LtBlue;
static Color SColorSpinButtonMonoColord = LtYellow;

CH_LOCSTYLE(EditField, Style, EditFieldStyleDFM)
{
	paper = Blend(SColorPaper(), SColorEditPaper);
	disabled = SColorFace();
	focus = paper;
	invalid = Blend(paper, Color(255, 0, 0), 32);
	text = SColorEditText;
	textdisabled = SColorDisabled;
	selected = SColorEditHighlight;
	selectedtext = SColorEditHighlightText;
	for(int i = 0; i < 4; i++)
		edge[i] = CtrlsImg::EFE();
	activeedge = false;
	vfm = 2;
}

CH_LOCSTYLE(Button, Style, ButtonStyleDFM)
{
	look[0] = Blend(SColorFace(), SColorButtonFace);
	look[1] = SColorFace();
	look[2] = LtBlue();
	look[3] = LtYellow();
//	CtrlsImageLook(look, StylerTestImg::I_B);
	
//	monocolor[0] = monocolor[1] = monocolor[2] = monocolor[3] =
//		Blend(Blend(SColorHighlight, SColorShadow), SColorLabel, 80);
	monocolor[0] = SColorSpinButtonMonoColor;
	monocolor[1] = SColorSpinButtonMonoColorh;
	monocolor[2] = SColorSpinButtonMonoColorp;
	monocolor[3] = SColorSpinButtonMonoColord;

	textcolor[0] = textcolor[1] = textcolor[2] = SColorButtonText;
	textcolor[3] = SColorDisabled();
	font = StdFont();
	pressoffset = Point(0, 0);
	focusmargin = 3;
	overpaint = 0;
	transparent = true;
}

CH_LOCSTYLE(SpinButtons, Style, SpinButtonStyleDFM)
{
	inc = dec = ButtonStyleDFM();
	CtrlsImageLook(inc.look, CtrlsImg::I_EB, CtrlsImg::SpU(), inc.monocolor);
	CtrlsImageLook(dec.look, CtrlsImg::I_EB, CtrlsImg::SpD(), dec.monocolor);
	width = 12;
}

void StylerTest::ReloadChamCB()
{
	chc.Update();
	ApplyOwnStyle();	
}

void StylerTest::ReloadCB()
{
	Refresh();	
}

void StylerTest::Menu(Bar& bar)
{
	bar.Add(true, "Refresh", CtrlImg::help(), THISBACK(ReloadCB));
	bar.Add(false, "Refresh(dis)", CtrlImg::help(), THISBACK(ReloadCB));
}

void StylerTest::ApplyOwnStyle()
{
	if(!overdef.Get()) return;

	//override default style settings, without changing ref in each ctrl
	{
	EditField::Style& s = EditField::StyleDefault().Write();
	s = EditFieldStyleDFM();
	}
	{
	Button::Style& s = Button::StyleNormal().Write();
	s = ButtonStyleDFM();
	}

	{
	SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
	s = SpinButtonStyleDFM();
	}
}

StylerTest::StylerTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();

	sc.WhenSkinChange = THISBACK(ReloadChamCB);
	Add(sc.LeftPos(0,200).BottomPos(0,200));

	chc.WhenSColorsChange = THISBACK(ApplyOwnStyle);
	Add(chc.RightPos(0,400).BottomPos(0,200));

	overdef <<= callback(&sc, &StylerCtrl::OnSkinAction); //emulate a skin change

	InitDummys();
	b.SetStyle(ButtonStyleDFM()); //to see statically without change

	overdef.Set(0); //dont override default by default
	
	sc.OnSkinAction();
}

void StylerTest::InitDummys()
{
	//dummys

	mbar.Set( THISBACK( Menu ) );
	tool.Set( THISBACK( Menu ) );

	AddFrame(mbar);
	AddFrame(tool);
	AddFrame(stbar);

	bo.SetLabel("But.Opt.");
	
	de.SetData("This is a\nDocEdit example text");
	le.SetData("LineEdit");
	ef.SetData("EditField");
	es.SetData("EditString");
	
	ei = 100;
	ei64 = 22342599399929;
	ed = 10.3;
	
	einn <<= 100;
	ei64nn <<= 22342599399929;
	ednn <<= 10.3;

	eis <<= 100;
	ei64s <<= 22342599399929;
	eds <<= 10.3;

	einns <<= 100;
	ei64nns <<= 22342599399929;
	ednns <<= 10.3;
	
	edt <<= GetSysDate();
	etm <<= GetSysTime();

	edtnn <<= GetSysDate();
	etmnn <<= GetSysTime();

	ddt <<= GetSysDate();
	dtm <<= GetSysTime();
	
	ac.AddColumn("One");
	ac.AddColumn("Two");
	ac.Add("Hello", 1234);
	ac.Add(23.8, "servus");
	ac.Add("foo", true);
	ac.Add("bar", "lee");
	ac.Add(23.8, "servus");
	ac.Add("foo", true);
	ac.Add("bar", "lee");
	ac.Add(23.8, "servus");
	ac.Add("foo", true);
	ac.Add("bar", "lee");
	ac.Add(23.8, "servus");
	ac.Add("foo", true);
	ac.Add("bar", "lee");

	StringStream s;
	Color cl;
	s % cl;

	sl <<= 0;
	
	dl.Add(0,"First");
	dl.Add(1,"Second");
	dl.Add(2,"Third");
	dl.Add(3,"Fourth");
	
	
	Vector<int> parent, parent2;
	parent.Add(0);
	tr.SetRoot(Image(), "The Tree");
	for(int i = 1; i < 10000; i++) {
		parent.Add(tr.Add(parent[rand() % parent.GetCount()], Image(),
		            FormatIntRoman(i, true)));
		if((rand() & 3) == 0)
			tr.Open(parent.Top());
	}
	tr.Open(0);

	{
	TreeCtrl& tree = dtr->MultiSelect();
	Vector<int> parent, parent2;
	parent.Add(0);
	tree.SetRoot(Image(), "The Tree");
	for(int i = 1; i < 10000; i++) {
		parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
		            FormatIntRoman(i, true)));
		if((rand() & 3) == 0)
			tree.Open(parent.Top());
	}
	tree.Open(0);
	}
	
	tb.Add(arc.Add(new EditString()).SizePos(),"First");
	tb.Add(arc.Add(new EditString()).SizePos(),"Second");
	tb.Add(arc.Add(new EditString()).SizePos(),"Third");
	
}

GUI_APP_MAIN
{
	StylerTest().Run();
}

