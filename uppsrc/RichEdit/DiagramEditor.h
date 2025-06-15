#define IMAGECLASS DiagramImg
#define IMAGEFILE <RichEdit/Diagram.iml>
#include <Draw/iml_header.h>

//#define LAYOUTFILE <Diagram/Diagram.lay>
//#include <CtrlCore/lay.h>

struct DiaRichEdit : RichEdit {
	bool Key(dword key, int count) override;
	
	Event<> WhenEnter;
	Event<> WhenEsc;
};

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
	void Layout() override;

private:
	Diagram data;
	
	Point          draghandle = Point(0, 0);
	Point          dragstart = Point(0, 0);
	Point          dragcurrent = Point(0, 0);
	Rect           dragfrom = Rect(0, 0, 0, 0);
	Vector<Point2> sdragfrom;
	bool           doselection = false; // we are doing rect selection
	bool           grid = true; // snap to grid
	bool           edit_text = false; // text editor is visible
	bool           creating = false; // right-click adding new item (allow resize)
	int            zoom_percent = 100;

	BinUndoRedo       undoredo;
	
	int         tool = 0;
	ToolBar     toolbar;
	DropList    shape, line_start, line_end, line_width, line_dash;
	DiaRichEdit text_editor;

	ColorButton ink, paper;
	
	ScrollBars  sb;

	void               CancelSelection();
	bool               IsCursor() const   { return cursor >= 0 && cursor < data.item.GetCount(); }
	DiagramItem&       CursorItem();
	const DiagramItem& CursorItem() const { return const_cast<DiagramEditor *>(this)->CursorItem(); }
	Rect   GetCursorRect() const          { return IsCursor() ? data.item[cursor].GetRect() : Null; }
	void   SetCursor(int i);
	void   KillCursor()                   { SetCursor(-1); }
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
	
	void   SetAttrs();
	void   GetAttrs();
	
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

	DiagramEditor();
};
