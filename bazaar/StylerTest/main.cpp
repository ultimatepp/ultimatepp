#include "StylerTest.h"

#define IMAGECLASS StylerTestImg
#define IMAGEFILE <StylerTest/StylerTest.iml>
#include <Draw/iml_source.h>

void StylerTest::ReloadChamCB()
{
	chc.Reload();	
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

StylerTest::StylerTest()
{
	CtrlLayout(*this, "Window title");

	Add(sc.LeftPos(0,200).BottomPos(0,200));
	sc <<= THISBACK(ReloadChamCB);

	Add(chc.RightPos(0,400).BottomPos(0,200));

	mbar.Set( THISBACK( Menu ) );
	tool.Set( THISBACK( Menu ) );

	AddFrame(mbar);
	AddFrame(tool);
	AddFrame(stbar);

	//dummys

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

