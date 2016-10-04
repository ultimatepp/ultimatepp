#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct TextViewCtrl : Ctrl {
	virtual void Paint(Draw& w);
	virtual bool Key(dword key, int count);
	
	int64   pagepos;
	Stream *in;

	int   Get(int64 pos);	
	int64 Next(int64 pos);
	int64 Prev(int64 pos);
};

int TextViewCtrl::Get(int64 pos)
{
	in->Seek(pos);
	return in->Term();
}

int64 TextViewCtrl::Next(int64 pos)
{
	for(;;) {
		int c = Get(pos);
		if(c < 0)
			return in->GetSize();
		if(c == '\n')
			return pos + 1;
		pos++;
	}
}

int64 TextViewCtrl::Prev(int64 pos)
{
	--pos;
	for(;;) {
		if(pos <= 0)
			return 0;
		if(Get(pos - 1) == '\n')
			return pos;
		pos--;
	}
}

void TextViewCtrl::Paint(Draw& w)
{
	Font font = Monospace(14);
	Size sz = GetSize();
	w.DrawRect(sz, White());
	int64 pos = pagepos;
	int y = 0;
	int x = 0;
	int n = 0;
	while(y < sz.cy && pos < in->GetSize()) {
		DDUMP(y);
		DDUMP(sz.cy);
		DDUMP(pos);
		wchar c = Get(pos);
		if(c == '\n') {
			y += max(font.GetCy(), 1);
			x = 0;
			n = 0;
		}
		else {
			w.DrawText(x, y, &c, font, Black(), 1);
			x += font['M'];
			if(++n > 200) {
				y += max(font.GetCy(), 1);
				x = 0;
				n = 0;
			}
		}
		pos++;
	}
}

bool TextViewCtrl::Key(dword key, int count)
{
	switch(key) {
	case K_DOWN:
		pagepos = Next(pagepos);
		return true;
	case K_UP:
		pagepos = Prev(pagepos);
		return true;
	}
}

GUI_APP_MAIN
{
	FileIn in("C:/xxx/20131116_ST_ZZSZ.xml");
	TextViewCtrl txt;
	txt.in = &in;
	txt.pagepos = 0;
	
	TopWindow win;
	win.Add(txt.SizePos());
	win.Run();
}
