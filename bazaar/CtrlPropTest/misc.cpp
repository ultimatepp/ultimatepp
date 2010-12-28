#include "CtrlPropTest.h"

void CtrlPropTest::InitDummies()
{
	bo.SetLabel("MyCtrlsImg");
	
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
	
	tb.Add(arc.Add(new EditInt()).SizePos(),"First");
	tb.Add(arc.Add(new EditInt()).SizePos(),"Second");
	tb.Add(arc.Add(new EditInt()).SizePos(),"Third");
}

void CtrlPropTest::Test()
{
	EditInt ei;	
	bool b;

	ei.Tip("Tooltip for this one");

	b = SetProperty(ei, "data", 10);
	RLOG(b);

	Value v;
	b = GetProperty(ei, "data", v);
	RLOG(b << v);
	
	b = SetProperty(ei, "min", -100);
	RLOG(b);
	b = SetProperty(ei, "max", 100);
	RLOG(b);
	
	b = GetProperty(ei, "min", v);
	RLOG(b << v);
	b = GetProperty(ei, "max", v);
	RLOG(b << v);
	
	v = Value();
	b = GetProperty(ei, "listset", v);
	RLOG(b << v);
	
	v = Value();
	b = GetProperty(ei, "listget", v);
	RLOG(b << v);
	
	ValueArray va = v;
	for(int i = 0; i < va.GetCount(); i++)
	{
		String s = va.Get(i);
		RLOG(s);
		v = Value();
		b = GetProperty(ei, s, v);
		RLOG(v << "\n");
	}
}
