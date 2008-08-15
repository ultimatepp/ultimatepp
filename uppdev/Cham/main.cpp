#include <CtrlLib/CtrlLib.h>

CH_INTS(ChA, 2, 1 << 2);
CH_INTS(ChB, 2, ChA);
CH_INT(ChC, 123456);

#define IMAGECLASS TImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml.h>

struct MyApp : TopWindow {
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), Cyan);
		ChPaint(w, GetSize(), TImg::TB1());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
	return;
	TopWindow w;
	Button b;
	FrameRight<Button> b1;
	w.Add(b.EdgeStyle().SizePos());
	b.AddFrame(b1);
	w.Sizeable().Run();
	return;

	PromptOKCancel("Test");

	DUMP(ChA(0));
	DUMP(ChA(1));
	DUMP(ChB(0));
	DUMP(ChB(1));
	VectorMap<String, Value> map;
	ChSet("ChB", 0, 55);
	DUMP(ChA(0));
	DUMP(ChA(1));
	DUMP(ChB(0));
	DUMP(ChB(1));
	DUMP(ChC());
	DUMP(ChGet("ChA", 1));

	DUMP(CtrlsImg::Iml().GetBinSize());

	int sz2 = 0;
	int szn = 0;
	for(int q = 0; q < CtrlsImg::GetCount(); q++) {
		sz2 += StoreImageAsString(CtrlsImg::Get(q)).GetLength();
		szn += CtrlsImg::GetId(q).GetLength() + 1;
	}
	DUMP(sz2);
	DUMP(szn);
	String h;
	for(int q = 0; q < CtrlsImg::GetCount(); q++) {
		Image m = CtrlsImg::Get(q);
		h.Cat((byte *)~m, 4 * m.GetLength());
	}
	DUMP(ZCompress(h).GetLength());
}
