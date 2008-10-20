#include "IconDes.h"

NAMESPACE_UPP

void IconDes::SetPen(int _pen)
{
	pen = _pen;
	SetBar();
}

bool IconDes::Key(dword key, int count)
{
	switch(key) {
	case K_SHIFT_LEFT:  KeyMove(-1, 0); return true;
	case K_SHIFT_RIGHT: KeyMove(1, 0); return true;
	case K_SHIFT_UP:    KeyMove(0, -1); return true;
	case K_SHIFT_DOWN:  KeyMove(0, 1); return true;
	}
	return false;
}

void IconDes::SetMagnify(int mag)
{
	if( !IsCurrent() )
		return;

	magnify = minmax(mag, 1, 27);

	sb = Point(0, 0);
	SetSb();
	Refresh();

	SetBar();
}


void IconDes::ZoomIn()
{
	SetMagnify(magnify+2);
}

void IconDes::ZoomOut()
{
	SetMagnify(magnify-2);
}

void IconDes::DoPaste()
{
	if(!IsCurrent())
		return;
	Image m = ReadClipboardImage();
	if(m)
		Paste(m);
}

void IconDes::DoCopy()
{
	if(!IsCurrent())
		return;
	WriteClipboardImage(IsPasting() ? Current().paste_image : Copy(SelectionRect()));
}

void IconDes::DoCut()
{
	if(!IsCurrent())
		return;
	DoCopy();
	if(IsPasting()) {
		Current().paste_image.Clear();
		MakePaste();
	}
	else
		Delete();
}

void IconDes::ToolEx(Bar& bar) {}

void IconDes::EditBar(Bar& bar)
{
	Slot *c = IsCurrent() ? &Current() : NULL;
	bar.Add(c, "Cut", CtrlImg::cut(), THISBACK(DoCut)).Key(K_DELETE).Key(K_CTRL_X);
	bar.Add(c, "Copy", CtrlImg::copy(), THISBACK(DoCopy)).Key(K_CTRL_C);
	bar.Add(c, "Paste", CtrlImg::paste(), THISBACK(DoPaste)).Key(K_CTRL_V);
	bar.Separator();
	bar.Add(c && c->undo.GetCount(), "Undo", CtrlImg::undo(), THISBACK(Undo))
	   .Key(K_CTRL_Z)
	   .Repeat();
	bar.Add(c && c->redo.GetCount(), "Redo", CtrlImg::redo(), THISBACK(Redo))
	   .Key(K_SHIFT_CTRL_Z)
	   .Repeat();
}

void IconDes::SettingBar(Bar& bar)
{
	Slot *c = IsCurrent() ? &Current() : NULL;
	bar.Add(c, "Zoom out", IconDesImg::ZoomMinus(), THISBACK(ZoomOut))
		.Enable(magnify > 1)
		.Key(K_SHIFT_CTRL_SUBRACT);
	bar.Add(c, "Zoom in",  IconDesImg::ZoomPlus(), THISBACK(ZoomIn))
		.Enable(magnify < 27)
		.Key(K_SHIFT_CTRL_ADD);
	bar.Add("Paste transparent/opaque",
	        paste_opaque ? IconDesImg::PasteOpaque()
	                     : IconDesImg::PasteTransparent(),
	        THISBACK(TogglePaste))
	   .Key(K_ALT_T);
}

void IconDes::SelectBar(Bar& bar)
{
	Slot *c = IsCurrent() ? &Current() : NULL;
	bar.Add(c, "Select", IconDesImg::Select(), THISBACK(Select))
	   .Check(doselection)
	   .Key(K_SPACE);
	bar.Add(c, "Invert selection", IconDesImg::InvertSelect(), THISBACK(InvertSelect))
	   .Key(K_CTRL_I);
	bar.Add(c, "Cancel selection", IconDesImg::CancelSelect(), THISBACK(CancelSelect))
	   .Key(K_CTRL_E);
	bar.Add(c, "Select and move rectangle", IconDesImg::SelectRect(), THISBACK(SelectRect))
	   .Check(selectrect)
	   .Key(K_CTRL_R);
	bar.Add(c, "Move", IconDesImg::Move(), THISBACK(Move))
	   .Check(IsPasting())
	   .Key(K_CTRL_M);
}

void IconDes::ImageBar(Bar& bar)
{
	Slot *c = IsCurrent() ? &Current() : NULL;
	if(c && c->supersampling)
		bar.Add(c, "Smooth resize 3x down", IconDesImg::ResizeDown(), THISBACK(ResizeDown))
		   .Key(K_TAB);
	else
		bar.Add(c && c->image.GetLength() < 256 * 256, "Resize 3x up (supersampling)",
	    	    IconDesImg::ResizeUp(), THISBACK(ResizeUp))
	       .Key(K_TAB);
	bar.Separator();
	bar.Add(c, "Set color", IconDesImg::SetColor(), THISBACK(SetColor));
	bar.Add(c, "Empty", IconDesImg::Delete(), THISBACK(DoDelete))
	   .Key(K_DELETE);
	bar.Add(c, "Interpolate", IconDesImg::Interpolate(), THISBACK(Interpolate));
	bar.Add(c, "Horizontal mirror", IconDesImg::MirrorX(), THISBACK(MirrorX));
	bar.Add(c, "Vertical mirror", IconDesImg::MirrorY(), THISBACK(MirrorY));
	bar.Add(c, "Horizontal symmetry", IconDesImg::SymmX(), THISBACK(SymmX));
	bar.Add(c, "Vertical symmetry", IconDesImg::SymmY(), THISBACK(SymmY));
	bar.Add(c, "Rotate", IconDesImg::Rotate(), THISBACK(Rotate));
	bar.Add(c, "Smooth Rescale", IconDesImg::Rescale(), THISBACK(SmoothRescale));
	bar.Add(c, "Blur/Sharpen", IconDesImg::BlurSharpen(), THISBACK(BlurSharpen));
	bar.Add(c, "Colorize", IconDesImg::Colorize(), THISBACK(Colorize));
	bar.Add(c, "Chroma", IconDesImg::Chroma(), THISBACK(Chroma));
	bar.Add(c, "Contrast", IconDesImg::Contrast(), THISBACK(Contrast));
	bar.Add(c, "Alpha", IconDesImg::AlphaI(), THISBACK(Alpha));
	bar.Add(c, "Colors", IconDesImg::Colors(), THISBACK(Colors));
}

void IconDes::DrawBar(Bar& bar)
{
	bool notpasting = !IsPasting();
	bar.Add("FreeHand", IconDesImg::FreeHand(), THISBACK1(SetTool, &IconDes::FreehandTool))
	   .Check(tool == &IconDes::FreehandTool && notpasting)
	   .Key(K_H);
	bar.Add("Lines", IconDesImg::Lines(), THISBACK1(SetTool, &IconDes::LineTool))
	   .Check(tool == &IconDes::LineTool && notpasting)
	   .Key(K_L);
	bar.Add("Ellipses", IconDesImg::Circles(), THISBACK1(SetTool, &IconDes::EllipseTool))
	   .Check(tool == &IconDes::EllipseTool && notpasting)
	   .Key(K_E);
	bar.Add("Empty ellipses", IconDesImg::EmptyCircles(), THISBACK1(SetTool, &IconDes::EmptyEllipseTool))
	   .Check(tool == &IconDes::EmptyEllipseTool && notpasting)
	   .Key(K_W);
	bar.Add("Rectangles", IconDesImg::Rects(), THISBACK1(SetTool, &IconDes::RectTool))
	   .Check(tool == &IconDes::RectTool && notpasting)
	   .Key(K_R);
	bar.Add("Empty rectangles", IconDesImg::EmptyRects(), THISBACK1(SetTool, &IconDes::EmptyRectTool))
	   .Check(tool == &IconDes::EmptyRectTool && notpasting && !selectrect)
	   .Key(K_T);
	bar.Add("Hotspots", IconDesImg::HotSpot(), THISBACK1(SetTool, &IconDes::HotSpotTool))
	   .Check(tool == &IconDes::HotSpotTool)
	   .Key(K_H);
	bar.Separator();
	for(int i = 1; i <= 6; i++)
		bar.Add("Pen " + AsString(i), IconDesImg::Get(IconDesImg::I_Pen1 + i - 1), THISBACK1(SetPen, i))
		   .Check(pen == i)
		   .Key(K_1 + i - 1);
}

void IconDes::MainToolBar(Bar& bar)
{
	EditBar(bar);
	bar.Separator();
	SelectBar(bar);
	bar.Separator();
	ImageBar(bar);
	bar.Break();
	DrawBar(bar);
	ToolEx(bar);
	bar.Separator();
	SettingBar(bar);
}

void IconDes::SetBar()
{
	toolbar.Set(THISBACK(MainToolBar));
}

struct CachedIconImage : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = q;
		if(IsNull(m))
			return;
		Size isz = m.GetSize();
		if(isz.cx > 200 || isz.cy > 200)
			m = IconDesImg::LargeImage();
		else
		if(isz.cx > r.GetWidth() || isz.cy > r.GetHeight())
			m = CachedRescale(m, GetFitSize(m.GetSize(), r.GetSize()));
		Point p = r.CenterPos(m.GetSize());
		w.DrawImage(p.x, p.y, m);
	}
};

void IconDes::SerializeSettings(Stream& s)
{

	void (IconDes::*toollist[])(Point p, dword flags) = {
		&IconDes::LineTool,
		&IconDes::FreehandTool,
		&IconDes::EllipseTool,
		&IconDes::EmptyEllipseTool,
		&IconDes::RectTool,
		&IconDes::EmptyRectTool,
		&IconDes::HotSpotTool,
	};

	int version = 2;
	s / version;
	s / magnify;
	s % leftpane % bottompane;
	int i;
	for(i = 0; i < __countof(toollist); i++)
		if(toollist[i] == tool)
			break;
	s % i;
	if(i >= 0 && i < __countof(toollist))
		tool = toollist[i];
	if(version >= 1)
		s % pen;
	SetSb();
	Refresh();
	SetBar();
	if(version >= 2)
		s % ImgFile();
}

IconDes::IconDes()
{
	sb.WhenScroll = THISBACK(Scroll);

	paste_opaque = false;
	doselection = false;

	tool = &IconDes::FreehandTool;

	AddFrame(leftpane);
	AddFrame(bottompane);
	AddFrame(toolbar);
	AddFrame(sb);
	AddFrame(ViewFrame());

	leftpane.Left(rgbactrl, 256);
	rgbactrl.SubCtrl(&list);
	rgbactrl <<= THISBACK(ColorChanged);

	list.AddColumn("", 4);
	list.AddColumn("").SetDisplay(Single<CachedIconImage>());
	list.NoHeader().NoVertGrid();
	list.WhenBar = THISBACK(ListMenu);
	list.WhenCursor = THISBACK(ListCursor);
	list.WhenLeftDouble = THISBACK(EditImage);
	list.NoWantFocus();

	bottompane.Bottom(iconshow, 64);

	SetBar();
	ColorChanged();
	BackPaint();

	magnify = 13;
	pen = 1;
}

END_UPP_NAMESPACE
