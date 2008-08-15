#ifndef _VectorDes_VectorDes_h_
#define _VectorDes_VectorDes_h_

#include <CtrlLib/CtrlLib.h>
#include <Web/Web.h>
#include <RichText/RichText.h>
#include "VectorObj.h"

One<Ctrl> CreateVectorFontCtrl();

class DragDropCtrl : public Ctrl
{
public:
	DragDropCtrl();

	// drag & drop interface
	virtual bool Push(Point pt, dword keyflags);
	virtual void Drag(Point pt, Point last, Point next, dword keyflags);
	virtual void Drop(Point pt, Point end, dword keyflags);
	virtual void Click(Point pt, dword keyflags);
	static const int DBLCLK = 0x40000000;

	virtual void DragRect(const Rect& last, const Rect& next, dword keyflags);
	virtual void DropRect(const Rect& rc, dword keyflags);

	void         DragShow(bool _show = true);
	void         DragHide()                    { DragShow(false); }
	void         DragStop(bool accept, dword keyflags);
	void         DragStop(bool accept = false) { DragStop(accept, last_key); }

	bool         IsDragging() const            { return state == ON || state == HIDDEN; }
	bool         IsPushed() const              { return state != OFF; }

	// control overrides
	virtual void LeftDown(Point pt, dword keyflags);
	virtual void LeftDouble(Point pt, dword keyflags);
	virtual void LeftUp(Point pt, dword keyflags);
	virtual void MouseMove(Point pt, dword keyflags);
	virtual bool Key(dword key, int repcnt);

private:
	Point        start, last;
	dword        last_key;
	char         state;
	enum { OFF, ON, POSSIBLE, HIDDEN };
};

class VectorCtrl : public DragDropCtrl
{
public:
	typedef VectorCtrl CLASSNAME;
	VectorCtrl();

	virtual void Layout();
	virtual void Paint(Draw& draw);

	virtual void SetData(const Value& value);
	virtual Value GetData() const;

	virtual Image CursorImage(Point pt, dword keyflags);
	virtual void MouseMove(Point pt, dword keyflags);
	virtual void RightDown(Point pt, dword keyflags);

	virtual bool Key(dword key, int repcount);

	virtual bool Push(Point pt, dword keyflags);
	virtual void Drag(Point pt, Point last, Point next, dword keyflags);
	virtual void Drop(Point pt, Point end, dword keyflags);
	virtual void Click(Point pt, dword keyflags);

	Ctrl&        GetArgParent()                   { return arg_parent; }

	void         SetZoom(Zoom zoom);
	Zoom         GetZoom() const                  { return zoom; }

	void         SetImageSize(Size sz);
	Size         GetImageSize() const             { return image.GetSize(); }

	void         SetImage(VectorImage img);
	VectorImage  GetImage() const                 { return image; }

	void         ClearSelection();
	void         AddSelection(int sel)            { AddSelection(sel, -1); }
	void         AddSelection(int sel, int track);
	void         AddSelection(const VectorMap< int, Index<int> >& sel);
	void         XorSelection(const VectorMap< int, Index<int> >& sel);
	void         PickSelection(pick_ VectorMap< int, Index<int> >& sel);
	void         RemoveSelection(const Vector<int>& rm_index);
	const Vector<int>& GetSelected() const { return selection.GetKeys(); }

	bool         IsSelection() const       { return !selection.IsEmpty(); }
	int          GetSelCount() const       { return selection.GetCount(); }
	int          GetSelObject(int i) const { return selection.GetKey(i); }
	const Index<int>& GetSelTrack(int i) const { return selection[i]; }
	bool         IsSelected(int i) const   { return selection.Find(i) >= 0; }
	const Index<int> &FindSelTrack(int i) const;

	void         PostSyncArgEditors();
	void         SyncArgEditors();

	void         SyncCoords(Point coords);

	Point        ClientToImage(Point pt) const;
	Point        ImageToClient(Point pt) const;

	Rect         ClientToImage(Rect rc) const;
	Rect         ImageToClient(Rect rc) const;

	Point        ClientToSnap(Point pt) const;

	int          AddObject(One<VectorObject> object);
	void         RemoveObject(int i);
	void         MoveObject(int i, int j);

	void         InsertMenu(Bar& bar);
	void         LocalMenu(Bar& bar);
	void         EditMenu(Bar& bar);
	void         AlignHorzMenu(Bar& bar);
	void         AlignVertMenu(Bar& bar);
	void         SizeMenu(Bar& bar);
	void         ViewMenu(Bar& bar);

	void         DoEditSetup();

	void         DoEditCopy();
	void         DoEditCut();
	void         DoEditPaste();
	void         DoEditDuplicate();
	void         DoEditEdit();
	void         DoEditDelete();

	bool         Copy();
	void         PickPaste(VectorImage img);

	void         DoAlign(Alignment halign, Alignment valign);
	void         DoSameSize(bool cx, bool cy);

	void         DoViewZoomIn();
	void         DoViewZoomOut();
	void         DoViewZoomFull();
	void         DoViewZoomArea();
	void         DoViewPan();

public:
	Callback     WhenArgLayout;
	Callback     WhenUserSelect;
	Callback     WhenRescan;
	Callback     WhenImageSize;

	struct Setup {
		Setup();

		bool   grid;
		bool   snap;
		bool   snap_to_objects;
		bool   grid_above_objects;
		Size   grid_size;
		Size   snap_size;
		int    grid_style;
		enum { GRID_LINES, GRID_POINTS };

		void   Serialize(Stream& stream);

		void   LoadGlobal();
		void   SaveGlobal();
	};

	Setup        setup;

private:
	void         KeyMove(int dx, int dy, bool resize);

	void         OnScroll();
	void         OnNewObject(const VectorInfo *i);
	void         OnArgEdit(int i, Ctrl *ctrl);
	void         OnArgMulti(Button *button, Ctrl *editor);
	void         PaintGrid(ScalingDraw& scaled);

//	bool         GetNewStyle(int& i1, int& i2);
//	Rect         ApplyRect(Point pt, Point end, Rect rc) const;

	Point        GetTrackMode(Point pt, Point& nearest) const;
	Scaling      GetImageScaling() const;
	Size         ClientOffset() const { return scrollbars.Get() - GAP; }

private:
	VectorImage  image;
	VectorMap< int, Index<int> > selection;
	One<VectorObject> new_object;
	One<VectorObject> empty_object;
	enum { IC_IDENT, IC_WIDTH, IC_HEIGHT, IC_IMAGE, IC_COUNT };

	int          editmode;
	enum { EDIT_TRACK, EDIT_ZOOM, EDIT_PAN };
	enum { MAXDIST2 = 20 * 20 };
	Point        trackmode;
	Point        pan_start;
	Point        track_start;
	Image        new_icon;

	Zoom         zoom;
	Size         pixel_size;
	ScrollBars   scrollbars;
	enum { GAP = 5 };

	StaticRect   arg_parent;
	EditField    tool_coords;

	struct ArgEditor {
		String        name;
		int           vtype;
		int           editor;
		Value         value;
		bool          multi;
		Vector<Point> obj_args;
	};
	struct ArgEditCtrl {
		Label         label;
		One<Ctrl>     editor;
	};

	ArrayMap<String, ArgEditor> sel_args;
	Array<Ctrl>      sel_editors;
	TimeCallback     tc_sync_editors;
	TimeCallback     tc_refresh;
};

class VectorDesHost
{
};

class VectorDes : public StaticRect
{
	friend class VectorDesigner;

public:
	typedef VectorDes CLASSNAME;
	VectorDes(VectorDesHost& host);
	virtual ~VectorDes();

	virtual bool   Key(dword key, int repcnt);
	virtual void   ChildGotFocus();

	void           CloseDesigner();
	void           FrameFocus();

	void           LoadVecFile(CParser& parser);
	String         SaveVecFile();

	void           LoadSingle(VectorImage img);
	VectorImage    SaveSingle() const;

	Size           GetImageSize() const { return editor.GetImageSize(); }

	void           EditMenu(Bar& bar);

public:
	Callback       WhenSyncTitle;
	Callback       WhenRescan;

private:
	void           Rescan() { toolbar.Set(THISBACK(EditMenu)); WhenRescan(); }
	void           KeyScan(int dx, int dy, bool resize);

	void           OnImageListCursor();
	void           OnImageSize();
	void           OnUserImageSize();

	void           OnArgLayout();
	void           OnArgScroll();
	void           OnUserSelect();

	void           PostObjectCursor();
	void           OnObjectCursor();

	void           ObjectMenu(Bar& bar);
	void           ObjectDelete();
	void           ObjectMoveBack();
	void           ObjectMoveFront();
//	void           ObjectProperties();

	void           OnObject();

	struct ConvertObject : Convert
	{
		ConvertObject(VectorCtrl& owner) : owner(owner) {}
		virtual Value Format(const Value& value) const
		{
			if(!IsNull(value) && IsNumber(value)) {
				int i = value;
				if(i >= 0 && i < owner.GetImage().GetCount())
					return AsString(owner.GetImage()[i]);
			}
			return Null;
		}
		VectorCtrl& owner;
	};

private:
	VectorDesHost& host;
//	String         cfgname;
//	String         filename;
	bool           single;

	Splitter       splitter;
	ArrayCtrl      image_list;
	enum { IC_IDENT, IC_WIDTH, IC_HEIGHT, IC_IMAGE, IC_COUNT };
	Splitter       vsplitter;
	ArrayCtrl      object_list;
	Splitter       asplitter;
	StaticRect     arg_pane;
	VScrollBar     arg_scroll;
	VectorCtrl     editor;
	ConvertObject  cv_obj_list;

	Array<VectorImage> images;

	EditField      list_ident;
	EditIntSpin    list_width;
	EditIntSpin    list_height;

	ToolBar        toolbar;

	TimeCallback   tc_object_cursor;
};

RichObjectType *RichObjectTypeVectorImage();
bool            RunVectorImageEditor(VectorImage& image);

#endif
