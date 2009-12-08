#include "Uvs2.h"
#pragma hdrstop

#include "version.h"
#include "Uvs2Img.h"

#include "textdiff.h"

inline Color HistoryBg() { return Color(255, 255, 0); }

struct GapDisplayCls : public Display {
	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const
	{
		draw.DrawRect(rc, paper);
		Rect clip = rc.Deflated(3, 0);
		draw.Clip(clip);
		WString txt = IsString(v) ? v : StdConvert().Format(v);
		Font fnt = StdFont();
		int tcy = GetTLTextHeight(txt, fnt);
		DrawTLText(draw, rc.left + 5, rc.top + max((rc.Height() - tcy) / 2, 0), rc.Width(), txt, fnt, ink);
		draw.End();
	}
};

struct HistoryRowDisplayCls : public GapDisplayCls {
	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const
	{
		GapDisplayCls::Paint(draw, rc, v, ink, style & CURSOR ? paper : HistoryBg(), style);
	}
};

const Display& GLOBAL_V(GapDisplayCls, GapDisplay);
const Display& GLOBAL_V(HistoryRowDisplayCls, HistoryRowDisplay);

class TextCompareCtrl : public Ctrl {
public:
	typedef TextCompareCtrl CLASSNAME;
	TextCompareCtrl();

	virtual void   Paint(Draw& draw);
	virtual void   Layout();
	virtual void   MouseWheel(Point pt, int zdelta, dword keyflags);
	virtual void   MouseMove(Point pt, dword keyflags);
	virtual void   LeftDown(Point pt, dword keyflags);
	virtual void   LeftUp(Point pt, dword keyflags);
	virtual bool   Key(dword key, int repcnt);

	void           SetCount(int c);
	void           AddCount(int c);
	int            GetCount() const { return lines.GetCount(); }

	void           SetFont(Font f, Font nf);
	Font           GetFont() const { return font; }
	Font           GetNumberFont() const { return number_font; }

	void           NumberBgColor(Color bg)  { number_bg = bg; Refresh(); }
	Color          GetNumberBgColor() const { return number_bg; }

	void           ShowSb(bool ssb)         { scroll.ShowY(ssb); }
	void           HideSb()                 { ShowSb(false); }

	void           Gutter(int size)         { gutter_width = size; Refresh(); }
	void           NoGutter()               { gutter_width = 0; Refresh(); }

	void           TabSize(int t);
	int            GetTabSize() const { return tabsize; }

	void           Set(int line, String text, Color color, int number, int level);
	String         GetText(int line) const { return lines[line].text; }
	Color          GetColor(int line) const { return lines[line].color; }
	int            GetNumber(int line) const { return lines[line].number; }

	Point          GetPos() const;
	void           SetPos(Point pos);

	int            GetSb() const { return scroll.Get().y; }
	void           SetSb(int y)  { scroll.Set(0, y); }

	Callback       ScrollWhen(TextCompareCtrl& pair) { return THISBACK1(PairScroll, &pair); }

public:
	Callback       WhenScroll;

private:
	void           SelfScroll();
	void           PairScroll(TextCompareCtrl *ctrl);
	void           UpdateWidth();
	String         ExpandTabs(const char *line) const;
	int            MeasureLength(const char *line) const;

private:
	struct Line {
		Line() : number(Null), level(0) {}
		int    number;
		Color  color;
		String text;
		int    level;
	};
	Array<Line>    lines;
	int            maxwidth;
	ScrollBars     scroll;
	Font           font;
	Font           number_font;
	Color          number_bg;
	Color          gutter_fg;
	Color          gutter_bg;
	Size           letter;
	int            tabsize;
	int            number_width;
	int            number_yshift;
	int            gutter_width;
};

TextCompareCtrl::TextCompareCtrl()
{
	letter = Size(1, 1);
	number_width = 0;
	number_yshift = 0;
	number_bg = WhiteGray();
	SetFrame(FieldFrame());
	AddFrame(scroll);
	SetFont(Courier(14), Courier(10));
	scroll.NoAutoHide();
	scroll.WhenScroll = THISBACK(SelfScroll);
	maxwidth = 0;
	tabsize = 4;
	gutter_width = 0;
	gutter_bg = Color(151, 190, 239);
	gutter_fg = SGreen;
}

void TextCompareCtrl::LeftDown(Point pt, dword keyflags)
{
	if(pt.x < gutter_width || HasCapture())
	{
		if(!HasCapture())
			SetCapture();
		Size sz = GetSize();
		int line = (pt.y * lines.GetCount()) / sz.cy;
		int page_lines = sz.cy / letter.cy;
		scroll.SetY(line - page_lines / 2);
	}
	SetWantFocus();
}

void TextCompareCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
}

void TextCompareCtrl::MouseMove(Point pt, dword keyflags)
{
	if(HasCapture())
	{
		LeftDown(pt, keyflags);
	}
}


bool TextCompareCtrl::Key(dword key, int repcnt)
{
	Point pos = scroll, newpos = pos, page = scroll.GetPage();
	switch(key) {
		case K_LEFT:       newpos.x--; break;
		case K_RIGHT:      newpos.x++; break;
		case K_CTRL_LEFT:  newpos.x -= page.x >> 1; break;
		case K_CTRL_RIGHT: newpos.x += page.x >> 1; break;
		case K_UP:         newpos.y--; break;
		case K_DOWN:       newpos.y++; break;
		case K_PAGEUP:     newpos.y -= page.y; break;
		case K_PAGEDOWN:   newpos.y += page.y; break;
		case K_HOME:       newpos.x = 0; break;
		case K_END:        newpos.x = maxwidth - page.x; break;
		case K_CTRL_HOME:  newpos.y = 0; break;
		case K_CTRL_END:   newpos.y = lines.GetCount() - page.y; break;
		case K_F3: {
			bool found = false;
			int i = max(pos.y + 2, 0);
			while(i < lines.GetCount() && lines[i].level)
				i++;
			while(i < lines.GetCount())
				if(lines[i++].level) {
					newpos.y = i - 2;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		case K_SHIFT_F3: {
			bool found = false;
			int i = min(pos.y, lines.GetCount() - 1);
			while(i > 0 && lines[i].level)
				i--;
			while(i >= 0)
				if(lines[i--].level) {
					newpos.y = i;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		default: return false;
	}
	if(newpos != pos)
		scroll.Set(newpos);
	return true;
}

void TextCompareCtrl::Paint(Draw& draw)
{
	Point sc = scroll.Get();
	Size offset = (Size)sc * letter;
	Size sz = GetSize();

	int lcnt = lines.GetCount();
	int first_line = offset.cy / letter.cy;
	int last_line = min(idivceil(sz.cy + offset.cy, letter.cy), lines.GetCount() - 1);

	if(gutter_width > 0)
	{
		int t = 0, b = 0;
		for(int i = 0; i < lcnt; i++)
			if(lines[i].level > 1) {
				b = idivceil(sz.cy * i, lcnt);
				if(b >= t) {
					draw.DrawRect(0, t, gutter_width, b - t, gutter_bg);
					draw.DrawRect(0, b, gutter_width, 1, gutter_fg);
					t = b + 1;
				}
			}

		draw.DrawRect(0, t, gutter_width, sz.cy - t, gutter_bg);

		int total = letter.cy * lcnt;
		int page_height = (sz.cy * sz.cy) / total;
		int ty = max(0, (sz.cy * offset.cy) / total);
		int by = min(sz.cy, ty + page_height);
		draw.DrawRect(0, ty, gutter_width, 2, Black);
		draw.DrawRect(0, by - 2, gutter_width, 2, Black);
		draw.DrawRect(0, ty, 2, by - ty, Black);
		draw.DrawRect(gutter_width - 2, ty, 2, by - ty, Black);
	}

	Font ifont = Font(font).Italic();
	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		draw.DrawRect(gutter_width, y, number_width, letter.cy, number_bg);
		if(!IsNull(l.number))
			draw.DrawText(gutter_width, y + number_yshift, FormatInt(l.number), number_font, l.color);
	}
	draw.Clip(gutter_width + number_width, 0, sz.cx - gutter_width - number_width, sz.cy);
	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		draw.DrawRect(0, y, sz.cx, letter.cy, SWhite());
		draw.DrawText(gutter_width + number_width - offset.cx, y, ExpandTabs(l.text), l.level == 1 ? ifont : font, l.color);
	}
	draw.End();
	int lcy = lcnt * letter.cy - offset.cy;
	draw.DrawRect(gutter_width, lcy, sz.cx, sz.cy - lcy, SGray());
}

void TextCompareCtrl::TabSize(int t)
{
	tabsize = t;
	UpdateWidth();
	Layout();
}

void TextCompareCtrl::SetFont(Font f, Font nf)
{
	font = f;
	number_font = nf;
	FontInfo fi = f.Info();
	FontInfo ni = nf.Info();
	letter.cy = fi.GetHeight();
	letter.cx = fi.GetAveWidth();
	number_width = 5 * ni.GetAveWidth();
	number_yshift = (fi.GetHeight() - ni.GetHeight() + 2) >> 1;
	Layout();
}

void TextCompareCtrl::Layout()
{
	scroll.Set(scroll, (scroll.GetReducedViewSize() - Size(number_width + gutter_width, 0)) / letter, Size(maxwidth, lines.GetCount()));
	Refresh();
}

void TextCompareCtrl::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	scroll.WheelY(zdelta);
}

void TextCompareCtrl::SetCount(int c)
{
	bool rl = (c < lines.GetCount());
	lines.SetCount(c);
	if(rl)
		UpdateWidth();
	Layout();
}

void TextCompareCtrl::AddCount(int c)
{
	lines.InsertN(lines.GetCount(), c);
	Layout();
}

void TextCompareCtrl::Set(int line, String text, Color color, int number, int level)
{
	Line& l = lines.At(line);
	int tl = MeasureLength(text);
	int lt = MeasureLength(l.text);
	bool rl = (tl < lt && lt == maxwidth);
	l.number = number;
	l.text = text;
	l.color = color;
	l.level = level;
	if(rl)
		UpdateWidth();
	else if(tl > maxwidth) {
		maxwidth = tl;
		Layout();
	}
}

void TextCompareCtrl::SelfScroll()
{
	Refresh();
	WhenScroll();
}

void TextCompareCtrl::PairScroll(TextCompareCtrl *ctrl)
{
	scroll.Set(ctrl->scroll.Get());
}

void TextCompareCtrl::UpdateWidth()
{
	maxwidth = 0;
	for(int i = 0; i < lines.GetCount(); i++)
		maxwidth = max(maxwidth, MeasureLength(lines[i].text));
}

int TextCompareCtrl::MeasureLength(const char *text) const
{
	int pos = 0;
	while(*text)
		if(*text++ == '\t')
			pos += tabsize - pos % tabsize;
		else
			pos++;
	return pos;
}

String TextCompareCtrl::ExpandTabs(const char *text) const
{
	String out;
	for(char c; c = *text++;)
		if(c == '\t')
			out.Cat(' ', tabsize - out.GetLength() % tabsize);
		else
			out.Cat(c);
	return out;
}

Point TextCompareCtrl::GetPos() const
{
	Point pos(0, 0);
	int ltop = minmax(scroll.Get().y, 0, lines.GetCount() - 1);
	while(ltop >= 0 && IsNull(lines[ltop].number)) {
		ltop--;
		pos.y++;
	}
	if(ltop >= 0)
		pos.x = lines[ltop].number;
	return pos;
}

void TextCompareCtrl::SetPos(Point pos)
{
	int l = FindFieldIndex(lines, &Line::number,pos.x);
	if(l < 0)
		l = 0;
	SetSb(l + pos.y);
}

#define LAYOUTFILE <Uvs2/DlgFiles.lay>
#include <CtrlCore/lay.h>

#define TOOL(x) \
	void        COMBINE(Tool, x)(Bar& bar); \
	void        COMBINE(On, x)();

static Color MidGreen(0, 192, 0);

String FormatShort(Time tm, bool do_time)
{
	if(IsNull(tm))
		return Null;
	if(tm == FromUvsTime(INT_MAX))
		return t_("current");
	Time sys = GetSysTime();
	String out;
	out << int(tm.day) << '.' << int(tm.month) << '.';
	if(tm.year != sys.year)
		out << tm.year;
	if(do_time)
		out << ' ' << NFormat("%d:%02d:%02d", tm.hour, tm.minute, tm.second);
	return out;
}

class DlgDecompress : public WithDecompressLayout<TopWindow>
{
public:
	typedef DlgDecompress CLASSNAME;
	DlgDecompress();

	void Run(UvsDataBlock& data);
};

DlgDecompress::DlgDecompress()
{
	CtrlLayoutOKCancel(*this, t_("Decompress archive"));
	list.AutoHideSb();
	list.AddColumn(t_("File"));
	list.AddColumn(t_("Date / time"));
	list.AddColumn(t_("Length"));
	list.AddColumn(t_("Author"));
}

void DlgDecompress::Run(UvsDataBlock& data)
{
	for(int i = 0; i < data.download.GetCount(); i++) {
		FileInfo fi = data.download[i];
		String time = FormatUvsTime(fi.filetime);
		if(fi.deleted)
			time << t_(" (deleted)");
		list.Add(data.download.GetKey(i), time, data.data[i].GetLength(), fi.author);
	}
	list.SetCursor(0);
	if(TopWindow::Run() != IDOK)
		return;
	FileSel dirsel;
	if(dirsel.ExecuteSelectDir(t_("Decompress into folder"))) {
	}
}

static int CharFilterPatternSep(int c) { return c == ' ' || c == ';' || c == ',' ? c : 0; }

class TextColorDisplay : public Display
{
public:
	TextColorDisplay(Color fg = Null, Color bg = Null, Font font = StdFont()) : fg(fg), bg(bg), font(font) {}

	void         Ink(Color f)    { fg   = f; }
	void         Paper(Color b)  { bg   = b; }
	void         SetFont(Font f) { font = f; }

	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword style) const;

private:
	Color        fg, bg;
	Font         font;
};

static TextColorDisplay cd_local_edit(MidGreen, Null);
static TextColorDisplay cd_new(MidGreen, LtYellow);
static TextColorDisplay cd_deleted(LtGray, Null);
static TextColorDisplay cd_host_edit(LtBlue, Null);
static TextColorDisplay cd_excluded(Black, WhiteGray);
static TextColorDisplay cf_conflict(Black, Color(255, 192, 192));

void TextColorDisplay::Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword style) const
{
	if(!(style & Display::CURSOR)) {
		i = Nvl(fg, i);
		p = Nvl(bg, p);
	}
	draw.DrawRect(rc, p);
	draw.DrawText(rc.left, rc.top, StdFormat(v), font, i);
}

enum
{
	OP_DOWNLOAD_UPLOAD,
	OP_DOWNLOAD,
	OP_UPLOAD,
};

class DlgSyncCheck : public WithSyncCheckLayout<TopWindow>
{
public:
	typedef DlgSyncCheck CLASSNAME;
	DlgSyncCheck();

	bool          Run(const ComplexDirInfo& cdi, const UvsJob& job,
		int& operation, bool& archive);

private:
	void          UpdateList(bool first = false);
//	Index<int>    GetSel() const;

	void          DoSort(int sort_method);
	void          DoSortName();
//	void          DoCheck();

private:
//	enum { SORT_ACTION, SORT_PATH, SORT_FILE, SORT_EXT, SORT_TIME, SORT_SIZE };
	enum { L_INDEX, L_ACTION, L_PATH, L_FILE, L_TIME, L_SIZE, /* L_CHECK, */ L_COUNT };
//	ArrayOptionEx check;
	int           sort;
	const ComplexDirInfo *cinfo;
	const UvsJob         *job;
};

DlgSyncCheck::DlgSyncCheck()
{
	sort = SORT_ACTION;
	cinfo = NULL;
	job = NULL;

	CtrlLayoutOKCancel(*this, t_("Synchronization"));
	Sizeable().Zoomable();
	list.AutoHideSb();
	ASSERT(list.GetIndexCount() == L_INDEX);
	list.AddIndex();
	ASSERT(list.GetIndexCount() == L_ACTION);
	list.AddColumn(t_("Action"), 3).SetDisplay(CenteredImageDisplay()).HeaderTab().WhenAction = THISBACK1(DoSort, SORT_ACTION);
	ASSERT(list.GetIndexCount() == L_PATH);
	list.AddColumn(t_("Folder"), 15).HeaderTab().WhenAction = THISBACK1(DoSort, SORT_PATH);
	ASSERT(list.GetIndexCount() == L_FILE);
	list.AddColumn(t_("File"), 15).HeaderTab().WhenAction = THISBACK(DoSortName);
	ASSERT(list.GetIndexCount() == L_TIME);
	list.AddColumn(t_("Modified"), 15).HeaderTab().WhenAction = THISBACK1(DoSort, SORT_TIME);
	ASSERT(list.GetIndexCount() == L_SIZE);
	list.AddColumn(t_("Length"), 10).HeaderTab().WhenAction = THISBACK1(DoSort, SORT_SIZE);
//	ASSERT(list.GetIndexCount() == L_CHECK);
//	check.AddColumn(list, "Sync", 3).HeaderTab().WhenAction = THISBACK(DoCheck);
	ASSERT(list.GetIndexCount() == L_COUNT);
	operation.Add(OP_DOWNLOAD_UPLOAD, t_("download & upload changes"));
	operation.Add(OP_DOWNLOAD, t_("download only"));
//	operation.Add(OP_UPLOAD, "jen uložit zmìny");
	operation <<= OP_DOWNLOAD_UPLOAD;
	archive.Hide();
}

bool DlgSyncCheck::Run(const ComplexDirInfo& cdi_, const UvsJob& job_, int& op_, bool& arch_)
{
	cinfo = &cdi_;
	job = &job_;
	operation <<= op_;
	archive = arch_;
	UpdateList(true);
	if(list.GetCount() == 0 || TopWindow::Run() == IDOK) {
		op_ = ~operation;
		arch_ = archive;
		return true;
	}
	return false;
}

/*
Index<int> DlgSyncCheck::GetSel() const
{
	Index<int> out;
	for(int i = 0; i < list.GetCount(); i++)
		if((int)list.Get(i, L_CHECK))
			out.Add(list.Get(i, L_INDEX));
	return out;
}
*/

void DlgSyncCheck::DoSort(int sort_method)
{
	sort = sort_method;
	UpdateList();
}

void DlgSyncCheck::DoSortName()
{
	sort = (sort == SORT_NAME ? SORT_EXT : SORT_NAME);
	UpdateList();
}

/*
void DlgSyncCheck::DoCheck()
{
	bool oc = false;
	if(list.IsCursor())
		oc = (int)list.Get(L_CHECK) > 0;
	else if(list.GetCount() > 0)
		oc = (int)list.Get(0, L_CHECK) > 0;
	int nc = oc ? 0 : 1;
	for(int i = 0; i < list.GetCount(); i++)
		list.Set(i, L_CHECK, nc);
}
*/

void DlgSyncCheck::UpdateList(bool first)
{
	Vector<String> order;
	Vector<int> index;
	int i;
	for(i = 0; i < cinfo -> GetCount(); i++) {
		const ComplexFileInfo& cfi = (*cinfo)[i];
		bool hedit = (cfi.IsHostEdit() && !cfi.ignorehost);
		if(!job -> IsExcluded(cfi.path) && (hedit || cfi.IsTreeEdit() || cfi.IsUpload())) {
			order.Add(cfi.GetSortKey(sort, hedit,
				cfi.IsTreeConflict() || hedit ? cfi.host_size : cfi.tree_size));
			index.Add(i);
		}
	}
	IndexSort(order, index, StdLess<String>());
//	Index<int> sel = GetSel();
	list.SetCount(index.GetCount());
	for(i = 0; i < index.GetCount(); i++) {
		int x = index[i];
		const ComplexFileInfo& cfi = (*cinfo)[x];
		list.Set(i, L_INDEX, x);
		Image action;
		int time;
		int64 size;
		if(!cfi.IsHostEdit()) {
			action = cfi.IsTreeDeleted() ? Uvs2Img::flag_tree_deleted() : Uvs2Img::flag_upload();
			time = cfi.tree_time;
			size = cfi.tree_size;
		}
		else if(!cfi.IsTreeEdit() && !cfi.IsTimeConflict()) {
			action = cfi.IsHostDeleted() ? Uvs2Img::flag_host_deleted() : Uvs2Img::flag_download();
			time = cfi.download.filetime;
			size = cfi.host_size;
		}
		else {
			action = (cfi.noconflict ? Uvs2Img::flag_noconflict()
				: cfi.IsTimeConflict() ? Uvs2Img::flag_timeconflict() : Uvs2Img::flag_conflict());
			time = max(cfi.tree_time, cfi.download.filetime);
			size = cfi.host_size;
		}
		list.Set(i, L_ACTION, action);
		String dir, file;
		SplitPath(cfi.path, dir, file);
		list.Set(i, L_PATH, dir);
		list.Set(i, L_FILE, file);
		list.Set(i, L_TIME, FormatUvsTime(time));
		list.Set(i, L_SIZE, size);
//		list.Set(i, L_CHECK, first || sel.Find(x) >= 0 ? 1 : 0);
	}
}

class DlgFiles : public TopWindow
{
public:
	typedef DlgFiles CLASSNAME;
	DlgFiles();

	void                              Run();

	virtual void                      Serialize(Stream& stream);
	virtual bool                      HotKey(dword key);

	bool                              OpenFile();
	bool                              OpenFile(String filename);
	bool                              CheckFileSave();

	bool                              SaveFile();
	bool                              SaveFileAs();

	void                              UpdateTitle();
	void                              UpdateTree();
	void                              SetTree();

private:
	void                              LayoutTable();
	void                              Rescan()    { menubar.Set(THISBACK(ToolMain)); toolbar.Set(THISBACK(ToolMain)); }
	void                              OnFileLRU(const String& fn);
	void                              OnTree();
	void                              UpdateDiff();
	void                              UpdateComp();
	void                              OnFileCursor();

	void                              AddTree(int index, bool found);

	LineEdit&                         ConCtrl(bool arch_con) { return arch_con ? archive_console : console; }
	void                              ConClear(bool arch_con = false);
	void                              ConAdd(const char *text, bool arch_con = false);
	void                              ConLine(const char *text, bool arch_con = false);

	TOOL(Main)

	TOOL(File)
		TOOL(FileNew)
		TOOL(FileOpen)
		TOOL(FileSave)
		TOOL(FileSaveAs)
		TOOL(FileSetup)
		TOOL(FileDecompress)
		TOOL(FileCompress)
		TOOL(FileQuit)

	TOOL(Sync)
//		TOOL(SyncOpen)
		TOOL(SyncReload)
		TOOL(SyncExec)
		TOOL(SyncImport)
		TOOL(SyncExport)
		TOOL(SyncBatch)
		TOOL(SyncCheckLocal)
		TOOL(SyncAdjustTime)
//		TOOL(SyncDownload)
//		TOOL(SyncUpload)
		TOOL(SyncStop)

	TOOL(View)
		TOOL(ViewTree)
		TOOL(ViewDiff)
		TOOL(ViewAll)
		TOOL(ViewFileDiff)
		TOOL(ViewFileComp)
		TOOL(ViewSortAction)
		TOOL(ViewSortPath)
		TOOL(ViewSortName)
		TOOL(ViewSortExt)
		TOOL(ViewSortTime)
		TOOL(ViewSortSize)
		TOOL(ViewFind)

	TOOL(Help)
		TOOL(HelpAbout)

	TOOL(List)
		TOOL(ListAdd)
		TOOL(ListRemove)

	TOOL(History)
		TOOL(HistoryUp)
		TOOL(HistoryDown)
		TOOL(HistoryCompUp)
		TOOL(HistoryCompDown)
		TOOL(HistoryCompSet)
		TOOL(HistoryBothUp)
		TOOL(HistoryBothDown)
		TOOL(HistoryRestore)

	bool                              IsUpload(const ComplexFileInfo& f);

	String                            GetHistory(int index, int& time);
	String                            GetHistory(int& time);
	void                              UpdateHistory(int index);

	void                              OnFileList();
	void                              OnOpenArchiveConsole();

	bool                              OnLoadGateHeaders(String data);
	bool                              OnLoadGateAll(String data);
	bool                              OnDecompressGate(int len, int total);
	bool                              OnFtpSaveGate(int, int);
	bool                              OnFtpLoadGate(int, int);
	bool                              OnSaveGate(int len, int total);
	bool                              OnTreeGate(int count, int total);
	bool                              OnFtpProgress(int, int);
	void                              OnWaitConnect();

	void                              DoViewSortName();
	void                              DoViewSort(int sort_mode);

	void                              DoSync();
	void                              DoSyncMerge(String repository);
	void                              DoSyncBackup(String backupcmd, String path);
	void                              DoSyncExport();

	bool                              SyncProject(bool confirm);
	void                              OpenFTP(bool retry = false);
	void                              LogLine(String line);
	int                               GetLocalBlock();
	UvsHostIndex                      GetHostIndex();
	void                              LockHost();
	void                              SaveLockFile();
	Array<UvsDataBlock>               Download(int first, int last);
	void                              RemoveLocal();
	void                              SaveIndexFile(const UvsHostIndex& hostindex);

	void                              SaveRetry(String file, String data);
	String                            LoadRetry(String file, bool allow_void);
	void                              DeleteRetry(String file);

	void                              RemoveDeep(String path, bool base = true);

	void                              SaveLocalDir(int idx);

	bool                              IsSyncTime();
	bool                              IsCanceled();
	int                               GetListIndex() const;

	void                              SetProgressSize(int size, int total, int start_msecs);
	void                              SetProgressCount(int count, int total);

	void                              UpdateCompFont();

	void                              BeginSync();
	void                              EndSync();

	void                              AddLRU(String fn);
	void                              RemoveLRU(String fn);

private:
	enum { RETRIES = 5 };

	FileSel                           jobfs;

	ComplexDirInfo                    cinfo;
	Index<String>                     last_download;

	class FileView : public LineEdit {
	public:
		virtual void SelectionChanged() { WhenSelection(); }

	public:
		Callback     WhenSelection;
	};

	Splitter                          splitter;
	Splitter                          hsplitter;
	Splitter                          psplitter;
	Splitter                          console_splitter;
	TreeCtrl                          tree;
	Ctrl                              view;
	ArrayCtrl                         history;
	ArrayCtrl                         filelist;
	FileView                          fileview;
	RichTextCtrl                      diffview;
	Splitter                          compview;
	TextCompareCtrl                   compleft;
	TextCompareCtrl                   compright;
	int                               compleft_ver;
	int                               compright_ver;
//	Label                             comp_ver_label;
//	DropList                          comp_version;
	Label                             comp_font_label;
	DropList                          comp_font;
	Button                            open_archive_console;
	LineEdit                          console;
	LineEdit                          archive_console;
	MenuBar                           menubar;
	ToolBar                           toolbar;
	Label                             file_cursor;
	LRUList                           lrulist;
	StatusBar                         statusbar;
	ProgressInfo                      progress;
	FtpClient                         ftp_client;
	enum {
		T_LOCAL    = 0x01,
		T_HOST     = 0x02,
		T_CONFLICT = 0x04,
		T_FOUND    = 0x08,
	};
	enum { L_INDEX, L_FLAGS, L_PATH, L_FILE, L_TIME, L_SIZE, L_COUNT };
	enum { D_FILE, D_DIFF, D_COMP };
//	enum { SORT_PATH, SORT_NAME, SORT_EXT, SORT_TIME, SORT_SIZE, SORT_ACTION };

	bool                              view_diff;
	bool                              view_all;
	int                               display_style;
	int                               view_sort;

	bool                              view_find;
	bool                              view_find_diff;
	Time                              view_find_time;

	int                               file_index;
	int                               comp_history_index;
	int                               load_start_msecs;
	int                               decompress_start_msecs;
	int                               save_start_msecs;

	String                            jobpath;
	bool                              jobdirty;
	UvsJob                            job;

	bool                              jobsync;
	bool                              jobcancel;
	int                               sync_time;
	enum { LOADSIZE = 1000 };

	Vector<String>                    sync_projects;
	String                            recent_compress;
	Vector<String>                    recent_projects;
	bool                              recent_confirm;
	int                               connect_time;
	int                               ftp_state;
};

void RunDlgFiles() { DlgFiles().Run(); }

DlgFiles::DlgFiles()
{
	ftp_state = Null;

	Icon(Uvs2Img::app_small(), Uvs2Img::app_large());
	Sizeable().Zoomable();
	WhenClose = THISBACK(OnFileQuit);

	view_diff = false;
	view_all = false;
	display_style = D_FILE;
	view_sort = SORT_NAME;

	view_find = false;
	view_find_diff = false;

	file_index = -1;
	comp_history_index = -1;
	recent_confirm = false;

	jobdirty = false;
	jobsync = false;

	jobfs.Type(t_("UVS Job"), "*.uvs");
	jobfs.DefaultExt("uvs");
	jobfs.AllFilesType();

	menubar << file_cursor.RightPos(1, 100).VSizePos(1, 1).SetFrame(InsetFrame());
	fileview.WhenSelection = THISBACK(OnFileCursor);
	AddFrame(menubar);
	AddFrame(toolbar);
	AddFrame(statusbar);
	Title(NFormat(t_("UVS version %s, %`"), UVS2_VERSION, UVS2_DATE));
	Add(splitter.SizePos());
	splitter.Horz(hsplitter, psplitter);
	splitter.SetPos(3000);
	hsplitter.Vert(tree, history);
	hsplitter.SetPos(6000);
	history.AutoHideSb();
	history.AddColumn(t_("Date/time")).Margin(0);
	history.AddColumn(t_("Author")).Margin(0);
	history.WhenEnterRow = THISBACK(OnHistory);
	history.WhenBar = THISBACK(ToolHistory);
	history.WhenLeftDouble = THISBACK(OnHistoryCompSet);
	psplitter.Vert(view, console_splitter);
	console.SetColor(LineEdit::PAPER_READONLY, SWhite());
	archive_console.SetColor(LineEdit::PAPER_READONLY, SWhite());
	console_splitter.Horz(console, archive_console);
	console.Add(open_archive_console.TopPos(0, 17).RightPos(0, 17));
	open_archive_console.SetImage(CtrlImg::right_arrow());
	open_archive_console <<= THISBACK(OnOpenArchiveConsole);
	console_splitter.Zoom(0);
	console.SetReadOnly();
	console.SetFont(Courier(12));
	archive_console.SetReadOnly();
	archive_console.SetFont(Courier(12));
	psplitter.SetPos(6000);
	psplitter.Zoom(0);
	compleft.Gutter(30);
	compright.NoGutter();
	compview.Horz(compleft, compright);
	compleft.WhenScroll = compright.ScrollWhen(compleft);
	compright.HideSb();
	compright.WhenScroll = compleft.ScrollWhen(compright);
	compright.NumberBgColor(HistoryBg());
	compleft_ver = compright_ver = -1;
//	comp_ver_label.VSizePos(2, 2).SetLabel(t_("Compare version:  "));
//	comp_version.VSizePos(2, 2) <<= THISBACK(UpdateComp);
	comp_font_label.SetLabel(t_("    Font:  ")).VSizePos(2, 2);
	comp_font.VSizePos(2, 2) <<= THISBACK(UpdateCompFont);
	for(int i = 8; i < 20; i++)
		comp_font.Add(i);
	comp_font <<= 14;
	view << filelist.SizePos() << fileview.SizePos() << diffview.SizePos() << compview.SizePos();
	fileview.SetFont(Courier(14));
	fileview.SetReadOnly();
	diffview.WantFocus();
	diffview.SetFrame(InsetFrame());
	diffview.Margins(10).Background(SWhite());

	progress.Text(t_("Synchronizing: "));
	statusbar.Sync();

//	lrulist.WhenSelect = THISBACK(OnFileLRU);

	tree <<= THISBACK(OnTree);

	LayoutTable();
	Rescan();
	UpdateCompFont();
}

void DlgFiles::Serialize(Stream& stream)
{
	int version = 3;
	stream / version;
	if(stream.IsError() || version < 1 || version > 3) {
		stream.SetError();
		return;
	}
	stream % lrulist;
	SerializePlacement(stream);
	stream % splitter;
	bool dummy;
	stream.Pack(view_diff, view_all, dummy);
	stream / view_sort;
	stream % recent_compress % recent_projects;
	stream % recent_confirm;
	stream % sync_projects;
	if(version >= 2)
		stream % display_style;
	if(version >= 3) {
		stream % comp_font;
		if(stream.IsLoading())
			UpdateCompFont();
	}
}

bool DlgFiles::HotKey(dword key)
{
	if(key == K_ESCAPE) {
		psplitter.Zoom(psplitter.GetZoom() < 0 ? 0 : -1);
		return true;
	}
	return TopWindow::HotKey(key);
}

void DlgFiles::Run()
{
	LoadFromGlobal(*this, "DlgFiles");
	LayoutTable();
	UpdateTree();
	Rescan();
	if(splitter.GetZoom() < 0)
		ActiveFocus(tree);
	else
		ActiveFocus(filelist);
	OnTree();
	Open();
	OnSyncBatch();
	TopWindow::Run();
	StoreToGlobal(*this, "DlgFiles");
}

void DlgFiles::LayoutTable()
{
	filelist.Reset();
	filelist.AutoHideSb();
	ASSERT(filelist.GetIndexCount() == L_INDEX);
	filelist.AddIndex();
	ASSERT(filelist.GetIndexCount() == L_FLAGS);
	filelist.AddColumn(t_("Action"), 3).SetDisplay(CenteredImageDisplay()).HeaderTab().WhenAction = THISBACK1(DoViewSort, SORT_ACTION);
	ASSERT(filelist.GetIndexCount() == L_PATH);
	bool is_folder = (splitter.GetZoom() == 1);
	if(is_folder)
		filelist.AddColumn(t_("Folder"), 15).HeaderTab().WhenAction = THISBACK1(DoViewSort, SORT_PATH);
	else
		filelist.AddIndex();
	ASSERT(filelist.GetIndexCount() == L_FILE);
	filelist.AddColumn(t_("Name"), 15).HeaderTab().WhenAction = THISBACK(DoViewSortName);
	ASSERT(filelist.GetIndexCount() == L_TIME);
	filelist.AddColumn(t_("Modified"), 15).HeaderTab().WhenAction = THISBACK1(DoViewSort, SORT_TIME);
	ASSERT(filelist.GetIndexCount() == L_SIZE);
	filelist.AddColumn(t_("Length"), 10).HeaderTab().WhenAction = THISBACK1(DoViewSort, SORT_SIZE);
	ASSERT(filelist.GetIndexCount() == L_COUNT);
	filelist.WhenBar = THISBACK(ToolList);
	filelist.WhenEnterRow = THISBACK(OnFileList);
}

void DlgFiles::ToolMain(Bar& bar)
{
	if(!jobsync)
		bar.Add(t_("File"), THISBACK(ToolFile))
			.Help(t_("Project file management"));
	bar.Add(t_("Sync"), THISBACK(ToolSync))
		.Help(t_("Synchronizing files with remote server"));
	bar.Add(t_("View"), THISBACK(ToolView))
		.Help(t_("File list view options"));
	bar.Add(t_("Help"), THISBACK(ToolHelp))
		.Help(t_("Application information"));
}

void DlgFiles::ToolFile(Bar& bar)
{
	ToolFileNew(bar);
	ToolFileOpen(bar);
	ToolFileSave(bar);
	ToolFileSaveAs(bar);
//	lrulist(bar);
	bar.Separator();
	ToolFileSetup(bar);
	bar.MenuSeparator();
	ToolFileCompress(bar);
	ToolFileDecompress(bar);
	bar.MenuSeparator();
	ToolFileQuit(bar);
	if(bar.IsScanKeys())
		ToolHistory(bar);
}

void DlgFiles::ToolFileNew(Bar& bar)
{
	bar.Add(t_("New"), CtrlImg::new_doc(), THISBACK(OnFileNew))
		.Key(K_CTRL_N)
		.Help(t_("Create new project"));
}

void DlgFiles::OnFileNew()
{
}

void DlgFiles::ToolFileOpen(Bar& bar)
{
	bar.Add(t_("Open"), CtrlImg::open(), THISBACK(OnFileOpen))
		.Key(K_CTRL_O)
		.Help(t_("Open an existing project"));
}

void DlgFiles::OnFileOpen()
{
	ConClear();
	ConClear(true);
	OpenFile();
}

bool DlgFiles::OpenFile()
{
	if(!CheckFileSave())
		return false;
	if(!jobfs.ExecuteOpen(t_("Open file")))
		return false;
	if(OpenFile(~jobfs))
		return true;
	Exclamation(NFormat(t_("Error opening file [* \1%s\1]."), ~jobfs));
	return false;
}

void DlgFiles::OnFileLRU(const String& fn)
{
	String s = fn;
	if(CheckFileSave()) {
		ConClear();
		ConClear(true);
		if(!OpenFile(s))
			Exclamation(NFormat(t_("Error opening project [* \1%s\1]."), s));
	}
}

bool DlgFiles::OpenFile(String fn_)
{
	String filename = fn_;
	UvsJob new_job;
	String df = LoadFile(filename);
	if(df.IsEmpty() || !new_job.Read(df)) {
		RemoveLRU(filename);
		return false;
	}
	AddLRU(jobpath = filename);
	job = new_job;
	last_download.Clear();
	UpdateTitle();
	UpdateTree();
	tree.Open(0);
	jobdirty = false;
	Rescan();
	return true;
}

void DlgFiles::ToolFileSave(Bar& bar)
{
	bar.Add(t_("Save"), CtrlImg::save(), THISBACK(OnFileSave))
		.Key(K_CTRL_S)
		.Help(t_("Save project definition to the disk"));
}

void DlgFiles::OnFileSave()
{
	SaveFile();
}

void DlgFiles::ToolFileSaveAs(Bar& bar)
{
	bar.Add(t_("Save &as..."), CtrlImg::save_as(), THISBACK(OnFileSaveAs))
		.Help(t_("Save project with a different name"));
}

void DlgFiles::OnFileSaveAs()
{
	SaveFileAs();
}

void DlgFiles::ToolFileSetup(Bar& bar)
{
	bar.Add(t_("Parameters"), Uvs2Img::BarJobEdit(), THISBACK(OnFileSetup))
		.Key(K_CTRL_J)
		.Help(t_("Display / edit project parameters"));
}

void DlgFiles::OnFileSetup()
{
	if(job.Edit()) {
		jobdirty = true;
		UpdateTitle();
		UpdateTree();
	}
}

void DlgFiles::ToolFileCompress(Bar& bar)
{
}

void DlgFiles::OnFileCompress()
{
}

void DlgFiles::ToolFileDecompress(Bar& bar)
{
	bar.Add(t_("Decompress file"), THISBACK(OnFileDecompress))
		.Help(t_("Decompress a selected UVS archive data block"));
}

void DlgFiles::OnFileDecompress()
{
	FileSelector fsel;
	fsel <<= recent_compress;
	fsel.Type(t_("Data blocks (*.udb)"), "*.udb");
	fsel.DefaultExt("udb");
	if(fsel.ExecuteOpen(t_("Open data block"))) {
		try {
			String data = LoadFile(~fsel);
			if(IsNull(data))
				throw Exc(t_("File not found."));
			Progress progress(t_("Unpacking archive"));
			UvsDataBlock block = UvsDataBlock::Decompress(data, false, progress);
			DlgDecompress().Run(block);
		}
		catch(Exc e) {
			Exclamation(NFormat(t_("Error reading file [* \1%s\1]: %s"), ~fsel, e));
			return;
		}
	}
}

void DlgFiles::ToolFileQuit(Bar& bar)
{
	bar.Add(t_("Quit"), THISBACK(OnFileQuit))
		.Help(t_("Quit the application"));
}

void DlgFiles::OnFileQuit()
{
	if(jobsync)
		jobcancel = true;
	else if(CheckFileSave())
		Break(IDOK);
}

void DlgFiles::ToolSync(Bar& bar)
{
//	ToolSyncOpen(bar);
	if(jobsync)
		ToolSyncStop(bar);
	else {
		ToolSyncReload(bar);
		ToolSyncCheckLocal(bar);
		ToolSyncAdjustTime(bar);
		bar.Separator();
		ToolSyncExec(bar);
		ToolSyncBatch(bar);
		bar.MenuSeparator();
		ToolSyncImport(bar);
		ToolSyncExport(bar);
	}
//	ToolSyncDownload(bar);
//	ToolSyncUpload(bar);
}

/*
void DlgFiles::ToolSyncOpen(Bar& bar)
{
	bar.Add("Stáhnout zmìny", Uvs2Img::sync_open(), THISBACK(OnSyncOpen))
		.Help("Naèíst adresáø s popisem zmìn v projektu");
}
*/

/*
void DlgFiles::OnSyncOpen()
{
	try {
		DlgSyncProgress progress("Stahování zmìn");
		FtpClient ftp;
		progress.SetText(String().Cat() << "Otevírám server '" << job.host << "'");
		OpenFTP(ftp);
		progress.SetText("Stahuji index posledního bloku");
		int hblock = GetHostBlock(ftp);
		if(IsNull(hblock))
			throw Exc("Vzdálený archiv je prázdný.");
		int lblock = GetLocalBlock();
		if(!IsNull(lblock)) {
			if(lblock > hblock)
				throw Exc(String().Cat() << "Index posledního bloku v lokálním archivu (" << lblock
				<< ") je vìtší než na serveru (" << hblock << ").");
			if(lblock == hblock)
				throw Exc(String().Cat() << "Vzdálený archiv neobsahuje žádné nové zmìny.");
		}
		Array<UvsDataBlock> blocks = Download(ftp, Nvl(lblock, 0) + 1, hblock, true, progress);
		for(int i = 0; i < blocks.GetCount(); i++)
			SetHost(cinfo, blocks[i].dirinfo);
		UpdateTree();
	}
	catch(Exc e) {
		ShowExc(e);
	}
}
*/

void DlgFiles::ToolSyncReload(Bar& bar)
{
	bar.Add(t_("Refresh"), Uvs2Img::sync_reload(), THISBACK(OnSyncReload))
		.Key(K_CTRL_T)
		.Help(t_("Refresh status of project files"));
}

void DlgFiles::OnSyncReload()
{
	UpdateTree();
}

void DlgFiles::ToolSyncCheckLocal(Bar& bar)
{
	bar.Add(t_("Check local archive"), THISBACK(OnSyncCheckLocal))
		.Help(t_("Check integrity of source file mirrors in the local archive"));
}

static bool CheckRepair(String qtf, bool& autorepair)
{
	if(autorepair)
		return true;
	WithCheckRepairLayout<TopWindow> repdlg;
	CtrlLayoutOKCancel(repdlg, t_("Correctable error"));
	repdlg.WhenClose = repdlg.Breaker(IDCANCEL);
	repdlg.text.SetQTF(qtf);
	repdlg.all <<= repdlg.Breaker(IDYES);
	repdlg.skip <<= repdlg.Breaker(IDRETRY);
	repdlg.ActiveFocus(repdlg.ok);
	switch(repdlg.Run()) {
	case IDOK: // repair
		return true;
	case IDYES: // repair all
		autorepair = true;
		return true;
	case IDRETRY: // skip
		return false;
	default:
		throw AbortExc();
	}
}

void DlgFiles::OnSyncCheckLocal()
{
	BeginSync();
	int errors = 0;
	bool auto_repair_lmiss = false;
	bool auto_repair_ldiff = false;
	bool auto_repair_author = false;
	bool auto_repair_rtime = false;
	bool auto_repair_syntax = false;
	bool auto_repair_dummy = false;
	try {
		for(int i = 0; i < cinfo.GetCount() && !jobcancel; i++) {
			if(IsSyncTime())
				SetProgressCount(i, cinfo.GetCount());
			const ComplexFileInfo& finfo = cinfo[i];
			if(job.IsExcluded(finfo.path))
				continue;
			if(finfo.IsTree() && !IsNull(finfo.local.filetime)) {
				String tfn = job.GetTreePath(finfo.path);
				String lfn = job.GetLocalPath(finfo.path), vfn = lfn + ".$v";
				String tfile = LoadFile(tfn), lfile = LoadFile(lfn), vfile = LoadFile(vfn);
				if(tfile.IsVoid()) {
					errors++;
					ConLine(NFormat(t_("File %s not found."), tfn));
				}
				else if(IsNull(tfile)) {
					errors++;
					ConLine(NFormat(t_("File %s is empty."), tfn));
				}
				bool repair = false;
				if(lfile.IsVoid()) {
					errors++;
					ConLine(NFormat(t_("File %s not found."), lfn));
					if(!IsNull(tfile))
						repair = CheckRepair(NFormat(t_("Source file [* \1%s\1] not found in local archive. Copy current version into local archive?"), finfo.path), auto_repair_lmiss);
				}
				else if(!finfo.IsTreeEdit() && tfile != lfile) {
					errors++;
					ConLine(NFormat(t_("File %s doesn't match current version %s."), lfn, tfn));
					if(!IsNull(tfile))
						repair = CheckRepair(NFormat(t_("File [* \1%s\1] in the source tree differs from the local archive although it has not been marked as changed. Ovewrite archive with current version?"),
							finfo.path), auto_repair_ldiff);
				}
				if(repair) {
					if(!SaveFileBackup(lfn, tfile))
						throw Exc(NFormat(t_("Error saving file '%s'."), lfn));
					lfile = tfile;
					if(!IsNull(lfile)) {
						String diff = Diff(tfile, lfile);
						diff << "#" << ToUvsTime(GetSysTime()) << " "
						<< AsCString(job.author) << "\n" << vfile;
						if(!SaveFileBackup(vfn, diff))
							throw Exc(NFormat(t_("Error saving file '%s'."), vfn));
						vfile = diff;
					}
					else {
						if(!IsNull(vfile))
							ConLine(NFormat(t_("Removing invalid history '%s'."), vfn));
						vfile = Null;
						String tfn = vfn + ".$old";
						DeleteFile(tfn);
						FileMove(vfn, tfn);
					}
				}
				if(!job.IsBinary(finfo.path) && !IsNull(lfile) && !IsNull(vfile)) {
					bool repv = false;
					String newv;
					const char *p = vfile;
					int line = 1;
					int lft = finfo.local.filetime;
					for(;;) {
						int vline = line;
						const char *b = p;
						while(*p && *p != '#') {
							while(*p && *p++ != '\n')
								;
							line++;
						}
						if(*p != '#') {
							if(b != p) {
								errors++;
								ConLine(NFormat(t_("Error in history %s, lines %d-%d: data not terminated with version header #..."), vfn, vline, line));
							}
							break;
						}
						String diff(b, p);
						int ft;
						String au;
						b = p;
						while(*p && *p++ != '\n')
							;
						String sep(b, p);
						int errpos;
						String ofile = PrevVersion(lfile, diff, &errpos);
						if(errpos >= 0) {
							errors++;
							ConLine(NFormat(t_("Error in history %s, line %d: invalid update record"),
								vfn, vline + errpos - 1));
							repv = false;
							break;
						}
						if(lfile == ofile) {
							ConLine(NFormat(t_("Minor error in history %s, line %d: dummy update record (file not modified)"), vfn, line));
							if(CheckRepair(NFormat(t_("Minor error in history of file [* \1%s\1]: dummy update record, [* \1%s, %s\1]; file not modified. Remove this record?"), lfn, FormatUvsTime(ft), au), auto_repair_dummy)) {
								repv = true;
								line++;
								continue;
							}
						}
						newv.Cat(diff);
						if(!ParseVer(b, ft, au)) {
							errors++;
							ConLine(NFormat(t_("Error in history %s, line %d: syntax error in version separator: %s"), vfn, line, sep));
							if(CheckRepair(NFormat(t_("Error in history of file [* \1%s\1]: syntax error in version separator: \n[* \1%S\1]\nCorrect heading (create an artificial heading based on next version)?"),
								finfo.path, sep), auto_repair_syntax)) {
								repv = true;
								ft = lft - 1;
								au = "Lost";
								sep = String().Cat() << '#' << ft << ' ' << AsCString(au) << '\n';
							}
						}
						else {
							if(ft > lft) {
								ConLine(NFormat(t_("Minor error in history %s, line %d: time reversion (%` -> %`)"),
									vfn, line, FormatUvsTime(lft), FormatUvsTime(ft)));
								int rt = lft - 1;
								if(CheckRepair(NFormat(t_("Minor error in history of file [* \1%s\1]: time reversion, %` -> %`. Correct the error (change former time to %`)?"),
									finfo.path, FormatUvsTime(lft), FormatUvsTime(ft) , FormatUvsTime(rt)),
									auto_repair_rtime)) {
									repv = true;
									ft = rt;
									sep = String().Cat() << '#' << ft << ' ' << AsCString(au) << '\n';
								}
							}
							if(IsNull(au)) {
								errors++;
								ConLine(NFormat(t_("Error in history %s, line %d: missing modification author's name"), vfn, line));
								if(CheckRepair(NFormat(t_("Error in history of file [* \1%s\1]: missing modification author's name. Change to [* Unknown]?"), finfo.path),
									auto_repair_author)) {
									repv = true;
									au = "Unknown";
									sep = String().Cat() << '#' << ft << ' ' << AsCString(au) << '\n';
								}
							}
							lft = ft;
						}
						lfile = ofile;
						line++;
						newv.Cat(sep);
					}
					if(repv) {
						ConLine(NFormat(t_("Saving corrected history '%s'."), vfn));
						if(!SaveFileBackup(vfn, newv))
							throw Exc(NFormat(t_("Error saving file '%s'."), vfn));
					}
				}
			}
		}
		if(jobcancel)
			ConLine(t_("Archive check aborted by user."));
		if(errors == 0)
			ConLine(t_("No error found during local archive check."));
		else
			ConLine(NFormat(t_("Local archive check finished with %d error(s)."), errors));
		EndSync();
	}
	catch(Exc e) {
		ConLine(e);
		EndSync();
		ShowExc(e);
	}
}

void DlgFiles::ToolSyncAdjustTime(Bar& bar)
{
	bar.Add(t_("Correct times"), THISBACK(OnSyncAdjustTime))
		.Help(t_("Set modification times for non-modified files according to uvs.$dir in the local archive"));
}

/*
bool IsSameTextFile(const char *a, const char *b)
{
	for(;;)
		if(*a == *b) {
			if(*a++ == 0)
				return true;
			b++;
		}
		else if(*a == 0) {
			while(*b && (byte)*b <= ' ')
				b++;
			return *b == 0;
		}
		else if(*b == 0) {
			while(*a && (byte)*a <= ' ')
				a++;
			return *a == 0;
		}
		else if((byte)*a <= ' ' && (byte)*b <= ' ') {
			while(*a && *a != '\n')
				if((byte)*a++ > ' ')
					return false;
			while(*b && *b != '\n')
				if((byte)*b++ > ' ')
					return false;
		}
		else
			return false;
}
*/

void DlgFiles::OnSyncAdjustTime()
{
	ConClear();
	progress.Info(statusbar);
	for(int i = 0; i < cinfo.GetCount(); i++) {
		if(cinfo[i].IsTree() && !IsNull(cinfo[i].local.filetime) && !cinfo[i].local.deleted) {
			bool difftime = (cinfo[i].local.filetime != cinfo[i].tree_time);
			String treefile = job.GetTreePath(cinfo[i].path);
			String localfile = job.GetLocalPath(cinfo[i].path);
			String treedata = LoadFile(treefile);
			String localdata = LoadFile(localfile);
			bool difffile = treedata != localdata
				&& (job.IsBinary(cinfo[i].path) || !IsSameTextFile(treedata, localdata));
			Time oldtime = FromUvsTime(cinfo[i].tree_time);
			Time localtime = FromUvsTime(cinfo[i].local.filetime);
			Time dsttime = oldtime;
			if(difffile && !difftime)
				dsttime = GetSysTime();
			else if(difftime && !difffile)
				dsttime = localtime;
			if(dsttime != oldtime) {
				Time dsttime = FromUvsTime(cinfo[i].local.filetime);
				ConLine(NFormat(t_("%s set to %` (old = %`)"), treefile, dsttime, oldtime));
				if(!FileSetTime(treefile, dsttime))
					ConLine(NFormat(t_("%s: error setting file time to %`"), treefile, dsttime));
			}
		}
		if(IsSyncTime())
			SetProgressCount(i, cinfo.GetCount());
	}
	UpdateTree();
}

void DlgFiles::ToolSyncExec(Bar& bar)
{
	bar.Add(t_("Download & upload"), Uvs2Img::sync_exec(), THISBACK(OnSyncExec))
		.Key(K_CTRL_R)
		.Help(t_("Download & upload all updates"));
}

void DlgFiles::OnSyncExec()
{
	DoSync();
}

void DlgFiles::ToolSyncBatch(Bar& bar)
{
	bar.Add(t_("Batch processing"), Uvs2Img::sync_batch(), THISBACK(OnSyncBatch))
		.Help(t_("Download & upload changes in recent projects"));
}

class DlgSyncBatch : public WithSyncBatchLayout<TopWindow>
{
public:
	typedef DlgSyncBatch CLASSNAME;
	DlgSyncBatch();

	String  Run(Vector<String>& sync_projects, bool& confirm, const Vector<String>& recent_files);

private:
	void    ToolLocal(Bar& bar);
	void    OnInsert();
	void    OnRemove() { list.DoRemove(); }

private:
	ArrayOption   option;
	String        recent_path;
};

DlgSyncBatch::DlgSyncBatch()
{
	CtrlLayoutOKCancel(*this, t_("Batch synchronization"));
	list.AddColumn(t_("Project"), 30);
	option.AddColumn(list, t_("Synchronize"), 10).InsertValue(1).HeaderTab().Fixed(30);
	list.WhenBar = THISBACK(ToolLocal);
	do_sync = true;
}

String DlgSyncBatch::Run(Vector<String>& sync_projects, bool& cfm, const Vector<String>& recent_projects)
{
	confirm = cfm;
	int i;
	for(i = 0; i < sync_projects.GetCount(); i++)
		list.Add(sync_projects[i], 1);
	for(i = 0; i < recent_projects.GetCount(); i++)
		if(FindIndex(sync_projects, recent_projects[i]) < 0)
			list.Add(recent_projects[i], 0);
	if(list.GetCount() > 0)
		list.SetCursor(0);
	if(!recent_projects.IsEmpty())
		recent_path = GetFileFolder(recent_projects[0]);
	else if(!sync_projects.IsEmpty())
		recent_path = GetFileFolder(sync_projects[0]);
	if(TopWindow::Run() != IDOK)
		return String::GetVoid();
	sync_projects.Clear();
	if(do_sync)
		for(i = 0; i < list.GetCount(); i++)
			if((int)list.Get(i, 1))
				sync_projects.Add(list.Get(i, 0));
	cfm = confirm;
	return list.Get(0);
}

void DlgSyncBatch::ToolLocal(Bar& bar)
{
	bar.Add(t_("Insert"), THISBACK(OnInsert))
		.Key(K_INSERT)
		.Help(t_("Add project into batch synchronization"));
	bar.Add(list.IsCursor(), t_("Remove"), THISBACK(OnRemove))
		.Key(K_DELETE)
		.Help(t_("Remove project from the list"));
}

void DlgSyncBatch::OnInsert()
{
	FileSel fsel;
	fsel.Type(t_("Projects (*.uvs)"), "*.uvs")
	.AllFilesType()
	.DefaultExt("uvs")
	.Multi()
	.ActiveDir(recent_path);
	if(fsel.ExecuteOpen(t_("Insert project")))
		for(int i = 0; i < fsel.GetCount(); i++) {
			String fn = fsel[i];
			UvsJob fjob;
			String data = LoadFile(fn);
			if(data.IsEmpty() || !fjob.Read(data)) {
				if(!fjob.Edit())
					continue;
				if(!SaveFile(fn, fjob.AsString())) {
					Exclamation(NFormat(t_("Error writing file [* \1%s\1]."), fn));
					continue;
				}
			}
			recent_path = GetFileFolder(fn);
			int n = list.GetCursor() + 1;
			list.Insert(n);
			list.Set(n, 0, fn);
			list.Set(n, 1, 1);
			list.SetCursor(n);
		}
}

void DlgFiles::OnSyncBatch()
{
	String res; {
		DlgSyncBatch batchdlg;
		res = batchdlg.Run(sync_projects, recent_confirm, recent_projects);
		if(res.IsVoid())
			return;
	}
	if(sync_projects.IsEmpty()) {
		if(!IsNull(res))
			OpenFile(res);
		return;
	}

	BeginSync();
	try {
		for(int i = 0; i < sync_projects.GetCount(); i++) {
			if(IsCanceled())
				throw AbortExc();
			String prj = sync_projects[i];
			if(!OpenFile(prj))
				throw Exc(NFormat(t_("Error opening project '%s'."), prj));
			ConLine(NFormat(t_("\nSynchronizing project '%s'..."), GetFileTitle(prj)));
			if(!SyncProject(recent_confirm))
				throw Exc(t_("Synchronization terminated with error(s), finishing batch processing."));
		}
		ConLine(t_("Project group synchronization finished successfully."));
	}
	catch(Exc e) {
		ConLine("\n" + e);
		ShowExc(e);
	}
	EndSync();
}

void DlgFiles::ToolSyncImport(Bar& bar)
{
	bar.Add(t_("Import archive"), THISBACK(OnSyncImport))
		.Help(t_("Add another local archive to the current local archive"));
}

void DlgFiles::OnSyncImport()
{
	if(IsNull(job.localrepository)) {
		Exclamation(t_("Local archive folder not specified; check project settings."));
		return;
	}
	FileSel fsel;
	if(fsel.ExecuteSelectDir(t_("Import local archive")))
		DoSyncMerge(~fsel);
}

void DlgFiles::ToolSyncExport(Bar& bar)
{
	bar.Add(t_("Export archive"), THISBACK(OnSyncExport))
		.Help(t_("Re-generate and upload full archive including history to the server"));
}

void DlgFiles::OnSyncExport()
{
	if(IsNull(job.localrepository)) {
		Exclamation(t_("Local archive folder not specified; check project settings."));
		return;
	}
	DoSyncExport();
}

/*
void DlgFiles::ToolSyncDownload(Bar& bar)
{
	bar.Add("Jen naèíst", Uvs2Img::sync_download(), THISBACK(OnSyncDownload))
		.Help("Jen stáhnout zmìnìné soubory, vlastní zmìny neukládat");
}
*/

/*
void DlgFiles::OnSyncDownload()
{
	DoSync(true, false, "Stahování zmìn");
}
*/

/*
void DlgFiles::ToolSyncUpload(Bar& bar)
{
	bar.Add("Jen uložit", Uvs2Img::sync_upload(), THISBACK(OnSyncUpload))
		.Help("Jen uložit vlastní zmìny, cizí zmìny nestahovat");
}
*/

/*
void DlgFiles::OnSyncUpload()
{
	DoSync(false, true, "Odeslání zmìn");
}
*/

void DlgFiles::ToolSyncStop(Bar& bar)
{
	bar.Add(t_("Stop"), CtrlImg::remove(), THISBACK(OnSyncStop))
		.Help(t_("Stop project synchronization"));
}

void DlgFiles::OnSyncStop()
{
	jobcancel = true;
}

void DlgFiles::ToolView(Bar& bar)
{
	ToolViewTree(bar);
	ToolViewDiff(bar);
	ToolViewAll(bar);
	ToolViewFileDiff(bar);
	ToolViewFileComp(bar);
	bar.Separator();
	ToolViewFind(bar);
	bar.MenuSeparator();
	ToolViewSortAction(bar);
	ToolViewSortPath(bar);
	ToolViewSortName(bar);
	ToolViewSortExt(bar);
	ToolViewSortTime(bar);
	ToolViewSortSize(bar);
	if(bar.IsToolBar() && display_style == D_COMP && compview.IsVisible()) {
		bar.Separator();
//		bar.Add(comp_ver_label);
//		bar.Add(comp_version, 200);
		bar.Add(comp_font_label);
		bar.Add(comp_font, 60);
	}
}

void DlgFiles::ToolViewTree(Bar& bar)
{
		bar.Add(t_("View tree"), Uvs2Img::setup_tree(), THISBACK(OnViewTree))
		.Check(splitter.GetZoom() < 0)
		.Help(t_("Show / hide project folder tree"));
}

void DlgFiles::OnViewTree()
{
	int ix, sps = splitter.GetZoom();
	if(filelist.HasFocusDeep() && filelist.IsCursor())
		ix = filelist.Get(L_INDEX);
	else if(sps < 0 && tree.HasFocusDeep() && tree.IsCursor())
		ix = ~tree;
	splitter.Zoom(sps < 0 ? 1 : -1);
	Rescan();
	LayoutTable();
	OnTree();
	int f;
	if(sps < 0) {
		filelist.FindSetCursor(ix, L_INDEX);
		filelist.SetWantFocus();
	}
	else if((f = tree.Find(ix)) >= 0) {
		tree.SetCursor(f);
		tree.SetWantFocus();
	}
}

void DlgFiles::ToolViewDiff(Bar& bar)
{
		bar.Add(t_("Modified files"), Uvs2Img::setup_diff(), THISBACK(OnViewDiff))
		.Check(view_diff)
		.Help(t_("Display only modified & conflicting files"));
}

void DlgFiles::OnViewDiff()
{
	view_diff = !view_diff;
	Rescan();
	SetTree();
}

void DlgFiles::ToolViewAll(Bar& bar)
{
	bar.Add(t_("All files"), Uvs2Img::setup_all(), THISBACK(OnViewAll))
		.Check(view_all)
		.Help(t_("Display all files (including files excluded from the project)"));
}

void DlgFiles::OnViewAll()
{
	view_all = !view_all;
	Rescan();
	SetTree();
}

void DlgFiles::ToolViewFileDiff(Bar& bar)
{
//	bar.Add(t_("Difference"), Uvs2Img::setup_file_diff(), THISBACK(OnViewFileDiff))
//		.Check(display_style == D_DIFF)
//		.Help(t_("Display differential changes from previous version"));
}

void DlgFiles::OnViewFileDiff()
{
	display_style = (display_style == D_DIFF ? D_FILE : D_DIFF);
	Rescan();
	OnTree();
}

void DlgFiles::ToolViewFileComp(Bar& bar)
{
	bar.Add(t_("Compare"), Uvs2Img::setup_file_comp(), THISBACK(OnViewFileComp))
		.Check(display_style == D_COMP)
		.Help(t_("Compare files in two windows"));
}

void DlgFiles::OnViewFileComp()
{
	display_style = (display_style == D_COMP ? D_FILE : D_COMP);
	Rescan();
	OnTree();
}

void DlgFiles::ToolViewFind(Bar& bar)
{
	bar.Add(t_("Find changes"), Uvs2Img::view_find(), THISBACK(OnViewFind))
		.Key(K_CTRL_F)
		.Check(view_find)
		.Help(t_("Find / highlight files modified since a given date"));
}

void DlgFiles::OnViewFind()
{
	if(view_find) {
		view_find = false;
		SetTree();
		return;
	}
	WithFindChangeLayout<TopWindow> dlgfind;
	CtrlLayoutOKCancel(dlgfind, "Vyhledat zmìny");
	dlgfind.changed_since <<= view_find_time;
	dlgfind.find_filter = view_find_diff;
	if(dlgfind.Run() == IDOK) {
		view_find = true;
		view_find_time = ToTime(~dlgfind.changed_since);
		view_find_diff = dlgfind.find_filter;
		SetTree();
	}
}

void DlgFiles::ToolViewSortAction(Bar& bar)
{
	bar.Add(t_("Sort action"), THISBACK1(DoViewSort, SORT_ACTION))
		.Check(view_sort == SORT_ACTION)
		.Help(t_("Sort files by action (conflict, external modification, own modification, unchanged)"));
}

void DlgFiles::ToolViewSortPath(Bar& bar)
{
	bar.Add(splitter.GetZoom() == 1, t_("Sort folders"), THISBACK1(DoViewSort, SORT_PATH))
		.Check(view_sort == SORT_PATH)
		.Help(t_("Sort files according to folder names"));
}

void DlgFiles::ToolViewSortName(Bar& bar)
{
	bar.Add(t_("Sort names"), THISBACK1(DoViewSort, SORT_NAME))
		.Check(view_sort == SORT_NAME)
		.Help(t_("Sort files by file names"));
}

void DlgFiles::ToolViewSortExt(Bar& bar)
{
	bar.Add(t_("Sort type"), THISBACK1(DoViewSort, SORT_EXT))
		.Check(view_sort == SORT_EXT)
		.Help(t_("Sort files by file types (extensions)"));
}

void DlgFiles::ToolViewSortTime(Bar& bar)
{
	bar.Add(t_("Sort times"), THISBACK1(DoViewSort, SORT_TIME))
		.Check(view_sort == SORT_TIME)
		.Help(t_("Sort files by last modification date / time"));
}

void DlgFiles::ToolViewSortSize(Bar& bar)
{
	bar.Add(t_("Sort size"), THISBACK1(DoViewSort, SORT_SIZE))
		.Check(view_sort == SORT_SIZE)
		.Help(t_("Sort files by file size"));
}

void DlgFiles::DoViewSortName()
{
	DoViewSort(view_sort == SORT_NAME ? SORT_EXT : SORT_NAME);
}

void DlgFiles::DoViewSort(int mode)
{
	view_sort = mode;
	OnTree();
}

void DlgFiles::ToolHelp(Bar& bar)
{
	ToolHelpAbout(bar);
}

void DlgFiles::ToolHelpAbout(Bar& bar)
{
	bar.Add(t_("About application"), THISBACK(OnHelpAbout))
		.Help(t_("Display application version and copyright"));
}

void DlgFiles::OnHelpAbout()
{
	WithAboutLayout<TopWindow> aboutdlg;
	CtrlLayout(aboutdlg, t_("Application information"));
	Font bf = aboutdlg.date.GetFont().Bold();
	aboutdlg.date.SetFont(bf);
	aboutdlg.date.SetLabel(Format(UVS2_DATE));
	aboutdlg.version.SetFont(bf);
	aboutdlg.version.SetLabel(UVS2_VERSION);
	aboutdlg.copyright.SetLabel(UVS2_COPYRIGHT);
	aboutdlg.Run();
}

void DlgFiles::ToolList(Bar& bar)
{
	ToolListAdd(bar);
	ToolListRemove(bar);
}

void DlgFiles::ToolListAdd(Bar& bar)
{
	int ix = GetListIndex();
	bar.Add(ix >= 0 && job.IsExcluded(cinfo[ix].path),
		t_("Add"), THISBACK(OnListAdd))
		.Help(t_("Add selected file to the project"));
}

void DlgFiles::OnListAdd()
{
	if(!filelist.IsCursor())
		return;
	String dir = filelist.Get(L_PATH);
	Vector<String> patterns;
	bool done = false;
	patterns = Split(job.excludedirs, &CharFilterPatternSep);
	for(int i = patterns.GetCount(); --i >= 0;)
		if(PatternMatch(patterns[i], dir)) {
			patterns.Remove(i);
			done = true;
		}
	if(done)
		job.excludedirs = Join(patterns, " ");
	else {
		String name = filelist.Get(L_FILE);
		patterns = Split(job.excludefiles, &CharFilterPatternSep);
		for(int i = patterns.GetCount(); --i >= 0;)
			if(PatternMatch(patterns[i], name))
				patterns.Remove(i);
		job.excludefiles = Join(patterns, " ");
	}
	jobdirty = true;
	UpdateTitle();
	OnTree();
}

void DlgFiles::ToolListRemove(Bar& bar)
{
	String fn = t_("Remove");
	int ix = GetListIndex();
	bool en = (ix >= 0 && !job.IsExcluded(cinfo[ix].path));
	if(en)
		fn << " '*" << GetFileExt((String)filelist.Get(L_FILE)) << "'";
	bar.Add(en, fn, THISBACK(OnListRemove))
		.Help(t_("Remove files of selected type from the project"));
}

void DlgFiles::OnListRemove()
{
	if(!filelist.IsCursor())
		return;
	if(!job.excludefiles.IsEmpty() && *job.excludefiles.Last() != ' ')
		job.excludefiles << ' ';
	job.excludefiles << "*" << ToUpper(GetFileExt((String)filelist.Get(L_FILE)));
	jobdirty = true;
	UpdateTitle();
	OnTree();
}

void DlgFiles::UpdateTitle()
{
	String title;
	title = Nvl(jobpath, t_("New job"));
	if(jobdirty) title << " *";
	Title(title);
	String name = (jobpath.IsEmpty() ? String(t_("Project")) : GetFileTitle(title));
	tree.SetRoot(Uvs2Img::app_small(), Null, name);
}

void DlgFiles::UpdateTree()
{
	cinfo.Clear();
	if(!IsNull(job.dir)) {
		progress.Info(statusbar);
		::SetTree(cinfo, job.dir, THISBACK(OnTreeGate));
		statusbar = t_("File tree has been updated");
	}
	if(!IsNull(job.localrepository)) {
		String localdir = job.GetLocalDirPath();
		String localfile = LoadFile(localdir);
		try {
			if(!localfile.IsVoid()) {
				DirInfo local = ReadDir(localfile);
				SetLocal(cinfo, local);
				for(int i = 0; i < cinfo.GetCount(); i++) {
					ComplexFileInfo& cfi = cinfo[i];
					if(!cfi.treeconflict && !IsNull(cfi.local.filetime) && !IsNull(cfi.tree_time)
					&& tabs(cfi.tree_time - cfi.local.filetime) > 1) {
						String lfn = job.GetLocalPath(cfi.path);
						String tfn = job.GetTreePath(cfi.path);
						if(NoCr(LoadFile(lfn)) == NoCr(LoadFile(tfn))) {
							Time tm = FromUvsTime(cfi.local.filetime);
							if(FileSetTime(tfn, tm)) {
								ConLine(NFormat("Filetime of '%s' corrected to %`", tfn, tm));
								cfi.tree_time = cfi.local.filetime;
							}
							else
								ConLine(NFormat("Error correcting filetime of '%s'", tfn));
						}
					}
				}
			}
		}
		catch(Exc e) {
			Exclamation(NFormat(t_("Error reading file [* \1%s\1]: \1%s"), localdir, e));
		}
	}
	SetTree();
}

void DlgFiles::SetTree()
{
	tree.Clear();
	UpdateTitle();
	int find_time = ToUvsTime(view_find_time);
	for(int i = 0; i < cinfo.GetCount(); i++) {
		const ComplexFileInfo& cfi = cinfo[i];
		if(!view_all && job.IsExcluded(cfi.path))
			continue;
		bool found = false;
		if(view_find) {
			found = (cfi.GetTimeModified() >= find_time);
			if(view_find_diff && !found)
				continue;
		}
		if(view_diff && !cfi.IsTreeConflict() && !found
		&& !cfi.IsTreeEdit() && last_download.Find(cinfo.GetKey(i)) < 0)
			continue;
		AddTree(i, found);
	}
	Rescan();
	tree.Open(0);
	OnTree();
}

bool DlgFiles::CheckFileSave()
{
	if(!jobdirty)
		return true;
	switch(PromptYesNoCancel(t_("Do you want to save changes to current job?"))) {
	case 1:   return SaveFile();
	case 0:   return true;
	default:  return false;
	}
}

bool DlgFiles::SaveFile()
{
	if(!jobdirty)
		return true;
	if(jobpath.IsEmpty())
		return SaveFileAs();
	else
	if(::SaveFile(jobpath, job.AsString())) {
		jobdirty = false;
		UpdateTitle();
		return true;
	}
	Exclamation(NFormat(t_("Unable to save job to [* \1%s\1]."), jobpath));
	RemoveLRU(jobpath);
	return false;
}

bool DlgFiles::SaveFileAs() {
	jobfs <<= jobpath;
	if(!jobfs.ExecuteSaveAs(t_("Save project as...")))
		return false;
	String s = jobfs;
	if(::SaveFile(s, job.AsString())) {
		jobpath = s;
		jobdirty = false;
		AddLRU(jobpath);
		UpdateTitle();
		return true;
	}
	Exclamation(NFormat(t_("Error saving file [* \1%s\1]."), s));
	RemoveLRU(s);
	return false;
}

void DlgFiles::AddTree(int index, bool found)
{
	const ComplexFileInfo& cfi = cinfo[index];
	bool exclude = job.IsExcluded(cfi.path);
	int style = exclude ? 0
		: (found ? T_FOUND : 0) | (cfi.IsTreeEdit() ? T_LOCAL : 0) | (cfi.IsTreeConflict() ? T_CONFLICT : 0);
	if(last_download.Find(cinfo.GetKey(index)) >= 0)
		style |= T_HOST;
	if((exclude || IsNull(cfi.tree_time)) && !view_all && !(style & T_HOST))
		return;
	if(!style && view_diff)
		return;
	int parent = 0;
	for(const char *p = cfi.path; *p;) {
		const char *b = p;
		while(*p && *p != '/')
			p++;

		String part(b, p);
		int i, cr = 1;
		bool is_dir = *p;
		int child = 0;
		for(i = tree.GetChildCount(parent); i > 0; i--) {
			child = tree.GetChild(parent, i - 1);
			bool is_p_dir = IsNull(Point(tree[child]).y);
			if(is_p_dir == is_dir) {
				String ctxt = tree.GetValue(child);
				if(!(cr = CompareNoCase(part, ctxt)))
					break;
				if((cr = GetLanguageInfo().Compare(part, ctxt)) >= 0)
					break;
			}
			else if(!is_dir)
				break;
		}
		Image img;
		int nclss = style;
		if(*p) {
			if(!cr)
				nclss |= Point(tree[child]).x;
			if(nclss & T_FOUND)
				img = Uvs2Img::folder_found();
			else if(nclss & T_CONFLICT)
				img = Uvs2Img::folder_conflict();
			else if(nclss & T_LOCAL)
				img = (nclss & T_HOST ? Uvs2Img::folder_both() : Uvs2Img::folder_us());
			else if(nclss & T_HOST)
				img = Uvs2Img::folder_them();
			else
				img = Uvs2Img::folder();
			parent = (cr ? tree.Insert(parent, i) : child);
			tree.SetNode(parent, TreeCtrl::Node(img, Point(nclss, Null), part));
			p++;
		}
		else {
			if(style & T_FOUND)
				img = Uvs2Img::flag_found();
			else if(exclude)
				img = Uvs2Img::flag_exclude();
			else if(cfi.IsTreeConflict())
				img = Uvs2Img::flag_conflict();
			else if(cfi.IsTreeDeleted())
				img = Uvs2Img::flag_tree_deleted();
			else if(IsUpload(cfi))
				img = Uvs2Img::flag_upload();
			else if(style & T_HOST)
				img = Uvs2Img::flag_download();
			else if(IsNull(cfi.tree_time))
				img = Uvs2Img::flag_deleted();
			else
				img = Uvs2Img::file();
			parent = tree.Insert(parent, i, img, Point(style, index), (Value)part);
		}
	}
}

void DlgFiles::OnTree()
{
	compleft_ver = compright_ver = -1;
	filelist.Clear();
	Vector<int> index;
	if(splitter.GetZoom() == 1) {
		filelist.Show();
		fileview.Hide();
		diffview.Hide();
		compview.Hide();
		index.SetCount(cinfo.GetCount());
		for(int i = 0; i < cinfo.GetCount(); i++)
			index[i] = i;
	}
	else {
		int item = tree.GetCursor();
		int ix;
		bool isdir = item < 0 || IsNull(ix = Point(tree[item]).y);
		filelist.Show(isdir);
		fileview.Show(!isdir && display_style == D_FILE);
		diffview.Show(!isdir && display_style == D_DIFF);
		compview.Show(!isdir && display_style == D_COMP);
		if(item < 0) {
			Rescan();
			return;
		}
		if(isdir) {
			String rpath;
			for(; item; item = tree.GetParent(item))
				rpath = String(tree.GetValue(item)) + "/" + rpath;
			rpath = ToLower(rpath);
			int len = rpath.GetLength();

			for(int i = 0; i < cinfo.GetCount(); i++) {
				String xpath = cinfo.GetKey(i);
				if(xpath.GetLength() > len && !memcmp(rpath, xpath, len) && !strchr(xpath.GetIter(len), '/'))
					index.Add(i);
			}
		}
		else {
			UpdateHistory(ix);
			history.SetCursor(0);
			Rescan();
			return;
		}
	}
	Vector<String> sortkeys;
	sortkeys.SetCount(index.GetCount());
	int i;
	for(i = 0; i < index.GetCount(); i++) {
		int x = index[i];
		const ComplexFileInfo& cfi = cinfo[x];
		sortkeys[i] = cfi.GetSortKey(view_sort, last_download.Find(cinfo.GetKey(x)) >= 0, cfi.tree_size);
	}

	IndexSort(sortkeys, index, StdLess<String>());
	for(i = 0; i < index.GetCount(); i++) {
		String dir, file;
		int x = index[i];
		const ComplexFileInfo& cfi = cinfo[x];
		SplitPath(cfi.path, dir, file);
		bool exclude = job.IsExcluded(dir, file);
		if((exclude || IsNull(cfi.tree_time) && cfi.local.deleted) && !view_all)
			continue;
		bool was_down = (last_download.Find(cinfo.GetKey(x)) >= 0);
		const Display *display = NULL;
		if(exclude)
			display = &cd_excluded;
		else if(cfi.IsTreeConflict())
			display = &cf_conflict;
		else if(cfi.IsTreeDeleted())
			display = &cd_deleted;
		else if(cfi.IsTreeNew())
			display = &cd_new;
		else if(cfi.IsTreeEdit())
			display = &cd_local_edit;
		else if(was_down)
			display = &cd_host_edit;
		else if(view_diff)
			continue;
		Image action;
		if(exclude)
			action = Uvs2Img::flag_exclude();
		else if(cfi.IsTreeConflict())
			action = Uvs2Img::flag_conflict();
		else if(cfi.IsTimeConflict())
			action = Uvs2Img::flag_timeconflict();
		else if(cfi.IsTreeDeleted())
			action = Uvs2Img::flag_tree_deleted();
		else if(cfi.IsTreeEdit())
			action = Uvs2Img::flag_upload();
		else if(was_down)
			action = Uvs2Img::flag_download();
		else if(IsNull(cfi.tree_time))
			action = Uvs2Img::flag_deleted();
		int r = filelist.GetCount();
		filelist.Add();
		filelist.Set(r, L_INDEX, index[i]);
		filelist.Set(r, L_FLAGS, action);
		filelist.Set(r, L_PATH,  dir);
		filelist.Set(r, L_FILE,  file);
		filelist.Set(r, L_SIZE,  cfi.tree_size);
		filelist.Set(r, L_TIME,  FormatUvsTime(cfi.tree_time));
		if(display)
			for(int i = 1; i < filelist.GetColumnCount(); i++)
				filelist.SetDisplay(r, i, *display);
	}

	Rescan();
//	if(filelist.GetCount() > 0)
//		filelist.SetCursor(0);
}

static bool SmallDiff(const char *s1, const char *s2)
{
	for(;;) {
		while(*s1 && (byte)*s1 <= ' ')
			s1++;
		while(*s2 && (byte)*s2 <= ' ')
			s2++;
		if(!*s1 || !*s2)
			return !*s1 && !*s2;
		if(*s1 != *s2)
			return false;
		while(*s1 && *s1 == *s2)
			s1++, s2++;
		if((byte)s1[-1] <= ' ')
			continue;
		if((byte)*s1 > ' ' || (byte)*s2 > ' ')
			return false;
	}
}

void DlgFiles::UpdateComp()
{
	String rel_path = cinfo[file_index].path;
	String act_path = job.GetTreePath(rel_path);
	String lrpath = job.GetLocalPath(rel_path);
	String lrdiff = lrpath + ".$v";
	String af = LoadFile(lrpath);
	String f = LoadFile(act_path);
	int left_version = history.GetCursor();
	int right_version = comp_history_index;
	compview.Zoom(left_version == right_version ? 0 : -1);

	int luvs, ruvs;
	String left_data = GetHistory(left_version, luvs);
	String right_data = GetHistory(right_version, ruvs);

	int dummy;
	String content = GetHistory(dummy);
	String data = ~fileview;
	LineEdit::EditPos fpos = fileview.GetEditPos();
	int dy = fileview.GetLine(fpos.cursor) - fpos.sby;
	int ln = fileview.GetCursorLine();
	fileview <<= content;
	int l = LocateLine(data, ln, content);
	fpos.sby = l - dy;
	fpos.cursor = fileview.GetPos(l);
	fileview.SetEditPos(fpos);
	fileview.SetFocus();

	Vector<String> ll = GetStringLineMap(left_data);
	Vector<String> rl = GetStringLineMap(right_data);
	Array<TextSection> sections = CompareLineMaps(ll, rl);

	Point left_pos = compleft.GetPos();
	Point right_pos = compright.GetPos();
	int sb_pos = compleft.GetSb();

	int outln = 0;
	compleft.SetCount(0);
	compright.SetCount(0);
	for(int i = 0; i < sections.GetCount(); i++) {
		const TextSection& sec = sections[i];
		bool diff = !sec.same;
		Color c1 = (diff ? LtBlue() : SBlack()), c2 = (diff ? LtBlue() : SBlack());
		int maxcount = max(sec.count1, sec.count2);
		compleft.AddCount(maxcount);
		int l;
		for(l = 0; l < sec.count1; l++) {
			int level = (diff ? l < sec.count2 && SmallDiff(ll[sec.start1 + l], rl[sec.start2 + l]) ? 1 : 2 : 0);
			compleft.Set(outln + l, ll[sec.start1 + l], c1, sec.start1 + l + 1, level);
		}
		for(; l < maxcount; l++)
			compleft.Set(outln + l, Null, c1, Null, 2);
		compright.AddCount(maxcount);
		for(l = 0; l < sec.count2; l++) {
			int level = (diff ? l < sec.count1 && SmallDiff(rl[sec.start2 + l], ll[sec.start1 + l]) ? 1 : 2 : 0);
			compright.Set(outln + l, rl[sec.start2 + l], c2, sec.start2 + l + 1, level);
		}
		for(; l < maxcount; l++)
			compright.Set(outln + l, Null, c2, Null, 2);
		outln += maxcount;
	}

	if(left_version == compleft_ver)
		compleft.SetPos(left_pos);
	else if(right_version == compright_ver)
		compright.SetPos(right_pos);
	else {
		compleft.SetSb(sb_pos);
		compright.SetSb(sb_pos);
	}
	compleft_ver = left_version;
	compright_ver = right_version;
}

void DlgFiles::UpdateDiff()
{
#if 0
	String rel_path = cinfo[file_index].path;
	String act_path = job.GetTreePath(rel_path);
	String lrpath = job.GetLocalPath(rel_path);
	String lrdiff = lrpath + ".$v";
	String af = LoadFile(lrpath);
	String f = LoadFile(act_path);
	int current_version = history.GetCursor();

	fileview.Set(Null);

	FileIn vf(lrdiff);
	String end_range;
	Color end_tbg;
	int tab_size = 4;
	Array<UvsDiffBlock> diff;
	Vector<int> ver_time;
	Vector<String> ver_author;
	ver_time.Add(cinfo[file_index].tree_time);
	ver_author.Add("*");
	ver_time.Add(cinfo[file_index].local.filetime);
	ver_author.Add(cinfo[file_index].local.author);
	int ver = 0;
	if(current_version <= 0) {
		StringStream fstrm(f);
		UvsDiffBlock::AddStream(diff, fstrm, ver++);
		StringStream dstrm(Diff(f, af));
		UvsDiffBlock::AddVersion(diff, dstrm, ver++);
	}
	else {
		f = af;
		while(++ver < current_version && !vf.IsEof()) {
			String prev;
			while(!vf.IsEof()) {
				String l = vf.GetLine();
				if(*l == '#') {
					ParseVer(l, ver_time.Add(), ver_author.Add());
					break;
				}
				prev << l << '\n';
			}
			f = PrevVersion(f, prev);
		}
		StringStream fstrm(f);
		UvsDiffBlock::AddStream(diff, fstrm, ver++);
		UvsDiffBlock::AddVersion(diff, vf, ver++);
		if(vf.Term() == '#')
			ParseVer(vf.GetLine(), ver_time.Add(), ver_author.Add());
	}


/*	ver_time.Add(Null);
	ver_author.Add(job.author);
	while(!vf.IsEof()) {
		UvsDiffBlock::AddVersion(diff, vf, ver++);
		if(vf.Term() == '#')
			ParseVer(vf.GetLine(), ver_time.Add(), ver_author.Add());
	}
*/
	UvsDiffBlock::Merge(diff);
	Document doc;
	static Font rfont = StdFont(85);
//	int localdelta = UvsLocalTimeDelta();
	for(int i = 0; i < diff.GetCount(); i++) {
		const UvsDiffBlock& udb = diff[i];
		int begintime = Null, endtime = Null;
		if(udb.newest_version > 0 && udb.newest_version <= ver_time.GetCount())
			endtime = ver_time[udb.newest_version - 1];
		int oldest = udb.newest_version + udb.line_index.GetCount() - 1;
		if(oldest < ver_time.GetCount())
			begintime = ver_time[oldest];
		Time bt = FromUvsTime(begintime);
		Time et = FromUvsTime(endtime);
		Time st = GetSysTime();
		Color tbg;
		bool active = false;
		if(udb.newest_version > current_version)
			tbg = LtGray;
		else if(oldest < current_version)
			tbg = Color(255, 192, 192);
		else {
			active = true;
			if(oldest == current_version)
				tbg = LtBlue;
			else
				tbg = Black;
		}
		String range;
		switch((IsNull(begintime) ? 0 : 1) | (IsNull(endtime) ? 0 : 2)) {
		case 0:
			range = t_("Current");
			break;

		case 1:
			range = FormatShort(bt, false);
			break;

		case 2:
			range << NFormat(t_("Before %s"), FormatShort(et, false));
			break;

		case 3: {
				bool prec = (Date(bt) == Date(et));
				range << FormatShort(bt, prec) << " - " << FormatShort(et, prec);
			}
			break;
		}
		range << " (" << ver_author[oldest];
		if(udb.newest_version > 0) {
			String deletor = ver_author[udb.newest_version - 1];
			if(deletor != ver_author[oldest])
				range << " .. " << deletor;
		}
		range << ")";
		Table& tbl = doc.AddTable();
		tbl.FrameWidth(0).NoHeaderRows();
		tbl(0, 0).Paper(tbg).FrameWidth(0).FrameSpace(20).Par().Cat(range, Font(rfont).Bold(), White);
		if(!IsNull(end_range))
			tbl(0, 1).Paper(end_tbg).FrameWidth(0).FrameSpace(20).Par().Right().Cat(end_range, rfont, White);
		end_range = range;
		end_tbg = tbg;
		int cxt = 5;
		for(int l = 0; l < udb.lines.GetCount(); l++) {
			String o;
			int pos = 0;
			for(const char *p = udb.lines[l]; *p;) {
				const char *b = p;
				while(*p && *p != '\t')
					p++;
				if(p > b) {
					o.Cat(b, p - b);
					pos += p - b;
				}
				while(*p == '\t') {
					int d = tab_size - (pos % tab_size);
					o.Cat(' ', d);
					pos += d;
					p++;
				}
			}
			Paragraph& par = doc.AddParagraph();
			if(o.IsEmpty())
				o << '\x1f';
			par.Cat(o, Courier(85), tbg);
			if(!IsNull(cxt) && udb.lines.GetCount() > 2 * cxt && l == cxt - 1) {
				Table& dots = doc.AddTable();
				dots.FrameWidth(0).NoHeaderRows();
				dots(0, 0).Paper(Color(0, 192, 0)).FrameWidth(0).FrameSpace(20).Par().Center().Cat("..........", Font(rfont).Bold(), White);
				l = udb.lines.GetCount() - cxt - 1;
			}
		}
	}
	if(!IsNull(end_range)) {
		Table& tbl = doc.AddTable();
		tbl.FrameWidth(0).NoHeaderRows();
		tbl(0, 0).Paper(end_tbg).FrameWidth(0).FrameSpace(20).Par().Right().Cat(end_range, rfont, White);
	}
	diffview.Pick(doc);
#endif
}

void DlgFiles::LogLine(String line)
{
	String logf = job.GetLocalLogPath();
	FileStream fs;
	if(!fs.Open(logf, FileStream::APPEND)) {
		ConLine(NFormat(t_("Cannot open file '%s'."), logf));
		return;
	}
	fs.Put(line);
	fs.PutCrLf();
	fs.Close();
	if(fs.IsError())
		ConLine(NFormat(t_("Error writing file '%s'."), logf));
}

int DlgFiles::GetLocalBlock()
{
	String fn = job.GetLocalIdxPath();
	String data = LoadFile(fn);
	if(IsNull(data))
		return 0;
	if(!IsDigit(*data))
		throw Exc(NFormat(t_("Invalid index file '%s': number expected, found: %s"),
			fn, StringSample(data, 20)));
	int t = ScanInt(data);
	if(IsNull(t) || t <= 0 || t > 1000000000)
		throw Exc(NFormat(t_("Invalid last block index in file '%s': %d"), fn, t));
	return t;
}

UvsHostIndex DlgFiles::GetHostIndex()
{
	String idxfn = "uvs.idx";
	String index = LoadRetry(idxfn, true);
	return UvsHostIndex::Decompress(index);
}

Array<UvsDataBlock> DlgFiles::Download(int first, int last)
{
	Array<UvsDataBlock> data;
	ConAdd(t_("Reading changes: "));
	int load_total = last - first + 1;
	for(int i = first; i <= last; i++) {
		ConAdd(String().Cat() << (i == first || (i - first) % 5 ? " " : "\n   ") << i);
		String path = FormatInt(i) + ".udb";
		String filename = job.GetLocalPath(path);
		String file = LoadFile(filename);
		bool saved = true;
		if(IsNull(file)) {
			file = LoadRetry(path, false);
			int blocklen = UvsDataBlock::GetLength(file, false);
			if(IsNull(blocklen) || blocklen == 0 || file.GetLength() != blocklen) {
				ConLine(NFormat("\nIncomplete update block %s, retrying...", path));
				i--;
				continue;
			}
			saved = false;
		}
		UvsDataBlock& block = data.Add();
		try {
			decompress_start_msecs = msecs();
			block = UvsDataBlock::Decompress(file, false, THISBACK(OnDecompressGate));
			if(!::SaveFile(filename, file))
				throw Exc(NFormat(t_("Error saving block file '%s'"), filename));
			block.temp_filename = filename;
		}
		catch(Exc e) {
			throw Exc(NFormat(t_("Error in file '%s': %s"), path, e));
		}
	}
	ConLine("");
	return data;
}

void DlgFiles::OpenFTP(bool retry)
{
	ftp_client.Close();
	if(IsNull(job.host))
		throw Exc(t_("Server address not specified."));
	if(IsNull(job.hostrepository))
		throw Exc(t_("Remote archive folder on UVS server not specified."));
	if(!retry)
		ConLine(NFormat(t_("Connecting to server: %s"), job.host));
	int rn = 0;
	connect_time = msecs();
	while(!ftp_client.Connect(job.host, job.user, job.password, job.passive)) {
		if(++rn > RETRIES)
			throw Exc(NFormat(t_("Error connecting to server '%s', user '%s' (%[0:active;passive]s)."), job.host, job.user, job.passive));
		ConLine(NFormat(t_("Error connecting to '%s', retrying (%d of %d); FTP error: %s"), job.host, rn, RETRIES, ftp_client.GetError()));
	}
	if(!ftp_client.Cd(job.hostrepository))
		throw Exc(NFormat(t_("Remote folder '%s' not found."), job.hostrepository));
}

void DlgFiles::DoSync()
{
	BeginSync();
	SyncProject(true);
	EndSync();
}

bool DlgFiles::IsUpload(const ComplexFileInfo& cfi)
{
	FileIn tin(job.GetTreePath(cfi.path));
	FileIn lin(job.GetLocalPath(cfi.path));
	for(;;) {
		if(tin.IsEof() && lin.IsEof()) return false;
		if(tin.Get() != lin.Get()) return true;
	}
}

bool DlgFiles::SyncProject(bool confirm)
{
	ftp_client.WhenProgress = THISBACK(OnFtpProgress);
	bool is_locked = false;

	try {
		RealizeDirectory(job.dir);
		RealizeDirectory(job.localrepository);
		OpenFTP();
		if(!job.anonymous)
			LockHost();
		is_locked = true;
		bool create = false;
		ConAdd(t_("Modification block: server - "));
		UvsHostIndex hostindex = GetHostIndex();
		if(hostindex.last_update > 0)
			ConAdd(FormatInt(hostindex.last_update));
		else {
			ConAdd(t_("empty"));
			if(!PromptOKCancel(t_("Server archive is empty. Create a new archive?")))
				throw AbortExc();
			create = true;
		}
		ConAdd(t_("; local - "));
		int lblock = GetLocalBlock();
		bool has_lblock = false;
		if(lblock) {
			ConLine(FormatInt(lblock));
			if(hostindex.last_update == 0)
				throw Exc(NFormat(t_("Last block index in the local archive (%d) doesn't match empty remote archive."), lblock));
			if(lblock > hostindex.last_update)
				throw Exc(NFormat(t_("Last block index in the local archive (%d) is greater than server block index (%d)."), lblock, hostindex.last_update));
			lblock++;
			has_lblock = true;
		}
		else
			ConLine(t_("empty"));
		LogLine(NFormat(t_("Synchronization %`; local block %d, server block %d"), GetSysTime(), lblock, hostindex.last_update));
		lblock = max(lblock, max(hostindex.last_full, 1));
		last_download.Clear();
		Array<UvsDataBlock> blocks;
		VectorMap<String, int> last_for_file;
		int down_files = 0;
		bool loaded_full = false;

		if(!has_lblock && !create) {
			ConLine(t_("Looking for full block..."));
			int idx = ScanInt(LoadRetry("full.idx", true));
			if(!IsNull(idx)) {
				for(;;) {
					String full = LoadRetry("full.udb", false);
					if(IsNull(full))
						break;
					int len = UvsDataBlock::GetLength(full, false);
					if(!IsNull(len) && len > 0 && full.GetLength() == len) {
						loaded_full = true;
						decompress_start_msecs = msecs();
						blocks.Add() = UvsDataBlock::Decompress(full, false, THISBACK(OnDecompressGate));
						lblock = idx + 1;
						break;
					}
					ConLine("Incomplete full update block 'full.udb' downloaded, retrying...");
				}
			}
		}

		if(hostindex.last_update >= lblock) {
			Array<UvsDataBlock> down = Download(lblock, hostindex.last_update);
			while(!down.IsEmpty())
				blocks.Add(down.Detach(0));
		}

		if(!blocks.IsEmpty()) {
			DirInfo prev;
			for(int i = 0; i < blocks.GetCount(); i++) {
				UvsDataBlock& block = blocks[i];
				SetHost(cinfo, block.download);
				if(i == 0)
					prev <<= block.dir;
				int f;
				for(f = 0; f < block.data.GetCount(); f++) {
					String ixname = ToLower(block.download.GetKey(f));
					ComplexFileInfo& cfi = cinfo.Get(ixname);
					if(job.IsExcluded(cfi.path) || !cfi.IsHostEdit()
					|| cfi.download.deleted && cfi.local.deleted) {
						block.ignore[f] = true;
						cfi.ignorehost = true;
						continue;
					}
					bool ndel = (!cfi.download.deleted && !cfi.local.deleted);
/*
					if(ndel && !IsNull(cfi.local.filetime)) {
						String lfile = job.GetLocalPath(cfi.path);
						String ldata = LoadFile(lfile), lver = LoadFile(lfile + ".$v");
						String ddata = job.NoCr(block.data[f], cfi.path);
						bool ign = false;
						if(ddata == job.NoCr(ldata, cfi.path))
							ign = true;
						const char *v = lver;
						while(!ign && *v) {
							int err;
							ldata = PrevVersion(ldata, v, &err);
							if(err >= 0)
								break;
							if(job.NoCr(ldata, cfi.path) == ddata) {
								ign = true;
								break;
							}
							while(*v && *v != '#')
								while(*v && *v++ != '\n')
									;
							while(*v && *v++ != '\n')
								;
						}
						if(ign) {
							block.ignore[f] = true;
							cfi.ignorehost = true;
							continue;
						}
					}
*/					down_files++;
					if(ndel && cfi.download.filetime <= cfi.local.filetime)
						cfi.timeconflict = true;

					cfi.host_size = block.data[f].GetLength();
					int ix = last_for_file.Find(ixname);
					if(ix >= 0)
						last_for_file[ix] = i;
					else {
						last_for_file.Add(ixname, i);
						if(cfi.IsTree() && (cfi.IsTreeEdit() || cfi.IsTimeConflict())) {
							String tdata = job.NoCr(LoadFile(job.GetTreePath(cfi.path)), cfi.path);
							if(tdata == job.NoCr(block.data[f], cfi.path))
								cfi.noconflict = true;
						}
					}
				}
			}
//			if(!prev.IsEmpty())
//				SetPrev(cinfo, prev);
		}

//		Index<int> todo;
		int operation = OP_DOWNLOAD_UPLOAD;
		bool archive = false;
		if(confirm) {
			if(!DlgSyncCheck().Run(cinfo, job, operation, archive))
				throw AbortExc();
//			down_files = todo.GetCount();
		}

		DoSyncBackup(job.tree_backup, job.dir);
		DoSyncBackup(job.local_backup, job.localrepository);
		if(down_files > 0) {
			ConLine(NFormat(t_("Saving downloaded files into local archive (%d total)"), down_files));
			int done_files = 0;
			for(int i = 0; i < blocks.GetCount(); i++) {
				const UvsDataBlock& block = blocks[i];
				for(int d = 0; d < block.download.GetCount(); d++) {
					String ixfile = ToLower(block.download.GetKey(d));
					ComplexFileInfo& cfi = cinfo.Get(ixfile);
					if(!cfi.IsHostEdit() && !cfi.IsHostNew() && !cfi.IsHostDeleted())
						continue;

					ConLine(cfi.path);
					LogLine(NFormat(t_("Saving file %s..."), cfi.path));
//					if(confirm && todo.Find(cfix) < 0)
//						continue;
					if(IsSyncTime()) {
						SetProgressCount(++done_files, down_files);
						if(IsCanceled())
							throw AbortExc();
					}

					bool binary = job.IsBinary(cfi.path);
					last_download.FindAdd(ixfile);
					String tree_path = job.GetTreePath(cfi.path);
					String local_path = job.GetLocalPath(cfi.path);
					String local_ver = local_path + ".$v";
					String old_ldata = LoadFile(local_path);
					String old_lver = LoadFile(local_ver);
					bool is_last = (last_for_file.Get(ixfile) == i);
					bool is_deleted = (block.download[d].deleted);

					if(!binary)
						old_ldata = NoCr(old_ldata);

					String ldata = block.data[d];
					String lver;
					if(d < block.version.GetCount())
						lver = block.version[d];
					else if(!is_deleted && (!IsNull(old_ldata) || !IsNull(old_lver))) {
						lver = Diff(ldata, old_ldata);
						lver << "#" << cfi.local.filetime << ' ' << AsCString(cfi.local.author) << "\n"
						<< old_lver;
					}
					String tdata = (block.download[d].deleted ? String(Null) : ldata);

					bool tree_err = !IsNull(cfi.tree_time) && (cfi.IsTreeEdit() || cfi.IsTimeConflict());
					if(is_last && tree_err)
						if(cfi.IsNoConflict()) {
							String msg = t_("Identical changes: ") + cfi.path;
							ConLine(msg);
							LogLine(msg);
							tree_err = false;
						}
						else {
							String msg = t_("Conflict: ") + cfi.path;
							ConLine(msg);
							LogLine(msg);
							String old_tdata = job.NoCr(LoadFile(tree_path), cfi.path);
							tdata = ConflictDiff(old_ldata, old_tdata, ldata);
							cfi.treeconflict = true;
						}

					if(!is_deleted || !block.version.IsEmpty()) {
						RealizePath(local_path);
						if(!binary)
							ldata = DoCr(ldata, job.usecr);
						if(!::SaveFile(local_path, ldata))
							throw Exc(NFormat(t_("Error saving file '%s'."), local_path));
						if(!::SaveFile(local_ver, lver))
							throw Exc(NFormat(t_("Error saving file '%s'."), local_ver));
					}
					if(!IsNull(tdata))
						is_deleted = false;
//					cfi.prev_loaded = true;
//					cfi.prev =
					cfi.local = block.download[d];
//					cfi.tree_time = (is_deleted ? int(Null) : cfi.prev.filetime);
					if(is_last || !tree_err) {
						cfi.tree_time = (is_deleted ? int(Null) : cfi.local.filetime);
						if(is_deleted) {
							DeleteFile(tree_path);
						}
						else {
							RealizePath(tree_path);
							if(!binary)
								tdata = DoCr(tdata, job.usecr);
							if(!::SaveFileTime(tree_path, tdata, FromUvsTime(cfi.tree_time)))
								throw Exc(NFormat(t_("Error saving file '%s'."), tree_path));
						}
					}
				}
				SaveLocalDir(lblock + i);
			}
			SaveLocalDir(hostindex.last_update);
		}

		for(int i = 0; i < blocks.GetCount(); i++)
			blocks[i].Cleanup();
		blocks.Clear();

		if(operation != OP_DOWNLOAD && !job.anonymous) { // upload
			int uvs_time = ToUvsTime(GetSysTime());

			int count_upload = 0;
			Vector<int> to_upload;
			UvsDataBlock out;
			for(int i = 0; i < cinfo.GetCount(); i++) {
				ComplexFileInfo& cfi = cinfo[i];
				if(!IsNull(cfi.local.filetime))
					out.dir.Add(cfi.path, cfi.local);
				if(!job.IsExcluded(cfi.path) && cfi.IsUpload())
					if(IsUpload(cfi)) {
						if(count_upload == 0)
							ConLine(t_("Modified files: "));
						ConLine(cfi.path);
						LogLine(t_("Modified: ") + cfi.path);
						to_upload.Add(i);
						count_upload++;
						FileInfo fi;
						fi.filetime = Nvl(cfi.tree_time, uvs_time);
						fi.author = job.author;
						fi.deleted = IsNull(cfi.tree_time);
						out.download.Add(cfi.path) = fi;
						String tdata = job.NoCr(LoadFile(job.GetTreePath(cfi.path)), cfi.path);
						out.data.Add(tdata);
					}
					else {
						ConLine(cfi.path + " has different time but has not changed - fixing the time");
						String tree_path = job.GetTreePath(cfi.path);
						String local_path = job.GetLocalPath(cfi.path);
						SaveFileTime(tree_path, LoadFile(local_path), FromUvsTime(cfi.local.filetime));
						cfi.local.deleted = false;
					}
			}
			if(!out.download.IsEmpty()) {
				SaveLockFile();
				hostindex.last_update++;

				if(job.fullblock) { // upload full block
					UvsDataBlock block;
					ConLine(t_("Preparing full update block"));
					for(int i = 0; i < cinfo.GetCount(); i++) {
						if(IsSyncTime()) {
							SetProgressCount(i, cinfo.GetCount());
							if(IsCanceled())
								throw AbortExc();
						}
						const ComplexFileInfo& cfi = cinfo[i];
						if(!IsNull(cfi.local.filetime)) {
							block.download.Add(cfi.path, cfi.local);
							String file = job.GetLocalPath(cfi.path);
							String data = LoadFile(file), ver = LoadFile(file + ".$v");
							if(data.IsVoid() && !cfi.local.deleted)
								throw Exc(NFormat(t_("Error reading file '%s'."), file));
							block.data.Add(data);
							block.version.Add(ver);
						}
					}
					ConLine(t_("Compressing update block"));
					save_start_msecs = msecs();
					String out = block.Compress(THISBACK(OnSaveGate));
					ConLine(NFormat(t_("Saving full block - %d B"), out.GetLength()));
					SaveRetry("full.udb", out);
					SaveRetry("full.idx", FormatInt(hostindex.last_update));
				}

				String fn = FormatInt(hostindex.last_update) + ".udb";
				ConLine(NFormat(t_("Compressing modified files (%d total)"), count_upload));
				save_start_msecs = msecs();
				String cblock = out.Compress(THISBACK(OnSaveGate));
				String bdesc = NFormat(t_("Saving changes (%d B)"), cblock.GetLength());
				ConLine(bdesc);
				LogLine(bdesc);
				SaveRetry(fn, cblock);
				SaveIndexFile(hostindex);
			}

			if(count_upload > 0)
				ConLine(t_("Updating local archive"));
			int done_upload = 0;
			for(int i = 0; i < to_upload.GetCount(); i++) {
				ComplexFileInfo& cfi = cinfo[to_upload[i]];
				done_upload++;
				if(!IsNull(cfi.tree_time)) {
					String tree_path = job.GetTreePath(cfi.path);
					String local_path = job.GetLocalPath(cfi.path);
					String local_ver = local_path + ".$v";
					String old_ldata = LoadFile(local_path);
					String old_lver = LoadFile(local_ver);
					String ldata = LoadFile(tree_path);
					String lver;
					if(!IsNull(old_ldata) || !IsNull(old_lver)) {
						lver = Diff(ldata, old_ldata);
						lver << "#" << cfi.local.filetime << ' '
						<< AsCString(cfi.local.author) << "\n" << old_lver;
					}
					RealizePath(local_path);
					if(!::SaveFile(local_path, ldata))
						throw Exc(NFormat(t_("Error saving file '%s'."), local_path));
					if(!::SaveFile(local_ver, lver))
						throw Exc(NFormat(t_("Error saving file '%s'."), local_ver));
				}
				cfi.local.filetime = Nvl(cfi.tree_time, uvs_time);
				cfi.local.author = job.author;
				cfi.local.deleted = IsNull(cfi.tree_time);
				if(IsSyncTime()) {
					SetProgressCount(done_upload, count_upload);
					if(IsCanceled())
						throw AbortExc();
				}
			}
			SaveLocalDir(hostindex.last_update);
		}
		is_locked = false;
		ConLine(t_("Unlocking server archive"));
		if(!job.anonymous)
			DeleteRetry("lock");
		ftp_client.Close();
		ConLine(t_("Synchronization finished successfully."));
		LogLine(NFormat(t_("Synchronization finished (%`)."), GetSysTime()));
		return true;
	}
	catch(Exc e) {
		if(ftp_client.IsOpen() && is_locked) {
			ConAdd(t_("\nSynchronization aborted, unlocking server archive"));
			ConLine(ftp_client.Delete("lock") ? String(t_(" - OK"))
				: String(t_(" - error: ") + ftp_client.GetError()));
		}
		ftp_client.Close();
		ConLine("\n" + e);
		ShowExc(e);
		LogLine(NFormat(t_("Synchronization finished with error(s): %s"), e));
		return false;
	}
}

void DlgFiles::SaveLocalDir(int idx)
{
	int uvs_time = ToUvsTime(GetSysTime());
	DirInfo newdir;
	for(int i = 0; i < cinfo.GetCount(); i++) {
		ComplexFileInfo& cfi = cinfo[i];
		if(!job.IsExcluded(cfi.path) && (cfi.IsTree() || !IsNull(cfi.local.filetime)))
			newdir.Add(cfi.path) = cfi.local;
	}
	String localdir = job.GetLocalDirPath();
	if(!::SaveFile(localdir, AsString(newdir)))
		throw Exc(NFormat(t_("Error saving file '%s'."), localdir));
	if(!IsNull(idx)) {
		String local_idx = job.GetLocalIdxPath();
		if(!::SaveFile(local_idx, FormatInt(idx)))
		   throw Exc(NFormat(t_("Error saving index file '%s'."), local_idx));
	}
}

void DlgFiles::ConClear(bool arc_con)
{
	ConCtrl(arc_con) <<= Null;
}

void DlgFiles::ConAdd(const char *text, bool arc_con)
{
	LineEdit& con = ConCtrl(arc_con);
	if(*text == 0) return;
	int l, h;
	con.GetSelection(l, h);
	if(con.GetCursor() == con.GetLength()) l = -1;
	LineEdit::EditPos p = con.GetEditPos();
	con.SetEditable();
	con.MoveTextEnd();
	con.Paste(text);
	con.SetReadOnly();
	if(l >= 0) {
		con.SetEditPos(p);
		con.SetSelection(l, h);
	}
	IsCanceled();
}

void DlgFiles::ConLine(const char *text, bool arc_con)
{
	ConAdd(String().Cat() << text << '\n', arc_con);
}

bool DlgFiles::IsSyncTime()
{
	return ((unsigned)msecs(sync_time) >= 200u);
}

bool DlgFiles::IsCanceled()
{
	if(IsSyncTime()) {
		statusbar.Sync();
		ProcessEvents();
		sync_time = msecs();
	}
	return jobcancel;
}

bool DlgFiles::OnLoadGateHeaders(String data)
{
	return OnLoadGateAll(data);
/*
	if(IsCanceled())
		return true;
	int len = GetBlockLength(data, true);
	if(IsNull(len))
		return false;
	SetFile(data.GetLength(), len);
	return (data.GetLength() >= len);
//*/
}

void DlgFiles::SetProgressCount(int count, int total)
{
	progress.Text(String().Cat() << count << " / " << total);
	progress.Set(count, total);
}

void DlgFiles::SetProgressSize(int size, int total, int start_msecs)
{
	if(IsNull(total) || total <= 0)
		return;
	const char *suffix = " B";
	progress.Set(size, total);
	int ms = msecs(start_msecs);
	double bps = Null;
	const char *spdsfx = NULL;
	int secs_left = Null;
	if(ms > 0) {
		bps = floor(size * 1000.0 / ms);
		spdsfx = " B/s";
		if(bps > 0)
			secs_left = fround((total - size) / bps);
		if(bps >= 1000) {
			bps /= 1000;
			spdsfx = " KB/s";
			if(bps >= 1000) {
				bps /= 1000;
				spdsfx = " MB/s";
			}
		}
	}
	if(total >= 10000) {
		size /= 1000;
		total /= 1000;
		suffix = " KB";
	}
	StringBuffer out;
	out << size << " / " << total << suffix;
	if(spdsfx) {
		out << " (" << FormatDouble(bps, 1) << spdsfx << ", " << (ms / 1000) << " secs elapsed";
		if(!IsNull(secs_left))
			out << ", " << secs_left << " left";
		out << ')';
	}
	progress.Text(out);
}

bool DlgFiles::OnFtpLoadGate(int, int)
{
	return OnLoadGateAll(ftp_client.GetLoadedPart());
}

bool DlgFiles::OnLoadGateAll(String data)
{
	if(!IsSyncTime())
		return false;
	SetProgressSize(data.GetLength(), UvsDataBlock::GetLength(data, false), load_start_msecs);
	return IsCanceled();
}

bool DlgFiles::OnDecompressGate(int len, int total)
{
	if(!IsSyncTime())
		return false;
	SetProgressSize(len, total, decompress_start_msecs);
	return IsCanceled();
}

bool DlgFiles::OnFtpSaveGate(int, int)
{
	return OnSaveGate(ftp_client.GetSavePos(), ftp_client.GetSaveTotal());
}

bool DlgFiles::OnSaveGate(int done, int total)
{
	if(!IsSyncTime())
		return false;
	SetProgressSize(done, total, save_start_msecs);
	return IsCanceled();
}

bool DlgFiles::OnFtpProgress(int, int)
{
	return IsCanceled();
}

bool DlgFiles::OnTreeGate(int count, int total)
{
	progress.Text(t_("Reading file tree..."));
	progress.Set(count, total);
	return IsCanceled();
}

int DlgFiles::GetListIndex() const
{
	return filelist.IsCursor() ? (int)filelist.Get(L_INDEX) : (int)Null;
}

void DlgFiles::LockHost()
{
	if(ftp_client.MkDir("lock")) {
		ftp_client.RmDir("lock");
		SaveLockFile();
		return;
	}


	String lockfile = LoadRetry("lock", true);
	if(!IsNull(lockfile)) {
		Time locktime;
		String lockauthor;
		try {
			CParser parser(lockfile);
			locktime = FromUvsTime(parser.ReadInt());
			lockauthor = parser.ReadOneString();
		}
		catch(Exc e) {
			throw Exc(NFormat(t_("Error in 'lock' file: %s"), e));
		}
		ConLine(NFormat(t_("Lock: %s, %s"), Format(locktime), lockauthor));
		if(lockauthor == job.author)
			ConLine(NFormat(t_("Ignoring my own lock (user %s)"), job.author));
		else if(locktime >= GetSysTime() - 1800)
			throw Exc(t_("Lock is active, synchronization cannot proceed."));
		else
			ConLine(t_("Ignoring obsolete lock..."));
	}
	SaveLockFile();
}

void DlgFiles::SaveLockFile()
{
	Time time = GetSysTime();
	ConLine(NFormat(t_("Locking server archive: %s, %s"), Format(time), job.author));
	String lockdata;
	lockdata << ToUvsTime(time) << " " << AsCString(job.author) << "\n";
	SaveRetry("lock", lockdata);
}

void DlgFiles::DoSyncBackup(String backupcmd, String path)
{
	if(IsNull(backupcmd))
		return;
	ConAdd(NFormat(t_("Backing up folder %s"), path), false);
#ifdef PLATFORM_POSIX
	if(chdir(NativePath(path)))
#else
	if(!SetCurrentDirectory(NativePath(path)))
#endif
	{
		ConLine(t_(" - folder is empty"), false);
		return;
	}
	ConLine(":", false);
	String cvt;
	for(const char *p = backupcmd; *p; p++)
		if(*p == '@' && p[1] == '@') {
			p++;
			cvt << FormatIntBase(ToUvsTime(GetSysTime()), 36, 6, '0', 0);
		}
		else
			cvt << *p;
	ConLine(cvt, false);
	ConCtrl(false).Sync();
	One<SlaveProcess> sp = StartLocalProcess(cvt);
	if(!sp)
		throw Exc(NFormat(t_("Error executing process '%s'."), cvt));
	for(String s; sp -> Read(s);) {
		ConAdd(s, true);
		ProcessEvents();
		if(IsCanceled()) {
			sp -> Kill();
			throw AbortExc();
		}
	}
	int exit = sp -> GetExitCode();
	if(exit)
		throw Exc(NFormat(t_("Archivation process '%s' returned error code %d."), cvt, exit));
	ConLine(NFormat(t_("Backup of folder %s finished successfully."), path));
}

void DlgFiles::UpdateHistory(int ix)
{
	file_index = ix;
	fileview.SetEditPos(LineEdit::EditPos());
	history.Clear();
	comp_history_index = 1;
	if(splitter.GetZoom() >= 0)
		return;
	String act = "* " + FormatUvsTime(cinfo[ix].tree_time);
	history.Add(act, job.author);
	FileInfo fi = cinfo[ix].local;
	if(!IsNull(fi.filetime)) {
		history.Add(FromUvsTime(fi.filetime), fi.author);
		String fhist = LoadFile(job.GetLocalPath(cinfo.GetKey(ix)) + ".$v");
		const char *p = fhist;
		while(*p) {
			if(*p == '#') {
				int ft = Null;
				String auth;
				ParseVer(p, ft, auth);
				String nm = Nvl(FormatUvsTime(ft), "(UVS #1 bug)");
				history.Add(nm, auth);
			}
			while(*p && *p++ != '\n')
				;
		}
	}
}

String DlgFiles::GetHistory(int index, int& uvstime)
{
	String content;
	uvstime = Null;
	if(file_index >= 0) {
		String rpath = cinfo[file_index].path;
		if(index <= 0)
			content = LoadFile(job.GetTreePath(rpath));
		else {
			String lpath = job.GetLocalPath(rpath);
			content = LoadFile(lpath);
			uvstime = cinfo[file_index].local.filetime;
			if(--index > 0) {
				String history = LoadFile(lpath + ".$v");
				const char *p = history;
				while(--index >= 0 && *p) {
					const char *b = p;
					while(*p && *p != '#')
						while(*p && *p++ != '\n')
							;
					content = PrevVersion(content, String(b, p));
					String dummy;
					ParseVer(p, uvstime, dummy);
					while(*p && *p++ != '\n')
						;
				}
			}
		}
	}
	return content;
}

String DlgFiles::GetHistory(int& uvstime)
{
	return GetHistory(history.GetCursor(), uvstime);
}

void DlgFiles::OnHistory()
{
	for(int i = 0; i < history.GetCount(); i++) {
		const Display& display = (display_style == D_COMP && comp_history_index == i
			? HistoryRowDisplay() : GapDisplay());
		for(int c = 0; c < history.GetColumnCount(); c++)
			history.SetDisplay(i, c, display);
	}
	if(display_style == D_DIFF) {
		int sb = diffview.GetSb();
		UpdateDiff();
		diffview.SetSb(sb);
	}
	else if(display_style == D_COMP)
		UpdateComp();
	else {
		int dummy;
		String content = GetHistory(dummy);
		String data = ~fileview;
		LineEdit::EditPos fpos = fileview.GetEditPos();
		int dy = fileview.GetLine(fpos.cursor) - fpos.sby;
		int ln = fileview.GetCursorLine();
		fileview <<= content;
		int l = LocateLine(data, ln, content);
		fpos.sby = l - dy;
		fpos.cursor = fileview.GetPos(l);
		fileview.SetEditPos(fpos);
		fileview.SetFocus();
	}
}

void DlgFiles::ToolHistory(Bar& bar)
{
	ToolHistoryUp(bar);
	ToolHistoryDown(bar);
	bar.MenuSeparator();
	ToolHistoryCompUp(bar);
	ToolHistoryCompDown(bar);
	ToolHistoryCompSet(bar);
	bar.MenuSeparator();
	ToolHistoryBothUp(bar);
	ToolHistoryBothDown(bar);
	bar.MenuSeparator();
	ToolHistoryRestore(bar);
}

void DlgFiles::ToolHistoryUp(Bar& bar)
{
	bar.Add(history.GetCursor() > 0, t_("Newer"), THISBACK(OnHistoryUp))
		.Key(K_CTRL_UP)
		.Help(t_("Move to newer history entry"));
}

void DlgFiles::OnHistoryUp()
{
	if(history.GetCursor() > 0)
		history.SetCursor(history.GetCursor() - 1);
}

void DlgFiles::ToolHistoryDown(Bar& bar)
{
	bar.Add(history.GetCursor() + 1 < history.GetCount(), t_("Older"), THISBACK(OnHistoryDown))
		.Key(K_CTRL_DOWN)
		.Help(t_("Move to older history entry"));
}

void DlgFiles::OnHistoryDown()
{
	if(history.GetCursor() + 1 < history.GetCount())
		history.SetCursor(history.GetCursor() + 1);
}

void DlgFiles::ToolHistoryCompUp(Bar& bar)
{
	bar.Add(display_style == D_COMP && comp_history_index > 0, t_("Compare newer"), THISBACK(OnHistoryCompUp))
		.Key(K_ALT_UP)
		.Help(t_("Compare with newer history entry"));
}

void DlgFiles::OnHistoryCompUp()
{
	if(display_style == D_COMP && comp_history_index > 0) {
		comp_history_index--;
		OnHistory();
	}
}

void DlgFiles::ToolHistoryCompDown(Bar& bar)
{
	bar.Add(display_style == D_COMP && comp_history_index + 1 < history.GetCount(), t_("Compare older"), THISBACK(OnHistoryCompDown))
		.Key(K_ALT_DOWN)
		.Help(t_("Compare with older history entry"));
}

void DlgFiles::OnHistoryCompDown()
{
	if(display_style == D_COMP && comp_history_index + 1 < history.GetCount()) {
		comp_history_index++;
		OnHistory();
	}
}

void DlgFiles::ToolHistoryCompSet(Bar& bar)
{
	bar.Add(display_style == D_COMP && history.IsCursor(), t_("Compare this"), THISBACK(OnHistoryCompSet))
		.Key(K_ALT_INSERT)
		.Help(t_("Compare with selected history entry"));
}

void DlgFiles::OnHistoryCompSet()
{
	if(display_style == D_COMP && history.IsCursor()) {
		comp_history_index = history.GetCursor();
		OnHistory();
	}
}

void DlgFiles::ToolHistoryBothUp(Bar& bar)
{
	bar.Add(display_style == D_COMP && history.GetCursor() > 0 && comp_history_index > 0, t_("Newer + compare"), THISBACK(OnHistoryBothUp))
		.Key(K_CTRL | K_ALT_UP)
		.Help(t_("Move current & compared version up one history entry"));
}

void DlgFiles::OnHistoryBothUp()
{
	if(display_style == D_COMP && history.GetCursor() > 0 && comp_history_index > 0) {
		comp_history_index--;
		history.SetCursor(history.GetCursor() - 1);
	}
}

void DlgFiles::ToolHistoryBothDown(Bar& bar)
{
	bar.Add(display_style == D_COMP && history.GetCursor() + 1 < history.GetCount()
		&& comp_history_index + 1 < history.GetCount(), t_("Older + compare"), THISBACK(OnHistoryBothDown))
		.Key(K_CTRL | K_ALT_DOWN)
		.Help(t_("Move to older history entry"));
}

void DlgFiles::OnHistoryBothDown()
{
	if(display_style == D_COMP && history.GetCursor() + 1 < history.GetCount()
	&& comp_history_index + 1 < history.GetCount()) {
		comp_history_index++;
		history.SetCursor(history.GetCursor() + 1);
	}
}

void DlgFiles::ToolHistoryRestore(Bar& bar)
{
	bar.Add(history.GetCursor() > 0, t_("Overwrite current"), THISBACK(OnHistoryRestore))
		.Help(t_("Overwrite current file with selected archive version"));
}

void DlgFiles::OnHistoryRestore()
{
	if(file_index < 0) {
		BeepExclamation();
		return;
	}
	String treefn = job.GetTreePath(cinfo[file_index].path);
	int filetime;
	String content = GetHistory(filetime);
	if(!IsNull(content) && PromptOKCancel(NFormat(t_("Overwrite file [* \1%s\1] with version dated %`?"),
		treefn, FromUvsTime(filetime))))
		try {
			if(!SaveFileBackup(treefn, content))
				throw Exc(NFormat(t_("Error saving file '%s'."), treefn));
			if(!IsNull(filetime))
				FileSetTime(treefn, FromUvsTime(filetime));
		}
		catch(Exc e) {
			ShowExc(e);
		}
}

void DlgFiles::OnFileList()
{
	UpdateHistory(filelist.Get(L_INDEX));
}

void DlgFiles::OnOpenArchiveConsole()
{
	if(console_splitter.GetZoom() == 0) {
		console_splitter.NoZoom();
		open_archive_console.SetImage(Uvs2Img::close_archive_console());
	}
	else {
		console_splitter.Zoom(0);
		open_archive_console.SetImage(CtrlImg::right_arrow());
	}
}

void DlgFiles::BeginSync()
{
	jobsync = true;
	jobcancel = false;
	psplitter.NoZoom();
	progress.Text(t_("Synchronizing..."));
	progress.Info(statusbar);
	statusbar.Sync();
	ConClear();
	ConClear(true);
	UpdateTree();
	Rescan();
}

void DlgFiles::EndSync()
{
	jobsync = false;
	UpdateTree();
	Rescan();
	statusbar = Null;
}

void DlgFiles::DoSyncMerge(String repository)
{
	BeginSync();
	try {
		DirInfo repdirinfo;
		String repfn = AppendFileName(repository, "uvs.$dir");
		String repfile = LoadFile(repfn);
		if(IsNull(repfile))
			throw Exc(NFormat(t_("File '%s' not found."), repfn));
		try {
			repdirinfo = ReadDir(repfile);
		}
		catch(Exc e) {
			throw Exc(NFormat(t_("Error in file '%s': %s"), repfn, e));
		}
		ConLine(NFormat(t_("Importing archive '%s'."), repository));
		last_download.Clear();
		for(int i = 0; i < repdirinfo.GetCount(); i++) {
			FileInfo repinfo = repdirinfo[i];
			String reprel = repdirinfo.GetKey(i), ixreprel = ToLower(reprel);
			if(IsSyncTime()) {
				SetProgressCount(i, repdirinfo.GetCount());
				if(IsCanceled()) {
					SaveLocalDir(Null);
					throw AbortExc();
				}
			}
			if(job.IsExcluded(reprel))
				continue;
			try {
				String repfile = AppendFileName(repository, NativePath(reprel));
				String repcurdata = LoadFile(repfile);
				if(repcurdata.IsVoid()) {
					ConLine(NFormat(t_("File '%s' not found."), repfile));
					continue;
				}
				String repverfile = repfile + ".$v";
				String repverdata = LoadFile(repverfile);
				if(repverdata.IsVoid()) {
					ConLine(NFormat(t_("File '%s' not found."), repverfile));
					continue;
				}
				FileVerInfo mergever = ReadVer(repverdata, repinfo, repcurdata);
				int myx = cinfo.Find(ixreprel);
				if(myx < 0) {
					myx = cinfo.GetCount();
					cinfo.Add(ixreprel);
					cinfo[myx].path = reprel;
				}
				ComplexFileInfo& cfi = cinfo[myx];
				bool binary = job.IsBinary(cfi.path);
				String myfile = job.GetLocalPath(cfi.path), myverfile = myfile + ".$v";
				String mycurdata;
				bool locedit = cfi.IsTreeEdit() && !IsNull(cfi.tree_time);
				bool savetree = false;
				if(IsNull(cfi.local.filetime)) {
					String myfile = job.GetLocalPath(reprel), myver = myfile + ".$v";
					RealizePath(myfile);
					if(!::SaveFile(myver, AsString(mergever)))
						throw Exc(NFormat(t_("error saving file '%s'."), myver));
					if(!binary)
						repcurdata = DoCr(repcurdata, job.usecr);
					if(!::SaveFile(myfile, repcurdata))
						throw Exc(NFormat(t_("error saving file '%s'."), myfile));
					cfi.local = repinfo;
					savetree = true;
				}
				else {
					mycurdata = LoadFile(myfile);
					FileVerInfo myver = ReadVer(LoadFile(myverfile), cfi.local, mycurdata);
					int oldmyver = myver.GetCount();
					if(MergeVer(myver, mergever)) {
						RealizePath(myfile);
						if(!::SaveFile(myverfile, AsString(myver)))
							throw Exc(NFormat(t_("error saving file '%s'."), myverfile));
						if(cfi.local.filetime < repinfo.filetime) {
							if(!binary)
								repcurdata = DoCr(repcurdata, job.usecr);
							if(!::SaveFile(myfile, repcurdata))
								throw Exc(NFormat(t_("error saving file '%s'."), myfile));
							cfi.local = repinfo;
							savetree = true;
						}
					}
				}
				if(savetree) {
					String tfile = job.GetTreePath(reprel);
					String tdata = (cfi.local.deleted ? String(Null) : repcurdata);
					if(locedit) {
						String old_tdata = LoadFile(tfile);
						if(old_tdata == tdata)
							ConLine(NFormat(t_("Identical changes: %s"), reprel));
						else {
							ConLine(NFormat(t_("Conflict: %s"), reprel));
							tdata = ConflictDiff(mycurdata, old_tdata, tdata);
						}
					}
					if(IsNull(tdata) && cfi.local.deleted)
						DeleteFile(tfile);
					else {
						RealizePath(tfile);
						if(!binary)
							tdata = DoCr(tdata, job.usecr);
						if(!::SaveFileTime(tfile, tdata, FromUvsTime(cfi.local.filetime)))
							throw Exc(NFormat(t_("error saving file '%s'."), tfile));
					}
				}
			}
			catch(Exc e) {
				throw Exc(NFormat(t_("File '%s': %s"), reprel, e));
			}
		}
		SaveLocalDir(Null);
		ConLine(NFormat(t_("Archive import from folder '%s' finished successfully."), repository));
	}
	catch(Exc e) {
		ConLine("\n" + e);
		ShowExc(e);
	}
	EndSync();
}

void DlgFiles::AddLRU(String fn)
{
	lrulist.NewEntry(fn);
	int f = FindIndex(recent_projects, fn);
	if(f >= 0)
		recent_projects.Remove(f);
	recent_projects.Insert(0, fn);
	if(recent_projects.GetCount() >= 10)
		recent_projects.SetCount(10);
}

void DlgFiles::RemoveLRU(String fn)
{
	lrulist.RemoveEntry(fn);
	int f = FindIndex(recent_projects, fn);
	if(f >= 0)
		recent_projects.Remove(f);
}

void DlgFiles::DoSyncExport()
{
	BeginSync();
	ftp_client.WhenProgress = THISBACK(OnFtpProgress);
	bool is_locked = false;

	try {
		OpenFTP();
		LockHost();
		is_locked = true;
//		ConAdd(t_("Last full block: server - "));
		UvsHostIndex hostindex = GetHostIndex();
/*
		if(hostindex.last_full) {
			ConLine(FormatInt(hostindex.last_full));
			DoSyncBackup(job.local_backup, job.localrepository);
			ConLine(t_("Removing existing local archive"));
			RemoveLocal();
			Array<UvsDataBlock> blocks = Download(ftp, hostindex.last_full, hostindex.last_update);
			cinfo.Clear();
			for(int i = 0; i < blocks.GetCount(); i++) {
				const UvsDataBlock& block = blocks[i];
				for(int d = 0; d < block.download.GetCount(); d++) {
					String file = block.download.GetKey(d), ixfile = ToLower(file);
					int prev = cinfo.Find(ixfile);
					String lfile = job.GetLocalPath(prev >= 0 ? cinfo[prev].path : file), verfile = lfile + ".$v";
					String ldata = block.data[d];
					String verdata;
					if(!block.version.IsEmpty())
						verdata = block.version[d];
					else if(prev >= 0) {
						String old_data = LoadFile(lfile);
						String old_ver = LoadFile(verfile);
						verdata << Diff(ldata, old_data)
						<< "#" << cinfo[prev].local.filetime << " " << AsCString(cinfo[prev].local.author) << "\n"
						<< old_ver;
					}
					RealizePath(lfile);
					if(!::SaveFile(lfile, ldata))
						throw Exc(NFormat(t_("Error saving file '%s'."), lfile));
					if(!::SaveFile(verfile, verdata))
						throw Exc(NFormat(t_("Error saving file '%s'."), verfile));
				}
				SetLocal(cinfo, block.download);
				blocks[i].Cleanup();
			}
			SaveLocalDir(hostindex.last_update);
		}
		else
			ConLine(t_("empty"));
*/

		UpdateTree();
		UvsDataBlock block;
		ConLine(t_("Preparing full update block"));
		for(int i = 0; i < cinfo.GetCount(); i++) {
			if(IsSyncTime()) {
				SetProgressCount(i, cinfo.GetCount());
				if(IsCanceled())
					throw AbortExc();
			}
			const ComplexFileInfo& cfi = cinfo[i];
			if(!IsNull(cfi.local.filetime)) {
				block.download.Add(cfi.path, cfi.local);
				String file = job.GetLocalPath(cfi.path);
				String data = LoadFile(file), ver = LoadFile(file + ".$v");
				if(data.IsVoid() && !cfi.local.deleted)
					throw Exc(NFormat(t_("Error reading file '%s'."), file));
				block.data.Add(data);
				block.version.Add(ver);
			}
		}
		hostindex.last_full = ++hostindex.last_update;
		ConLine(t_("Compressing update block"));
		save_start_msecs = msecs();
		String out = block.Compress(THISBACK(OnSaveGate));
		ConLine(NFormat(t_("Saving update block %d - %d B"), (hostindex.last_full), out.GetLength()));
		String nb = FormatInt(hostindex.last_full) + ".udb";
		SaveRetry(nb, out);
		SaveIndexFile(hostindex);
		SaveLocalDir(hostindex.last_update);
		is_locked = false;
		ConLine(t_("Unlocking server archive"));
		DeleteRetry("lock");
		ftp_client.Close();
		ConLine(t_("Full update block saved successfully."));
	}
	catch(Exc e) {
		if(ftp_client.IsOpen() && is_locked) {
			ConAdd(t_("\nExport aborted, unlocking server archive"));
			ConLine(ftp_client.Delete("lock") ? String(t_(" - OK")) : String(t_(" - error: ") + ftp_client.GetError()));
		}
		ftp_client.Close();
		ConLine("\n" + e);
		ShowExc(e);
	}
	EndSync();
}

void DlgFiles::RemoveDeep(String path, bool base)
{
	Vector<String> files, folders;
	FindFile ff;
	if(ff.Search(AppendFileName(path, "*")))
		do
			if(ff.IsFile()) {
				if(!base || stricmp(GetFileExtPos(ff.GetName()), ".udb"))
					files.Add(ff.GetName());
			}
			else if(ff.IsFolder())
				folders.Add(ff.GetName());
		while(ff.Next());
	while(!files.IsEmpty()) {
		String fn = AppendFileName(path, files.Pop());
		if(!DeleteFile(fn))
			ConLine(NFormat(t_("Cannot delete file '%s'."), fn));
	}
	while(!folders.IsEmpty()) {
		String fn = AppendFileName(path, folders.Pop());
		RemoveDeep(fn, false);
#ifdef PLATFORM_POSIX
		if(rmdir(fn))
#else
		if(!RemoveDirectory(fn))
#endif
			ConLine(NFormat(t_("Cannot remove folder '%s'"), fn));
	}
}

void DlgFiles::RemoveLocal()
{
	RemoveDeep(NativePath(job.localrepository));
}

void DlgFiles::SaveIndexFile(const UvsHostIndex& hostindex)
{
	ConLine(t_("Saving index file"));
	int retry = 0;
	for(;;)
		try {
			SaveRetry("uvs.new", hostindex.Compress());
			ftp_client.Delete("uvs.old");
			ftp_client.Rename("uvs.idx", "uvs.old");
			if(!ftp_client.Rename("uvs.new", "uvs.idx"))
				throw Exc(t_("Error updating index file 'uvs.idx'."));
			return;
		}
		catch(Exc e) {
			if(++retry > RETRIES)
				throw;
			ConLine(NFormat(t_("%s, retrying (%d of %d)"), e, retry, RETRIES));
		}
}

void DlgFiles::SaveRetry(String file, String data)
{
	int retry = 0;
	for(;;) {
		this->progress.Text(NFormat(t_("Writing file: %s"), file));
		statusbar.Sync();
		save_start_msecs = msecs();
		ftp_client.WhenProgress = THISBACK(OnFtpSaveGate);
		bool done = ftp_client.Save(file, data);
		ftp_client.WhenProgress = THISBACK(OnFtpProgress);
		if(done)
			return;
		if(IsCanceled())
			throw AbortExc();
		if(++retry > RETRIES)
			throw Exc(NFormat(t_("Error saving file '%s': %s"), file, ftp_client.GetError()));
		ConLine(NFormat(t_("Error saving file '%s', retrying (%d of %d); FTP error: %s"), file, retry, RETRIES, ftp_client.GetError()));
		ftp_client.Close();
		OpenFTP();
	}
}

String DlgFiles::LoadRetry(String file, bool allow_empty)
{
	int retry = 0;
	for(;;) {
		this->progress.Text(NFormat(t_("Reading file: %s"), file));
		statusbar.Sync();
		load_start_msecs = msecs();
		ftp_client.WhenProgress = THISBACK(OnFtpLoadGate);
		String s = ftp_client.Load(file);
		ftp_client.WhenProgress = THISBACK(OnFtpProgress);
		if(IsCanceled())
			throw AbortExc();
		if(!IsNull(s))
			return s;
		if(++retry > RETRIES) {
			if(allow_empty)
				return s;
			throw Exc(NFormat(t_("Error reading file '%s': %s"), file, ftp_client.GetError()));
		}
		if(!allow_empty)
			ConLine(NFormat(t_("Error reading file '%s', retrying (%d of %d); FTP error: %s"), file, retry, RETRIES, ftp_client.GetError()));
		OpenFTP(true);
	}
}

void DlgFiles::DeleteRetry(String file)
{
	int retry = 0;
	while(!ftp_client.Delete(file)) {
		if(++retry > RETRIES)
			throw Exc(NFormat(t_("Error deleting file '%s': %s"), file, ftp_client.GetError()));
		ConLine(NFormat(t_("Error deleting file '%s', retrying (%d of %d); FTP error: %s"), file, retry, RETRIES, ftp_client.GetError()));
		OpenFTP(true);
	}
}

void DlgFiles::OnFileCursor()
{
	String text;
	if(fileview.IsShown()) {
		Point pt = fileview.GetColumnLine(fileview.GetCursor());
		text = NFormat(t_(" Ln %d, Col %d"), pt.y + 1, pt.x + 1);
	}
	file_cursor.SetLabel(text);
}

void DlgFiles::UpdateCompFont()
{
	int f = ~comp_font;
	if(f > 0) {
		Font text = Courier(f), number = Courier(f - min(4, f - 10));
		compleft.SetFont(text, number);
		compright.SetFont(text, number);
	}
}

GUI_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);
	SetDefaultCharset(CHARSET_WIN1250);
	String cfgfile = ConfigFile();

	//Ctrl::ShowRepaint = 30;

	FileStream strm;
	if(strm.Open(cfgfile, FileStream::READ)) {
		SerializeGlobalConfigs(strm);
		strm.Close();
		if(strm.IsError())
			Exclamation(NFormat(t_("Error reading configuration file [* \1%s\1]."), cfgfile));
	}
	void RunDlgFiles();
	RunDlgFiles();
	bool ok = strm.Open(cfgfile, FileStream::CREATE);
	if(ok) {
		SerializeGlobalConfigs(strm);
		strm.Close();
		if(strm.IsError())
			ok = false;
	}
	if(!ok)
		Exclamation(NFormat(t_("Error saving configuration file [* \1%s\1]."), cfgfile));
}
