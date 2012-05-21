#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ArrayCtrl a, b;
	Splitter  s;

	StatusBar status;

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
	
	void A()
	{
		Exclamation("LeftDouble");
		Exit();
	}
	
	void B()
	{
		Exclamation("LeftClick");
	}

	typedef App CLASSNAME;

	App() {
		a.AddColumn("\1Text alsdfjla fdlajd flajd falsjkd fla fals fj").HeaderTab().WhenLeftDouble = THISBACK(A);
		a.AddColumn("asdf").HeaderTab().WhenLeftClick = THISBACK(B);
		a.MultiSelect();
		a.NoGrid();
		a.WhenDropInsert = THISBACK(DnDInsert);
		a.WhenDrop = THISBACK(DnD);
		a.WhenDrag = THISBACK(Drag);
		a.WhenLeftDouble = THISBACK(A);
		
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
		AddFrame(status);
	}
};

GUI_APP_MAIN
{
	DUMP("Test");
	App().Run();
}


String HttpResponse(int code, const char *phrase, const String& data, const char *content_type)
{
	String r;
	r <<
		"HTTP/1.0 " << code << ' ' << phrase << "\r\n"
		"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
		"Server: Centrum-Nos SaleCrm\r\n"
		"Content-Length: " << data.GetCount() << "\r\n"
		"Connection: close\r\n";
	if(content_type)
		r << "Content-Type: text/xml\r\n";
	r << "\r\n" << data;
	return r;
}

