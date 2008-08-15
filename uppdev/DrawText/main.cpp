#include "CtrlLib/CtrlLib.h"

using namespace Upp;

class MyApp : public TopWindow {
public:
	EditString es;
	virtual void Paint(Draw& w);

	MyApp()
	{
	//	Add(es);
		es.SetRect(10, 10, 200, 40);
//		es.SetFont(Arial(10));
		DUMP(es.GetFont());
	}
};

void DoDraw(Draw& w, Size sz)
{
	w.DrawRect(sz, White);
	for(int i = 0; i < 3600; i += 300)
		w.DrawText(300, 300, i, "                       " + AsString(i), Arial(24).Underline().Strikeout());
}

/*	Size sz = GetSize();
	DoDraw(w, GetSize());
	ImageDraw h(500, 500);
	DoDraw(h, Size(500, 500));
	w.DrawImage(600, 0, h);
*/

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SWhite);
	WString txt = "皆の日本語";
	DUMP(txt[0]);
	for(int q = 0; q < 2; q++) {
		Font fnt[4] = { StdFont(30), Arial(30), Roman(30), Courier(30) };
		for(int i = 0; i < 4; i++) {
			Size sz = GetTextSize(txt, fnt[i]);

	/*	w.SetFont(fnt[i]);
		GetTextExtentPoint32W(
			w.GetHandle(),           // handle to DC
			~txt,  // text string
			5,      // characters in string
			sz      // string size
		);*/

			w.DrawRect(10 + q * 250, 100 + 50 * i, sz.cx, sz.cy, LtGray());
			w.DrawText(10 + q * 250, 100 + 50 * i, txt, fnt[i]);
		}
		txt = "ČŠĚŠČěščěšč";
	}
}


struct UniDisplay : Display {
	Font fnt;

	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const {
		w.DrawRect(r, White);
		WString h = q;
		Size sz = GetTextSize(h, fnt);
		int yy = r.top + (sz.cy - r.GetHeight()) / 2;
		w.DrawRect(r.left + 10, yy, sz.cx, sz.cy, LtGray);
		w.DrawText(r.left + 10, yy, h, fnt);
	}
};

GUI_APP_MAIN
{
Font fnt = Font(Font::FindFaceNameIndex("DejaVuSans"), 30);
	LOG("Started");
	FontInfo f = fnt.Info();
//	for(int i = 46*256; i < 128 * 256; i++)
//		LOG(i << ' ' << f[i]);
	DUMP(f[30342]);
	DUMP(f[65000]);
	int pgsz = 1024;
	for(int p = 0; p < 65536; p += pgsz) {
		VectorMap<int, int> map;
		for(int i = 0; i < pgsz; i++) {
			map.GetAdd(f[i + p], 0)++;
		}
		LOG(p << ' ' << f[p + 512]);
		Vector<int> o = GetSortOrder(map.GetValues(), StdGreater<int>());
		for(int i = 0; i < map.GetCount(); i++) {
			int q = o[i];
			LOG("   " << map.GetKey(q) << "  " << map[q]);
		}
	}
	SetDefaultCharset(CHARSET_UTF8);
	ArrayCtrl a;
	UniDisplay d;
	d.fnt = fnt;
	a.AddColumn("Code");
	a.AddColumn("Char").SetDisplay(d);
	a.AddColumn("Width");
	a.SetLineCy(40);
	for(int i = 0; i < 65536; i++)
		a.Add(i, WString(i, 1), f[i]);
	TopWindow win;
	win.Add(a.SizePos());
	win.Run();
//	return;
	MyApp().Run();
}
