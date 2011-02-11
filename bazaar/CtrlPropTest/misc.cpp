#include "CtrlPropTest.h"

void CtrlPropTest::InitDummies()
{
	vis.bo.SetLabel("MyCtrlsImg");
	
	vis.de.SetData("This is a\nDocEdit example text");
	vis.le.SetData("LineEdit");
	vis.ef.SetData("EditField");
	vis.es.SetData("EditString");
	
	vis.ei = 100;
	vis.ei64 = 22342599399929;
	vis.ed = 10.3;
	
	vis.einn <<= 100;
	vis.ei64nn <<= 22342599399929;
	vis.ednn <<= 10.3;

	vis.eis <<= 100;
	vis.ei64s <<= 22342599399929;
	vis.eds <<= 10.3;

	vis.einns <<= 100;
	vis.ei64nns <<= 22342599399929;
	vis.ednns <<= 10.3;
	
	vis.edt <<= GetSysDate();
	vis.etm <<= GetSysTime();

	vis.edtnn <<= GetSysDate();
	vis.etmnn <<= GetSysTime();

	vis.ddt <<= GetSysDate();
	vis.dtm <<= GetSysTime();
	
	vis.ac.AddColumn("One");
	vis.ac.AddColumn("Two");
	vis.ac.Add("Hello", 1234);
	vis.ac.Add(23.8, "servus");
	vis.ac.Add("foo", true);
	vis.ac.Add("bar", "lee");

	vis.sl <<= 0;
	
	vis.dl.Add(0,"First");
	vis.dl.Add(1,"Second");
	vis.dl.Add(2,"Third");
	vis.dl.Add(3,"Fourth");
	
	
	Vector<int> parent, parent2;
	parent.Add(0);
	vis.tr.SetRoot(Image(), "The Tree");
	for(int i = 1; i < 10000; i++) {
		parent.Add(vis.tr.Add(parent[rand() % parent.GetCount()], Image(),
		            FormatIntRoman(i, true)));
		if((rand() & 3) == 0)
			vis.tr.Open(parent.Top());
	}
	vis.tr.Open(0);

	{
	TreeCtrl& tree = vis.dtr->MultiSelect();
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
	
	vis.tb.Add(arc.Add(new EditInt()).SizePos(),"First");
	vis.tb.Add(arc.Add(new EditInt()).SizePos(),"Second");
	vis.tb.Add(arc.Add(new EditInt()).SizePos(),"Third");
}

void CtrlPropTest::Test()
{
	EditInt ei;	
	bool b;

	ei.Tip("Tooltip for this one");

	b = Props<Ctrl>::Set(ei, "data", 10);
	RLOG(b);

	Value v;
	b = Props<Ctrl>::Get(ei, "data", v);
	RLOG(b << v);
	
	b = Props<Ctrl>::Set(ei, "min", -100);
	RLOG(b);
	b = Props<Ctrl>::Set(ei, "max", 100);
	RLOG(b);
	
	b = Props<Ctrl>::Get(ei, "min", v);
	RLOG(b << v);
	b = Props<Ctrl>::Get(ei, "max", v);
	RLOG(b << v);
	
	v = Value();
	b = Props<Ctrl>::Get(ei, "listset", v);
	RLOG(b << v);
	
	v = Value();
	b = Props<Ctrl>::Get(ei, "listget", v);
	RLOG(b << v);
	
	ValueArray va = v;
	for(int i = 0; i < va.GetCount(); i++)
	{
		String s = va.Get(i);
		RLOG(s);
		v = Value();
		b = Props<Ctrl>::Get(ei, s, v);
		RLOG(v << "\n");
	}
	
	//other method
	
	AccessorMap am;
	b = Props<Ctrl>::SetupAccessorMap(ei, am);
	
	b = am.Get("password").set(true);
	v = Value();
	b = am.Get("enable").get(v);	
}
