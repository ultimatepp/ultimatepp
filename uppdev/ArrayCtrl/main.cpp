#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ArrayCtrl a, b;
	Splitter  s;

	void DnD(PasteClip& d)
	{
		if(AcceptText(d))
			a.Add(GetString(d), GetString(d));
	}
	
	void DnDInsert(int line, PasteClip& d)
	{
		if(AcceptInternal<ArrayCtrl>(d, "array"))
			a.InsertDrop(line, d);
		if(AcceptText(d)) {
			a.Insert(line);
			a.Set(line, 0, GetString(d));
			a.SetCursor(line);
		}
	}

	void DnDInsertB(int line, PasteClip& d)
	{
	}

	void Drag()
	{
	}

	void DragB()
	{
	}

	typedef App CLASSNAME;

	App() {
		a.AddColumn("Text");
		a.MultiSelect();
		a.NoGrid();
		a.WhenDropInsert = THISBACK(DnDInsert);
		a.WhenDrop = THISBACK(DnD);
		a.WhenDrag = THISBACK(Drag);
		
		b.AddColumn("Text");
		b.MultiSelect();
		b.WhenDropInsert = THISBACK(DnDInsertB);
		b.WhenDrag = THISBACK(DragB);

		Add(s.Horz(a, b));
		for(int i = 0; i < 200; i++) {
			a.Add(i);
			b.Add(FormatIntRoman(i, true));
			if((i & 3) == 0)
				a.AddSeparator();
		}
		Sizeable();
	}
};

GUI_APP_MAIN
{
	DUMP("Test");
	App().Run();
}
