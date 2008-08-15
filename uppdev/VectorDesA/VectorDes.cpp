#include "VectorDes.h"
#pragma hdrstop

#define LLOG(x) // RLOG(x)

#define IMAGEFILE <VectorDes/VectorDes.iml>
#define IMAGESPACE VecImg
#include <Draw/iml.h>

#define LAYOUTFILE <VectorDes/VectorDes.lay>
#include <CtrlCore/lay.h>

static const char setup_key[] = "vectordes-setup";

INITBLOCK {
	RegisterGlobalConfig(setup_key);
};

class VectorFontCtrl : public WithVectorDesFontLayout<StaticRect> {
public:
	typedef VectorFontCtrl CLASSNAME;
	VectorFontCtrl();

	virtual Value GetData() const;
	virtual void  SetData(const Value& v);
};

VectorFontCtrl::VectorFontCtrl()
{
	CtrlLayout(*this);
	VectorMap<String, int> face_map;
	for(int i = 0; i < Font::GetFaceCount(); i++)
		face_map.FindAdd(Font::GetFaceName(i), i);
	Vector<String> fname = face_map.PickKeys();
	Vector<int> find = face_map.PickValues();
	IndexSort(fname, find, GetLanguageInfo());
	for(int i = 0; i < fname.GetCount(); i++)
		font.Add(find[i], fname[i]);
	font <<= height <<= bold <<= italic <<= underline <<= strikeout
	<<= THISBACK(Action);
}

Value VectorFontCtrl::GetData() const
{
	Font out;
	out.Face(~font).Height(Nvl((int)~height, 0))
	.Bold(bold).Italic(italic).Underline(underline).Strikeout(strikeout);
	return out;
}

void VectorFontCtrl::SetData(const Value& v)
{
	Font f = v;
	font <<= f.GetFace();
	height <<= f.GetHeight();
	bold = f.IsBold();
	italic = f.IsItalic();
	underline = f.IsUnderline();
	strikeout = f.IsStrikeout();
}

One<Ctrl> CreateVectorFontCtrl()
{
	return new VectorFontCtrl;
}

class DlgVectorDesSetup : public WithVectorDesSetupLayout<TopWindow> {
public:
	typedef DlgVectorDesSetup CLASSNAME;
	DlgVectorDesSetup();

	bool Run(VectorCtrl::Setup& setup);

private:
	void OnSnap();
	void OnGrid();
};

DlgVectorDesSetup::DlgVectorDesSetup()
{
	CtrlLayoutOKCancel(*this, "Preferences");
	snap <<= THISBACK(OnSnap);
	grid <<= THISBACK(OnGrid);
}

bool DlgVectorDesSetup::Run(VectorCtrl::Setup& setup)
{
	CtrlRetriever rtvr;
	rtvr
		(grid, setup.grid)
		(grid_cx, setup.grid_size.cx)
		(grid_cy, setup.grid_size.cy)
		(grid_style, setup.grid_style)
		(grid_above_objects, setup.grid_above_objects)
		(snap, setup.snap)
		(snap_cx, setup.snap_size.cx)
		(snap_cy, setup.snap_size.cy)
		(snap_to_objects, setup.snap_to_objects)
	;
	snap_to_objects.Hide(); // temporary
	OnGrid();
	OnSnap();
	if(TopWindow::Run() != IDOK)
		return false;
	rtvr.Retrieve();
	return true;
}

void DlgVectorDesSetup::OnSnap()
{
	snap_cx.Enable(snap);
	snap_cy.Enable(snap);
}

void DlgVectorDesSetup::OnGrid()
{
	grid_cx.Enable(grid);
	grid_cy.Enable(grid);
	grid_style.Enable(grid);
}

inline Rect SortRect(Point p1, Point p2)
{
	return Rect(min(p1.x, p2.x), min(p1.y, p2.y), max(p1.x, p2.x) + 1, max(p1.y, p2.y) + 1);
}

bool IsDragDistance(Point pt1, Point pt2)
{
#ifdef PLATFORM_WIN32
	return tabs(pt1.x - pt2.x) >= GetSystemMetrics(SM_CXDRAG)
		|| tabs(pt1.y - pt2.y) >= GetSystemMetrics(SM_CYDRAG);
#endif
#ifdef PLATFORM_POSIX
	enum { CXDRAG = 4, CYDRAG = 4 };
	// todo? are there any CXDRAG / CYDRAG system metrics in LINUX?
	return tabs(pt1.x - pt2.x) >= CXDRAG || tabs(pt1.y - pt2.y) >= CYDRAG;
#endif
}

int GetRectDragMask(Rect rc, Point pt, int tolerance)
{
	Point center = rc.CenterPoint();
	int m = (tabs(rc.left   - pt.x) <= tolerance ?  1 : 0)
		|   (tabs(rc.top    - pt.y) <= tolerance ?  2 : 0)
		|   (tabs(rc.right  - pt.x) <= tolerance ?  4 : 0)
		|   (tabs(rc.bottom - pt.y) <= tolerance ?  8 : 0);
	if(m & 5)
		if((m & 10) || tabs(center.y - pt.y) <= tolerance)
			return m;
	if(m & 10)
		if((m & 5) || tabs(center.x - pt.x) <= tolerance)
			return m;
	return 0;
}

DragDropCtrl::DragDropCtrl()
: state(0)
{
}

bool DragDropCtrl::Push(Point pt, dword keyflags)
{
	return true;
}

void DragDropCtrl::Drag(Point pt, Point last, Point next, dword keyflags)
{
	Rect rc_last = Null, rc_next = Null;
	if(!IsNull(last))
		rc_last = RectSort(pt, last);
	if(!IsNull(next))
		rc_next = RectSort(pt, next);
	if(rc_last != rc_next)
		DragRect(rc_last, rc_next, keyflags);
}

void DragDropCtrl::Drop(Point pt, Point end, dword keyflags)
{
	DropRect(RectSort(pt, end), keyflags);
}

void DragDropCtrl::Click(Point pt, dword keyflags)
{
	// no-op, should be implemented in derived class
}

void DragDropCtrl::DragRect(const Rect& last, const Rect& next, dword keyflags)
{
	ViewDraw draw(this);
	DrawDragRect(draw, last, next, draw.GetClip(), 1, Yellow(), NULL);
}

void DragDropCtrl::DropRect(const Rect& rc, dword keyflags)
{
	// no-op, should be implemented in derived class
}

void DragDropCtrl::DragShow(bool _show)
{
	if(_show && state == HIDDEN) {
		Drag(start, Null, last, last_key);
		state = ON;
	}
	if(!_show && state == ON) {
		Drag(start, last, Null, last_key);
		state = HIDDEN;
	}
}

void DragDropCtrl::LeftDown(Point pt, dword keyflags)
{
	SetWantFocus();
	LLOG("DragDropCtrl::LeftDown -> " << pt << ", keyflags " << FormatIntHex(keyflags));
	if(Push(pt, last_key = keyflags))
	{ // begin drag & drop
		state = POSSIBLE;
		start = last = pt;
		SetCapture();
	}
}

void DragDropCtrl::LeftDouble(Point pt, dword keyflags)
{
	SetWantFocus();
	Click(pt, keyflags | DBLCLK);
}

void DragDropCtrl::DragStop(bool accept, dword keyflags)
{
	ReleaseCapture();
	DragHide();
	if(state == HIDDEN && accept)
		Drop(start, last, last_key = keyflags);
	else if(state == POSSIBLE && accept)
		Click(start, last_key = keyflags);
	state = OFF;
}

void DragDropCtrl::LeftUp(Point pt, dword keyflags)
{
	LLOG("DragDropCtrl::LeftUp -> " << pt);
	DragStop(true, keyflags);
}

void DragDropCtrl::MouseMove(Point pt, dword keyflags)
{
	LLOG("DragDropCtrl::MouseMove -> " << pt);
	if(keyflags != last_key)
		DragHide();
	if(state == POSSIBLE && IsDragDistance(pt, start))
	{
		state = ON;
		Drag(start, Null, last = pt, last_key = keyflags);
	}
	else if(state == ON || state == HIDDEN)
	{
		Point plast = (state == ON ? last : Point(Null));
		last = pt;
		last_key = keyflags;
		state = ON;
		Drag(start, plast, pt, last_key);
	}
}

bool DragDropCtrl::Key(dword key, int repcnt)
{
	if(key == K_ESCAPE)
	{
		DragStop(false);
		return true;
	}
	return Ctrl::Key(key, repcnt);
}

VectorCtrl::Setup::Setup()
{
	snap = grid = snap_to_objects = grid_above_objects = false;
	grid_style = GRID_POINTS;
	grid_size = Size(8, 8);
	snap_size = Size(4, 4);
}

void VectorCtrl::Setup::Serialize(Stream& stream)
{
	int version = 2;
	stream / version;
	stream.Pack(grid, snap, snap_to_objects, grid_above_objects);
	stream % grid_size % snap_size;
	if(version >= 2)
		stream / grid_style;
}

void VectorCtrl::Setup::LoadGlobal()
{
	LoadFromGlobal(*this, setup_key);
}

void VectorCtrl::Setup::SaveGlobal()
{
	StoreToGlobal(*this, setup_key);
}

VectorCtrl::VectorCtrl()
{
	BackPaint();

	editmode = EDIT_TRACK;

	tool_coords.SetReadOnly().NoWantFocus();

	SetFrame(InsetFrame());
	AddFrame(scrollbars);
	scrollbars.NoAutoHide().NoAutoDisable();
	scrollbars.WhenScroll = THISBACK(OnScroll);
	SetZoom(Zoom(1, 1));
	trackmode = Point(-1, -1);
	setup.LoadGlobal();
}

void VectorCtrl::SetData(const Value& value)
{
	SetImage(value);
}

Value VectorCtrl::GetData() const
{
	return image;
}

void VectorCtrl::SetZoom(::Zoom z)
{
	zoom = z;
	Layout();
}

void VectorCtrl::Layout()
{
	pixel_size = zoom * image.GetSize();
	scrollbars.Set(scrollbars, scrollbars.GetReducedViewSize(), pixel_size + 2 * GAP);
	Refresh();
}

Scaling VectorCtrl::GetImageScaling() const
{
	return Scaling(image.GetSize(), pixel_size);
}

void VectorCtrl::Paint(Draw& draw)
{
	DragHide();
	draw.DrawRect(draw.GetClip(), IsEnabled() ? SWhite() : SGray());
	Rect rc(pixel_size);
	rc.Offset(Point(GAP, GAP) - scrollbars);
	draw.DrawRect(rc, SLtGray());
	draw.Offset(rc.TopLeft());
	ScalingDraw scaled(&draw, GetImageScaling());
	if(!setup.grid_above_objects)
		PaintGrid(scaled);
	image.DesignPaint(scaled);
	Size sz = image.GetSize() * scaled;
	if(IsEditable() && IsEnabled()) {
		DrawHotRect(draw, sz.cx, (sz.cy >> 1) - 2, 5, 5, true, LtBlue());
		DrawHotRect(draw, (sz.cx >> 1) - 2, sz.cy, 5, 5, true, LtBlue());
		DrawHotRect(draw, sz.cx, sz.cy, 5, 5, true, LtBlue());
	}
	for(int i = 0; i < selection.GetCount(); i++)
		image[selection.GetKey(i)].SelPaint(scaled, selection[i], i == selection.GetCount() - 1);
	if(setup.grid_above_objects)
		PaintGrid(scaled);
	draw.End();
}

void VectorCtrl::PaintGrid(ScalingDraw& scaled)
{
	if(!setup.grid)
		return;
	if(scaled.X(setup.grid_size.cx) <= 2 || scaled.Y(setup.grid_size.cy) <= 2)
		return;
	Rect clip = scaled.draw->GetClip() & Rect(scaled.pixel_size);
	switch(setup.grid_style) {
		case Setup::GRID_LINES: {
			Size sz = image.GetSize();
			for(int i = 0; i < sz.cx; i += setup.grid_size.cx)
				scaled.draw->DrawRect(scaled.X(i), clip.top, 1, clip.Height(), SGray());
			for(int i = 0; i < sz.cy; i += setup.grid_size.cy)
				scaled.draw->DrawRect(clip.left, scaled.Y(i), clip.Width(), 1, SGray());
			break;
		}

		case Setup::GRID_POINTS: {
			Size count = image.GetSize() / setup.grid_size + 1;
			Buffer<int> xpos(count.cx), ypos(count.cy);
			for(int i = 0; i < count.cx; i++)
				xpos[i] = scaled.X(i * setup.grid_size.cx);
			for(int i = 0; i < count.cy; i++)
				ypos[i] = scaled.Y(i * setup.grid_size.cy);
			for(int y = 0; y < count.cy; y++)
				for(int x = 0; x < count.cx; x++)
					scaled.draw->DrawRect(xpos[x], ypos[y], 1, 1, SGray());
			break;
		}
	}
}

void VectorCtrl::MouseMove(Point pt, dword keyflags)
{
	SyncCoords(ClientToSnap(pt));
	DragDropCtrl::MouseMove(pt, keyflags);
}

Image VectorCtrl::CursorImage(Point pt, dword keyflags)
{
	Point mode = trackmode, nearest;
	if(!IsDragging())
		mode = GetTrackMode(pt, nearest);
	return Nvl(image.Cursor(mode, keyflags), Image::Arrow());
/*
	switch(mode) {
		case DM_NEW:
			return !IsNull(new_icon) ? new_icon : Image::Arrow();

		case DM_SELECT:
			return CtrlImg::HandCursor();

		case DM_POINT:
			return CtrlImg::SizeVeHo0();

		case DM_RECT: {
			switch(rm) {
				case 1: case 4: return CtrlImg::SizeHorz0();
				case 2: case 8: return CtrlImg::SizeVert0();
				case 1 | 2: case 4 | 8: return CtrlImg::SizeHoVe0();
				case 1 | 8: case 4 | 2: return CtrlImg::SizeVeHo0();
			}
			break;
		}

		case DM_MOVE: {
			return Image::SizeAll();
		}

		case DM_IMG_SIZE: {
			switch(rm) {
				case 1: return CtrlImg::SizeHorz0();
				case 2: return CtrlImg::SizeVert0();
				case 3: return CtrlImg::SizeHoVe0();
			}
			return Image::Arrow();
		}
	}

	return DragDropCtrl::CursorImage(pt, keyflags);
*/
}

void VectorCtrl::RightDown(Point pt, dword keyflags)
{
	if(IsEditable())
		MenuBar::Execute(this, THISBACK(LocalMenu), GetScreenView().TopLeft() + pt);
}

void VectorCtrl::LocalMenu(Bar& bar)
{
	InsertMenu(bar);
	bar.MenuSeparator();
	EditMenu(bar);
}

void VectorCtrl::EditMenu(Bar& bar)
{
	bar.Add("Edit", VecImg::edit(), THISBACK(DoEditEdit))
		.Check(!new_object && editmode == EDIT_TRACK);
	bar.MenuSeparator();
	bar.Add(IsSelection(), "Copy", CtrlImg::copy(), THISBACK(DoEditCopy))
		.Key(K_CTRL_C)
		.Help("Copy selected objects to the clipboard");
	bar.Add(IsSelection(), "Cut", CtrlImg::cut(), THISBACK(DoEditCut))
		.Key(K_CTRL_X)
		.Help("Remove selected object from the image and place them on the clipboard");
	bar.Add("Paste", CtrlImg::paste(), THISBACK(DoEditPaste))
		.Key(K_CTRL_V)
		.Help("Insert objects from the clipboard");
	bar.Add("Duplicate", THISBACK(DoEditDuplicate))
		.Key(K_CTRL_D)
		.Help("Create a copy of the selected objects");
	bar.Add(!selection.IsEmpty(), "Delete", THISBACK(DoEditDelete))
		.Key(K_DELETE)
		.Help("Delete selected object(s)");
	bar.MenuSeparator();
	bar.Add("Align horz", THISBACK(AlignHorzMenu));
	bar.Add("Align vert", THISBACK(AlignVertMenu));
	bar.Add("Equal size", THISBACK(SizeMenu));
	bar.Separator();
	bar.Add("Preferences", VecImg::edit_setup(), THISBACK(DoEditSetup))
		.Help("View / edit vector designer preferences");
}

void VectorCtrl::DoEditEdit()
{
	new_object = NULL;
	editmode = EDIT_TRACK;
	WhenRescan();
}

bool VectorCtrl::Copy()
{
	if(!IsSelection()) {
		BeepExclamation();
		return false;
	}
	VectorImage clipimg;
	clipimg.SetName("clip");
	for(int i = 0; i < selection.GetCount(); i++)
		clipimg.Add(image[selection.GetKey(i)].Copy());
	if(!WriteClipboardText(clipimg.SaveVec(0))) {
		Exclamation("Error writing system clipboard.");
		return false;
	}
	BeepInformation();
	return true;
}

void VectorCtrl::DoEditCopy()
{
	Copy();
}

void VectorCtrl::DoEditCut()
{
	if(Copy())
		DoEditDelete();
}

void VectorCtrl::PickPaste(VectorImage img)
{
	int insertpos = image.GetCount();
	if(!selection.IsEmpty()) {
		insertpos = selection.GetKey(0);
		for(int i = 1; i < selection.GetCount(); i++)
			insertpos = max(insertpos, selection.GetKey(i));
		insertpos++;
	}
	ClearSelection();
	for(int i = 0; !img.IsEmpty(); i++) {
		image.Insert(insertpos + i, img.Detach(0));
		AddSelection(insertpos + i);
	}
	UpdateActionRefresh();
}

void VectorCtrl::DoEditPaste()
{
	try {
		String s = ReadClipboardText();
		if(IsNull(s)) {
			BeepExclamation();
			return;
		}
		CParser parser(s);
		VectorImage clipimg;
		clipimg.LoadVec(parser);
		PickPaste(clipimg);
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void VectorCtrl::DoEditDuplicate()
{
	if(!IsSelection()) {
		BeepExclamation();
		return;
	}
	Point move(10, 10);
	if(setup.grid)
		move = setup.grid_size;
	VectorImage help;
	for(int i = 0; i < selection.GetCount(); i++) {
		One<VectorObject> obj = image[selection.GetKey(i)].Copy();
		obj->Track(VectorObject::TRACK_MOVE, Point(0, 0), move, 0);
		help.Add(obj);
	}
	PickPaste(help);
}

void VectorCtrl::DoEditDelete()
{
	if(selection.IsEmpty()) {
		BeepExclamation();
		return;
	}
	Vector<int> objix;
	objix <<= selection.GetKeys();
	ClearSelection();
	Sort(objix);
	for(int i = objix.GetCount(); --i >= 0;)
		image.Remove(objix[i]);
	UpdateActionRefresh();
	WhenUserSelect();
}

void VectorCtrl::AlignHorzMenu(Bar& bar)
{
	bool msel = (selection.GetCount() >= 2);
	bar.Add(msel, "Left", VecImg::AlignLeft(), THISBACK2(DoAlign, ALIGN_LEFT, ALIGN_NULL))
		.Key(K_SHIFT|K_ALT|K_LEFT);
	bar.Add(msel, "Center", VecImg::AlignHCenter(), THISBACK2(DoAlign, ALIGN_CENTER, ALIGN_NULL))
		.Key(K_SHIFT_F9);
	bar.Add(msel, "Right", VecImg::AlignRight(), THISBACK2(DoAlign, ALIGN_RIGHT, ALIGN_NULL))
		.Key(K_ALT|K_SHIFT|K_RIGHT);
}

void VectorCtrl::AlignVertMenu(Bar& bar)
{
	bool msel = (selection.GetCount() >= 2);
	bar.Add(msel, "Top", VecImg::AlignTop(), THISBACK2(DoAlign, ALIGN_NULL, ALIGN_TOP))
		.Key(K_ALT|K_SHIFT|K_UP);
	bar.Add(msel, "Center", VecImg::AlignVCenter(), THISBACK2(DoAlign, ALIGN_NULL, ALIGN_CENTER))
		.Key(K_F9);
	bar.Add(msel, "Bottom", VecImg::AlignBottom(), THISBACK2(DoAlign, ALIGN_NULL, ALIGN_BOTTOM))
		.Key(K_ALT|K_SHIFT|K_DOWN);
}

void VectorCtrl::DoAlign(Alignment halign, Alignment valign)
{
	if(selection.GetCount() < 2) {
		BeepExclamation();
		return;
	}
	int lkey = selection.TopKey();
	const Index<int>& leader = selection.Top();
	Point align = Null;
	for(int i = 0; i < leader.GetCount(); i++)
		if(leader[i] & VectorObject::ARG_MASK) {
			const VectorObject& obj = image[lkey];
			if(leader[i] & VectorObject::RECT_MASK) {
				Rect rc = obj.Info().RectArgs()->Get(&obj, leader[i] >> VectorObject::INDEX_SHIFT);
				switch(halign) {
					case ALIGN_LEFT:   align.x = rc.left; break;
					case ALIGN_CENTER: align.x = (rc.left + rc.right) >> 1; break;
					case ALIGN_RIGHT:  align.x = rc.right; break;
				}
				switch(valign) {
					case ALIGN_TOP:    align.y = rc.top; break;
					case ALIGN_CENTER: align.y = (rc.top + rc.bottom) >> 1; break;
					case ALIGN_BOTTOM: align.y = rc.bottom; break;
				}
			}
			else
				align = obj.Info().PointArgs()->Get(&obj, leader[i] >> VectorObject::INDEX_SHIFT);
			break;
		}
	if(IsNull(align.x) && halign != ALIGN_NULL || IsNull(align.y) && valign != ALIGN_NULL) {
		BeepExclamation();
		return;
	}
	for(int i = selection.GetCount() - 2; i >= 0; i--)
		image[selection.GetKey(i)].Align(halign, valign, align);
	UpdateActionRefresh();
}

void VectorCtrl::SizeMenu(Bar& bar)
{
	bool msel = (selection.GetCount() >= 2);
	bar.Add(msel, "Equal width", VecImg::SameWidth(), THISBACK2(DoSameSize, true, false))
		.Key(K_SHIFT_F8);
	bar.Add(msel, "Equal height", VecImg::SameHeight(), THISBACK2(DoSameSize, false, true))
		.Key(K_CTRL_F8);
	bar.Add(msel, "Equal size", VecImg::SameSize(), THISBACK2(DoSameSize, true, true))
		.Key(K_F8);
}

void VectorCtrl::DoSameSize(bool cx, bool cy)
{
	if(selection.GetCount() < 2) {
		BeepExclamation();
		return;
	}
	Rect box = image[selection.TopKey()].GetBox();
	if(IsNull(box)) {
		BeepExclamation();
		return;
	}
	Size sz(cx ? box.Width() : (int)Null, cy ? box.Height() : (int)Null);
	for(int i = selection.GetCount() - 2; i >= 0; i--)
		image[selection.GetKey(i)].EqualSize(sz);
	UpdateActionRefresh();
}

void VectorCtrl::InsertMenu(Bar& bar)
{
	int lng = GetCurrentLanguage();
	String current_type;
	if(new_object)
		current_type = new_object->Info().type;
	for(int i = 0; i < VectorObject::RegisteredObjects().GetCount(); i++) {
		const VectorInfo *e = VectorObject::RegisteredObjects()[i];
		bar.Add(e->GetName(lng), e->icon, THISBACK1(OnNewObject, e))
			.Check(e->type == current_type);
	}
}

/*
Rect VectorCtrl::ApplyRect(Point pt, Point end, Rect rc) const
{
	if(rectmask & 1) rc.left = end.x;
	if(rectmask & 2) rc.top = end.y;
	if(rectmask & 4) rc.right = end.x;
	if(rectmask & 8) rc.bottom = end.y;
	rc.Normalize();
	return rc;
}
*/

void VectorCtrl::ViewMenu(Bar& bar)
{
	bar.Add("Zoom in", VecImg::view_zoom_in(), THISBACK(DoViewZoomIn))
		.Key(K_CTRL_ADD);
	bar.Add("Zoom out", VecImg::view_zoom_out(), THISBACK(DoViewZoomOut))
		.Key(K_CTRL_SUBTRACT);
	bar.Add("Zoom full", VecImg::view_zoom_full(), THISBACK(DoViewZoomFull))
		.Key(K_CTRL_MULTIPLY);
	bar.Add("Zoom area", VecImg::view_zoom_area(), THISBACK(DoViewZoomArea))
		.Check(editmode == EDIT_ZOOM);
	bar.MenuSeparator();
	bar.Add("Pan", VecImg::view_pan(), THISBACK(DoViewPan))
		.Check(editmode == EDIT_PAN);
	if(bar.IsToolBar())
		bar.Add(tool_coords.SizePos(), 200);
}

void VectorCtrl::DoViewZoomIn()
{
	Point center = GetImageSize() >> 1;
	Point pos = scrollbars.Get(), page = scrollbars.GetPage(), total = scrollbars.GetTotal();
	if(page.x < total.x)
		center.x = (pos.x + (page.x >> 1)) / zoom;
	if(page.y < total.y)
		center.y = (pos.y + (page.y >> 1)) / zoom;
	SetZoom(Zoom(zoom.m * 2, zoom.d));
	scrollbars.Set(ImageToClient(center) - Point(GetSize() >> 1));
}

void VectorCtrl::DoViewZoomOut()
{
	Point center = GetImageSize() >> 1;
	Point pos = scrollbars.Get(), page = scrollbars.GetPage(), total = scrollbars.GetTotal();
	if(page.x < total.x)
		center.x = (pos.x + (page.x >> 1)) / zoom;
	if(page.y < total.y)
		center.y = (pos.y + (page.y >> 1)) / zoom;
	if(zoom.m >= 10)
		SetZoom(Zoom(zoom.m >> 1, zoom.d));
	scrollbars.Set(ImageToClient(center) - Point(GetSize() >> 1));
}

void VectorCtrl::DoViewZoomFull()
{
	Size image_size = GetImageSize();
	Size pixel_size = GetFitSize(image_size, scrollbars.GetReducedViewSize() - 2 * GAP);
	SetZoom(Zoom(pixel_size.cx, image_size.cx));
}

void VectorCtrl::DoViewZoomArea()
{
	DragStop();
	editmode = EDIT_ZOOM;
	WhenRescan();
}

void VectorCtrl::DoViewPan()
{
	DragStop();
	editmode = EDIT_PAN;
	WhenRescan();
}

Point VectorCtrl::GetTrackMode(Point pt, Point& nearest) const
{
	nearest = Point(-1, -1);
	static const int TOLERANCE = 5;
	Size imgsize = ImageToClient(Point(image.GetSize()));
	int szcx = tabs(pt.x - (imgsize.cx >> 1)) <= TOLERANCE ? 0 : tabs(pt.x - imgsize.cx) <= TOLERANCE ? 1 : -1;
	int szcy = tabs(pt.y - (imgsize.cy >> 1)) <= TOLERANCE ? 0 : tabs(pt.y - imgsize.cy) <= TOLERANCE ? 1 : -1;
	Point mode;
	if((szcx | szcy) > 0)
		mode = Point(-1, ((szcx | (szcy << 1)) << VectorObject::INDEX_SHIFT) | VectorObject::TRACK_IMAGE_SIZE);
	else {
		mode = image.Nearest(GetImageScaling(), pt + ClientOffset(), MAXDIST2, selection.GetKeys());
		if(mode.x < 0)
			mode = image.Nearest(GetImageScaling(), pt + ClientOffset(), MAXDIST2);
	}

	bool sel = (mode.x >= 0 && selection.Find(mode.x) >= 0);
	if(new_object)
		mode = Point(-1, VectorObject::TRACK_NEW);
	else if(sel && (mode.y & VectorObject::TRACK_MASK) == VectorObject::TRACK_RECT && mode.y & VectorObject::ARG_MASK) {
		const VectorObject& obj = image[mode.x];
		const VectorInfo& ii = obj.Info();
		if(const VectorGeomArg<Rect> *ra = ii.RectArgs()) {
			Rect rc = ra->Get(&obj, mode.y >> VectorObject::INDEX_SHIFT);
			int rm = GetRectDragMask(rc.Inflated(0) * GetImageScaling(), pt + ClientOffset(), TOLERANCE);
			if(rm)
				mode.y = rm | (mode.y & ~VectorObject::TRACK_MASK);
			else
				mode.y = VectorObject::TRACK_MOVE;
		}
	}
	nearest = mode;

	if(mode.x >= 0 && !sel)
		mode.y = (mode.y & ~VectorObject::TRACK_MASK) | VectorObject::TRACK_SELECT;

	return mode;
}

bool VectorCtrl::Push(Point pt, dword keyflags)
{
	switch(editmode) {
		case EDIT_ZOOM: {
			return true;
		}
		case EDIT_PAN: {
			pan_start = scrollbars.Get();
			return true;
		}
		case EDIT_TRACK: {
			break;
		}
	}

	if(!IsEditable())
		return false;
	Point nearest;
	trackmode = GetTrackMode(pt, nearest);
	track_start = image.TrackPoint(trackmode);
	bool is_sel = IsSelected(trackmode.x);
	int tm = trackmode.y & VectorObject::TRACK_MASK;
	if(tm == VectorObject::TRACK_SELECT
	|| !is_sel || keyflags & (K_SHIFT | K_CTRL)) {
		if(nearest.x >= 0) {
			if(tm == VectorObject::TRACK_SELECT)
				trackmode = nearest;
			VectorMap< int, Index<int> > sel;
			sel.Add(trackmode.x).Add(trackmode.y);
			if(keyflags & K_CTRL)
				XorSelection(sel);
			else if((keyflags & K_SHIFT) || is_sel)
				AddSelection(sel);
			else if(!is_sel)
				PickSelection(sel);
			WhenUserSelect();
		}
	}
	return true;
}

const Index<int>& VectorCtrl::FindSelTrack(int i) const
{
	static Index<int> dummy;
	return selection.Get(i, dummy);
}

void VectorCtrl::Click(Point pt, dword keyflags)
{
	if(IsEditable() && (trackmode.y & VectorObject::TRACK_MASK) == VectorObject::TRACK_NEW && !!new_object) {
		ClearSelection();
		new_object->Create(pt, pt, keyflags);
		int nobj = AddObject(-new_object);
		new_object = empty_object->Copy();
		UpdateActionRefresh();
		AddSelection(nobj);
		WhenUserSelect();
	}
	else if(!(keyflags & (K_SHIFT | K_CTRL)) && trackmode.y < 0) {
		ClearSelection();
		WhenUserSelect();
	}
}

void VectorCtrl::Drag(Point pt, Point last, Point next, dword keyflags)
{
	switch(editmode) {
		case EDIT_ZOOM: {
			DragDropCtrl::Drag(pt, last, next, keyflags);
			return;
		}
		case EDIT_PAN: {
			if(!IsNull(next)) {
				scrollbars.Set(pan_start - (next - pt));
			}
			return;
		}
		case EDIT_TRACK: {
			if(trackmode.y < 0) {
				DragDropCtrl::Drag(pt, last, next, keyflags);
				return;
			}
			break;
		}
	}

	if(!IsEditable())
		return;
	ViewDraw draw(this);
#ifdef PLATFORM_WIN32
	SetROP2(draw, R2_NOTXORPEN);
#endif
	draw.Offset(Point(GAP, GAP) - scrollbars);
	ScalingDraw scaled(&draw, GetImageScaling());
	Point pi = (!IsNull(track_start) ? track_start : ClientToSnap(pt));
	bool l = !IsNull(last), n = !IsNull(next);
	Point li = l ? ClientToSnap(last) : pi;
	Point ni = n ? ClientToSnap(next) : pi;

	if(!((trackmode.y & VectorObject::TRACK_MASK) & ~VectorObject::TRACK_RECT_SIDES)) {
		for(int i = 0; i < selection.GetCount(); i++) {
			const VectorObject& o = image[selection.GetKey(i)];
			One<VectorObject> c1 = o.Copy(), c2 = o.Copy();
			if(l) c1->Track(trackmode.y, pi, li, keyflags);
			if(n) c2->Track(trackmode.y, pi, ni, keyflags);
			if(l) c1->DragPaint(scaled);
			if(n) { c2->DragPaint(scaled); tool_coords <<= c2->FormatCoords(); }
		}
		return;
	}

	switch(trackmode.y & VectorObject::TRACK_MASK) {
		case VectorObject::TRACK_NEW: {
			if(!!new_object) {
				One<VectorObject> c1 = new_object->Copy(), c2 = new_object->Copy();
				if(l) c1->Create(pi, li, keyflags);
				if(n) c2->Create(pi, ni, keyflags);
				if(l) c1->DragPaint(scaled);
				if(n) { c2->DragPaint(scaled); tool_coords <<= c2->FormatCoords(); }
			}
			break;
		}

		case VectorObject::TRACK_IMAGE_SIZE: {
			if(n) {
				Size sz = image.GetSize();
				if(trackmode.y & (1 << VectorObject::INDEX_SHIFT)) sz.cx = minmax(ni.x, 10, 1000000);
				if(trackmode.y & (2 << VectorObject::INDEX_SHIFT)) sz.cy = minmax(ni.y, 10, 1000000);
				if(sz != image.GetSize()) {
					RLOG("SetImageSize: " << image.GetSize() << " -> " << sz << ", next = " << next);
					SetImageSize(sz);
					WhenImageSize();
					tool_coords <<= NFormat("%d x %d", sz.cx, sz.cy);
				}
			}
			break;
		}

		case VectorObject::TRACK_MOVE: {
			for(int i = 0; i < selection.GetCount(); i++) {
				const VectorObject& obj = image[selection.GetKey(i)];
				One<VectorObject> c1 = obj.Copy(), c2 = obj.Copy();
				if(l) c1->Track(trackmode.y, pi, li, keyflags);
				if(n) c2->Track(trackmode.y, pi, ni, keyflags);
				if(l) c1->DragPaint(scaled);
				if(n) { c2->DragPaint(scaled); tool_coords <<= c2->FormatCoords(); }
			}
			break;
		}

		case VectorObject::TRACK_SELECT: {
			DragDropCtrl::Drag(pt, last, next, keyflags);
			tool_coords <<= AsString(RectSort(last, next));
			break;
		}

		default: {
			if(trackmode.x >= 0) {
				const VectorObject& obj = image[trackmode.x];
				One<VectorObject> c1 = obj.Copy(), c2 = obj.Copy();
				if(l) c1->Track(trackmode.y, pi, li, keyflags);
				if(n) c2->Track(trackmode.y, pi, ni, keyflags);
				if(l) c1->DragPaint(scaled);
				if(n) { c2->DragPaint(scaled); tool_coords <<= c2->FormatCoords(); }
				break;
			}
		}
	}
}

void VectorCtrl::Drop(Point pt, Point end, dword keyflags)
{
	switch(editmode) {
		case EDIT_ZOOM: {
			Rect rc = ClientToImage(RectSort(pt, end));
			Size out = scrollbars.GetReducedViewSize(), in = rc.Size();
			int m = out.cx, d = in.cx;
			if(out.cx * in.cy > out.cy * in.cx)
				m = out.cy, d = in.cy;
			SetZoom(Zoom(m, d));
			scrollbars.Set(ImageToClient(rc.CenterPoint()) - (GetSize() >> 1) + scrollbars.Get());
			return;
		}
		case EDIT_PAN: {
			return;
		}
		default: {
			break;
		}
	}

	if(!IsEditable())
		return;

	Point pi = (!IsNull(track_start) ? track_start : ClientToSnap(pt));
	Point ei = ClientToSnap(end);

	if(trackmode.y < 0) {
		Rect rc = RectSort(pt, end);
		VectorMap< int, Index<int> > cont = image.Contains(GetImageScaling(), rc + ClientOffset());
		if(keyflags & K_CTRL)
			XorSelection(cont);
		else if(keyflags & K_SHIFT)
			AddSelection(cont);
		else
			PickSelection(cont);
		WhenUserSelect();
		return;
	}

	if(!((trackmode.y & VectorObject::TRACK_MASK) & ~VectorObject::TRACK_RECT_SIDES)) {
		for(int i = 0; i < selection.GetCount(); i++)
			image[selection.GetKey(i)].Track(trackmode.y, pi, ei, keyflags);
		UpdateActionRefresh();
		return;
	}

	switch(trackmode.y & VectorObject::TRACK_MASK) {
		case VectorObject::TRACK_NEW: {
			if(!!new_object) {
				ClearSelection();
				new_object->Create(pi, ei, keyflags);
				int nobj = AddObject(-new_object);
				new_object = empty_object->Copy();
				UpdateActionRefresh();
				AddSelection(nobj);
				WhenUserSelect();
			}
			break;
		}

		case VectorObject::TRACK_IMAGE_SIZE:
			break;

		case VectorObject::TRACK_MOVE: {
			for(int i = 0; i < selection.GetCount(); i++)
				image[selection.GetKey(i)].Track(trackmode.y, pi, ei, keyflags);
			UpdateActionRefresh();
			break;
		}

		case VectorObject::TRACK_SELECT: {
			PickSelection(image.Contains(GetImageScaling(), SortRect(pt + ClientOffset(), end + ClientOffset())));
			WhenUserSelect();
			break;
		}

		default: {
			if(trackmode.x >= 0) {
				image[trackmode.x].Track(trackmode.y, pi, ei, keyflags);
				UpdateActionRefresh();
				break;
			}
		}
	}
}

bool VectorCtrl::Key(dword key, int repcnt)
{
	if(IsEditable() && IsSelection() && (key & K_SHIFT)) {
		bool size = (key & K_CTRL);
		switch(key & ~K_SHIFT & ~K_CTRL) {
			case K_UP:    KeyMove(0, -1, size); return true;
			case K_DOWN:  KeyMove(0, +1, size); return true;
			case K_LEFT:  KeyMove(-1, 0, size); return true;
			case K_RIGHT: KeyMove(+1, 0, size); return true;
		}
	}
	return DragDropCtrl::Key(key, repcnt);
}

void VectorCtrl::KeyMove(int dx, int dy, bool resize)
{
	if(IsSelection()) {
		if(setup.snap) {
			int topi = selection.TopKey();
			const Index<int>& topsel = selection.Top();
			const VectorObject& obj = image[topi];
			const VectorInfo& info = obj.Info();
			dx *= setup.snap_size.cx;
			dy *= setup.snap_size.cy;
			for(int i = 0; i < topsel.GetCount(); i++)
				if((topsel[i] & VectorObject::ARG_MASK)) {
					Point pt;
					int t = topsel[i] >> VectorObject::INDEX_SHIFT;
					if(topsel[i] & VectorObject::RECT_MASK) {
						Rect rc = info.RectArgs()->Get(&obj, t);
						pt = (resize ? rc.BottomRight() : rc.TopLeft());
					}
					else
						pt = info.PointArgs()->Get(&obj, t);
					if(dx && setup.snap_size.cx > 1)
						dx = idivfloor(dx + pt.x + (dx > 0 ? 0 : setup.snap_size.cx - 1), setup.snap_size.cx) * setup.snap_size.cx - pt.x;
					if(dy && setup.snap_size.cy > 1)
						dy = idivfloor(dy + pt.y + (dy > 0 ? 0 : setup.snap_size.cy - 1), setup.snap_size.cy) * setup.snap_size.cy - pt.y;
					break;
				}
		}
		for(int i = 0; i < selection.GetCount(); i++) {
			VectorObject& obj = image[selection.GetKey(i)];
			obj.Track(resize ? (int)VectorObject::TRACK_SIZE : (int)VectorObject::TRACK_MOVE,
			          Point(0, 0), Point(dx, dy), 0);
		}
		UpdateActionRefresh();
		SyncCoords(Null);
	}
}

void VectorCtrl::SetImage(VectorImage img)
{
	image = img;
	Layout();
}

void VectorCtrl::SetImageSize(Size sz)
{
	image.SetSize(sz);
	Layout();
}

void VectorCtrl::ClearSelection()
{
	selection.Clear();
	PostSyncArgEditors();
}

void VectorCtrl::AddSelection(int sel, int track)
{
	Index<int> mask;
	int f = selection.Find(sel);
	if(f >= 0) {
		mask = selection[f];
		selection.Remove(f);
	}
	if(track >= 0)
		mask.FindAdd(track);
	selection.Add(sel) = mask;
	PostSyncArgEditors();
}

void VectorCtrl::AddSelection(const VectorMap< int, Index<int> >& sel)
{
	for(int i = 0; i < sel.GetCount(); i++) {
		int key = sel.GetKey(i);
		Index<int> mask;
		mask <<= sel[i];
		int f = selection.Find(key);
		if(f >= 0) {
			FindAppend(mask, selection[f]);
			selection.Remove(f);
		}
		selection.Add(key) = mask;
	}
	PostSyncArgEditors();
}

void VectorCtrl::XorSelection(const VectorMap< int, Index<int> >& sel)
{
	for(int i = 0; i < sel.GetCount(); i++) {
		int key = sel.GetKey(i);
		Index<int> selmask;
		selmask <<= sel[i];
		int f = selection.Find(key);
		if(f >= 0) {
			Index<int> oldmask = selection[f];
			selection.Remove(f);
			for(int o = 0; o < oldmask.GetCount(); o++) {
				int s = selmask.Find(oldmask[o]);
				if(s >= 0)
					selmask.Remove(s);
				else
					selmask.Add(oldmask[o]);
			}
		}
		if(!selmask.IsEmpty())
			selection.Add(key) = selmask;
	}
	PostSyncArgEditors();
}

void VectorCtrl::PickSelection(pick_ VectorMap< int, Index<int> >& sel)
{
	selection = sel;
	PostSyncArgEditors();
}

void VectorCtrl::RemoveSelection(const Vector<int>& rm_index)
{
	Index<int> found;
	for(int i = 0; i < rm_index.GetCount(); i++) {
		int f = selection.Find(rm_index[i]);
		if(f >= 0)
			found.FindAdd(f);
	}
	Vector<int> flist = found.PickKeys();
	Sort(flist);
	while(!flist.IsEmpty())
		selection.Remove(flist.Pop());
	PostSyncArgEditors();
}

void VectorCtrl::PostSyncArgEditors()
{
	sel_args.Clear();
	tc_sync_editors.KillSet(200, THISBACK(SyncArgEditors));
}

void VectorCtrl::SyncArgEditors()
{
	tc_sync_editors.Kill();
	sel_args.Clear();
	sel_editors.Clear();
	int name_len = 0;
	for(int i = 0; i < selection.GetCount(); i++) {
		int isel = selection.GetKey(i);
		const VectorObject& obj = image[isel];
		const VectorInfo& info = obj.Info();
		for(int a = 0; a < info.GetCount(); a++) {
			const VectorArg& varg = info[a];
			Value v = obj.GetArg(a);
			int f;
			for(f = sel_args.Find(varg.name); f >= 0; f = sel_args.FindNext(f))
				if(sel_args[f].vtype == varg.vtype && sel_args[f].editor == varg.editor) {
					sel_args[f].obj_args.Add(Point(a, isel));
					if(sel_args[f].value != v)
						sel_args[f].multi = true;
					break;
				}
			if(f < 0) {
				ArgEditor& ed = sel_args.Add(varg.name);
				ed.name = varg.name;
				ed.vtype = varg.vtype;
				ed.editor = varg.editor;
				ed.value = v;
				ed.multi = false;
				ed.obj_args.Add(Point(a, isel));
				name_len = max(name_len, GetSmartTextSize(ScreenInfo(), ed.name + ":  ", StdFont()).cx);
			}
		}
	}
	Sort(sel_args, FieldRelation(&ArgEditor::name, GetLanguageInfo()));
	int ypos = 4, yend = ypos;
	for(int i = 0; i < sel_args.GetCount(); i++) {
		const ArgEditor& sa = sel_args[i];
		One<Ctrl> editctrl = VectorArg::CreateEditor(sa.vtype, sa.editor);
		if(!!editctrl) {
			One<Label> label = new Label;
 			label->SetLabel(sa.name + ':');
			arg_parent << label->LeftPos(4, name_len).TopPos(ypos, 19);
			int ht = editctrl->GetRect().Height();
			arg_parent << editctrl->HSizePos(4 + name_len, 4).TopPos(ypos, ht);
			if(sa.multi) {
				One<Button> multi = new Button;
				multi->SetLabel("(various)");
				*multi <<= THISBACK2(OnArgMulti, ~multi, ~editctrl);
				arg_parent << multi->SetPos(editctrl->GetPos());
				sel_editors.Add(-multi);
				editctrl->Hide();
			}
			else
				editctrl->SetData(sa.value);
			*editctrl <<= THISBACK2(OnArgEdit, i, ~editctrl);
			editctrl->ClearModify();
			sel_editors.Add(-label);
			sel_editors.Add(-editctrl);
			yend = ypos + max(19, ht);
			ypos = yend + 2;
		}
	}
	arg_parent.HSizePos(0, 0).TopPos(arg_parent.GetRect().top, yend + 4);
	WhenArgLayout();
	Refresh();
}

void VectorCtrl::SyncCoords(Point mouse)
{
	String out;
	if(!IsNull(mouse))
		out = NFormat("[%d, %d]", mouse.x, mouse.y);
	if(!selection.IsEmpty()) {
		out << (out.IsEmpty() ? "" : "; ") << image[selection.TopKey()].FormatCoords();
	}
	tool_coords <<= out;
}

void VectorCtrl::OnNewObject(const VectorInfo *i)
{
	ClearSelection();
	empty_object = i->New();
	for(int a = 0; a < i->GetCount(); a++)
		(*i)[a].Set(~empty_object, (*i)[a].dflt);
	new_object = empty_object->Copy();
	new_icon = i->icon;
	Refresh();
	WhenRescan();
}

void VectorCtrl::OnArgEdit(int i, Ctrl *ctrl)
{
	if(i >= 0 && i < sel_args.GetCount()) {
		const ArgEditor& sa = sel_args[i];
		Value v = ctrl->GetData();
		for(int o = 0; o < sa.obj_args.GetCount(); o++)
			image[sa.obj_args[o].y].SetArg(sa.obj_args[o].x, v);
	}
	Refresh();
}

void VectorCtrl::OnArgMulti(Button *button, Ctrl *editor)
{
	button->Hide();
	editor->Show();
	if(!editor->SetWantFocus())
		IterateFocusForward(editor, editor);
}

void VectorCtrl::OnScroll()
{
	Refresh();
}

Point VectorCtrl::ClientToImage(Point pt) const
{
	return (pt - GAP + scrollbars) / zoom;
}

Point VectorCtrl::ClientToSnap(Point pt) const
{
	Point pi = ClientToImage(pt);
	if(setup.snap)
		pi = Size(pi + (setup.snap_size >> 1)) / setup.snap_size * setup.snap_size;
	return pi;
}

Point VectorCtrl::ImageToClient(Point pt) const
{
	return zoom * pt + GAP - Point(scrollbars);
}

Rect VectorCtrl::ClientToImage(Rect rc) const
{
	return Rect(ClientToImage(rc.TopLeft()), ClientToImage(rc.BottomRight()));
}

Rect VectorCtrl::ImageToClient(Rect rc) const
{
	return Rect(ImageToClient(rc.TopLeft()), ImageToClient(rc.BottomRight()));
}

int VectorCtrl::AddObject(One<VectorObject> object)
{
	image.Add(object);
	UpdateActionRefresh();
	return image.GetCount() - 1;
}

void VectorCtrl::RemoveObject(int i)
{
	image.Remove(i);
	UpdateActionRefresh();
}

void VectorCtrl::MoveObject(int i, int j)
{
	image.Move(i, j);
	int ipos = selection.Find(i);
	int jpos = selection.Find(j);
	if(ipos >= 0) selection.SetKey(ipos, j);
	if(jpos >= 0) selection.SetKey(jpos, i);
	UpdateActionRefresh();
}

void VectorCtrl::DoEditSetup()
{
	DlgVectorDesSetup setupdlg;
	if(setupdlg.Run(setup)) {
		setup.SaveGlobal();
		Refresh();
	}
}

VectorDes::VectorDes(VectorDesHost& host)
: host(host)
, cv_obj_list(editor)
{
	single = false;

	AddFrame(toolbar);
	image_list.AutoHideSb();
	ASSERT(image_list.GetIndexCount() == IC_IDENT);
	image_list.AddColumn("Identifier", 20).Edit(list_ident);
	ASSERT(image_list.GetIndexCount() == IC_WIDTH);
	image_list.AddColumn("Width", 7).Edit(list_width).InsertValue(500);
	list_width.MinMax(1, 100000).NotNull();
	list_width <<= THISBACK(OnImageSize);
	ASSERT(image_list.GetIndexCount() == IC_HEIGHT);
	image_list.AddColumn("Height", 7).Edit(list_height).InsertValue(500);
	list_height.MinMax(1, 100000).NotNull();
	list_height <<= THISBACK(OnImageSize);
	ASSERT(image_list.GetIndexCount() == IC_IMAGE);
	image_list.AddCtrl(editor).InsertValue(VectorImage());
	ASSERT(image_list.GetIndexCount() == IC_COUNT);
	image_list.AfterBeforeInserting().Removing();
	image_list.WhenCursor = THISBACK(OnImageListCursor);
	object_list.AutoHideSb().NoHeader().NoGrid().MultiSelect();
	object_list.AddRowNumColumn("").SetConvert(cv_obj_list);
	object_list.WhenBar = THISBACK(ObjectMenu);
	object_list.WhenCursor = THISBACK(PostObjectCursor);
	arg_pane.AddFrame(arg_scroll);
	arg_scroll.AutoHide();
	editor.WhenArgLayout = THISBACK(OnArgLayout);
	editor.WhenUserSelect = THISBACK(OnUserSelect);
	arg_scroll.WhenScroll = THISBACK(OnArgScroll);
	arg_pane.Add(editor.GetArgParent());
	asplitter.Vert(object_list, arg_pane);
	vsplitter.Vert(image_list, asplitter);
	splitter.SetPos(3000);
	splitter.Horz(vsplitter, editor);
	Add(splitter.SizePos());
	editor <<= THISBACK(OnObject);
	editor.WhenRescan = THISBACK(Rescan);
	editor.WhenImageSize = THISBACK(OnUserImageSize);
	Rescan();
}

VectorDes::~VectorDes()
{
	Shutdown();
}

void VectorDes::LoadVecFile(CParser& parser)
{
	single = false;
	vsplitter.NoZoom();
	image_list.Clear();
	while(!parser.IsEof()) {
		VectorImage image;
		image.LoadVec(parser);
		if(!memcmp(image.GetName(), "vi__", 4))
			image.SetName(Null);
		int r = image_list.GetCount();
		image_list.Add();
		image_list.Set(r, IC_IDENT, image.GetName());
		image_list.Set(r, IC_WIDTH, image.GetSize().cx);
		image_list.Set(r, IC_HEIGHT, image.GetSize().cy);
		image_list.Set(r, IC_IMAGE, Value(image));
	}
	image_list.SetCursor(0);
	OnImageListCursor();
}

String VectorDes::SaveVecFile()
{
	String out;
	for(int i = 0; i < image_list.GetCount(); i++) {
		VectorImage im = image_list.Get(i, IC_IMAGE);
		im.SetName(image_list.Get(i, IC_IDENT));
		im.SetSize(Size(image_list.Get(i, IC_WIDTH), image_list.Get(i, IC_HEIGHT)));
		out << im.SaveVec(i) << '\n';
	}
	return out;
}

void VectorDes::LoadSingle(VectorImage img)
{
	single = true;
	image_list.NoInserting().NoRemoving();
	image_list.SetCount(1);
	image_list.SetCursor(0);
	image_list.Set(IC_IDENT, img.GetName());
	image_list.Set(IC_WIDTH, img.GetSize().cx);
	image_list.Set(IC_HEIGHT, img.GetSize().cy);
	image_list.Set(IC_IMAGE, Value(img));
	OnImageListCursor();
	vsplitter.Zoom(1);
}

VectorImage VectorDes::SaveSingle() const
{
	VectorImage im;
	if(image_list.GetCount() > 0) {
		im = image_list.Get(0, IC_IMAGE);
		im.SetName(image_list.Get(0, IC_IDENT));
		im.SetSize(Size(image_list.Get(0, IC_WIDTH), image_list.Get(0, IC_HEIGHT)));
	}
	return im;
}

void VectorDes::CloseDesigner()
{
}

void VectorDes::FrameFocus()
{
}

void VectorDes::EditMenu(Bar& bar)
{
	bar.Add("Object", THISBACK(ObjectMenu));
	bar.Add("Insert", callback(&editor, &VectorCtrl::InsertMenu));
	bar.Add("Edit", callback(&editor, &VectorCtrl::EditMenu));
	bar.Add("View", callback(&editor, &VectorCtrl::ViewMenu));
}

bool VectorDes::Key(dword key, int repcnt)
{
	return false;
}

void VectorDes::OnImageListCursor()
{
	editor.ClearSelection();
	editor.Enable(image_list.IsCursor());
	OnImageSize();
	OnObject();
}

void VectorDes::OnImageSize()
{
	if(image_list.IsCursor()) {
		int cx = image_list.Get(IC_WIDTH);
		int cy = image_list.Get(IC_HEIGHT);
		editor.SetImageSize(Size(Nvl(cx, 100), Nvl(cy, 100)));
		image_list.Set(IC_IMAGE, ~editor);
		WhenSyncTitle();
	}
}

void VectorDes::OnUserImageSize()
{
	if(image_list.IsCursor()) {
		image_list.Set(IC_WIDTH, editor.GetImageSize().cx);
		image_list.Set(IC_HEIGHT, editor.GetImageSize().cy);
		WhenSyncTitle();
	}
}

void VectorDes::OnObject()
{
	object_list.SetCount(editor.GetImage().GetCount()); // SetVirtualCount!!!
	object_list.Refresh();
	Rescan();
}

void VectorDes::ObjectMenu(Bar& bar)
{
	bar.Add(editor.IsSelection() && !editor.IsSelected(0),
		"Move to back", VecImg::move_to_back(), THISBACK(ObjectMoveBack))
		.Key(K_CTRL_UP);
	bar.Add(editor.IsSelection() && !editor.IsSelected(object_list.GetCount() - 1),
		"Move to front", VecImg::move_to_front(), THISBACK(ObjectMoveFront))
		.Key(K_CTRL_DOWN);
	bar.Separator();
	bar.Add(editor.IsSelection(), "Delete", CtrlImg::remove(), THISBACK(ObjectDelete));
}

void VectorDes::ObjectDelete()
{
	int c = object_list.GetCursor();
	editor.DoEditDelete();
}

void VectorDes::ObjectMoveBack()
{
	Vector<int> sorted;
	sorted <<= editor.GetSelected();
	Sort(sorted);
	if(sorted.IsEmpty() || sorted[0] == 0) {
		BeepExclamation();
		return;
	}
	for(int i = 0; i < sorted.GetCount(); i++)
		editor.MoveObject(sorted[i], sorted[i] - 1);
	int c = object_list.GetCursor();
	if(c > 0)
		object_list.SetCursor(c - 1);
	OnUserSelect();
}

void VectorDes::ObjectMoveFront()
{
	Vector<int> sorted;
	sorted <<= editor.GetSelected();
	Sort(sorted);
	if(sorted.IsEmpty() || sorted.Top() >= object_list.GetCount() - 1) {
		BeepExclamation();
		return;
	}
	for(int i = sorted.GetCount(); --i >= 0;)
		editor.MoveObject(sorted[i], sorted[i] + 1);
	int c = object_list.GetCursor();
	if(c >= 0 && c + 1 < object_list.GetCount())
		object_list.SetCursor(c + 1);
	OnUserSelect();
}

/*
void VectorDes::ObjectProperties()
{
	int c = object_list.GetCount();
	if(c < 0) {
		BeepExclamation();
		return;
	}
	// properties
}
*/

void VectorDes::OnArgLayout()
{
	arg_scroll.Set(arg_scroll, GetSize().cy, editor.GetArgParent().GetRect().Height());
	OnArgScroll();
}

void VectorDes::OnArgScroll()
{
	Ctrl& parent = editor.GetArgParent();
	parent.HSizePos(0, 0).TopPos(-arg_scroll.Get(), parent.GetRect().Height());
}

void VectorDes::OnUserSelect()
{
	if(!editor.IsSelection())
		object_list.KillCursor();
	int isel = -1;
	for(int i = 0; i < object_list.GetCount(); i++) {
		bool issel = editor.IsSelected(i);
		object_list.Select(i, issel);
		if(issel)
			isel = i;
	}
	if(isel >= 0) {
		int dsel = object_list.GetCursor();
		if(dsel < 0 || !editor.IsSelected(dsel))
			object_list.SetCursor(isel);
	}
	Rescan();
}

void VectorDes::ChildGotFocus()
{
	if(!IsShutdown() && IsOpen() && IsVisible())
		if(Ctrl *ctrl = GetFocusCtrl()) {
			Rect rc = ctrl->GetScreenRect();
			rc.Offset(-GetScreenRect().TopLeft());
			bool up = rc.top < 0, down = rc.bottom > GetSize().cy;
			if(up && !down)
				arg_scroll.Set(arg_scroll.Get() + rc.top);
			if(down && !up)
				arg_scroll.Set(arg_scroll.Get() + rc.bottom - GetSize().cy);
		}
}

void VectorDes::PostObjectCursor()
{
	tc_object_cursor.KillSet(200, THISBACK(OnObjectCursor));
}

void VectorDes::OnObjectCursor()
{
	Vector<int> remove;
	int cursor = object_list.GetCursor();
	for(int i = editor.GetSelCount(); --i >= 0;)
		if(!object_list.IsSelected(editor.GetSelObject(i)) && editor.GetSelObject(i) != cursor)
			remove.Add(editor.GetSelObject(i));
	if(!remove.IsEmpty())
		editor.RemoveSelection(remove);
	Vector<int> add;
	for(int i = 0; i < object_list.GetCount(); i++)
		if((i == cursor || object_list.IsSelected(i)) && !editor.IsSelected(i))
			editor.AddSelection(i);
	editor.SyncArgEditors();
	Rescan();
}

class VectorImageEditor : public TopWindow, public VectorDesHost
{
public:
	typedef VectorImageEditor CLASSNAME;
	VectorImageEditor();

	bool         Run(VectorImage& image);

	void         SyncTitle();

private:
	void         Rescan() { menubar.Set(THISBACK(MainMenu)); }

	void         MainMenu(Bar& bar);

private:
	MenuBar      menubar;
	One<VectorDes> designer;
};

VectorImageEditor::VectorImageEditor()
{
	designer = new VectorDes(*this);
	designer->WhenSyncTitle = THISBACK(SyncTitle);
	designer->WhenRescan = THISBACK(Rescan);
	AddFrame(menubar);
	AddFrame(TopSeparatorFrame());
	Add(designer->SizePos());
	Sizeable().MaximizeBox();
	Icon(VecImg::ide_icon());
}

bool VectorImageEditor::Run(VectorImage& image)
{
	designer->LoadSingle(image);
	SyncTitle();
	Rescan();
	if(TopWindow::Run() == IDCANCEL)
		return false;
	image = designer->SaveSingle();
	return true;
}

void VectorImageEditor::SyncTitle()
{
	Title(NFormat("Vektorový obrázek [ %d x %d ]",
		designer->GetImageSize().cx, designer->GetImageSize().cy));
}

void VectorImageEditor::MainMenu(Bar& bar)
{
	designer->EditMenu(bar);
}

bool RunVectorImageEditor(VectorImage& image)
{
	VectorImageEditor editor;
	return editor.Run(image);
}
