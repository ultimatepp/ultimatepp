#include "IconDes.h"

NAMESPACE_UPP

#define KEYNAMESPACE IconDesKeys
#define KEYGROUPNAME "Icon designer"
#define KEYFILE      <IconDes/IconDes.key>
#include             <CtrlLib/key_source.h>

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
	case K_PAGEUP:      ChangeSlot(-1); return true;
	case K_PAGEDOWN:    ChangeSlot(1); return true;
	case K_CTRL_F:      search.SetFocus(); return true;
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
	using namespace IconDesKeys;
	Slot *c = IsCurrent() ? &Current() : NULL;
	bar.Add(c, AK_ZOOM_IN, IconDesImg::ZoomMinus(), THISBACK(ZoomOut))
		.Enable(magnify > 1);
	bar.Add(c, AK_ZOOM_OUT,  IconDesImg::ZoomPlus(), THISBACK(ZoomIn))
		.Enable(magnify < 27);
	bar.Add(AK_PASTE_MODE,
	        paste_opaque ? IconDesImg::PasteOpaque()
	                     : IconDesImg::PasteTransparent(),
	        THISBACK(TogglePaste));
}

void IconDes::SelectBar(Bar& bar)
{
	using namespace IconDesKeys;
	Slot *c = IsCurrent() ? &Current() : NULL;
	bar.Add(c, AK_SELECT, IconDesImg::Select(), THISBACK(Select))
	   .Check(doselection);
	bar.Add(c, AK_INVERT_SEL, IconDesImg::InvertSelect(), THISBACK(InvertSelect));
	bar.Add(c, AK_CANCEL_SEL, IconDesImg::CancelSelect(), THISBACK(CancelSelect));
	bar.Add(c, AK_SELECT_MOVE, IconDesImg::SelectRect(), THISBACK(SelectRect))
	   .Check(selectrect);
	bar.Add(c, AK_MOVE, IconDesImg::Move(), THISBACK(Move))
	   .Check(IsPasting());
}

void IconDes::ImageBar(Bar& bar)
{
	using namespace IconDesKeys;
	Slot *c = IsCurrent() ? &Current() : NULL;
	if(c && c->supersampling)
		bar.Add(c, "Smooth resize 3x down", IconDesImg::ResizeDown(), THISBACK(ResizeDown))
		   .Key(AK_SUPERSAMPLING);
	else
		bar.Add(c && c->image.GetLength() < 256 * 256, "Resize 3x up (supersampling)",
	    	    IconDesImg::ResizeUp(), THISBACK(ResizeUp))
	       .Key(AK_SUPERSAMPLING);
	bar.Separator();
	bar.Add(c, AK_SETCOLOR, IconDesImg::SetColor(), THISBACK(SetColor));
	bar.Add(c, AK_EMPTY, IconDesImg::Delete(), THISBACK(DoDelete));
	bar.Add(c, AK_INTERPOLATE, IconDesImg::Interpolate(), THISBACK(Interpolate));
	bar.Add(c, AK_HMIRROR, IconDesImg::MirrorX(), THISBACK(MirrorX));
	bar.Add(c, AK_VMIRROR, IconDesImg::MirrorY(), THISBACK(MirrorY));
	bar.Add(c, AK_HSYM, IconDesImg::SymmX(), THISBACK(SymmX));
	bar.Add(c, AK_VSYM, IconDesImg::SymmY(), THISBACK(SymmY));
	bar.Add(c, AK_ROTATE, IconDesImg::Rotate(), THISBACK(Rotate));
	bar.Add(c, AK_FREE_ROTATE, IconDesImg::FreeRotate(), THISBACK(FreeRotate));
	bar.Add(c, AK_RESCALE, IconDesImg::Rescale(), THISBACK(SmoothRescale));
	bar.Add(c, AK_BLUR, IconDesImg::BlurSharpen(), THISBACK(BlurSharpen));
	bar.Add(c, AK_COLORIZE, IconDesImg::Colorize(), THISBACK(Colorize));
	bar.Add(c, AK_CHROMA, IconDesImg::Chroma(), THISBACK(Chroma));
	bar.Add(c, AK_CONTRAST, IconDesImg::Contrast(), THISBACK(Contrast));
	bar.Add(c, AK_ALPHA, IconDesImg::AlphaI(), THISBACK(Alpha));
	bar.Add(c, AK_COLORS, IconDesImg::Colors(), THISBACK(Colors));
	bar.Add(c, AK_SMOOTHEN, IconDesImg::Smoothen(), THISBACK(Smoothen));
}

void IconDes::DrawBar(Bar& bar)
{
	using namespace IconDesKeys;
	bool notpasting = !IsPasting();
	bar.Add(AK_FREEHAND, IconDesImg::FreeHand(), THISBACK1(SetTool, &IconDes::FreehandTool))
	   .Check(tool == &IconDes::FreehandTool && notpasting);
	bar.Add(AK_LINES, IconDesImg::Lines(), THISBACK1(SetTool, &IconDes::LineTool))
	   .Check(tool == &IconDes::LineTool && notpasting);
	bar.Add(AK_ELLIPSES, IconDesImg::Circles(), THISBACK1(SetTool, &IconDes::EllipseTool))
	   .Check(tool == &IconDes::EllipseTool && notpasting);
	bar.Add(AK_EMPTY_ELLIPSES, IconDesImg::EmptyCircles(), THISBACK1(SetTool, &IconDes::EmptyEllipseTool))
	   .Check(tool == &IconDes::EmptyEllipseTool && notpasting);
	bar.Add(AK_RECTANGLES, IconDesImg::Rects(), THISBACK1(SetTool, &IconDes::RectTool))
	   .Check(tool == &IconDes::RectTool && notpasting);
	bar.Add(AK_EMPTY_RECTANGLES, IconDesImg::EmptyRects(), THISBACK1(SetTool, &IconDes::EmptyRectTool))
	   .Check(tool == &IconDes::EmptyRectTool && notpasting && !selectrect);
	bar.Add(AK_HOTSPOTS, IconDesImg::HotSpot(), THISBACK1(SetTool, &IconDes::HotSpotTool))
	   .Check(tool == &IconDes::HotSpotTool);
	bar.Add(AK_TEXT, IconDesImg::Text(), THISBACK(Text))
	   .Check(textdlg.IsOpen());
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
	SetSb();
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
	virtual Size GetStdSize(const Value& q) const
	{
		Image m = q;
		if(IsNull(m))
			return Size(0, 0);
		Size isz = m.GetSize();
		return isz.cx < 200 && isz.cy < 200 ? isz : IconDesImg::LargeImage().GetSize();
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
	AddFrame(toolbar);
	AddFrame(bottompane);
	AddFrame(sb);
	AddFrame(ViewFrame());

	leftpane.Left(rgbactrl, 256);
	rgbactrl.SubCtrl(&imgs);

	rgbactrl <<= THISBACK(ColorChanged);

	search.NullText("Search (Ctrl+F)");
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterToUpper);

	int cy = EditString::GetStdHeight();
	imgs.Add(search.HSizePos().TopPos(0, cy));
	imgs.Add(ilist.HSizePos().VSizePos(cy, 0));

	ilist.AddKey();
	ilist.AddColumn("", 4);
	ilist.AddColumn("").SetDisplay(Single<CachedIconImage>());
	ilist.NoHeader().NoVertGrid();
	ilist.WhenBar = THISBACK(ListMenu);
	ilist.WhenCursor = THISBACK(ListCursor);
	ilist.WhenLeftDouble = THISBACK(EditImage);
	ilist.NoWantFocus();

	search <<= THISBACK(Search);
	search.SetFilter(CharFilterToUpper);

	bottompane.Bottom(iconshow, 64);
	
	SetBar();
	ColorChanged();
	BackPaint();

	magnify = 13;
	pen = 1;
	
	single_mode = false;
}

END_UPP_NAMESPACE
