#include <CtrlLib/CtrlLib.h>
#include "TypeReader.h"

using namespace Upp;

#define MAKE_TT_TABLE_NAME(c1, c2, c3, c4) \
   (((uint32)c4) << 24 | ((uint32)c3) << 16 | ((uint32)c2) << 8 | ((uint32)c1))

#define CMAP (MAKE_TT_TABLE_NAME('c','m','a','p'))

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

GUI_APP_MAIN
{
#if 0
//	Font font(Font::FindFaceNameIndex("Microsoft JhengHei"), 20);
	Font font = StdFont();
	
//	SaveFile("d:/xxx/font.bin", GetFontDataSys(font, 0));
	
	String cmap = GetFontDataSys(font, CMAP);
	LOGHEXDUMP(~cmap, 256);

	FontTypeReader r;
//	r.Open(Arial(20).GetData());
	r.Open(font.GetData());
	return;
	
	
	return;
#endif
	Progress pi;
	pi.SetTotal(Font::GetFaceCount());
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		String f = Font::GetFaceName(i);
		pi.SetText(f);
		if(pi.StepCanceled())
			break;
		Font fnt;
		fnt.FaceName(f);
		DLOG("===========");
		DDUMP(f);
		FontTypeReader r;
	//	r.Open(Arial(20).GetData());
		r.Open(fnt.GetData());
		Sort(r.ranges, [](Tuple<int, int> a, Tuple<int, int> b) { return a.a < b.a; });
		DDUMP(r.ranges);
		while(r.ranges.GetCount() > 8) {
			int mini = 0;
			int mind = INT_MAX;
			for(int i = 0; i < r.ranges.GetCount() - 1; i++) {
				int d = r.ranges[i + 1].a - r.ranges[i].b;
				if(d < mind) {
					mind = d;
					mini = i;
				}
			}
			r.ranges[mini].b = r.ranges[mini + 1].b;
			r.ranges.Remove(mini + 1);
		}
		DDUMP(r.ranges);
	}
	
//	r.Open(Font(Font::FindFaceNameIndex("Segoe UI Emoji"), 20).GetData());
}
