#ifndef _TextDiffCtrl_TextDiffCtrl_h
#define _TextDiffCtrl_TextDiffCtrl_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {

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
	virtual void   LeftUp(Point pt, dword keyflags);
	virtual bool   Key(dword key, int repcnt);

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

	typedef TextCompareCtrl CLASSNAME;

public:
	Callback       WhenScroll;

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

	TextCompareCtrl();
};

class TextDiffCtrl : public Splitter {
	TextCompareCtrl left;
	TextCompareCtrl right;

public:
	void Set(Stream& l, Stream& r);
	void Set(const String& l, const String& r);
	void InsertFrameLeft(CtrlFrame& f)                     { left.InsertFrame(0, f); }
	void InsertFrameRight(CtrlFrame& f)                    { right.InsertFrame(0, f); }
	void AddFrameLeft(CtrlFrame& f)                        { left.AddFrame(f); }
	void AddFrameRight(CtrlFrame& f)                       { right.AddFrame(f); }
	
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
	
	DiffDlg();
};

FileSel& DiffFs();

struct FileDiff : DiffDlg {
	FrameTop<DataPusher> r;
	
	virtual void Open();
	void Execute(const String& f);
	
	typedef FileDiff CLASSNAME;
	
	FileDiff(FileSel& fs);
	
	FileSel& fs;
};

struct PatchDiff : FileDiff {
	PatchDiff(FileSel& fs) : FileDiff(fs) {}
	
	virtual void Open();
	
	void Copy(FileIn& in, FileIn& oin, int& l, int ln, int n);
	void LoadDiff(const char *fn);
};

};

#endif
