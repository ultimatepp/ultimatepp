#ifndef _HexView_HexView_h
#define _HexView_HexView_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {

#define LAYOUTFILE <HexView/HexView.lay>
#include <CtrlCore/lay.h>

class HexViewInfo : public FrameBottom<Ctrl> {
	virtual void Paint(Draw& w);

private:
	Font  font;
	int64 pos;
	int   data[80];
	int   mode;
	bool  longmode;
	bool  empty = false;

	void  PrintValue(Draw& w, int x, int y, int bytes, bool be);

public:
	void  SetEmpty()                   { empty = true; Refresh(); }
	void  SetPos(int64 p, bool lm)     { pos = p; longmode = lm; empty = false; Refresh(); }
	void  Set(int i, int d)            { ASSERT(i >= 0 && i < 80); data[i] = d; Refresh(); }
	void  SetMode(int m);
	int   GetMode() const              { return mode; }

	HexViewInfo();
};

class HexView : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void LeftDown(Point p, dword);
	virtual bool Key(dword key, int);
	virtual void MouseWheel(Point, int zdelta, dword);
	virtual void RightDown(Point, dword);

private:
	Font      font;
	int       fcx3;
	Size      fsz;
	int       fixed;
	int       columns, rows, bytes;
	int       sbm;
	uint64    sc = 0;
	uint64    cursor = 0;
	uint64    total = 0;
	uint64    start = 0;
	byte      charset;
	ScrollBar sb;

	HexViewInfo info;

	WithHexGotoLayout<TopWindow> go;

	void      SetSb();
	void      Scroll();
	void      SetCharset(int chr);
	void      SetColumns(int x);
	void      SetInfo(int m);
	void      Goto();
	void      RefreshInfo();

public:
	virtual int Byte(int64 adr);

	Event<Bar&>          WhenBar;
	Event<const String&> WhenGoto;
	Event<>              WhenGotoDlg;

	void   ColumnsMenu(Bar& bar);
	void   CharsetMenu(Bar& bar);
	void   InfoMenu(Bar& bar);
	void   StdMenu(Bar& bar);

	void   StdGoto(const String& text);

	void   GotoAddHistory()            { go.text.AddHistory(); }

	bool   IsLongMode() const          { return total > 0xffffffff; }
	void   SetStart(uint64 start);
	void   SetTotal(uint64 _total);
	void   SetSc(uint64 address);
	int64  GetSc() const               { return sc; }
	void   SetCursor(uint64 address);
	int64  GetCursor() const           { return cursor; }

	void   SerializeSettings(Stream& s);

	HexView& SetFont(Font fnt);
	HexView& Charset(byte chrset)     { charset = chrset; Refresh(); return *this; }
	HexView& FixedColumns(int c = 0)  { fixed = c; Layout(); Refresh(); return *this; }
	HexView& InfoMode(int m = 1)      { info.SetMode(m); return *this; }

	typedef HexView CLASSNAME;

	HexView();
};

}

#endif
