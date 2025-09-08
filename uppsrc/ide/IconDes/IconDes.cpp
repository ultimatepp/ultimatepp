#include "IconDes.h"

void IconDes::MaskSelection()
{
	Slot& c = Current();
	ImageBuffer ib(c.image);
	Size isz = ib.GetSize();
	CoFor(isz.cy, [&](int y) {
		RGBA *t = ib[y];
		const RGBA *e = t + isz.cx;
		const RGBA *s = c.base_image[y];
		const RGBA *k = c.selection[y];
		while(t < e) {
			if(!k->r)
				*t = *s;
			t++;
			s++;
			k++;
		}
	});
	Current().image = ib;
	Refresh();
	SyncShow();
}

IconDes::Slot& IconDes::Current()
{
	if(ilist.IsCursor())
		return slot[ilist.GetKey()];
	NEVER();
	return dummy;
}

Image& IconDes::CurrentImage()
{
	return doselection ? Current().selection : Current().image;
}

Size IconDes::GetImageSize()
{
	return IsCurrent() ? Current().image.GetSize() : Size(0, 0);
}

bool IconDes::InImage(int x, int y)
{
	if(!IsCurrent())
		return false;
	Size sz = GetImageSize();
	return x >= 0 && x < sz.cx && y >= 0 && y < sz.cy;
}

void IconDes::SyncShow()
{
	iconshow.image.Clear();
	if(IsCurrent()) {
		Slot& c = Current();
		iconshow.image = c.image;
		iconshow.flags = c.flags;
		iconshow.show_downscaled = show_downscaled;
		iconshow.show_synthetics = show_synthetics;
		ilist.Set(2, RawToValue(MakeTuple(c.image, c.flags)));
	}
	iconshow.Refresh();
}

void IconDes::SetSb()
{
	magnify = max(magnify, 1);
	if(IsCurrent()) {
		sb.SetTotal(GetImageSize());
		sb.SetPage(sb.GetReducedViewSize() / magnify);
	}
}

void IconDes::Scroll()
{
	magnify = max(magnify, 1);
	Refresh();
}

void IconDes::Layout()
{
	SetSb();
}

void IconDes::SetCurrentImage(ImageBuffer& ib)
{
	CurrentImage() = ib;
	Refresh();
	SyncShow();
}

Point IconDes::GetPos(Point p)
{
	return p / max(magnify, 1) + sb;
}

void IconDes::FinishPaste()
{
	CloseText();
	if(IsCurrent()) {
		Current().pastepos = Null;
		Current().base_image.Clear();
	}
	Refresh();
	rgbactrl.Show();
}

void IconDes::MakePaste()
{
	if(!IsCurrent() || !IsPasting())
		return;
	Slot& c = Current();
	c.image = c.base_image;
	if(paste_mode == PASTE_OPAQUE)
		UPP::Copy(c.image, c.pastepos, c.paste_image, c.paste_image.GetSize());
	else
	if(paste_mode == PASTE_BACK) {
		Image h = c.image;
		UPP::Copy(c.image, c.pastepos, c.paste_image, c.paste_image.GetSize());
		UPP::Over(c.image, Point(0, 0), h, c.image.GetSize());
	}
	else
		UPP::Over(c.image, c.pastepos, Premultiply(c.paste_image), c.paste_image.GetSize());
	MaskSelection();
}

void IconDes::DoBuffer(Event<ImageBuffer&> tool)
{
	Image h = Current().base_image;
	ImageBuffer ib(h);
	tool(ib);
	CurrentImage() = ib;
	if(!doselection)
		MaskSelection();
	Refresh();
}

void IconDes::DoPainter(Event<Painter&> tool)
{
	DoBuffer([&](ImageBuffer& ib) {
		BufferPainter iw(ib);
		iw.Co();
		iw.Translate(0.5, 0.5);
		tool(iw);
	});
}

void IconDes::DoDraw(Event<IconDraw&> tool)
{
	DoBuffer([&](ImageBuffer& ib) {
		IconDraw iw(ib);
		tool(iw);
	});
}

void IconDes::DoTool(Event<IconDraw&> tool, Event<Painter&> aa_tool)
{
	if(antialiased && !doselection)
		DoPainter(aa_tool);
	else
		DoDraw(tool);
}

void IconDes::LineTool(Point p, dword flags)
{
	RGBA c = CurrentColor();
	DoTool(
		[&](IconDraw& iw) {
			iw.DrawLine(startpoint, p, pen, c);
			if(pen == 1 && Rect(iw.image.GetSize()).Contains(p))
				iw.image[p.y][p.x] = c;
		},
		[&](Painter& sw) {
			sw.Move(startpoint).Line(p).Stroke(pen, c);
		}
	);
}

void IconDes::EllipseTool0(Point p, dword flags, bool fill_empty)
{
	RGBA c = CurrentColor();
	DoTool(
		[&](IconDraw& iw) {
			iw.DrawEllipse(Rect(startpoint, p), fill_empty,
			               doselection ? c : RGBAZero(), pen, c);
		},
		[&](Painter& sw) {
			sw.DrawEllipse(Rect(startpoint, p).Normalized(),
			               fill_empty ? LtGray() : Null, pen, c);
		}
	);
}

void IconDes::EllipseTool(Point p, dword flags)
{
	EllipseTool0(p, flags, Null);
}

void IconDes::EmptyEllipseTool(Point p, dword flags)
{
	EllipseTool0(p, flags, GrayColor(128));
}

void IconDes::RadialTool(Point p, dword f)
{
	DoPainter([&](Painter& iw) {
		double r = Distance(startpoint, p);
		iw.Circle(startpoint.x, startpoint.y, Distance(startpoint, p))
		  .Fill(startpoint.x, startpoint.y, startcolor, r, CurrentColor());
	});
}

void IconDes::LinearTool(Point p, dword f)
{
	DoPainter([&](Painter& iw) {
		Pointf pf = p;
		Pointf sf = startpoint;
		Pointf vec = 10000 * Orthogonal(sf - pf);
		iw.Move(sf - vec).Line(pf - vec).Line(pf + vec).Line(sf + vec)
		  .Fill(sf, startcolor, pf, CurrentColor());
	});
}

void IconDes::RectTool0(Point p, dword flags, bool empty)
{
	DoDraw([&](IconDraw& iw) {
		rect = Rect(startpoint, p + 1).Normalized();
		Size sz = rect.GetSize();
		if(empty)
			iw.DrawRect(rect.left, rect.top, sz.cx, sz.cy,
			            doselection ? CurrentColor() : RGBAZero());
		iw.DrawFrame(rect.left, rect.top, sz.cx, sz.cy, CurrentColor(), pen);
	});
}

void IconDes::RectTool(Point p, dword flags)
{
	RectTool0(p, flags, false);
}

void IconDes::EmptyRectTool(Point p, dword flags)
{
	RectTool0(p, flags, true);
}

void IconDes::Freehand(Point p, int pen)
{
	DoDraw([&](IconDraw& iw) {
		iw.DrawRect(p.x - pen / 2, p.y - pen / 2, pen, pen, CurrentColor());
		iw.DrawLine(startpoint, p, pen, CurrentColor());
	});
	Current().base_image = CurrentImage();
	startpoint = p;
}

void IconDes::FreehandTool(Point p, dword flags)
{
	Freehand(p, pen);
}

void IconDes::DoFill(int tolerance)
{
	Image src = Current().image;
	ImageBuffer ib(CurrentImage());
	if(!doselection) {
		RGBA c = CurrentColor();
		c.r += 127;
		MaskFill(ib, c, 0);
	}
	FloodFill(src, ib, CurrentColor(), startpoint, tolerance);
	SetCurrentImage(ib);
	if(!doselection)
		MaskSelection();
}

void IconDes::HotSpotTool(Point p, dword f)
{
	if(p != Current().image.GetHotSpot()) {
		ImageBuffer ib(Current().image);
		ib.SetHotSpot(p);
		Current().image = ib;
		Refresh();
	}
}

Image IconDes::MakeIconDesCursor(const Image& arrow, const Image& cmask)
{
	RGBA c = CurrentColor();
	c.a = 255;
	Image ucmask = Unmultiply(cmask);
	ImageBuffer ib(ucmask.GetSize());
	const RGBA *m = ~ucmask;
	RGBA *t = ~ib;
	RGBA *e = ib.End();
	while(t < e) {
		*t = c;
		t->a = m->a;
		m++;
		t++;
	}
	Image cm(ib);
	Image r = arrow;
	Over(r, Point(0, 0), Premultiply(cm), r.GetSize());
	return r;
}

void IconDes::ColorChanged()
{
	cursor_image = MakeIconDesCursor(IconDesImg::Arrow(), IconDesImg::ArrowColor());
	cursor_image_free = MakeIconDesCursor(IconDesImg::Arrow(), IconDesImg::ArrowColorFree());
	fill_cursor = MakeIconDesCursor(IconDesImg::Fill(), IconDesImg::FillColor());
	fill_cursor2 = MakeIconDesCursor(IconDesImg::Fill2(), IconDesImg::FillColor());
	fill_cursor3 = MakeIconDesCursor(IconDesImg::Fill3(), IconDesImg::FillColor());
	antifill_cursor = MakeIconDesCursor(IconDesImg::Fill(), IconDesImg::AntiFill());
	PasteText();
	SetBar();
}

void IconDes::SetTool(void (IconDes::*_tool)(Point p, dword flags))
{
	FinishPaste();
	if(HasCapture())
		ReleaseCapture();
	tool = _tool;
	SetBar();
}

RGBA IconDes::CurrentColor()
{
	return rgbactrl.Get();
}

void IconDes::Paste(const Image& img)
{
	FinishPaste();
	if(!IsCurrent())
		return;
	SaveUndo();
	Slot& c = Current();
	c.base_image = c.image;
	c.paste_image = img;
#if 1
	Point p = GetPos(GetMousePos() - GetScreenView().TopLeft());
	if(InImage(p)) {
		Size isz = GetImageSize();
		Size sz = img.GetSize();
		if(p.x + sz.cx > isz.cx)
			p.x = max(isz.cx - sz.cx, 0);
		if(p.y + sz.cy > isz.cy)
			p.y = max(isz.cy - sz.cy, 0);
		c.pastepos = p;
	}
	else
		c.pastepos = Point(0, 0);
#else
	c.pastepos = Point(0, 0);
#endif
	MakePaste();
	SetBar();
}

Rect  IconDes::SelectionRect()
{
	if(!IsCurrent())
		return Null;
	Size isz = GetImageSize();
	int minx = isz.cx - 1;
	int maxx = 0;
	int miny = isz.cy - 1;
	int maxy = 0;
	for(int y = 0; y < isz.cy; y++) {
		const RGBA *k = Current().selection[y];
		for(int x = 0; x < isz.cx; x++)
			if((k++)->r == 255) {
				if(x < minx) minx = x;
				if(x > maxx) maxx = x;
				if(y < miny) miny = y;
				if(y > maxy) maxy = y;
			}
	}
	return Rect(minx, miny, maxx + 1, maxy + 1);
}

Image IconDes::Copy(const Rect& r)
{
	if(!IsCurrent() || r.IsEmpty())
		return Image();
	Slot& c = Current();
	if(Rect(GetImageSize()) == r) // return whole image with correct hotspots
		return c.image;
	ImageBuffer ib(r.GetSize());
	for(int y = r.top; y < r.bottom; y++) {
		const RGBA *s = c.image[y] + r.left;
		const RGBA *e = c.image[y] + r.right;
		const RGBA *k = c.selection[y] + r.left;
		RGBA *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			if(!k->r)
				*t = RGBAZero();
			t++;
			k++;
			s++;
		}
	}
	return Image(ib);
}

void IconDes::Delete()
{
	SetColor0(RGBAZero());
}

void IconDes::SetSelect(int a)
{
	if(!IsCurrent())
		return;
	ImageBuffer ib(GetImageSize());
	memset(ib, a, ib.GetLength() * sizeof(RGBA));
	Current().selection = ib;
	Refresh();
}

void IconDes::Select()
{
	if(!IsCurrent())
		return;
	ReleaseCapture();
	FinishPaste();
	doselection = !doselection;
	rgbactrl.Mask(doselection);
	if(doselection && SelectionRect() == GetImageSize()) {
		SetSelect(0);
		rgbactrl.MaskSet(255);
	}
	Refresh();
	SetBar();
}

void IconDes::CancelSelect()
{
	ReleaseCapture();
	doselection = false;
	rgbactrl.Mask(false);
	FinishPaste();
	SetSelect(255);
	SetBar();
}

void IconDes::InvertSelect()
{
	if(!IsCurrent())
		return;
	ImageBuffer ib(Current().selection);
	RGBA *s = ib;
	const RGBA *e = s + ib.GetLength();
	while(s < e) {
		s->r = s->g = s->b = s->a = ~s->r;
		s++;
	}
	Current().selection = ib;
	Refresh();
}

void IconDes::Move()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	selectrect = false;
	if(IsPasting())
		FinishPaste();
	else {
		doselection = false;
		rgbactrl.Mask(false);
		rgbactrl.Hide();
		Rect r = SelectionRect();
		Image m = Copy(r);
		Delete();
		SetSelect(255);
		c.base_image = c.image;
		c.paste_image = m;
		c.pastepos = r.TopLeft();
		MakePaste();
	}
	SetBar();
}

void IconDes::SelectRect()
{
	doselection = false;
	Select();
	selectrect = true;
	rect = Null;
	SetBar();
}

String PackUndo(const Vector<ImageIml>& img)
{
	return FastCompress(PackImlDataUncompressed(img));
}

Vector<ImageIml> UnpackUndo(const String& data)
{
	return UnpackImlDataUncompressed(FastDecompress(data));
}

void IconDes::SaveUndo()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	Vector<ImageIml> undo = UnpackUndo(c.undo);
	int maxn = minmax((single_mode ? 40000000 : 2000000) / max((int)c.image.GetLength(), 1), 4, 128);
	while(undo.GetCount() > maxn)
		undo.Remove(0);
	if(undo.GetCount() && undo.Top().image == c.image)
		return;
	undo.Add().image = c.image;
	c.undo = PackUndo(undo);
	c.redo.Clear();
	SetBar();
	undo.Clear();
}

void IconDes::Undo()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	Vector<ImageIml> undo = UnpackUndo(c.undo);
	if(undo.GetCount() == 0)
		return;
	Vector<ImageIml> redo = UnpackUndo(c.redo);
	redo.Add().image = c.image;
	c.image = undo.Pop().image;
	c.undo = PackUndo(undo);
	c.redo = PackUndo(redo);
	SyncImage();
	SetBar();
}

void IconDes::Redo()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	Vector<ImageIml> redo = UnpackUndo(c.redo);
	if(redo.GetCount() == 0)
		return;
	Vector<ImageIml> undo = UnpackUndo(c.undo);
	undo.Add().image = c.image;
	c.image = redo.Pop().image;
	c.undo = PackUndo(undo);
	c.redo = PackUndo(redo);
	SyncImage();
	SetBar();
}

void IconDes::SyncImage()
{
	ReleaseCapture();
	SyncShow();
	if(IsCurrent()) {
		Slot& c = Current();
		SyncShow();
		c.pastepos = Null;
		if(c.selection.GetSize() != c.image.GetSize())
			SetSelect(255);
		if(single_mode)
			info.SetLabel(Format("%d x %d", c.image.GetWidth(), c.image.GetHeight()));
	}
	selectrect = false;
	SyncList();
	SetBar();
	Refresh();
}

void IconDes::Reset()
{
	SetSelect(255);
	SyncImage();
}

void IconDes::MaskFill(ImageBuffer& ib, RGBA color, int mask)
{
	if(!IsCurrent())
		return;
	if(doselection)
		Fill(ib, color, ib.GetLength());
	else {
		RGBA *t = ib;
		RGBA *e = ib + ib.GetLength();
		const RGBA *s = Current().selection;
		while(t < e) {
			if((s++)->r == mask)
				*t = color;
			t++;
		}
	}
}

void IconDes::SetColor0(RGBA color)
{
	FinishPaste();
	doselection = false;
	rgbactrl.Mask(false);
	SaveUndo();
	ImageBuffer ib(Current().image);
	MaskFill(ib, color, 255);
	Refresh();
	SetCurrentImage(ib);
	SyncShow();
	SetBar();
}

void IconDes::SetColor()
{
	SetColor0(CurrentColor());
}

void IconDes::DoDelete()
{
	SetColor0(RGBAZero());
}

void IconDes::BeginResize()
{
	CancelSelect();
	FinishPaste();
	SaveUndo();
	Refresh();
	SyncShow();
}

void IconDes::ResizeUp()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	BeginResize();
	c.image = Magnify(c.image, 3, 3);
	Reset();
}

void IconDes::ResizeDown()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	BeginResize();
	c.image = (c.image.GetSize() / 3).IsEmpty() ? CreateImage(Size(1, 1), White)
	                                            : DownSample3x(c.image);
	Reset();
}

void IconDes::ResizeUp2()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	BeginResize();
	c.image = Magnify(c.image, 2, 2);
	Reset();
}

void IconDes::ResizeDown2()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	BeginResize();
	c.image = (c.image.GetSize() / 3).IsEmpty() ? CreateImage(Size(1, 1), White)
	                                            : DownSample2x(c.image);
	Reset();
}

void IconDes::Upscale()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	BeginResize();
	c.image = Upscale2x(c.image);
	Reset();
}

void IconDes::SingleMode()
{
	single_mode = true;
	ilist.Ctrl::Remove();
	rgbactrl.SubCtrl(&single);
	Size fsz = GetTextSize("Resize", StdFont());
	single.Add(info.HSizePos().TopPos(0, fsz.cy));
	resize.SetLabel("Resize");
	single.Add(resize.LeftPos(0, fsz.cx + 2 * fsz.cy).TopPos(4 * fsz.cy / 3, 4 * fsz.cy / 3));
	resize <<= THISBACK(EditImage);
}
