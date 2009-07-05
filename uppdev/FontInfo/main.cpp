#include "FontInfo.h"

using namespace Upp;

struct CharDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top, AsString(q), StdFont(), ink);
		Font fnt;
		fnt.FaceName("Bitstream Vera Sans Mono");
//		fnt.Face(Font::ARIAL);
		fnt.Height(20);
		WString txt((int)q, 1);
		w.DrawText(r.left + 40, r.top, txt, Courier(20), ink);
		GlyphInfo gi = GetGlyphMetrics(fnt, q);
		w.DrawRect(r.left + 60, r.top, gi.width, r.GetHeight(), LtCyan());
		gi = GetGlyphInfo(fnt, q);
		if(!gi.IsMissing()) {
			if(gi.IsNormal())
				w.DrawText(r.left + 60, r.top, txt, fnt);
			else
			if(gi.IsReplaced())
				w.DrawText(r.left + 60, r.top, txt, fnt().Face(gi.lspc), Magenta());
			else
			if(gi.IsComposed()) {
				ComposedGlyph g;
				if(Compose(fnt, q, g)) {
					w.DrawText(r.left + 60, r.top, WString(g.basic_char, 1), fnt, LtBlue());
					w.DrawText(r.left + 60 + g.mark_pos.x, r.top + g.mark_pos.y,
					           WString(g.mark_char, 1), g.mark_font, LtRed());
				}
			}
		}
		else
			w.DrawText(r.left + 60, r.top, txt, fnt, ink);
		w.DrawRect(r.left, r.bottom - 1, r.Width(), 1, Black());
	}

};

Size GetTextSizeNew(const wchar *text, Font font, int n)
{
	int cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		cx += GetGlyphInfo(font, *wtext++).width;
		n--;
	}
	return Size(cx, GetFontInfo(font).height);
}

struct App : TopWindow {
	virtual void Paint(Draw& w) {
		WString h = "Hello ěščřžýáíéúů ";
		for(int i = 0; i < 10; i++)
			h.Cat(20000 + i);
		w.DrawRect(GetSize(), White());
		Font fnt;
		fnt.FaceName("Bitstream Vera Sans Mono");
		fnt.Height(13);
		DoDrawText(w, 100, 100, 0, h, fnt, Black(), h.GetCount(), NULL);
		DoDrawText(w, 100, 300, 300, h, fnt, Black(), h.GetCount(), NULL);
	}
};

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_UTF8);
	
	
#if 0
	static dword t[] = { 0x9d15, 0x3092,0x4e26,0xe20,0xf780,0x994,0x636,0xfeb2,0xfebe,0x2126 };
	VectorMap<String, int> stat;
	for(int i = 0; i < __countof(t); i++) {
		int chr = t[i];
		LOG("-------- " << chr);
		for(int i = 0; i < Font::GetFaceCount(); i++) {
			Font fnt;
			fnt.Height(20);
			fnt.Face(i);
			if(GetGlyphInfoSys(fnt, chr).IsNormal()) {
				LOG(fnt.GetFaceName());
				stat.GetAdd(fnt.GetFaceName(), 0)++;
			}
		}
	}
	LOG("--------------------------");
	Vector<int> o = GetSortOrder(stat.GetValues());
	for(int i = 0; i < o.GetCount(); i++)
		LOG(stat.GetKey(o[i]) << ": " << stat[o[i]]);
	LOG("--------------------------");
#endif
	App app;
	app.Open();
	GetGlyphInfo(Font().FaceName("Bitstream Vera Sans Mono").Height(20), 461);
	ColumnList list;
	list.SetDisplay(Single<CharDisplay>());
	list.ItemHeight(25).Columns(12);
	for(int i = 32; i < 65536; i++)
		list.Add(i);
	TopWindow win;
//	list.SetCursor(461);
	win.Add(list.SizePos());
	win.Run();

	return;

	FontInfo fi = Arial(100).Bold().Info();
	DUMP(fi.GetAscent());
	DUMP(fi.GetDescent());
	DUMP(fi.GetMaxWidth());
	
	{
		int from = 0;
		for(int i = 0; i < 65536; i++) {
			GlyphInfo g = GetGlyphInfoSys(Arial(100).Bold(), i);
			if(g.IsMissing()) {
//			if(!fi.HasChar(i)) {
				if(from < i)
					LOG(from << " - " << i - 1);
				from = i + 1;
			}
		}
		if(from < 65535)
			LOG(from << " - " << 65535);
	}

	Vector<FaceInfo> fa = GetAllFacesSys();
	for(int i = 0; i < fa.GetCount(); i++)
		LOG(fa[i].name << ": scalable: " << (fa[i].info & Font::SCALEABLE) << ", fixed: " << (fa[i].info & Font::FIXEDPITCH));
	CommonFontInfo f = GetFontInfoSys(Arial(100).Bold());
	DUMP(f.ascent);
	DUMP(f.descent);
	DUMP(f.lineheight);
	DUMP(f.maxwidth);
//	DUMP(f.path);

	
	for(int c = '0'; c < 'z'; c++) {
		LOG("-------------------------");
		DUMP((char)c);
		GlyphInfo g = GetGlyphInfoSys(Arial(100).Bold(), c);
		DUMP(g.width);
		DUMP(fi[c]);
		DUMP(g.lspc);
		DUMP(fi.GetLeftSpace(c));
		DUMP(g.rspc);
		DUMP(fi.GetRightSpace(c));
	}
	
	
	
	
	
#if 0
	WString x = "Just a little test!";
	RDUMP(GetTextSize(x, Arial(20), x.GetCount()));
	RDUMP(GetTextSizeNew(x, Arial(20), x.GetCount()));
	RDUMP(sizeof(GlyphInfo));
	RDUMP(sizeof(CharEntry));
	RDUMP(sizeof(CommonFontInfo));
	#ifndef _DEBUG
	int cx = 0;
	{
		RTIMING("Old");
		for(int i = 0; i < 1000000; i++) {
			cx += GetTextSize(x, Arial(20), x.GetCount()).cx;
		}
	}
	{
		RTIMING("New");
		for(int i = 0; i < 1000000; i++) {
			cx += GetTextSizeNew(x, Arial(20), x.GetCount()).cx;
		}
	}
	#endif
#endif
}
