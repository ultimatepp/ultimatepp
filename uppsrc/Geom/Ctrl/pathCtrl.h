//////////////////////////////////////////////////////////////////////
// pathCtrl: Ctrl-related path functions.

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::

namespace Upp {

class PathEditorCtrl : public DragDropCtrl, public CtrlFrame
{
public:
	typedef PathEditorCtrl CLASSNAME;
	PathEditorCtrl();

	void               SetOwner(PathStyle& s) { style = &s; }
	void               SetCoords(Ctrl& c)     { coords = &c; }

	void               PumpTraces(bool write);

	virtual void       FrameLayout(Rect& rc);
	virtual void       FrameAddSize(Size& sz);
	virtual void       FramePaint(Draw& draw, const Rect& rc);
	virtual void       FrameAdd(Ctrl& ctrl);
	virtual void       FrameRemove();

	virtual void       Layout();
	virtual void       Paint(Draw& draw);

	virtual Image      CursorImage(Point pt, dword keyflags);

	Pointf             ClientToUnits(Point pt) const;
	Point              UnitsToClient(Pointf pt) const;

	Rectf              ClientToUnits(const Rect& rc) const;
	Rect               UnitsToClient(const Rectf& rc) const;

	Pointf             GetScale() const  { return Pointf(hruler.GetScale(), vruler.GetScale()); }
	Pointf             GetDelta() const  { return Pointf(hruler.GetDelta(), vruler.GetDelta()); }

	Pointf             Snap(Pointf pt) const;
	Pointf             ClientToSnap(Point pt) const;

	virtual bool       Key(dword key, int repcnt);

	virtual bool       Push(Point pt, dword keyflags);
	virtual void       Drag(Point pt, Point last, Point next, dword keyflags);
	virtual void       Drop(Point pt, Point end, dword keyflags);
	virtual void       Click(Point pt, dword keyflags);

	virtual void       MouseMove(Point pt, dword keyflags);
	virtual void       RightDown(Point pt, dword keyflags);

	const Vector<int>& GetSelection() const { return selection.GetKeys(); }
	Rectf              GetSelectionExtent() const;
	int                GetSelectionLeader() const;
	void               RefreshSelection();
	void               ClearSelection();
	void               AddSelection(const Vector<int>& list);
	void               AddSelection(int object)    { AddSelection(Vector<int>() << object); }
	void               XorSelection(const Vector<int>& list);
	void               XorSelection(int object)    { XorSelection(Vector<int>() << object); }
	void               RemoveSelection(const Vector<int>& list);
	void               RemoveSelection(int object) { RemoveSelection(Vector<int>() << object); }
	void               SetSelection(const Vector<int>& list);
	void               SetSelection(int object)    { SetSelection(Vector<int>() << object); }
	bool               IsSelection() const         { return !selection.IsEmpty(); }

	Rectf              GetFullExtent() const;
	int                FindObject(Pointf pt) const;
	Vector<int>        FindObject(const Rectf& rc) const;
	int                AddObject(const PathStyle::Trace& trace);
	void               SetObject(int i, const PathStyle::Trace& trace);
	void               RefreshObject(int i);

	bool               WriteClipboard();
	bool               ReadClipboard();

	int                GetTrackStyle(Pointf pt, Pointf& start) const;
	int                GetDragSize(Pointf pt) const;

	void               UpdateSample();

public:
	TOOL(Edit)
		TOOL(EditDelete)
		TOOL(EditSelectAll)
		TOOL(EditCut)
		TOOL(EditCopy)
		TOOL(EditPaste)
		TOOL(EditColor)

	TOOL(View)
		TOOL(ViewZoomIn)
		TOOL(ViewZoomOut)
		TOOL(ViewZoomFull)
		TOOL(ViewZoomHorzIn)
		TOOL(ViewZoomHorzOut)
		TOOL(ViewZoomHorzFull)
		TOOL(ViewZoomVertIn)
		TOOL(ViewZoomVertOut)
		TOOL(ViewZoomVertFull)
		TOOL(ViewPan)

	TOOL(Setup)
		TOOL(SetupGrid)
		TOOL(SetupSnap)
		TOOL(SetupRuler)
		TOOL(SetupAxis)
		TOOL(SetupStyle)
		TOOL(SetupSetup)

	void               UpdateSetup();
	void               UpdateScroll();
	void               OnScroll();
	void               OnSampleSize();
	void               OnSampleWidth();

public:
	class Setup
	{
	public:
		Setup() : do_grid(true), do_snap(true), do_ruler(true), do_axis(true), sample_size(40), sample_width(5), grid(0.1), snap(0.1) {}

		void   Serialize(Stream& stream);

	public:
		bool   do_grid;
		bool   do_snap;
		bool   do_ruler;
		bool   do_axis;
		int    sample_size;
		int    sample_width;
		double grid;
		double snap;
	};

	class ViewPlotter : public Plotter
	{
	public:
		ViewPlotter(PathEditorCtrl *ctrl);

	public:
		ViewDraw draw;
	};

	friend class ViewPlotter;

public:
	enum
	{
		GAP = 10,
		HGAP = 100,
		MAX_WIDTH = 100,
	};

	Callback           WhenRescan;
	PathStyle         *style;
	Ctrl              *coords;
	Setup              setup;

protected:
	Rect               outer_sample, inner_sample;
	RulerCtrl          vruler, hruler;
	HScrollBar         hscroll;
	VScrollBar         vscroll;
	Button             zoom_horz_in, zoom_horz_out, zoom_horz_full;
	Button             zoom_vert_in, zoom_vert_out, zoom_vert_full;
	Button             zoom_full;
	Label              sample_size_tag, sample_width_tag;
	DropList           sample_size;
	EditIntSpin        sample_width;

	Array<PathStyle::Trace> traces;
	Index<int>         selection;

	enum { EDIT_NORMAL, EDIT_ZOOM, EDIT_PAN };
	int                edit_mode;
	enum
	{
		DRAG_NONE, DRAG_INSERT, DRAG_SELECT, DRAG_MOVE, DRAG_TRACK,
		DRAG_BEGIN, DRAG_SEGMENT, DRAG_END,
		DRAG_ZOOM, DRAG_PAN,
	};
	int                drag_mode;
	int                track_style;
	Pointf             track_start;
	Rectf              track_limit;
};

bool RunDlgPathStyleSetup(PathStyleMisc& style);

//////////////////////////////////////////////////////////////////////

class PathStyleMapCtrl : public Ctrl
{
public:
	typedef PathStyleMapCtrl CLASSNAME;
	PathStyleMapCtrl();

	void                Set(PathStyleMap *m); // sorts the map as well

	virtual void        Layout();
	virtual void        Paint(Draw& draw);

	virtual bool        Key(dword key, int repcnt);

	virtual void        LeftDown(Point pt, dword keyflags);
	virtual void        LeftDouble(Point pt, dword keyflags);
	virtual void        LeftUp(Point pt, dword keyflags);
	virtual void        RightDown(Point pt, dword keyflags);

	int                 ClientToIndex(Point pt) const;
	Size                ClientToRange(const Rect& rc) const; // cx = min, cy = max
	Rect                IndexToClient(int i) const;

	bool                IsValid(int i) const { return i >= 0 && i < map -> GetCount(); }
	bool                IsCursor() const     { return IsValid(cursor); }
	int                 GetCursor() const    { return cursor; }
	void                SetCursor(int cursor);
	void                SetCursor(const String& name) { SetCursor(map -> FindSortName(name)); }
	void                KillCursor()         { SetCursor(-1); }

	void                RefreshItem(int i);

	TOOL(Local)
		TOOL(New)
		TOOL(Edit)
		TOOL(Rename)
		TOOL(Copy)
		TOOL(Export)
		TOOL(Import)
		TOOL(Remove)

public:
	Callback1<Bar&>     WhenBar;
	Callback            WhenLeftDouble;
	Callback            WhenRename;

private:
	void                OnScroll();
	static Rect         GetEditRect(int right, int bottom);
	static bool         InEditRect(int right, int bottom, Point mouse);

private:
	enum
	{
		GAP = 10,
		IGAP = 4,
		WIDTH = 200,
		TEXT_HEIGHT = 35,
		LINE_HEIGHT = 20,
		HEIGHT = TEXT_HEIGHT + LINE_HEIGHT + 2 * IGAP,
		FONT_HEIGHT = TEXT_HEIGHT / 2,
	};

	PathStyleMap       *map;

	Size                count, cell, pos_add, total;
	Point               offset, gap_offset;
	int                 scroll_pos;

	VScrollBar          scroll;

	int                 cursor;
};

//////////////////////////////////////////////////////////////////////

bool RunDlgPathEditor(PathStyle& style, const String& title = Null);
bool RunDlgPathStyleMap(PathStyleMap& map, String& style, bool editor = false, bool read_only = false);

//////////////////////////////////////////////////////////////////////

class PathStyleCtrl : public DataPusher
{
public:
	PathStyleCtrl();

	void             PathMap(PathStyleMap *pm) { path_map = pm; display.PathMap(pm); }
	PathStyleMap    *GetPathMap() const        { return path_map; }

private:
	virtual void     DoAction();

private:
	PathStyleMap    *path_map;
	PathStyleDisplay display;
};

//////////////////////////////////////////////////////////////////////

}
