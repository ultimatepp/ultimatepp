#define IMAGECLASS DiagramImg
#define IMAGEFILE <RichEdit/Diagram.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <RichEdit/Diagram.lay>
#include <CtrlCore/lay.h>

struct DiaRichEdit : RichEdit {
	bool Key(dword key, int count) override;

	Event<> WhenEnter;
	Event<> WhenEsc;
};

class ColumnPopUp : public Ctrl {
	int  cursor = -1;

	void Paint(Draw& w) override;
	void MouseMove(Point p, dword keyflags) override;
	void LeftUp(Point, dword keyflags) override;
	void MouseLeave() override;
	void Deactivate() override;

public:
	void PopUp(const Rect& sr, Ctrl *owner);

	int Execute(const Rect& sr, Ctrl *owner);
	int  Execute();

	int  columns = 4;
	Size isz = Size(64, 32);
	int  count = 18;

	Event<int>                    WhenSelect;
	Event<Draw&, Size, int, bool> WhenPaintItem;

	ColumnPopUp();
};

const VectorMap<String, Vector<Tuple<int, String>>>& UnicodeSymbols();

String SelectFontSymbolSvg(Sizef& sz);

class DiagramEditor : public Ctrl, Diagram::PaintInfo {
public:
	void Paint(Draw& w) override;
	Image CursorImage(Point p, dword keyflags) override;
	void LeftDown(Point p, dword keyflags) override;
	void LeftDouble(Point p, dword keyflags) override;
	void MouseMove(Point p, dword keyflags) override;
	void LeftUp(Point, dword keyflags) override;
	void RightDown(Point p, dword keyflags) override;
	void RightUp(Point p, dword keyflags) override;
	void HorzMouseWheel(Point p, int zdelta, dword keyflags) override;
	void MouseWheel(Point p, int zdelta, dword keyflags) override;
	bool Key(dword key, int count) override;
	void Layout() override;
	void Skin() override;

private:
	struct DropColumns : public MultiButton, public Display {
		ColumnPopUp popup;
		void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const override;
		
		DropColumns();
	};

	Diagram        data;

	bool           allow_dark_content = false;
	bool           dark_content = false;

	Point          sizehandle = Point(0, 0);
	Point          draghandle = Point(0, 0);
	Point          dragstart = Point(0, 0);
	Point          dragcurrent = Point(0, 0);
	Rectf          dragfrom = Rect(0, 0, 0, 0);
	Pointf         drag_cp = Point(0, 0);
	double         base_rotate = 0;
	Vector<Point2> sdragfrom;
	bool           doselection = false; // we are doing rect selection
	bool           grid = true; // snap to grid
	bool           edit_text = false; // text editor is visible
	int            zoom_percent = 100;
	int            paint_ms = 0;
	
	struct Cn : Moveable<Cn> {
		int mi; // item index
		int ci; // connection index
		int li; // line item index
		int pi; // point index
	};
	
	bool           moving = false; // moving hysteresis
	bool           moved = false; // move detection (for fast paints)
	
	Vector<Cn>     conns; // connections, created at the drag start, updates line connections

	BinUndoRedo    undoredo;

	ToolBar     toolbar;
	DropColumns shape, line_start, line_end, line_dash, line_width;
	DiaRichEdit text_editor;

	ColorButton ink, paper;
	
	int         tool = -1;
	int         tool_count = 2; // TODO
	DiagramItem tl[2];

	ScrollBars  sb;

	void SetupDark(ColorPusher& c) const;
	bool IsDarkContent() const;

	void               CancelSelection();
	bool               IsCursor() const   { return cursor >= 0 && cursor < data.item.GetCount(); }
	DiagramItem&       CursorItem();
	const DiagramItem& CursorItem() const { return const_cast<DiagramEditor *>(this)->CursorItem(); }
	Rectf  GetCursorRect() const          { return IsCursor() ? data.item[cursor].GetRect() : Null; }
	void   SetCursor(int i);
	void   KillCursor()                   { SetCursor(-1); }
	Point  GetSizeHandle(Point p) const;
	Point  GetHandle(int item, Point p) const;
	void   TheBar(Bar& bar);
	void   SetBar();
	void   SyncEditor();
	void   SyncEditorRect();
	void   Sync();
	void   Reset();
	void   ResetUndo();
	void   Commit();
	int    FindItem(Point p) const;
	String GetCurrent();
	bool   SetCurrent(const String& s);
	void   StartText();
	void   FinishText();
	void   MoveFrontBack(bool back);
//	void   ChangeSize();
	void   Zoom();
	double GetZoom() const               { return DPI(1) * 0.01 * zoom_percent; }
	void   Map(Point& p);
	Image  MakeIcon(DiagramItem& m, Size isz);
	Size   IconSz()                      { return Size(DPI(24), DPI(16)); }
	Image  ShapeIcon(int i);
	Image  CapIcon(int start, int end);
	Image  DashIcon(int i);
	Image  WidthIcon(int i);
	void   PrepareConns();
	void   UseConns();
	void   Grid(int shape, Point& p);
	void   Grid(const DiagramItem& m, Point& p) { Grid(m.shape, p); }
	void   ChangeSize();
	void   PopPaint(Draw& w, const Image& m, bool sel);
	void   Shapes(ColumnPopUp& shape);
	void   Caps(ColumnPopUp& m, bool left);
	void   Dashes(ColumnPopUp& m);
	void   Widths(ColumnPopUp& m);


	void   FixPositions();
	void   ForEachConst(Event<const DiagramItem&> fn) const;
	void   ForEach(Event<DiagramItem&> fn);
	enum {
		ATTR_SHAPE = 0x0001,
		ATTR_CAP0 = 0x0002,
		ATTR_CAP1 = 0x0004,
		ATTR_WIDTH = 0x0008,
		ATTR_DASH = 0x0010,
		ATTR_INK = 0x0020,
		ATTR_PAPER = 0x0040,
		ATTR_ALL = 0xffffffff
	};
	void   SetAttrs(DiagramItem& m, dword attrs);
	void   SetAttrs(dword attr);
	void   GetAttrs(const DiagramItem& m);
	void   GetAttrs();

	void   ComputeAspectSize(DiagramItem& m, Sizef& sz1, Sizef& sz2);

	DiagramItem& AddItem(int shape);

	void   Copy();
	void   Cut();
	void   Paste();
	void   Delete();
	void   Duplicate();
	void   SelectAll();

	void   Align(bool horz, int align);

public:
	String Save() const;
	bool   Load(const String& s);

	DiagramEditor& DarkContent(bool b = true);
	DiagramEditor& AllowDarkContent(bool b = true);

	DiagramEditor();
};
