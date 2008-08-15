#ifndef _Circles_Circles_h
#define _Circles_Circles_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Circles/Circles.lay>
#include <CtrlCore/lay.h>

struct CircleShape {
	Point point;
	int   radius;

	Rect GetRect() const          { return Rect(point - radius, point + radius); }
};

class CircleView;

class CircleDoc {
	struct CircleUndo {
		bool        insert;
		int         index;
		CircleShape shape;
	};

	Rect                 bound;
	Array<CircleShape>   data;
	Array<CircleUndo>    undo;
	Array<CircleUndo>    redo;
	Array<CircleView>    view;

	void               Update(const Rect& s);
	void               UpdateFull();
	void               UndoRedo(Array<CircleUndo>& src, Array<CircleUndo>& dst);

public:
	void               AddCircle(Point p, int radius);
	void               RemoveCircle(int i);
	const CircleShape& operator[](int i) const  { return data[i]; }
	int                GetCount() const         { return data.GetCount(); }
	Rect               GetBoundingRect() const  { return bound; }

	void               Clear();

	bool   IsUndo() const                       { return undo.GetCount(); }
	bool   IsRedo() const                       { return redo.GetCount(); }
	void   Undo()                               { UndoRedo(undo, redo); }
	void   Redo()                               { UndoRedo(redo, undo); }

	void   NewView();
	void   RemoveView(CircleView *view);
	int    GetViewCount() const                 { return view.GetCount(); }

	bool   LoadText(const char *text);
	String SaveText() const;

	void   Serialize(Stream& s);
	bool   LoadBinary(const String& data);
	String SaveBinary() const;

	CircleDoc()                                 { Clear(); }
};

class CircleView : public TopWindow {
public:
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword);
	virtual void RightDown(Point p, dword);
	virtual void Close();
	virtual void Layout();

private:
	CircleDoc&   doc;
	ScrollBars   sb;
	MenuBar      menu;
	ToolBar      tool;

	void  Scroll();

	void  SetBar();
	void  FileBar(Bar& bar);
	void  EditBar(Bar& bar);
	void  MainMenu(Bar& bar);
	void  MainBar(Bar& bar);

public:
	void UpdateView(const Rect& r);

	typedef CircleView CLASSNAME;

	CircleView(CircleDoc& doc);
};

class Circles : public WithCirclesLayout<TopWindow> {
public:
	typedef Circles CLASSNAME;
	Circles();
};

#endif
