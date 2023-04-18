#include <CtrlLib/CtrlLib.h>
#include "TypeReader.h"

using namespace Upp;

#define MAKE_TT_TABLE_NAME(c1, c2, c3, c4) \
   (((uint32)c4) << 24 | ((uint32)c3) << 16 | ((uint32)c2) << 8 | ((uint32)c1))

#define CMAP (MAKE_TT_TABLE_NAME('c','m','a','p'))

#ifdef PLATFORM_WIN32

namespace Upp {
HFONT GetWin32Font(Font fnt, int angle);
};

String GetFontDataSys(Font font, dword table)
{
	String r;
	HFONT hfont = GetWin32Font(font, 0);
	if(hfont) {
		HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		DWORD size = GetFontData(hdc, table, 0, NULL, 0);
		if(size == GDI_ERROR) {
			return Null;
		}
		else {
			StringBuffer b(size);
			GetFontData(hdc, table, 0, b, size);
			r = b;
		}
		::SelectObject(hdc, ohfont);
		::DeleteDC(hdc);
	}
	return r;
}

#endif

String ReadFontTable(Stream& in, const char *table, int fonti = 0)
{
	in.Seek(0);
	int q = in.Get32be();
	if(q == 0x74746366) { // true type collection
		in.Get32(); // skip major/minor version
		int nfonts = in.Get32be();
		if(fonti >= nfonts)
			return Null;
		in.SeekCur(fonti * 4);
		int offset = in.Get32be();
		if(offset < 0 || offset >= in.GetSize())
			return Null;
		in.Seek(offset);
		q = in.Get32be();
	}
	if(q != 0x74727565 && q != 0x00010000)
		return Null;
	int n = in.Get16be();
	in.Get32();
	in.Get16();
	while(n--) {
		if(in.IsError() || in.IsEof()) return Null;
		String tab = in.Get(4);
		in.Get32();
		int offset = in.Get32be();
		int length = in.Get32be();
		if(tab == table) {
			if(offset < 0 || length < 0 || offset + length > in.GetSize())
				return Null;
			in.Seek(offset);
			return in.Get(length);
		}
	}
	return Null;
}
/*
struct MyApp : TopWindow {
	void Paint(Draw& w) {
		String text;
		text << WString(0x1F970, 1).ToString();
		text << WString(0x1F603, 1).ToString();
		text << WString(0x1F923, 1).ToString();
		text << " Quick brown fox, 訓民正音 (훈민정음) ";
		
		w.DrawRect(GetSize(), White());
		int y = 10;
		int x = 10;
		Font fnt(Font::FindFaceNameIndex("Noto Color Emoji"), 20);
		FontTypeReader r;
		r.Open(fnt);
		for(auto r : r.ranges) {
			for(int c = r.a; c <= r.b; c++) {
				w.DrawText(x, y, WString(c, 1), fnt);
				y += 30;
				if(y > GetSize().cy - 30) {
					y = 10;
					x += 100;
				}
			}
		}
	}
};

GUI_APP_MAIN {
	MyApp().Sizeable().Zoomable().Run();
}
*/

GUI_APP_MAIN
{
//	FileIn in("C:/Windows/Fonts/arial.ttf");
/*
	FileIn in("D:\\xxx\\msyh.ttc");
	DDUMPHEX(ReadFontTable(in, "cmap", 0));
	DDUMPHEX(ReadFontTable(in, "cmap", 1));
	DDUMPHEX(ReadFontTable(in, "cmap", 2));
	return;
*/
#if 0
/*	for(int i = 0; i < Font::GetFaceCount(); i++)
		DDUMP(Font::GetFaceName(i));
	return;*/
	Font font(Font::FindFaceNameIndex("Noto Color Emoji"), 20);
	FontTypeReader r;
	r.Open(font);
	for(auto r : r.ranges) {
		DUMPHEX(r.a);
		DUMPHEX(r.b);
		RLOG("=================");
	}
	return;
#endif
	Progress pi;
	pi.SetTotal(Font::GetFaceCount());
	Vector<String> name;
	for(int i = 0; i < Font::GetFaceCount(); i++)
		name.Add(Font::GetFaceName(i));
	for(int i : GetSortOrder(name)) {
		String f = Font::GetFaceName(i);
		pi.SetText(f);
		if(pi.StepCanceled())
			break;
		Font fnt;
		fnt.FaceName(f);
		FontTypeReader r;
	//	r.Open(Arial(20).GetData());
		r.Open(fnt);
		if(r.panose.GetCount() == 10) {
			dword h[8] = {0};
			for(auto r : r.ranges) {
				for(int c = r.a; c <= r.b; c++) {
					c = max(c, 0);
					if(c < 2048)
						h[0] |= 0x80000000 >> (c >> 6);
					else {
						int bi = clamp(c - 2048, 0, 7*32*1024 - 1) >> 10;
						ASSERT((bi >> 5) + 1 < 8);
						h[(bi >> 5) + 1] |= 0x80000000 >> (bi & 31);
					}
				}
			}
			String l;
			l << "{ " << AsCString(f);
			l << ", { ";
			bool first = true;
			for(int h : r.panose) {
				if(first)
					first = false;
				else
					l << ",";
				l << h;
			}
			l << " }, { ";
			for(int i = 0; i < 8; i++) {
				if(i) l << ",";
				l << Format("0x%08x", (int)h[i]);
			}
			l << " } },";
			LOG(l);
		}
	}
	
//	r.Open(Font(Font::FindFaceNameIndex("Segoe UI Emoji"), 20).GetData());
}
