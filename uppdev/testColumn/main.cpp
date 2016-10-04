#include <CtrlLib/CtrlLib.h>
#define LAYOUTFILE <testColumn/testColumn.lay>
#include <CtrlCore/lay.h>

class testColumn : public WithColumnLayout<ParentCtrl> {
public:
	typedef testColumn CLASSNAME;
	testColumn();
	void HideArray();
};

void testColumn::HideArray()
{ console.Hide();
}

testColumn::testColumn()
{   int i;
	CtrlLayout(*this);

	AddFrame(ThinOutsetFrame());

	for (i=0; i<7; i++) slist.Add("this is a string           knknkn          mkm   ");

	console.AddColumn("Time", t_("Time"), 8);
	console.AddColumn("Tournament", t_("Tournament"), 5);
	console.AddColumn("System", t_("System"), 5);
	console.AddColumn("Round", t_("Round"), 3);
	console.AddColumn("Action", t_("Action"), 13);
	console.AddColumn("Status", t_("Status"), 13);
	for (i=0; i<7; i++) console.HeaderTab(i).SetInk(SLtRed);
	console.Add(AsString(GetSysTime()), "...", "..." , "0", "  0,  0,  0", t_("Hello!"), t_("Open"));
	btnClose.WhenAction = THISBACK(HideArray);
}

GUI_APP_MAIN
{ testColumn a, b;
  TopWindow w;
  Splitter spls;
  w.Add(spls);
  spls.Vert();
  spls.Vert(a, b);
  w.Run();
}
