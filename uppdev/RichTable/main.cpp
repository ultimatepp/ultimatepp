#include <CtrlLib/CtrlLib.h>
#include <RichText/RichText.h>

struct App : public TopWindow {
	Zoom         zoom;
	Rect         page;
	int          cursor, anchor;
	int          osell, oselh;
	RichText     text;
	int          table;
	Rect         tabsel;
	Label        hotpos, pos, rpos;
	RichText     clip;
	RichText::Formating formating;
	RichCaret    cursorc;
	RichPos      cursorp;
	StatusBar    status;

	ScrollBar sb;

	void Scroll();

	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void LeftDown(Point p, dword);
	virtual bool Key(dword key, int);
	virtual void MouseMove(Point p, dword);

	void   Finish();
	int    GetPosY(PageY py);

	typedef App CLASSNAME;

	App();
};

enum { PAGES = 300, PAGECY = 300 };

struct AppPageDraw : public PageDraw {
	virtual Draw& Page(int _page);
	virtual Draw& Info()                   { return w; }

	Draw&              w;
	int                page;
	Size               size;

	AppPageDraw(Draw& w) : w(w) { w.Begin(); page = -1; }
	~AppPageDraw() { w.End(); }
};

Draw& AppPageDraw::Page(int _page)
{
	if(page == _page) return w;
	page = _page;
	w.End();
	w.Clipoff(0, (size.cy + 2) * page, size.cx, size.cy);
	return w;
}

void Test(PageDraw& ww)
{
	for(int i = 0; i < 10; i++) {
		Draw& w = ww.Page(i);
		w.DrawText(0, 0, AsString(i), Arial(80));
	}
}

int  App::GetPosY(PageY py)
{
	return py.page * (zoom * page.Height()) + py.y * zoom + py.page * 2;
}

void App::Finish()
{
	if(anchor != cursor)
		cursor = text.AdjustCursor(anchor, cursor);
	RichCaret pr = text.GetCaret(cursor, page);
//	if(pr.table)
//		rpos = AsString(text.GetCellPos(pr.table, pr.row, pr.column));
//	else
	status = AsString(cursor) + ": " + AsString(text.GetRichPos(cursor));
	int h = pr.Height();
	SetCaret(RectC(pr.left * zoom, GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom - sb,
	               2, (pr.caretascent + pr.caretdescent) * zoom));
	PageY top, bottom;
//	RichPos tp = pr;
//	pos = AsString(tp);
	int sell = min(cursor, anchor);
	int selh = max(cursor, anchor);
	if(text.GetInvalid(top, bottom, page, sell, selh, osell, oselh)) {
		int y = GetPosY(top);
		int yy = GetPosY(bottom);
		Refresh(0, y, GetSize().cx, yy - y);
		text.Validate();
		osell = sell;
		oselh = selh;
	}
//	Refresh();
}

void App::LeftDown(Point p, dword)
{
	PageY py;
	int x = p.x / zoom;
	py.page = p.y / (PAGECY + 2);
	py.y = (p.y % (PAGECY + 2)) / zoom;
	anchor = cursor = text.GetPos(x, py, page);
	Finish();
}

void App::MouseMove(Point p, dword)
{
	PageY py;
	int x = p.x / zoom;
	py.page = p.y / (PAGECY + 2);
	py.y = (p.y % (PAGECY + 2)) / zoom;
	RichHotPos pos = text.GetHotPos(x, py, 50, page);
	hotpos = Format("table: %d, column: %d", pos.table, pos.column);
}

void App::Paint(Draw& w)
{
	Size sz = GetSize();

	w.DrawRect(sz, SWhite);

	w.Offset(0, -sb);
	AppPageDraw ww(w);
	ww.size = Size(GetSize().cx, PAGECY);
	for(int i = 0; i < PAGES; i++)
		w.DrawRect(0, (ww.size.cy + 2) * i - 2, sz.cx, 2, SLtBlue);

	PaintInfo pi;
	pi.zoom = Zoom(100, 1000);

	pi.top = PageY((int)sb / (ww.size.cy + 2), 0);
	pi.bottom = PageY(((int)sb + sz.cy) / (ww.size.cy + 2) + 1, 0);

	pi.usecache = true;

	pi.tablesel = table;
	pi.cells = tabsel;

	pi.sell = min(cursor, anchor);
	pi.selh = max(cursor, anchor);

	text.Paint(ww, page, pi);

	w.End();
}

void App::Layout()
{
	sb.SetPage(GetSize().cy);
	Refresh();
}

void App::Scroll()
{
	Refresh();
}

bool App::Key(dword key, int)
{
	RichPos tp = cursorp;
	if(key == K_LEFT && cursor > 0) {
		cursor--;
		anchor = cursor;
	}
	if(key == K_RIGHT && cursor < text.GetLength()) {
		cursor++;
		anchor = cursor;
	}
	if(key == K_SHIFT_RIGHT && cursor < text.GetLength()) {
		cursor++;
	}
	if(key == K_SHIFT_LEFT && cursor > 0) {
		cursor--;
	}
	if(key == K_CTRL_L) {
		table = tp.table;
		tabsel.left = tp.cell.x;
		tabsel.top = tp.cell.y;
	}
	if(key == K_CTRL_H) {
		table = tp.table;
		tabsel.right = tp.cell.x;
		tabsel.bottom = tp.cell.y;
	}
	if(key == K_DELETE)
		text.Remove(cursor, 1);
	if(key >= ' ' && key <= 65000 || key == 9) {
		RichPara p;
		p.Cat(WString(key, 1), RichPara::Format());
		RichText txt;
		txt.SetStyles(text.GetStyles());
		txt.Set(0, p, text.GetStyles());
		text.Insert(cursor++, txt);
		anchor = cursor;
	}
	if(key == K_CTRL_C) {
		clip = text.Copy(min(cursor, anchor), abs(cursor - anchor));
	}
	if(key == K_CTRL_V) {
		RichPara p;
		text.Insert(cursor++, clip);
	}
	if(key == K_CTRL_T) {
		RichPara p;
		p.Cat(WString('X', 1), RichPara::Format());
		RichText txt;
		txt.SetStyles(text.GetStyles());
		txt.Set(0, p, text.GetStyles());
		text.Insert(cursor++, txt);
		anchor = cursor;
	}
	if(key == K_CTRL_R) {
		formating = text.SaveFormat(min(cursor, anchor), abs(cursor - anchor));
		RichText::FormatInfo f;
		(Font&)f = Courier(250);
		f.charvalid = RichText::FACE|RichText::HEIGHT;
		f.paravalid = 0;
		text.ApplyFormatInfo(min(cursor, anchor), f, abs(cursor - anchor));
	}
	if(key == K_CTRL_Z) {
		text.RestoreFormat(min(cursor, anchor), formating);
	}
	Finish();
	return true;
}

App::App()
{
	AddFrame(sb);
	sb.SetTotal(30 * PAGES);
	sb.WhenScroll = THISBACK(Scroll);
	Sizeable().Zoomable();

	zoom = Zoom(100, 1000);
	page = Rect(0, 0, 1000 / zoom, PAGECY / zoom);

	text = ParseQTF("1&2&3&41&2&3&41&2&3&41&2&3&41&2&3&41&2&3&41&2&3&41&2&3&41&2&3&41&2&3&4");
/*
	RichTable table = GenTab(7, 7);
	{
		RichText h;
		h.SetPick(0, GenTab(3, 3));
		table.SetPick(0, 2, h);
	}
	{
		RichText h;
		h.SetPick(0, GenTab(3, 3));
		table.SetPick(2, 3, h);
	}

	RichTable::Format fmt = table.GetFormat();
	fmt.lm = 200;
	fmt.rm = 300;
	table.SetFormat(fmt);

	text.SetPick(4, table);
*/
	BackPaint();
	sb.SetLine(32);
//	sb = 320;

	cursor = anchor = 0;
	Maximize();
/*
	this->table = 1;
	tabsel.left = 0;
	tabsel.top = 0;
	tabsel.right = 2;
	tabsel.bottom = 3;
*/

/*
	hotpos.NoTransparent();
	Add(hotpos.BottomPos(0, 20).RightPos(0, 200));
	pos.NoTransparent();
	Add(pos.TopPos(0, 20).RightPos(0, 500));
	rpos.NoTransparent();
	Add(rpos.TopPos(20, 20).RightPos(0, 500));
*/

	AddFrame(status);

	text = ParseQTF(
		"1&2&3&4"
		"1&2&3&4"
		"++A||B||C--X||Y++"
		"{{1:1@3-1 A:: ::! C::a30/40 {{1:1:1 1:: 2:: 3}}::! E&E1:: F {{1:1:1~ A:: B:: C}}}}"
		"1&2&3&4"
		"1&2&3&4"
		"1&2&3&4"
		"1&2&3&4"
	);

	DUMP(AsQTF(text));
}

GUI_APP_MAIN
{
	Ctrl::ShowRepaint = 10;

	App p;
	p.Run();
	p.text = ParseQTF(AsQTF(p.text));
	DUMP(AsQTF(p.text));
//	p.Finish();
//	p.Run();
}
