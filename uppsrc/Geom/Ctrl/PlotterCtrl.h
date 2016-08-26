namespace Upp {

class PlotterDragDrop;

class PlotterCtrl : public DragDropCtrl, public CtrlFrame
{
public:
	class ViewPlot : public Plotter
	{
	public:
		ViewPlot(PlotterCtrl& ctrl, int extra_gap = 0);
		ViewPlot(PlotterCtrl& ctrl, Pointf scale, Pointf delta, int extra_gap = 0);
		ViewPlot(PlotterCtrl& ctrl, const Matrixf& preform, int extra_gap = 0);

	private:
		ViewDraw viewdraw;
	};
	friend class ViewPlot;

	typedef PlotterCtrl CLASSNAME;
	PlotterCtrl();
	virtual ~PlotterCtrl();

	virtual void            FrameAdd(Ctrl& ctrl);
	virtual void            FrameRemove();
	virtual void            FrameLayout(Rect& rc);
	virtual void            FrameAddSize(Size& sz);

	virtual void            Layout();
	virtual void            Paint(Draw& draw);
//	void                    AsyncPaint();
	bool                    IsPainting() const               { return is_painting; }

	virtual void            LostFocus()                      { ResetPush(); }

	ImageDraw&              BeginBufferPaint();
	virtual void            EndBufferPaint();

	virtual void            Plot(Plotter& plotter) = 0;
	virtual void            AbortPlot()                      {}

	void                    BufferPaint(bool bp = true)      { buffer_paint = bp; RefreshBuffer(); }
	bool                    IsBufferPaint() const            { return buffer_paint; }
	void                    RefreshBuffer();
	void                    RefreshBuffer(const Rect& rc);

	void                    BufferPan(bool bp = true)        { buffer_pan = bp; }
	bool                    IsBufferPan() const              { return buffer_pan; }
	void                    PanOffset(Point o);
	void                    ClearPanOffset()                 { PanOffset(Point(0, 0)); }
	Point                   GetPanOffset() const             { return pan_offset; }

	void                    ShowScroll(bool ssc)             { show_scroll = ssc; Layout(); }
	bool                    IsScrollShown() const            { return show_scroll; }

	void                    MaxScale(double d)               { max_scale.cx = max_scale.cy = d; }
	void                    MaxScale(Sizef ms)               { max_scale = ms; }
	Sizef                   GetMaxScale() const              { return max_scale; }

	void                    Gap(Rect g)                      { gap = g; Layout(); }
	void                    Gap(int i)                       { gap = Rect(i, i, i, i); Layout(); }
	Rect                    GetGap() const                   { return gap; }

	virtual Image           CursorImage(Point pt, dword keyflags);
	virtual bool            Push(Point pt, dword keyflags);
	virtual void            Drag(Point start, Point prev, Point curr, dword keyflags);
	virtual void            Drop(Point start, Point end, dword keyflags);
	virtual void            Click(Point pt, dword keyflags);
	virtual void            Cancel();
	virtual bool            Key(dword key, int repcnt);
	virtual void            MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void            MouseMove(Point pt, dword keyflags);

	virtual void            RefreshPos()                     {}
	virtual void            RefreshDragDrop()                { RefreshBuffer(); }

	Pointf                  FromClient(Point pt) const       { return (Pointf(pt) - delta) / scale; }
	Pointf                  FromClientNull(Point pt) const   { return IsNull(pt) ? Pointf(Null) : FromClient(pt); }
	Rectf                   FromClient(const Rect& rc) const;

	Pointf                  FromPushClient(Point pt) const   { return (Pointf(pt) - push_delta) / push_scale; }
	Pointf                  FromPushClientNull(Point pt) const { return IsNull(pt) ? Pointf(Null) : FromPushClient(pt); }

	Point                   ToClient(Pointf pt) const        { return PointfToPoint(Sizef(pt) * scale + delta); }
	Point                   ToClientNull(Pointf pt) const    { return IsNull(pt) ? Point(Null) : PointfToPoint(Sizef(pt) * scale + delta); }
	Rect                    ToClient(const Rectf& rc) const;

	Point                   ToPushClient(Pointf pt) const    { return PointfToPoint(Sizef(pt) * push_scale + push_delta); }
	Point                   ToPushClientNull(Pointf pt) const { return IsNull(pt) ? Point(Null) : ToPushClient(pt); }

	void                    SyncPush();

	void                    SetExtent(const Rectf& extent);
	const Rectf&            GetExtent() const                { return extent; }

	Rectf                   GetViewRect() const              { return FromClient(Rect(GetSize()).Deflated(gap)); }

	void                    ScrollInto(Rectf rc);
	void                    ScrollInto(Pointf pt);

	void                    SetAspectRatio(double aspect);
	void                    NoAspectRatio()                  { SetAspectRatio(0); }
	double                  GetAspectRatio() const           { return aspect; }
	bool                    IsAspectRatio() const            { return aspect != 0; }

	void                    SetAspectLock(bool a = true);
	void                    NoAspectLock()                   { SetAspectLock(false); }
	bool                    IsAspectLock() const             { return aspect_lock; }
	bool                    IsAspectLocked() const           { return IsAspectRatio() || IsAspectLock(); }

	void                    EnableLock(bool e = true);
	void                    DisableLock()                    { EnableLock(false); }
	bool                    IsLockEnabled() const            { return enable_lock; }\

	void                    SetScale(Sizef scale);
	void                    SetDelta(Pointf delta);
	void                    SetZoom(Sizef scale, Pointf delta);
	void                    SetZoom(double scale, Pointf delta);
	void                    SetZoomSc(Sizef scale);
	void                    SetZoomSc(double sx, double sy)  { SetZoomSc(Pointf(sx, sy)); }
	void                    SetZoomSc(double z);

	Sizef                   GetScale() const                 { return scale; }
	Sizef                   GetPushScale() const             { return push_scale; }
	double                  GetAvgScale() const;
	Pointf                  GetDelta() const                 { return delta; }
	Pointf                  GetPushDelta() const             { return push_delta; }
	
	bool                    IsReversedX() const              { return rev_x; }
	bool                    IsReversedY() const              { return rev_y; }
	
	bool                    IsReversing() const              { return scale.cx * scale.cy < 0; }

	Sizef                   GetPhysicalZoom() const;

	void                    AdjustPos(Pointf& delta, Point scpos) const;
	Sizef                   AdjustScale(Sizef sc) const;

	Pointf                  GetSc() const;
	void                    SetSc(Pointf center);

	void                    ZoomInX();
	void                    ZoomOutX();
	void                    ZoomFullX()                      { if(extent.left < extent.right) ZoomX(extent.left, extent.right); }
	void                    ZoomX(double min, double max, bool add_gap = true);
	void                    ZoomInY();
	void                    ZoomOutY();
	void                    ZoomFullY()                      { if(extent.top < extent.bottom) ZoomY(extent.right, extent.bottom); }
	void                    ZoomY(double min, double max, bool add_gap = true);
	void                    ZoomIn()                         { ZoomInX(); ZoomInY(); }
	void                    ZoomOut()                        { ZoomOutX(); ZoomOutY(); }
	void                    ZoomFull()                       { Zoom(extent, false); }
	void                    Zoom(const Rectf& rc, bool keep_ratio = true, bool add_gap = true);

	void                    UserZoomInX();
	void                    UserZoomOutX();
	void                    UserZoomFullX();
	void                    UserZoomX(double min, double max);
	void                    UserZoomInY();
	void                    UserZoomOutY();
	void                    UserZoomFullY();
	void                    UserZoomY(double min, double max);
	void                    UserZoomIn();
	void                    UserZoomOut();
	virtual void            UserZoomFull();
	void                    UserAspectLock();
	void                    UserZoom(const Rectf& rc, bool keep_ratio = true);

	void                    PickDragDrop(One<PlotterDragDrop> dd);
	One<PlotterDragDrop>    ClearDragDrop();
	PlotterDragDrop        *GetDragDrop()                    { return ~drag_drop; }
	const PlotterDragDrop  *GetDragDrop() const              { return ~drag_drop; }

	void                    UpdateMousePos();
	Pointf                  GetMousePos() const              { return mouse_pos; }

	void                    DoSetFocus()                     { SetWantFocus(); }
	
	void                    ResetPush()                      { reset_push = true; }

	TOOL(View)
		TOOL(ViewZoomInX)
		TOOL(ViewZoomOutX)
		TOOL(ViewZoomFullX)
		TOOL(ViewZoomInY)
		TOOL(ViewZoomOutY)
		TOOL(ViewZoomFullY)
		TOOL(ViewZoomIn)
		TOOL(ViewZoomOut)
		TOOL(ViewZoomFull)
		TOOL(ViewAspectLock)
		TOOL(ViewPan)

	void                    PostRefresh();

public:
	Callback                WhenRescan;
	Callback                WhenZoom;
	Callback                WhenUserZoom;
	Callback                WhenMousePos;

protected:
	void                    AdjustPos(Point scpos, int xymask); // 1 = x, 2 = y

protected:
	bool                    rev_x, rev_y, aspect_lock, enable_lock;
	Rect                    gap;
	Alignment               halign, valign;
	Color                   background;
	Sizef                   scale;
	Pointf                  delta;
	Sizef                   push_scale;
	Pointf                  push_delta;
	Sizef                   max_scale;
	Pointf                  mouse_pos;
	Rectf                   extent;
	double                  aspect;

	Image                   paint_buffer;
	One<ImageDraw>          paint_draw;
	bool                    is_painting;
	bool                    abort_repaint;
	bool                    buffer_paint;
	bool                    buffer_pan;
	Point                   pan_offset;

private:
	void                    OnHScroll();
	void                    OnVScroll();
	void                    Refresh0()                       { RefreshBuffer(); }

private:
	TimeCallback            tcb_refresh;
	Size                    old_size;
	One<PlotterDragDrop>    drag_drop;
	Pointf                  drag_start;
	HScrollBar              hscroll;
	VScrollBar              vscroll;
	Button                  horz_in, horz_out, horz_full;
	Button                  vert_in, vert_out, vert_full;
	Button                  full;
	bool                    reset_push;
	bool                    show_scroll;
	bool                    lock_drag_drop;
	bool                    lock_short_drag_drop;
	int                     drag_mode;
	enum { DRAG_NONE, DRAG_CUSTOM, DRAG_PAN, DRAG_ZOOM_IN, DRAG_ZOOM_OUT };
};

template <class T>
inline const T *GetDragDrop(const PlotterCtrl *ctrl, T * = 0)
{
	if(const T *p = dynamic_cast<const T *>(ctrl->GetShortDragDrop()))
		return p;
	return dynamic_cast<const T *>(ctrl->GetDragDrop());
}

template <class T>
inline T *GetDragDrop(PlotterCtrl *ctrl, T * = 0)
{
	return dynamic_cast<T *>(ctrl->GetDragDrop());
}

template <class T>
inline bool IsDragDrop(const PlotterCtrl *ctrl, T * = 0)
{
	return dynamic_cast<const T *>(ctrl->GetDragDrop());;
}
// make member template as soon as possible

class PlotterDragDrop
{
public:
	virtual ~PlotterDragDrop() {}

	virtual PlotterCtrl&    GetOwner() = 0;

	virtual void            Plot(Plotter& plotter)                                    {}
	virtual bool            Push(Pointf pt, dword keyflags)                           { return true; }
	virtual Image           Cursor(Pointf pt, dword keyflags, bool dragging) const    { return Image::Arrow(); }
	virtual void            Drag(Pointf pt, Pointf prev, Pointf curr, dword keyflags);
	virtual void            DragRect(const Rectf& prev, const Rectf& curr, dword keyflags);
	virtual void            Drop(Pointf pt, Pointf end, dword keyflags);
	virtual void            DropRect(const Rectf& rc, dword keyflags)                    {}
	virtual void            Click(Pointf pt, dword keyflags)                          {}
	virtual bool            Key(dword key)                                            { return false; }
	virtual void            Cancel()                                                  {}
};

class EmptyDragDrop : public PlotterDragDrop
{
public:
	EmptyDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl& GetOwner()                      { return owner; }

	virtual bool         Push(Pointf pt, dword keyflags) { return false; }

private:
	PlotterCtrl&         owner;
};

class ZoomInDragDrop : public PlotterDragDrop
{
public:
	ZoomInDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    GetOwner() { return owner; }

	virtual Image           Cursor(Pointf pt, dword keyflags, bool dragging) const;
	virtual void            DropRect(const Rectf& rc, dword keyflags);
	virtual void            Click(Pointf pt, dword keyflags);

private:
	PlotterCtrl&            owner;
};

class ZoomOutDragDrop : public PlotterDragDrop
{
public:
	ZoomOutDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    GetOwner() { return owner; }

	virtual Image           Cursor(Pointf pt, dword keyflags, bool dragging) const;
	virtual bool            Push(Pointf pt, dword keyflags);
	virtual void            DropRect(const Rectf& rc, dword keyflags);
	virtual void            Click(Pointf pt, dword keyflags);

private:
	PlotterCtrl&            owner;
};

class PanDragDrop : public PlotterDragDrop
{
public:
	PanDragDrop(PlotterCtrl& owner) : owner(owner) {}

	virtual PlotterCtrl&    GetOwner() { return owner; }

	virtual Image           Cursor(Pointf pt, dword keyflags, bool dragging) const;
	virtual bool            Push(Pointf pt, dword keyflags);
	virtual void            Drag(Pointf start, Pointf prev, Pointf curr, dword keyflags);
	virtual void            Drop(Pointf start, Pointf end, dword keyflags);
	virtual void            Cancel();

private:
	PlotterCtrl&            owner;
};

}
