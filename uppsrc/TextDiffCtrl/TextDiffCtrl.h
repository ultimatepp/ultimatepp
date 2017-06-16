#ifndef _TextDiffCtrl_TextDiffCtrl_h
#define _TextDiffCtrl_TextDiffCtrl_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {

#define IMAGECLASS DiffImg
#define IMAGEFILE <TextDiffCtrl/Diff.iml>
#include <Draw/iml_header.h>

class TextSection
{
public:
	TextSection(int start1, int count1, int start2, int count2, bool same)
		: start1(start1), count1(count1), start2(start2), count2(count2), same(same) {}

public:
	int      start1;
	int      count1;
	int      start2;
	int      count2 : 31;
	unsigned same   : 1;
};

Array<TextSection> CompareLineMaps(const Vector<String>& l1, const Vector<String>& l2);
Vector<String>     GetLineMap(Stream& stream);
Vector<String>     GetFileLineMap(const String& path);
Vector<String>     GetStringLineMap(const String &s);

class TextCompareCtrl : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	virtual void   Layout();
	virtual void   MouseWheel(Point pt, int zdelta, dword keyflags);
	virtual void   MouseMove(Point pt, dword keyflags);
	virtual void   LeftDown(Point pt, dword keyflags);
	virtual void   LeftDouble(Point pt, dword keyflags);
	virtual void   LeftUp(Point pt, dword keyflags);
	virtual void   RightDown(Point p, dword keyflags);
	virtual bool   Key(dword key, int repcnt);

private:
	void           SelfScroll();
	void           PairScroll(TextCompareCtrl *ctrl);
	void           UpdateWidth();
	WString        ExpandTabs(const wchar *line) const;
	int            MeasureLength(const wchar *line) const;
	bool           GetSelection(int& l, int& h);
	void           DoSelection(int y, bool shift);
	void           Copy();
	int            GetLineNo(int y, int& yy);
	int            GetMatchLen(const wchar *s1, const wchar *s2, int len);
	bool           LineDiff(bool left, Vector<LineEdit::Highlight>& hln, Color eq_color,
	                        const wchar *s1, int l1, int h1,
	                        const wchar *s2, int l2, int h2, int depth);

private:
	struct Line {
		Line() : number(Null), level(0) {}
		int    number;
		bool   diff;
		String text;
		int    level;
		String text_diff;
		int    number_diff;
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
	int            cursor;
	int            anchor;
	bool           gutter_capture;
	bool           show_line_number;
	bool           show_white_space;
	bool           show_diff_highlight;
	bool           change_paper_color;
	bool           left = false;

	typedef TextCompareCtrl CLASSNAME;

public:
	Event<>        WhenScroll;
	Callback2<int, int> WhenLeftDouble;
	Event<Vector<LineEdit::Highlight>&, const WString&> WhenHighlight;

	void           SetCount(int c);
	void           AddCount(int c);
	int            GetCount() const { return lines.GetCount(); }

	void           SetFont(Font f, Font nf);
	void           SetFont(Font f);
	Font           GetFont() const { return font; }
	Font           GetNumberFont() const { return number_font; }

	void           NumberBgColor(Color bg)  { number_bg = bg; Refresh(); }
	Color          GetNumberBgColor() const { return number_bg; }

	void           AutoHideSb(bool ssb=true){ scroll.AutoHide(ssb); }
	void           ShowSb(bool ssb)         { scroll.ShowY(ssb); }
	void           HideSb()                 { ShowSb(false); }

	void           Gutter(int size)         { gutter_width = size; Refresh(); }
	void           NoGutter()               { gutter_width = 0; Refresh(); }

	void           TabSize(int t);
	int            GetTabSize() const { return tabsize; }

	void           Set(int line, String text, bool diff, int number, int level, String text_diff, int number_diff);
	String         GetText(int line) const { return lines[line].text; }
	bool           GetDiff(int line) const { return lines[line].diff; }
	int            GetNumber(int line) const { return lines[line].number; }
	int            GetNumberDiff(int line) const { return lines[line].number_diff; }

	Point          GetPos() const;
	void           SetPos(Point pos);

	int            GetSb() const { return scroll.Get().y; }
	void           SetSb(int y)  { scroll.Set(0, y); }

	void           ClearSelection()           { cursor = Null; Refresh(); }
	void           SetSelection(int l, int h) { cursor = l; anchor = h; }

	void           ShowLineNumber(bool sln)   { show_line_number = sln; Refresh(); }
	void           HideLineNumber()           { ShowLineNumber(false); }

	void           ShowWhiteSpace(bool sws)   { show_white_space = sws; Refresh(); }
	void           HideWhiteSpace()           { ShowWhiteSpace(false); }

	void           DiffHighlight(bool dh)     { show_diff_highlight = dh; Refresh(); }
	void           NoDiffHighlight()          { DiffHighlight(false); }

	void           ChangePaperColor(bool cpc) { change_paper_color = cpc; Refresh(); }
	void           NoChangePaperColor()       { ChangePaperColor(false); }
	
	void           SetLeft()                  { left = true; }

	Event<>        ScrollWhen(TextCompareCtrl& pair) { return THISBACK1(PairScroll, &pair); }

	TextCompareCtrl();
};

struct TextDiffCtrl : public Splitter {
	TextCompareCtrl left;
	TextCompareCtrl right;

	typedef TextDiffCtrl CLASSNAME;

	void Set(Stream& l, Stream& r);
	void Set(const String& l, const String& r);
	void InsertFrameLeft(CtrlFrame& f)                     { left.InsertFrame(0, f); }
	void InsertFrameRight(CtrlFrame& f)                    { right.InsertFrame(0, f); }
	void AddFrameLeft(CtrlFrame& f)                        { left.AddFrame(f); }
	void AddFrameRight(CtrlFrame& f)                       { right.AddFrame(f); }
	void SetFont(Font f, Font nf)                          { left.SetFont(f, nf); right.SetFont(f, nf); }
	void SetFont(Font f)                                   { left.SetFont(f); right.SetFont(f); }

	void GetLeftLine(int number, int line);
	void GetRightLine(int number, int line);

	Callback1<int> WhenLeftLine;
	Callback1<int> WhenRightLine;

	TextDiffCtrl();
};

struct DiffDlg : public TopWindow {
	TextDiffCtrl         diff;
	FrameTop<StaticRect> p;
	DataPusher           l;
	Button               write;
	String               editfile;
	String               extfile;

	typedef DiffDlg CLASSNAME;

	void Write();
	void Execute(const String& f);

	static Event<const String&, Vector<LineEdit::Highlight>&, const WString&> WhenHighlight;

	DiffDlg();
};

FileSel& DiffFs();

struct FileDiff : DiffDlg {
	FrameTop<DataPusher> r;

	virtual void Open();
	void Execute(const String& f);

	typedef FileDiff CLASSNAME;

	String GetExtPath() const { return ~r; }

	FileDiff(FileSel& fs);

	FileSel& fs;
};

struct PatchDiff : FileDiff {
	PatchDiff(FileSel& fs) : FileDiff(fs) {}

	virtual void Open();

	void Copy(FileIn& in, FileIn& oin, int& l, int ln, int n);
	void LoadDiff(const char *fn);
};

class DirDiffDlg : public TopWindow {
public:
	virtual bool HotKey(dword key);

private:
	Splitter                   files_diff;
	ParentCtrl                 files_pane;
	FileList                   files;

	SelectDirButton            seldir1;
	WithDropChoice<EditString> dir1;
	SelectDirButton            seldir2;
	WithDropChoice<EditString> dir2;
	Option                     hidden;
	Button                     compare;
	Label                      info;
	
	Option                     removed, added, modified;
	EditString				   find;
	Button					   clearFind;

	FrameTop<ParentCtrl>       left, right;
	EditString                 lfile, rfile;
	Button                     copyleft, copyright;
	
	Array<Tuple<String, String, String, int>> list;

	void GatherFilesDeep(Index<String>& files, const String& base, const String& path);
	void Compare();
	void ShowResult();
	void ClearFiles();
	void File();
	void Copy(bool left);

public:
	TextDiffCtrl               diff;

	typedef DirDiffDlg CLASSNAME;

	void SetFont(Font fnt)                      { diff.SetFont(fnt); }
	void Dir1(const String& dir)                { dir1 <<= dir; }
	void Dir2(const String& dir)                { dir2 <<= dir; }
	void Dir1AddList(const String& dir)         { dir1.AddList(dir); }
	void Dir2AddList(const String& dir)         { dir2.AddList(dir); }

	String GetLeftFile() const                  { return ~lfile; }
	String GetRightFile() const                 { return ~rfile; }

	DirDiffDlg();
};


};

#endif
