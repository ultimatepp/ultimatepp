#include "GeomCtrl.h"

namespace Upp {

#define LAYOUTFILE <Geom/Ctrl/pathedit.lay>
#include           <CtrlCore/lay.h>

#define IMAGEFILE  <Geom/Ctrl/pathedit.iml>
#define IMAGECLASS PathImg
#include           <Draw/iml.h>

class DlgPathStyleSetup
{
public:
	typedef DlgPathStyleSetup CLASSNAME;
	DlgPathStyleSetup();

	bool Run(PathStyleMisc& style);

	void PutHelp();

private:
	void Pump(PathStyleMisc& style, bool write);
	void OnMiter();

private:
//	WithHelp<
	WithPathStyleSetupLayout<TopWindow>
//	>
	dialog;
};

bool RunDlgPathStyleSetup(PathStyleMisc& style) { return DlgPathStyleSetup().Run(style); }

RegisterHelpTopicObjectTitle(DlgPathStyleSetup, "Vlastnosti stylu")

DlgPathStyleSetup::DlgPathStyleSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathStyleSetupHelpTitle());
	dialog.HelpTopic("DlgPathStyleSetup");
	dialog.width.MinMax(0.001, 20);
	dialog.begin.MinMax(0, 100);
	dialog.segment.MinMax(0, 100);
	dialog.end.MinMax(0, 100);
	dialog.miter.Hide();
	dialog.miter.GetPrev()->Hide();
	dialog.miter.Add(PathStyle::MITER_ROUND, "zaoblit");
	dialog.miter.Add(PathStyle::MITER_SHARP, "ostrý roh");
	dialog.miter.Add(PathStyle::MITER_FLAT,  "zkosit");
	dialog.miter <<= THISBACK(OnMiter);
	dialog.chamfer.NotNull().MinMax(0, 5);
}

bool DlgPathStyleSetup::Run(PathStyleMisc& style)
{
	Pump(style, false);
	OnMiter();
	if(dialog.Run() != IDOK)
		return false;
	Pump(style, true);
	return true;
}

void DlgPathStyleSetup::OnMiter()
{
	dialog.chamfer.Enable((int)~dialog.miter == PathStyle::MITER_SHARP);
	if(dialog.chamfer.IsEnabled() && dialog.chamfer.GetText().IsEmpty())
		dialog.chamfer <<= STD_CHAMFER;
}

void DlgPathStyleSetup::Pump(PathStyleMisc& style, bool write)
{
	UPP::Pump pump;
	pump
		<< PumpData(style.width,   dialog.width)
		<< PumpData(style.begin,   dialog.begin)
		<< PumpData(style.segment, dialog.segment)
		<< PumpData(style.end,     dialog.end)
		<< PumpData(style.miter,   dialog.miter);
	if(style.miter == style.MITER_SHARP)
		pump << PumpData(style.chamfer, dialog.chamfer);
	pump << write;
}

static double CalcDecadicStep(double scale, int minfine, int mincoarse, int& fine)
{
	double mu = mincoarse / scale;
	if(mu <= 1e-20)
	{
		fine = 0;
		return Null;
	}
	int exp = ffloor(log10(mu));
	double lo = pow(10.0, exp);
	int tc = (lo * 5 <= mu ? 5 : lo * 2 <= mu ? 2 : 1);
	double mf = scale * lo * tc / minfine;
	int tf = 1;
	while(tf < 100 && mf >= 10)
		mf /= 10, tf *= 10;
	if(tf <= tc)
		tf = 1;
	else
		tf *= (mf >= 5 && tc != 2 ? 5 : mf >= 2 && tc != 5 ? 2 : 1);
	fine = tf;
	return lo * tc;
}

//////////////////////////////////////////////////////////////////////
// DlgPathEditorSetup::

class DlgPathEditorSetup
{
public:
	typedef DlgPathEditorSetup CLASSNAME;
	DlgPathEditorSetup();

	bool Run(PathEditorCtrl::Setup& setup);
	void PutHelp();

private:
	void Pump(PathEditorCtrl::Setup& setup, bool write);

private:
//	WithHelp<
	WithPathEditorSetupLayout<TopWindow>
//	>
	dialog;
};

bool RunDlgPathEditorSetup(PathEditorCtrl::Setup& setup) { return DlgPathEditorSetup().Run(setup); }

RegisterHelpTopicObjectTitle(DlgPathEditorSetup, "Vlastnosti editoru")

DlgPathEditorSetup::DlgPathEditorSetup()
{
	CtrlLayoutOKCancel(dialog, DlgPathEditorSetupHelpTitle());
	dialog.HelpTopic("DlgPathEditorSetup");
	dialog.grid.MinMax(1e-5, 100);
	dialog.snap.MinMax(1e-5, 100);
}

bool DlgPathEditorSetup::Run(PathEditorCtrl::Setup& setup)
{
	Pump(setup, false);
	if(dialog.Run() != IDOK)
		return false;
	Pump(setup, true);
	return true;
}

void DlgPathEditorSetup::Pump(PathEditorCtrl::Setup& setup, bool write)
{
	UPP::Pump pump;
	pump
		<< PumpEnumData(setup.do_grid,   dialog.do_grid)
		<< PumpData    (setup.grid,      dialog.grid)
		<< PumpEnumData(setup.do_snap,   dialog.do_snap)
		<< PumpData    (setup.snap,      dialog.snap)
		<< PumpEnumData(setup.do_ruler,  dialog.do_ruler)
		<< PumpEnumData(setup.do_axis,   dialog.do_axis)
		<< write;
}

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::Setup::

void PathEditorCtrl::Setup::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 4, 4, 4, "PathEditorCtrl::Setup");
	if(version >= 1)
	{
		stream.Pack(do_grid, do_snap, do_ruler, do_axis);
		stream % grid % snap / sample_size / sample_width;
	}
}

//////////////////////////////////////////////////////////////////////
// PathEditorCtrl::ViewPlotter::

PathEditorCtrl::ViewPlotter::ViewPlotter(PathEditorCtrl *ctrl)
: draw(ctrl)
{
	Set(draw, ctrl->GetScale(), ctrl->GetDelta(), 10);
	PathMap(&PathStyleMap::App());
}

PathEditorCtrl::PathEditorCtrl()
: style(0)
{
	edit_mode = EDIT_NORMAL;
	drag_mode = DRAG_NONE;
	SetFrame(InsetFrame());
	zoom_horz_in  .SetImage(PathImg::view_zoom_in());
	zoom_vert_in  .SetImage(PathImg::view_zoom_in());
	zoom_horz_out .SetImage(PathImg::view_zoom_out());
	zoom_vert_out .SetImage(PathImg::view_zoom_out());
	zoom_horz_full.SetImage(PathImg::view_zoom_horz_full());
	zoom_vert_full.SetImage(PathImg::view_zoom_vert_full());
	zoom_full     .SetImage(PathImg::view_zoom_full());
	zoom_horz_in   <<= THISBACK(OnViewZoomHorzIn);
	zoom_horz_out  <<= THISBACK(OnViewZoomHorzOut);
	zoom_vert_in   <<= THISBACK(OnViewZoomVertIn);
	zoom_vert_out  <<= THISBACK(OnViewZoomVertOut);
	zoom_horz_full <<= THISBACK(OnViewZoomHorzFull);
	zoom_vert_full <<= THISBACK(OnViewZoomVertFull);
	zoom_full      <<= THISBACK(OnViewZoomFull);
	sample_size_tag.SetLabel("Ukázka: ");
	sample_size.Add(0,  "skrýt");
	sample_size.Add(40, "malá");
	sample_size.Add(70, "støední");
	sample_size.Add(100, "velká");
//	sample_size.SetRect(LayoutZoom(80, 0));
	sample_size <<= 40;
	sample_size <<= THISBACK(OnSampleSize);
	sample_width_tag.SetLabel("Šíøka: ");
	sample_width.NotNull().MinMax(0, MAX_WIDTH);
	sample_width <<= THISBACK(OnSampleWidth);
	sample_width <<= 2;
//	sample_width.SetRect(LayoutZoom(40, 0));
	zoom_horz_in.NoWantFocus();
	zoom_horz_out.NoWantFocus();
	zoom_horz_full.NoWantFocus();
	zoom_vert_in.NoWantFocus();
	zoom_vert_out.NoWantFocus();
	zoom_vert_full.NoWantFocus();
	zoom_full.NoWantFocus();
	AddFrame(*this);
	hruler.Horz();
	vruler.Vert();
	hruler.SetZoom(500, 0);
	hruler.MinMax(0, 20);
//	hruler.Units(1, 5);
	vruler.SetZoom(500, 0);
	vruler.MinMax(-5, 5);
//	vruler.Units(1, 5);
	hscroll <<= vscroll <<= THISBACK(OnScroll);
	hscroll.NoAutoDisable();
	vscroll.NoAutoDisable();
}

void PathEditorCtrl::FrameLayout(Rect& rc)
{
	if(!style)
		return;

	outer_sample = rc;
	rc.right -= setup.sample_size;
	rc.bottom -= setup.sample_size;
	inner_sample = rc;

	Size avail = rc.Size();
	int box = ScrollBarSize();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		avail -= rsz;
	double pwd = hruler.GetLength() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.GetLength() * vruler.GetScale() + 2 * GAP;
	zoom_horz_in.SetFrameRect(rc.left, rc.bottom - box, box, box);
	hscroll.SetFrameRect(rc.left + box, rc.bottom - box, rc.Width() - 4 * box, box);
	zoom_horz_out.SetFrameRect(rc.right - 3 * box, rc.bottom - box, box, box);
	zoom_horz_full.SetFrameRect(rc.right - 2 * box, rc.bottom - box, box, box);
	hscroll.Set(hscroll, avail.cx - box, fround(pwd));
	zoom_vert_in.SetFrameRect(rc.right - box, rc.top, box, box);
	vscroll.SetFrameRect(rc.right - box, rc.top + box, box, rc.Height() - 4 * box);
	zoom_vert_out.SetFrameRect(rc.right - box, rc.bottom - 3 * box, box, box);
	zoom_vert_full.SetFrameRect(rc.right - box, rc.bottom - 2 * box, box, box);
	vscroll.Set(vscroll, avail.cy - box, fround(pht));
	zoom_full.SetFrameRect(rc.right - box, rc.bottom - box, box, box);
	rc.right  -= box;
	rc.bottom -= box;
	if(setup.do_ruler)
	{
		hruler.SetFrameRect(rc.left + rsz, rc.top, rc.Width() - rsz, rsz);
		vruler.SetFrameRect(rc.left, rc.top + rsz, rsz, rc.Height() - rsz);
		rc.left += rsz;
		rc.top += rsz;
	}
}

void PathEditorCtrl::FrameAddSize(Size& sz)
{
	if(!style)
		return;
	int box = ScrollBarSize();
	int rsz = RulerCtrl::GetStdWidth();
	if(setup.do_ruler)
		sz += rsz;
	sz += box;
}

static void PaintSamplePath(PathDraw& path, const Rect& mid)
{
	path.MoveTo(mid.left, mid.bottom);
	path.LineTo(mid.left + 20, mid.bottom);
	path.LineTo(mid.left + 40, mid.bottom - 10);
	path.LineTo(mid.left + 50, mid.bottom + 10);
	path.LineTo(mid.left + 70, mid.bottom);
	if(mid.Width() >= 200)
	{
		path.LineTo(mid.left + 100, mid.bottom);
		path.LineTo(mid.left + 130, mid.bottom - 10);
		path.LineTo(mid.left + 170, mid.bottom + 10);
		path.LineTo(mid.left + 200, mid.bottom);
	}
	if(mid.Width() >= 300)
	{
		path.LineTo(mid.left + 240, mid.bottom - 10);
		path.LineTo(mid.left + 250, mid.bottom + 10);
		path.LineTo(mid.left + 260, mid.bottom - 10);
		path.LineTo(mid.left + 300, mid.bottom);
	}
	path.LineTo(mid.right, mid.bottom);
	path.LineTo(mid.right, mid.bottom - 20);
	path.LineTo(mid.right - 10, mid.bottom - 40);
	path.LineTo(mid.right + 10, mid.bottom - 50);
	path.LineTo(mid.right, mid.bottom - 70);
	if(mid.Height() >= 200)
	{
		path.LineTo(mid.right,      mid.bottom - 100);
		path.LineTo(mid.right - 10, mid.bottom - 130);
		path.LineTo(mid.right + 10, mid.bottom - 170);
		path.LineTo(mid.right,      mid.bottom - 200);
	}
	path.LineTo(mid.right, mid.top);
	path.Paint();
}

void PathEditorCtrl::FramePaint(Draw& draw, const Rect& r)
{
	Rect rc = r;
	if(setup.sample_size > 0)
	{
		PumpTraces(true);
		draw.Clip(outer_sample);
		draw.ExcludeClip(inner_sample);
		draw.DrawRect(outer_sample, SLtGray);
		PathDraw path(draw, *style, Black, setup.sample_width);
		int half = setup.sample_size >> 1;
		Rect mid(outer_sample.left + 10, outer_sample.top + 10,
			(outer_sample.right  + inner_sample.right)  >> 1,
			(outer_sample.bottom + inner_sample.bottom) >> 1);
		PaintSamplePath(path, mid);
		if(setup.do_axis)
		{
			path.Set(draw, PathStyle::solid(), LtRed, 0);
			PaintSamplePath(path, mid);
		}
		draw.End();
		rc = inner_sample;
	}
	if(setup.do_ruler)
	{
		int r = RulerCtrl::GetStdWidth();
		draw.DrawRect(rc.left, rc.top, r, r, SGray);
	}
	Size scb(vscroll.GetRect().Width(), hscroll.GetRect().Height());
	if(scb.cx && scb.cy)
		draw.DrawRect(rc.right - scb.cx, rc.bottom - scb.cy, scb.cx, scb.cy, SGray);
}

void PathEditorCtrl::FrameAdd(Ctrl& ctrl)
{
	ctrl << hruler << vruler << (Ctrl &)hscroll << (Ctrl &)vscroll
		<< zoom_horz_in << zoom_horz_out << zoom_horz_full
		<< zoom_vert_in << zoom_vert_out << zoom_vert_full
		<< zoom_full;
}

void PathEditorCtrl::FrameRemove()
{
	hruler.Remove();
	vruler.Remove();
	hscroll.Remove();
	vscroll.Remove();
	zoom_horz_in.Remove();
	zoom_horz_out.Remove();
	zoom_horz_full.Remove();
	zoom_vert_in.Remove();
	zoom_vert_out.Remove();
	zoom_vert_full.Remove();
	zoom_full.Remove();
}

static inline bool PathStyleTraceLeftLess(const PathStyle::Trace& a, const PathStyle::Trace& b)
{ return a.left < b.left; }

void PathEditorCtrl::PumpTraces(bool write)
{
	if(write)
	{
		style->traces <<= traces;
		Sort(style->traces, &PathStyleTraceLeftLess);
	}
	else
	{
		selection.Clear();
		traces <<= style->traces;
	}
}

void PathEditorCtrl::Layout()
{
	if(!style)
		return;
	Size size = GetSize();
	double cx = style->begin + style->segment + style->end, cy = style->width / 2;
	int fine;
	double dstep = CalcDecadicStep(hruler.GetScale(), 5, 100, fine);
	hruler.MinMax(0, cx);
	hruler.SetTextStep(dstep);
	hruler.SetSmallStep(dstep / fine);
	vruler.MinMax(-cy, cy);
	dstep = CalcDecadicStep(vruler.GetScale(), 5, 100, fine);
	vruler.SetTextStep(dstep);
	vruler.SetSmallStep(dstep / fine);
/*	if(cx * hruler.GetScale() <= size.cx)
		hruler.SetZoomDelta(GAP);
	if(style->width * vruler.GetScale() <= size.cy)
		vruler.Delta(GAP + cy * vruler.GetScale());
*/	OnScroll();
	Refresh();
}

void PathEditorCtrl::UpdateScroll()
{
	Size size = GetSize();
	double pwd = hruler.GetLength() * hruler.GetScale() + 2 * GAP + HGAP;
	double pht = vruler.GetLength() * vruler.GetScale() + 2 * GAP;
	hruler.SetZoom(hruler.GetScale(), pwd <= size.cx ? GAP : minmax(hruler.GetDelta(), size.cx - double(GAP) - pwd, double(GAP)));
	double t = style->width / 2 * vruler.GetScale();
	vruler.SetZoom(vruler.GetScale(), pht <= size.cy ? pht / 2 : minmax(vruler.GetDelta(), size.cy - double(GAP) - t, double(GAP) + t));
	int hpos = GAP - fround(hruler.GetDelta());
	int vpos = GAP + fround(style->width * vruler.GetScale() / 2 - vruler.GetDelta());
	Layout();
	hscroll = hpos;
	vscroll = vpos;
	OnScroll();
	Refresh();
}

void PathEditorCtrl::OnScroll()
{
	double dx = hruler.GetDelta(), dy = vruler.GetDelta();
	if(!hscroll.GetRect().IsEmpty())
		dx = GAP - (int)hscroll;
	if(!vscroll.GetRect().IsEmpty())
		dy = GAP + style->width * vruler.GetScale() / 2 - (int)vscroll;
	if(dx != hruler.GetDelta() || dy != vruler.GetDelta())
	{
		hruler.SetZoom(hruler.GetScale(), dx);
		vruler.SetZoom(vruler.GetScale(), dy);
		Refresh();
	}
}

void PathEditorCtrl::UpdateSetup()
{
	sample_size <<= setup.sample_size;
	sample_width <<= setup.sample_width;
	Layout();
	sample_width.Enable(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleSize()
{
	setup.sample_size = ~sample_size;
	Layout();
	sample_width.Enable(setup.sample_size > 0);
}

void PathEditorCtrl::OnSampleWidth()
{
	setup.sample_width = ~sample_width;
	UpdateSample();
}

static void PaintSizeBreak(PlotterTool& tool, double x, double y, Pointf size)
{
	tool.MoveTo(x, -y + size.y);
	tool.LineTo(x - size.x, -y);
	tool.LineTo(x + size.x, -y);
	tool.LineTo(x, -y + size.y);
	tool.LineTo(x, y - size.y);
	tool.LineTo(x + size.x, y);
	tool.LineTo(x - size.x, y);
	tool.LineTo(x, y - size.y);
	tool.Paint();
}

void PathEditorCtrl::Paint(Draw& draw)
{
	Rectf full = GetFullExtent();
	Rectf vis = ClientToUnits(draw.GetPaintRect()) & full;
	Rect cvis = UnitsToClient(vis) & draw.GetPaintRect();
	DrawRectMinusRect(draw, draw.GetPaintRect(), cvis, SGray);
	if(cvis.IsEmpty())
		return;
	draw.Clip(cvis);
	draw.DrawRect(cvis, White);
	if(setup.do_grid && setup.grid >= 1e-10)
	{
		double s = setup.grid;
		while(s * hruler.GetScale() < 4)
			s *= 10;
		int l, h;
		for(l = ffloor(vis.left / s), h = fceil(vis.right / s); l <= h; l++)
		{
			Point top = UnitsToClient(Pointf(l * s, vis.top));
			draw.DrawRect(top.x, cvis.top, 1, cvis.Height(), LtGray);
		}
		s = setup.grid;
		while(s * vruler.GetScale() < 4)
			s *= 10;
		for(l = ffloor(vis.top / s), h = fceil(vis.bottom / s); l <= h; l++)
		{
			Point left = UnitsToClient(Pointf(vis.left, l * s));
			draw.DrawRect(cvis.left, left.y, cvis.Width(), 1, LtGray);
		}
	}
	Plotter plotter;
	plotter.Set(draw, GetScale(), GetDelta(), 10);
	plotter.PathMap(&PathStyleMap::App());
	AreaTool area;
	PathTool path;
	for(int i = 0; i < traces.GetCount(); i++)
	{
		const PathStyle::Trace& trace = traces[i];
		int s = (selection.Find(i) >= 0 ? i == selection.Top() ? 2 : 1 : 0);
		Color color = Nvl(trace.color, Black);
		Color outline = (s == 0 ? color : s == 1 ? LtBlue : Color(192, 192, 255));
		if(trace.IsAreaEmpty())
		{
			path.Set(plotter, ".dot", outline, 3);
			if(path.SetExtent(trace.GetExtent()))
				trace.Paint(path, true, outline);
		}
		else
		{
			area.Set(plotter, Nvl(trace.color, Black), I64(0xaa55aa55aa55aa55), Null, outline, 3);
			if(area.SetExtent(trace.GetExtent()))
				trace.Paint(area, false, outline);
		}
	}
	draw.End();
	{ // draw begin & end bar
		Pointf size = Pointf(16, 16) / GetScale();
		AreaTool area;
		area.Set(plotter, LtGreen, 0, Null, Green, 2);
		double x = style->begin;
		double h = style->width / 2;
		PaintSizeBreak(area, x, h, size);
		x += style->segment;
		PaintSizeBreak(area, x, h, size);
		x += style->end;
		PaintSizeBreak(area, x, h, size);
	}
	if(!selection.IsEmpty())
	{
		const PathStyle::Trace& trace = traces[selection.Top()];
		Pointf list[] =
		{
			trace.LeftTop(), trace.CenterTop(), trace.RightTop(),
			trace.LeftCenter(), trace.RightCenter(),
			trace.LeftBottom(), trace.CenterBottom(), trace.RightBottom(),
		};
		for(int i = 0; i < __countof(list); i++)
			if(vis.Contains(list[i]))
			{
				enum { DELTA = 3 };
				Point pt = UnitsToClient(list[i]);
				Rect rc(pt.x - DELTA, pt.y - DELTA, pt.x + DELTA + 1, pt.y + DELTA + 1);
				draw.DrawRect(rc, LtRed);
				rc.Inflate(1);
				DrawFrame(draw, rc, White, Black);
			}
	}
}

Image PathEditorCtrl::CursorImage(Point pt, dword keyflags)
{
	switch(edit_mode)
	{
	case EDIT_ZOOM: return PathImg::drag_zoom_cursor();
	case EDIT_PAN:  return PathImg::drag_pan_cursor();
	}
	if(IsDragging())
		switch(drag_mode)
		{
		default: NEVER();
		case DRAG_SELECT: return Image::Arrow();
		case DRAG_BEGIN:
		case DRAG_SEGMENT:
		case DRAG_END:    return Image::SizeHorz();
		case DRAG_INSERT: return PathImg::drag_insert_cursor();
		case DRAG_TRACK:  return PathStyle::Trace::GetTrackCursor(track_style);
		case DRAG_MOVE:   return Image::SizeAll();
		}
	Pointf up = ClientToUnits(pt), start;
	int i = GetTrackStyle(up, start);
	if(i)
		return PathStyle::Trace::GetTrackCursor(i);
	if(GetDragSize(up))
		return Image::SizeHorz();
	i = FindObject(up);
	if(i >= 0 && selection.Find(i) >= 0)
		return Image::SizeAll(); // move
	return Image::Arrow();
}

int PathEditorCtrl::AddObject(const PathStyle::Trace& trace)
{
	int i = traces.GetCount();
	traces.Add(trace);
	RefreshObject(i);
	Update();
	UpdateSample();
	return i;
}

void PathEditorCtrl::SetObject(int i, const PathStyle::Trace& trace)
{
	RefreshObject(i);
	traces[i] = trace;
	RefreshObject(i);
	Update();
	UpdateSample();
}

void PathEditorCtrl::UpdateSample()
{
	if(setup.sample_size > 0)
	{
		RefreshFrame(Rect(outer_sample.left,  outer_sample.top,    outer_sample.right, inner_sample.top));
		RefreshFrame(Rect(outer_sample.left,  inner_sample.bottom, outer_sample.right, outer_sample.bottom));
		RefreshFrame(Rect(outer_sample.left,  inner_sample.top,    outer_sample.right, inner_sample.bottom));
		RefreshFrame(Rect(inner_sample.right, inner_sample.top,    outer_sample.right, inner_sample.bottom));
	}
}

void PathEditorCtrl::RefreshObject(int item)
{
	if(item >= 0 && item < traces.GetCount())
	{
		const PathStyle::Trace& trace = traces[item];
		Rectf rc(trace.left, min(trace.left_top, trace.right_top), trace.right, max(trace.left_bottom, trace.right_bottom));
		Rect cl = UnitsToClient(rc);
		cl.Inflate(selection.Find(item) >= 0 ? 10 : 5);
		Refresh(cl);
	}
}

bool PathEditorCtrl::WriteClipboard()
{
	if(!IsSelection())
		return false; // no-op
	PathStyle::Clip clip;
	for(int i = 0; i < selection.GetCount(); i++)
		clip.traces.Add(traces[selection[i]]);
	clip.Write();
	return true;
//	return clip.Write();
}

bool PathEditorCtrl::ReadClipboard()
{
	PathStyle::Clip clip;
	if(!clip.Read())
		return false;
	Rectf extent = clip.GetExtent();
	Rectf full = GetFullExtent();
	bool setwd = false, setsg = false;
	if(extent.top < full.top || extent.bottom > full.bottom)
	{
		int r = PromptYesNoCancel("Vybrané objekty pøesahují limit daný šíøkou èáry. "
			"Chcete šíøku èáry upravit?");
		if(r < 0)
			return true;
		setwd = !!r;
	}
	if(extent.right > full.right)
	{
		int r = PromptYesNoCancel("Vybrané objekty pøesahují limit daný délkou segmentu. "
			"Chcete délku segmentu upravit?");
		if(r < 0)
			return true;
		setsg = !!r;
	}

	if(setwd)
		style->width = max(style->width, max(-extent.top, extent.bottom));

	if(setsg)
		style->segment += full.right - extent.right;

	ClearSelection();
	int c = traces.GetCount();
	Append(traces, clip.traces);
	Vector<int> to_add;
	while(c < traces.GetCount())
		to_add.Add(c++);
	AddSelection(to_add);

	if(setwd || setsg)
		Layout();
	UpdateSample();
	return true;
}

Rectf PathEditorCtrl::GetFullExtent() const
{
	return Rectf(0, -style->width / 2,
		style->begin + style->segment + style->end, style->width / 2);
}

Rectf PathEditorCtrl::GetSelectionExtent() const
{
	if(selection.IsEmpty())
		return Null;
	Rectf rc = traces[selection[0]].GetExtent();
	for(int i = 1; i < selection.GetCount(); i++)
		rc |= traces[selection[i]].GetExtent();
	return rc;
}

int PathEditorCtrl::GetSelectionLeader() const
{
	return selection.IsEmpty() ? -1 : selection.Top();
}

void PathEditorCtrl::AddSelection(const Vector<int>& list)
{
	int old = GetSelectionLeader();
	for(int i = 0; i < list.GetCount(); i++)
		if(selection.Find(list[i]) < 0)
		{
			RefreshObject(list[i]);
			selection.Add(list[i]);
		}
	RefreshObject(old);
	WhenRescan();
}

void PathEditorCtrl::XorSelection(const Vector<int>& list)
{
	int old_leader = GetSelectionLeader();
	for(int i = 0; i < list.GetCount(); i++)
	{
		RefreshObject(list[i]);
		int pos = selection.Find(list[i]);
		if(pos >= 0)
			selection.Remove(pos);
		else
			selection.Add(list[i]);
	}
	int new_leader = GetSelectionLeader();
	if(new_leader != old_leader)
	{
		RefreshObject(old_leader);
		RefreshObject(new_leader);
	}
	WhenRescan();
}

void PathEditorCtrl::RemoveSelection(const Vector<int>& list)
{
	int count = selection.GetCount();
	for(int i = 0; i < list.GetCount(); i++)
	{
		int pos = selection.Find(list[i]);
		if(pos >= 0)
		{
			RefreshObject(list[i]);
			selection.Remove(pos);
		}
	}
	if(selection.GetCount() != count && !selection.IsEmpty())
		RefreshObject(selection.Top());
	WhenRescan();
}

void PathEditorCtrl::RefreshSelection()
{
	for(int i = 0; i < selection.GetCount(); i++)
		RefreshObject(selection[i]);
}

void PathEditorCtrl::ClearSelection()
{
	RefreshSelection();
	selection.Clear();
	WhenRescan();
}

void PathEditorCtrl::SetSelection(const Vector<int>& list)
{
	ClearSelection();
	Append(selection, list);
//	selection <<= list;
	RefreshSelection();
	WhenRescan();
}

Pointf PathEditorCtrl::ClientToUnits(Point pt) const
{
	return Pointf(hruler.FromClient(pt.x), vruler.FromClient(pt.y));
}

Point PathEditorCtrl::UnitsToClient(Pointf pt) const
{
	return Point(hruler.ToClient(pt.x), vruler.ToClient(pt.y));
}

Rectf PathEditorCtrl::ClientToUnits(const Rect& rc) const
{
	return SortRectf(ClientToUnits(rc.TopLeft()), ClientToUnits(rc.BottomRight()));
}

Rect PathEditorCtrl::UnitsToClient(const Rectf& rc) const
{
	return RectSort(UnitsToClient(rc.TopLeft()), UnitsToClient(rc.BottomRight()));
}

Pointf PathEditorCtrl::Snap(Pointf pt) const
{
	if(IsNull(pt) || !setup.do_snap || setup.snap <= 1e-10)
		return pt;
	return Pointf(floor(pt.x / setup.snap + 0.5) * setup.snap, floor(pt.y / setup.snap + 0.5) * setup.snap);
}

Pointf PathEditorCtrl::ClientToSnap(Point pt) const
{
	return Snap(ClientToUnits(pt));
}

bool PathEditorCtrl::Key(dword key, int repcnt)
{
	Size shift(0, 0);
	switch(key)
	{
	case K_ADD:      if(setup.sample_width < MAX_WIDTH) { sample_width <<= ++setup.sample_width; UpdateSample(); } return true;
	case K_SUBTRACT: if(setup.sample_width > 0)         { sample_width <<= --setup.sample_width; UpdateSample(); }; return true;

	case K_LEFT:  shift.cx = -1; break;
	case K_UP:    shift.cy = -1; break;
	case K_RIGHT: shift.cx = +1; break;
	case K_DOWN:  shift.cy = +1; break;

	default:
		return DragDropCtrl::Key(key, repcnt);
	}

	if((shift.cx | shift.cy) && IsSelection())
	{
		Rectf ext = GetSelectionExtent();
		Rectf full = GetFullExtent();
		Pointf snap(setup.snap, setup.snap);
		if(!setup.do_snap || setup.snap <= 1e-10)
			snap = Pointf(1, 1) / GetScale();
		Pointf step = snap * shift;
		Pointf lt = traces[GetSelectionLeader()].LeftTop();
		if(step.x)
			step.x = snap.x * floor((lt.x + step.x) / snap.x + 0.5) - lt.x;
		if(step.y)
			step.y = snap.y * floor((lt.y + step.y) / snap.y + 0.5) - lt.y;
		for(int i = 0; i < selection.GetCount(); i++)
			SetObject(selection[i], traces[selection[i]].GetMove(step));
	}

	return true;
}

bool PathEditorCtrl::Push(Point pt, dword keyflags)
{
	if(edit_mode == EDIT_ZOOM)
	{
		drag_mode = DRAG_ZOOM;
		return true;
	}
	if(edit_mode == EDIT_PAN)
	{
		track_start = GetDelta();
		drag_mode = DRAG_PAN;
		return true;
	}

	track_limit = GetFullExtent();

	Pointf up = ClientToUnits(pt);
	if(track_style = GetTrackStyle(up, track_start))
	{
		drag_mode = DRAG_TRACK;
		return true;
	}
	int i = FindObject(up);
	if(i < 0)
	{
		if((drag_mode = GetDragSize(up)) == 0)
			drag_mode = (keyflags & (K_SHIFT | K_CTRL) ? DRAG_SELECT : DRAG_INSERT);
		return true;
	}
	if(keyflags & K_CTRL)
		XorSelection(i);
	else if(keyflags & K_SHIFT)
	{
		RemoveSelection(i);
		AddSelection(i);
	}
	else if(selection.Find(i) < 0)
		SetSelection(i);
	track_start = traces[i].LeftTop();
	drag_mode = DRAG_MOVE;
	Rectf rc = GetSelectionExtent();
	track_limit.left   -= rc.left;
	track_limit.top    -= rc.top;
	track_limit.right  -= rc.right;
	track_limit.bottom -= rc.bottom;
	return true;
}

void PathEditorCtrl::MouseMove(Point pt, dword keyflags)
{
	Pointf up = ClientToSnap(pt);
//	hruler.Mouse(up.x);
	hruler.Sync();
//	vruler.Mouse(up.y);
	vruler.Sync();
	if(coords)
	{
		String s;
		s << "x = " << up.x << ", y = " << up.y;
		*coords <<= s;
		coords->Sync();
	}

	DragDropCtrl::MouseMove(pt, keyflags);
}

void PathEditorCtrl::RightDown(Point pt, dword keyflags)
{
	Pointf up = ClientToUnits(pt);
	int i = FindObject(up);
	if(i >= 0)
	{
		RemoveSelection(i);
		AddSelection(i);
	}
	MenuBar::Execute(THISBACK(ToolEdit));
}

void PathEditorCtrl::Drag(Point pt, Point last, Point next, dword keyflags)
{
	ViewPlotter plotter(this);
	PathTool path;
	bool drag_size = (drag_mode == DRAG_BEGIN || drag_mode == DRAG_SEGMENT || drag_mode == DRAG_END);
	path.Set(plotter, ".dot", drag_mode == DRAG_SELECT ? LtRed : drag_size ? Green : LtBlue, 3);
	Pointf up = ClientToSnap(pt);
//	int rop = SetROP2(plotter.draw, R2_NOTXORPEN);
	if(drag_mode == DRAG_INSERT)
	{
		if(!IsNull(next))
			path.Rectangle(SortRectf(up, ClientToSnap(next)) & track_limit, true);
		if(!IsNull(last))
			path.Rectangle(SortRectf(up, ClientToSnap(last)) & track_limit, true);
	}
	else if(drag_mode == DRAG_SELECT)
	{
		up = ClientToUnits(pt);
		if(!IsNull(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!IsNull(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		if(IsSelection())
		{
			const PathStyle::Trace& base = traces[selection.Top()];
			if(!IsNull(next))
				PathStyle::Trace(base).Track(ClientToSnap(next) - track_start, track_style).Bind(track_limit).Paint(path);
			if(!IsNull(last))
				PathStyle::Trace(base).Track(ClientToSnap(last) - track_start, track_style).Bind(track_limit).Paint(path);
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		if(IsSelection())
		{
			Size delta = UnitsToClient(track_start) - pt;
			Pointf un(Null), ul(Null);
			if(!IsNull(next))
				un = fpminmax(ClientToSnap(next + delta) - track_start, track_limit);
			if(!IsNull(last))
				ul = fpminmax(ClientToSnap(last + delta) - track_start, track_limit);
			for(int i = 0; i < selection.GetCount(); i++)
			{
				const PathStyle::Trace& base = traces[selection[i]];
				if(!IsNull(next))
					base.GetMove(un).Paint(path, true);
				if(!IsNull(last))
					base.GetMove(ul).Paint(path, true);
			}
		}
		else
			NEVER();
	}
	else if(drag_size)
	{
		Pointf size = Pointf(16, 16) / GetScale();
		if(!IsNull(next))
		{
			Pointf un = ClientToSnap(next);
			PaintSizeBreak(path, un.x, style->width / 2, size);
		}
		if(!IsNull(last))
		{
			Pointf ul = ClientToSnap(last);
			PaintSizeBreak(path, ul.x, style->width / 2, size);
		}
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		up = ClientToUnits(pt);
		if(!IsNull(next))
			path.Rectangle(SortRectf(up, ClientToUnits(next)), true);
		if(!IsNull(last))
			path.Rectangle(SortRectf(up, ClientToUnits(last)), true);
		path.Paint();
	}
	else if(drag_mode == DRAG_PAN)
	{
		Pointf new_offset = track_start;
		if(!IsNull(next))
			new_offset += next - pt;
		hruler.SetZoom(hruler.GetScale(), new_offset.x);
		vruler.SetZoom(vruler.GetScale(), new_offset.y);
		UpdateScroll();
		if(!IsNull(next))
			Sync();
	}
	else
		NEVER();
	path.Paint();
}

void PathEditorCtrl::Drop(Point pt, Point end, dword keyflags)
{
	Pointf up = ClientToSnap(end);
	if(drag_mode == DRAG_INSERT)
	{
		Rectf rc = SortRectf(ClientToSnap(pt), up) & track_limit;
		if(rc.left == rc.right && rc.top == rc.bottom)
			return;
		PathStyle::Trace trace;
		trace.left = rc.left;
		trace.right = rc.right;
		trace.left_top = trace.right_top = rc.top;
		trace.left_bottom = trace.right_bottom = rc.bottom;
		SetSelection(AddObject(trace));
	}
	else if(drag_mode == DRAG_SELECT)
	{
		Rectf rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		Vector<int> list = FindObject(rc);
		if(keyflags & K_SHIFT)
			AddSelection(list);
		else if(keyflags & K_CTRL)
			XorSelection(list);
		else
			SetSelection(list);
	}
	else if(drag_mode == DRAG_TRACK)
	{
		up -= track_start;
		if(IsSelection())
		{
			int i = selection.Top();
			SetObject(i, PathStyle::Trace(traces[i]).Track(up, track_style).Bind(track_limit));
		}
		else
			NEVER();
	}
	else if(drag_mode == DRAG_MOVE)
	{
		Size delta = UnitsToClient(track_start) - pt;
		up = fpminmax(ClientToSnap(end + delta) - track_start, track_limit);
		for(int i = 0; i < selection.GetCount(); i++)
			SetObject(selection[i], traces[selection[i]].GetMove(up));
	}
	else if(drag_mode == DRAG_BEGIN || drag_mode == DRAG_SEGMENT || drag_mode == DRAG_END)
	{
		double lse = style->begin + style->segment;
		double le = lse + style->end;
		switch(drag_mode)
		{
		case DRAG_BEGIN:
			style->begin = max(0.0, up.x);
			style->segment = max(0.0, lse - style->begin);
			style->end = max(0.0, le - style->segment - style->begin);
			break;

		case DRAG_SEGMENT:
			style->segment = up.x - style->begin;
			if(style->segment < 0)
			{
				style->begin = max(0.0, style->begin + style->segment);
				style->segment = 0;
			}
			style->end = max(0.0, le - style->begin - style->segment);
			break;

		case DRAG_END:
			style->end = up.x - style->segment - style->begin;
			if(style->end < 0)
			{
				if(up.x < style->begin)
					style->begin = max(0.0, up.x);
				if(up.x < style->begin + style->segment)
					style->segment = max(0.0, up.x - style->begin);
				style->end = 0;
			}
			break;

		default:
			NEVER();
		}
		Layout();
		UpdateSample();
	}
	else if(drag_mode == DRAG_ZOOM)
	{
		Rectf rc = SortRectf(ClientToUnits(pt), ClientToUnits(end));
		rc &= GetFullExtent();
		Size client = GetSize();
		Size avail = max(client - 2 * GAP, Size(1, 1));
		double ratio = min(avail.cx / max(rc.Width(), 1e-3), avail.cy / max(rc.Height(), 1e-3));
		hruler.SetZoom(ratio, (client.cx - (rc.left + rc.right)  * ratio) / 2);
		vruler.SetZoom(ratio, (client.cy - (rc.top  + rc.bottom) * ratio) / 2);
		UpdateScroll();
	}
	else if(drag_mode == DRAG_PAN)
	{
		Pointf dest = track_start + Sizef(end - pt);
		hruler.SetZoom(hruler.GetScale(), dest.x);
		vruler.SetZoom(vruler.GetScale(), dest.y);
		UpdateScroll();
	}
	else
		NEVER();
}

void PathEditorCtrl::Click(Point pt, dword keyflags)
{
	if(drag_mode == DRAG_INSERT && !(keyflags & (K_CTRL | K_SHIFT)))
		ClearSelection();
}

int PathEditorCtrl::FindObject(Pointf pt) const
{
	enum { TOLERANCE = 10 };
	Pointf inflate = Pointf(TOLERANCE, TOLERANCE) / GetScale();
	double best = TOLERANCE;
	int found = -1;
	for(int i = 0; i < traces.GetCount(); i++)
	{
		PathStyle::Trace t = traces[i];
		t.Inflate(inflate);
		if(!t.Contains(pt))
			continue;
		double d = t.GetDistance(pt);
		if(d < best)
		{
			best = d;
			found = i;
		}
	}
	return found;
}

Vector<int> PathEditorCtrl::FindObject(const Rectf& rc) const
{
	Vector<int> list;
	for(int i = 0; i < traces.GetCount(); i++)
		if(rc.Contains(traces[i].GetExtent()))
			list.Add(i);
	return list;
}

int PathEditorCtrl::GetTrackStyle(Pointf pt, Pointf& track_start) const
{
	Pointf tolerance = Pointf(10, 10) / GetScale();
	return selection.IsEmpty() ? 0
		: traces[selection.Top()].GetTrackStyle(pt, tolerance, track_start);
}

int PathEditorCtrl::GetDragSize(Pointf pt) const
{
	Pointf tolerance = Pointf(16, 16) / GetScale();
	double h = style->width / 2;
	if(pt.y >= -h + tolerance.y && pt.y <= h - tolerance.y)
		return 0;
	double db = fabs(pt.x - style->begin);
	double ds = fabs(pt.x - style->begin - style->segment);
	double de = fabs(pt.x - style->begin - style->segment - style->end);
	if(db >= tolerance.x && ds >= tolerance.x && de >= tolerance.x)
		return 0;
	return (ds <= db && ds <= de ? DRAG_SEGMENT : de <= db && de <= ds ? DRAG_END : DRAG_BEGIN);
}

void PathEditorCtrl::ToolEdit(Bar& bar)
{
	ToolEditColor(bar);
	bar.Separator();
	ToolEditCut(bar);
	ToolEditCopy(bar);
	ToolEditPaste(bar);
	ToolEditDelete(bar);
	bar.Separator();
//	bar.Add("Základní", THISBACK(ToolEditBasic))
//		.Help("Naèíst do editoru jeden ze základních stylù èár");
//	bar.Separator();
	ToolEditSelectAll(bar);
}

void PathEditorCtrl::ToolEditCopy(Bar& bar)
{
	bar.Add(IsSelection(), "Kopírovat", CtrlImg::copy(), THISBACK(OnEditCopy))
		.Key(K_CTRL_C) //, K_CTRL_INSERT)
		.Help("Zkopírovat vybrané objekty do schránky");
}

void PathEditorCtrl::OnEditCopy()
{
	if(IsSelection() && !WriteClipboard())
		PromptOK("Chyba pøi zápisu do schránky.");
}

void PathEditorCtrl::ToolEditCut(Bar& bar)
{
	bar.Add(IsSelection(), "Vyjmout", CtrlImg::cut(), THISBACK(OnEditCut))
		.Key(K_CTRL_X) //, K_CTRL_DELETE)
		.Help("Odstranit vybrané objekty ze stylu a pøesunout je do schránky");
}

void PathEditorCtrl::OnEditCut()
{
	if(IsSelection())
		if(WriteClipboard())
			OnEditDelete();
		else
			PromptOK("Chyba pøi zápisu do schránky.");
}

void PathEditorCtrl::ToolEditPaste(Bar& bar)
{
	bar.Add("Vložit", CtrlImg::paste(), THISBACK(OnEditPaste))
		.Key(K_CTRL_V) //, K_SHIFT_INSERT)
		.Help("Zkopírovat vybrané objekty do schránky");
}

void PathEditorCtrl::OnEditPaste()
{
	if(IsSelection() && !ReadClipboard())
		PromptOK("Chyba pøi zápisu do schránky.");
}

void PathEditorCtrl::ToolEditSelectAll(Bar& bar)
{
	bar.Add("Vybrat vše", THISBACK(OnEditSelectAll))
		.Key(K_CTRL_A)
		.Help("Oznaèit všechny úseky jako vybrané");
}

void PathEditorCtrl::OnEditSelectAll()
{
	ClearSelection();
	Vector<int> to_add;
	for(int i = 0; i < traces.GetCount(); i++)
		to_add.Add(i);
	AddSelection(to_add);
}

void PathEditorCtrl::ToolEditColor(Bar& bar)
{
	bar.Add(IsSelection(), "Barva...", PathImg::edit_color(), THISBACK(OnEditColor))
		.Key(K_CTRL_R)
		.Help("Nastavit barvu vybraných objektù");
}

void PathEditorCtrl::OnEditColor()
{
	if(!IsSelection())
		return;
	bool ok;
	Color c = RunDlgSelectColor(traces[GetSelectionLeader()].color, false, "Barva objektù...", &ok);
	if(ok)
		for(int i = 0; i < selection.GetCount(); i++)
			SetObject(selection[i], traces[selection[i]].GetTraceColor(c));
}

void PathEditorCtrl::ToolEditDelete(Bar& bar)
{
	bar.Add(IsSelection(), "Smazat", CtrlImg::remove(), THISBACK(OnEditDelete))
		.Key(K_DELETE)
		.Help("Smazat vybrané objekty");
}

void PathEditorCtrl::OnEditDelete()
{
	RefreshSelection();
	Vector<int> sel = selection.PickKeys();
	selection.Clear();
	Sort(sel);
	while(!sel.IsEmpty())
		traces.Remove(sel.Pop());
	WhenRescan();
	Update();
	UpdateSample();
}

void PathEditorCtrl::ToolView(Bar& bar)
{
	ToolViewZoomIn(bar);
	ToolViewZoomOut(bar);
	if(bar.IsMenuBar())
		ToolViewZoomFull(bar);
	ToolViewPan(bar);
	bar.MenuSeparator();
	ToolViewZoomHorzIn(bar);
	ToolViewZoomHorzOut(bar);
	ToolViewZoomVertIn(bar);
	ToolViewZoomVertOut(bar);
}

void PathEditorCtrl::ToolViewZoomIn(Bar& bar)
{
	bar.Add("Zvìtšit", PathImg::view_zoom_in(), THISBACK(OnViewZoomIn))
		.Check(edit_mode == EDIT_ZOOM)
		.Help("Zvìtšit vybrané místo nebo oblast");
}

void PathEditorCtrl::OnViewZoomIn()
{
	edit_mode = (edit_mode == EDIT_ZOOM ? EDIT_NORMAL : EDIT_ZOOM);
	WhenRescan();
}

void PathEditorCtrl::ToolViewZoomOut(Bar& bar)
{
	bar.Add("Zmenšit", PathImg::view_zoom_out(), THISBACK(OnViewZoomOut))
		.Help("Zmenšit mìøítko zobrazení stylu èáry");
}

void PathEditorCtrl::OnViewZoomOut()
{
	OnViewZoomHorzOut();
	OnViewZoomVertOut();
}

void PathEditorCtrl::ToolViewZoomFull(Bar& bar)
{
	bar.Add("Podle okna", PathImg::view_zoom_full(), THISBACK(OnViewZoomFull))
		.Help("Nastavit mìøítko zobrazení podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomFull()
{
	OnViewZoomHorzFull();
	OnViewZoomVertFull();
}

void PathEditorCtrl::ToolViewZoomHorzIn(Bar& bar)
{
	bar.Add("Horiz. zvìtšit", THISBACK(OnViewZoomHorzIn))
		.Help("Zvìtšit mìøítko vodorovné osy");
}

void PathEditorCtrl::OnViewZoomHorzIn()
{
	int half = GetSize().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(min(hruler.GetScale() * 1.5, 1000.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzOut(Bar& bar)
{
	bar.Add("Horiz. zmenšit", THISBACK(OnViewZoomHorzOut))
		.Help("Zmenšit mìøítko vodorovné osy");
}

void PathEditorCtrl::OnViewZoomHorzOut()
{
	int half = GetSize().cx >> 1;
	double mpos = hruler.FromClient(half);
	hruler.SetZoom(max(hruler.GetScale() / 1.5, 1.0), 0);
	hruler.SetZoom(hruler.GetScale(), half - hruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomHorzFull(Bar& bar)
{
	bar.Add("Horiz. podle okna", THISBACK(OnViewZoomHorzFull))
		.Help("Nastavit vodorovné mìøítko podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomHorzFull()
{
	int avail = GetSize().cx - 2 * GAP;
	double wd = max(1e-3, style->begin + style->segment + style->end);
	hruler.SetZoom(avail / wd, GAP);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertIn(Bar& bar)
{
	bar.Add("Vert. zvìtšit", THISBACK(OnViewZoomVertIn))
		.Help("Zvìtšit mìøítko svislé osy");
}

void PathEditorCtrl::OnViewZoomVertIn()
{
	int half = GetSize().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(min(vruler.GetScale() * 1.5, 1000.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertOut(Bar& bar)
{
	bar.Add("Vert. zmenšit", THISBACK(OnViewZoomVertOut))
		.Help("Zmenšit mìøítko svislé osy");
}

void PathEditorCtrl::OnViewZoomVertOut()
{
	int half = GetSize().cy >> 1;
	double mpos = vruler.FromClient(half);
	vruler.SetZoom(max(vruler.GetScale() / 1.5, 1.0), 0);
	vruler.SetZoom(vruler.GetScale(), half - vruler.ToClient(mpos));
	UpdateScroll();
}

void PathEditorCtrl::ToolViewZoomVertFull(Bar& bar)
{
	bar.Add("Vert. podle okna", THISBACK(OnViewZoomVertFull))
		.Help("Nastavit mìøítko svislé osy podle velikosti okna");
}

void PathEditorCtrl::OnViewZoomVertFull()
{
	int avail = GetSize().cy - 2 * GAP;
	double ht = max(1e-3, style->width);
	vruler.SetZoom(avail / ht, GetSize().cy >> 1);
	UpdateScroll();
}

void PathEditorCtrl::ToolViewPan(Bar& bar)
{
	bar.Add("Posunout", PathImg::view_pan(), THISBACK(OnViewPan))
		.Check(edit_mode == EDIT_PAN)
		.Help("Posunout myší zobrazený výøez");
}

void PathEditorCtrl::OnViewPan()
{
	edit_mode = (edit_mode == EDIT_PAN ? EDIT_NORMAL : EDIT_PAN);
	WhenRescan();
}

void PathEditorCtrl::ToolSetup(Bar& bar)
{
	ToolSetupGrid(bar);
	ToolSetupSnap(bar);
	ToolSetupRuler(bar);
	ToolSetupAxis(bar);
//	bar.ThinBar();
	bar.Separator();
	ToolSetupStyle(bar);
	ToolSetupSetup(bar);
	if(bar.IsToolBar())
	{
		bar.Separator();
//		bar.ThinBar();
		bar.Add(sample_size_tag);
		bar.Add(sample_size.SizePos(), 80);
		bar.Add(sample_width_tag);
		bar.Add(sample_width.SizePos(), 40);
	}
}

void PathEditorCtrl::ToolSetupGrid(Bar& bar)
{
	bar.Add("Møížka", PathImg::setup_grid(), THISBACK(OnSetupGrid))
		.Check(setup.do_grid)
		.Help("Zobrazit / skrýt pomocnou møížku");
}

void PathEditorCtrl::OnSetupGrid()
{
	setup.do_grid = !setup.do_grid;
	WhenRescan();
	Refresh();
}

void PathEditorCtrl::ToolSetupRuler(Bar& bar)
{
	bar.Add("Pravítko", PathImg::setup_ruler(), THISBACK(OnSetupRuler))
		.Check(setup.do_ruler)
		.Help("Zobrazit / skrýt pravítko");
}

void PathEditorCtrl::OnSetupRuler()
{
	setup.do_ruler = !setup.do_ruler;
	WhenRescan();
	Layout();
}

void PathEditorCtrl::ToolSetupAxis(Bar& bar)
{
	bar.Add("Osa", PathImg::setup_axis(), THISBACK(OnSetupAxis))
		.Check(setup.do_axis)
		.Help("Zobrazit / skrýt osu ukázkové úseèky");
}

void PathEditorCtrl::OnSetupAxis()
{
	setup.do_axis = !setup.do_axis;
	WhenRescan();
	UpdateSample();
}

void PathEditorCtrl::ToolSetupSnap(Bar& bar)
{
	bar.Add("Pøichytit do møížky", PathImg::setup_snap(), THISBACK(OnSetupSnap))
		.Check(setup.do_snap)
		.Help("Pøichytit souøadnice myši do møížky");
}

void PathEditorCtrl::OnSetupSnap()
{
	setup.do_snap = !setup.do_snap;
	WhenRescan();
	Refresh();
}

void PathEditorCtrl::ToolSetupStyle(Bar& bar)
{
	bar.Add("Styl èáry", THISBACK(OnSetupStyle))
		.Help("Zobrazit/zmìnit vlastnosti stylu èáry");
}

void PathEditorCtrl::OnSetupStyle()
{
	if(RunDlgPathStyleSetup(*style))
	{
		WhenRescan();
		Layout();
		UpdateSample();
	}
}

void PathEditorCtrl::ToolSetupSetup(Bar& bar)
{
	bar.Add("Editor", PathImg::setup_setup(), THISBACK(OnSetupSetup))
		.Help("Zobrazit/zmìnit vlastnosti editoru stylù èar");
}

void PathEditorCtrl::OnSetupSetup()
{
	if(RunDlgPathEditorSetup(setup))
	{
		WhenRescan();
		Layout();
	}
}

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

class DlgPathEditor
{
public:
	typedef DlgPathEditor CLASSNAME;
	DlgPathEditor();

	bool              Run(PathStyle& style, const String& title);
	void              Serialize(Stream& stream);
	static ConfigItem& config();

	void              Rescan() { tool_bar.Set(THISBACK(ToolRoot)); menu_bar.Set(THISBACK(ToolRoot)); }

public:
	TOOL(Root)
	TOOL(Edit)
		TOOL(EditSave)

private:
	TopWindow         dialog;
	MenuBar           menu_bar;
	ToolBar           tool_bar;
	StatusBar         status_bar;
	EditField         coords;

	PathEditorCtrl    path;
};

CONFIG_ITEM(DlgPathEditor::config, "DlgPathEditor", 1, 1, 1)

bool RunDlgPathEditor(PathStyle& style, const String& title) { return DlgPathEditor().Run(style, title); }

//////////////////////////////////////////////////////////////////////
// DlgPathEditor::

DlgPathEditor::DlgPathEditor()
{
#ifdef DEBUG_DRAW
	dialog.NoBackPaint();
#endif
	dialog.AddFrame(menu_bar);
	dialog.AddFrame(tool_bar);
	dialog.AddFrame(status_bar);
	status_bar << coords.VSizePos(0, 0).RightPos(0, 200);
	coords.SetReadOnly();
	menu_bar.Set(THISBACK(ToolRoot));
	tool_bar.Set(THISBACK(ToolRoot));
	dialog << path.SizePos();
	dialog.Sizeable().Zoomable();
	dialog.SetMinSize(Size(300, 200));
	path.WhenRescan = THISBACK(Rescan);
}

bool DlgPathEditor::Run(PathStyle& style, const String& title)
{
	dialog.Title(Nvl(title, "Styl èáry").ToWString());
	ReadConfigSelf();
	path.SetOwner(style);
	path.SetCoords(coords);
	path.PumpTraces(false);
	dialog.Open();
	path.OnViewZoomFull();
	bool ok = (dialog.Run() == IDOK);
	WriteConfigSelf();
	if(ok)
		path.PumpTraces(true);
	Ctrl::IgnoreMouseUp();
	return ok;
}

void DlgPathEditor::Serialize(Stream& stream)
{
	stream % path.setup;
	if(stream.IsLoading())
		path.UpdateSetup();
}

void DlgPathEditor::ToolRoot(Bar& bar)
{
	bar.Add("Edit", THISBACK(ToolEdit))
		.Help("Základní editaèní pøíkazy");
	bar.Add("Pohled", callback(&path, &PathEditorCtrl::ToolView))
		.Help("Mìøítko a poloha zobrazeného výøezu");
	bar.Add("Vlastnosti", callback(&path, &PathEditorCtrl::ToolSetup))
		.Help("Vlastnosti èáry a editoru");
}

void DlgPathEditor::ToolEdit(Bar& bar)
{
	if(bar.IsToolBar())
	{
		ToolEditSave(bar);
//		bar.ThinBar();
		bar.Separator();
	}
	path.ToolEdit(bar);
	if(bar.IsMenuBar())
	{
		bar.Separator();
		ToolEditSave(bar);
	}
}

void DlgPathEditor::ToolEditSave(Bar& bar)
{
	bar.Add("Uložit a zavøít", CtrlImg::save(), THISBACK(OnEditSave))
		.Key(K_CTRL_S)
		.Help("Zavøít editor a použít nadefinovaný styl èáry");
}

void DlgPathEditor::OnEditSave()
{
	dialog.AcceptBreak(IDOK);
}

template <class T>
class WithPopupEdit : public T
{
public:
	WithPopupEdit();

	virtual void Deactivate();
	virtual bool Key(dword key, int);
	bool         PopUp(Ctrl *parent, const Rect& prect);

public:
	Callback     WhenCancel;
	Callback     WhenSelect;

protected:
	virtual void DoClose(bool ok);

protected:
	bool         open;
};

template <class T>
WithPopupEdit<T>::WithPopupEdit()
{
	this->SetFrame(BlackFrame());
	open = false;
}

template <class T>
void WithPopupEdit<T>::Deactivate()
{
	if(open) {
		if(!this->Accept())
		{
			this->SetFocus();
			return;
		}
		DoClose(true);
		this->IgnoreMouseClick();
		WhenSelect();
	}
}

template <class T>
void WithPopupEdit<T>::DoClose(bool ok)
{
	open = false;
//	EndModalLoop(ok ? 1 : 0);
	this->EndLoop(ok ? 1 : 0);
}

template <class T>
bool WithPopupEdit<T>::Key(dword key, int repcnt)
{
	if(key == K_ENTER)
	{
		if(this->Accept())
		{
			DoClose(true);
			WhenSelect();
		}
		return true;
	}
	else if(key == K_ESCAPE)
	{
		this->Reject();
		DoClose(false);
		WhenCancel();
		return true;
	}
	return T::Key(key, repcnt);
}

template <class T>
bool WithPopupEdit<T>::PopUp(Ctrl *parent, const Rect& prect)
{
	ASSERT(parent);
	if(!this->IsOpen())
	{
		Ctrl *wnd = parent->GetTopCtrl();
		if(!wnd || !wnd->IsOpen() || !wnd->IsVisible())
			return false;
		open = false;
		this->SetRect(prect);
		T::PopUp(parent);
	}
	this->Enable();
	this->SetFocus();
	open = true;
	EventLoop(this);
	bool ok = !!GetExitCode();
//	bool ok = !!RunModalLoop();
	this->Disable();
	return ok;
}

PathStyleMapCtrl::PathStyleMapCtrl()
{
	cursor = -1;
	map = 0;
	scroll_pos = 0;
	SetFrame(InsetFrame());
	scroll.NoAutoHide().NoAutoDisable();
	scroll.WhenScroll = THISBACK(OnScroll);
	AddFrame(scroll);
	WhenBar = THISBACK(ToolLocal);
}

void PathStyleMapCtrl::Set(PathStyleMap *m)
{
	map = m;
	Layout();
}

bool PathStyleMapCtrl::Key(dword key, int repcnt)
{
	int c = cursor;
	int pg = max(1, GetSize().cy / pos_add.cx - 1);
	switch(key)
	{
	case K_LEFT:     c--; break;
	case K_RIGHT:    c++; break;
	case K_UP:       c -= count.cx; break;
	case K_DOWN:     c += count.cx; break;
	case K_PAGEUP:   c -= count.cx * pg; break;
	case K_PAGEDOWN: c += count.cx * pg; break;
	case K_HOME:     c = 0; break;
	case K_END:      c = map->GetCount() - 1; break;
	default:
		return Bar::Scan(WhenBar, key) || Ctrl::Key(key, repcnt);
	}
	SetCursor(minmax(c, 0, map->GetCount() - 1));
	return true;
}

void PathStyleMapCtrl::Layout()
{
	if(!map)
	{
		count = total = Size(0, 0);
		pos_add = Size(1, 1);
		cell = Size(0, 0);
		gap_offset = offset = Point(0, 0);
		scroll_pos = 0;
		return;
	}
	Size size = scroll.GetReducedViewSize();
	count.cx = max(1, (size.cx - GAP) / WIDTH);
	count.cy = idivceil(map->map.GetCount(), count.cx);
	pos_add.cx = (size.cx - GAP) / count.cx;
	pos_add.cy = HEIGHT + GAP;
	cell = pos_add - GAP;
	total = count * pos_add - GAP;
	offset.x = (size.cx - total.cx) >> 1;
	offset.y = GAP;
	gap_offset.x = offset.x - (GAP >> 1);
	gap_offset.y = GAP >> 1;
	scroll.Set(scroll_pos, size.cy, 2 * GAP + total.cy);
	scroll_pos = scroll;
	Refresh();
}

Rect PathStyleMapCtrl::GetEditRect(int right, int bottom)
{
	Size size = PathImg::rename().GetSize();
	return RectC(right - size.cx, bottom - size.cy, size.cx, size.cy);
}

bool PathStyleMapCtrl::InEditRect(int right, int bottom, Point mouse)
{
	Rect rc = GetEditRect(right, bottom).Inflated(2);
	if(!rc.Contains(mouse))
		return false;
	Point pt = rc.CenterPoint();
	return mouse.x + mouse.y >= pt.x + pt.y;
}

void PathStyleMapCtrl::Paint(Draw& draw)
{
	Rect rc = draw.GetPaintRect();
	draw.Begin();
	Size todo = ClientToRange(rc);
	for(int i = todo.cx; i < todo.cy; i++)
	{
		Rect item = IndexToClient(i);
		if(item && rc)
		{
			DrawFrame(draw, item, Black, White);
			draw.DrawRect(item.Deflated(1), i == cursor ? LtCyan : SLtGray);
			Rect box = item.Deflated(IGAP);
			draw.Clipoff(Rect(box.left, box.top, box.right, box.top + TEXT_HEIGHT));
			String qtf;
			qtf << "[=A+108";
			if(i == cursor)
				qtf << '*';
			qtf << " \1" << map->GetSortName(i);
			Document doc(qtf);
			doc.Paint(DOC_SCREEN_ZOOM, draw, 0, 0, box.Width(), SLtGray);
			draw.DrawImage(GetEditRect(box.Width(), TEXT_HEIGHT), PathImg::rename());
			draw.End();
			draw.Clipoff(Rect(box.left, box.top + TEXT_HEIGHT, box.right, box.bottom));
			PathDraw path;
			path.Set(draw, map->GetSortStyle(i), Black, LINE_HEIGHT / 2);
			Rect er = GetEditRect(box.Width(), box.Height() - TEXT_HEIGHT);
			path.Line(0, LINE_HEIGHT / 2, er.left, LINE_HEIGHT / 2);
			path.Paint();
			draw.DrawImage(er, PathImg::edit());
			draw.End();
			draw.ExcludeClip(item);
		}
	}
	draw.DrawRect(rc, SLtGray);
	draw.End();
}

int PathStyleMapCtrl::ClientToIndex(Point pt) const
{
	pt = idivfloor(pt - gap_offset + Size(0, scroll_pos), pos_add);
	if(pt.x < 0 || pt.x >= count.cx || pt.y < 0)
		return -1;
	return pt.x + pt.y * count.cx;
}

Size PathStyleMapCtrl::ClientToRange(const Rect& rc) const
{
	if(rc.IsEmpty())
		return Size(-1, -1);
	int top = idivfloor(rc.top - gap_offset.y + scroll_pos, pos_add.cy);
	int bottom = idivfloor(rc.bottom - 1 - gap_offset.y + scroll_pos, pos_add.cy);
	int left = idivfloor(rc.left - gap_offset.x, pos_add.cx);
	int right = idivfloor(rc.right - 1 - gap_offset.x, pos_add.cx);
	if(bottom < 0 || right < 0 || left >= count.cx)
		return Size(-1, -1);
	if(top < bottom) // multiple lines
		return Size(max(0, count.cx * top), min(map->GetCount(), count.cx * (bottom + 1)));
	return Size(max(0, count.cx * top + max(0, left)), min(map->GetCount(), count.cx * top + min(count.cx, right + 1)));
}

Rect PathStyleMapCtrl::IndexToClient(int i) const
{
	return Rect(offset - Size(0, scroll_pos) + pos_add * Size(i % count.cx, i / count.cx), cell);
}

void PathStyleMapCtrl::OnScroll()
{
	scroll_pos = scroll;
	Refresh();
}

void PathStyleMapCtrl::LeftDown(Point pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(IsValid(i))
		SetCursor(i);
	SetWantFocus();
	Rect rc = IndexToClient(i).Deflated(IGAP);
	if(InEditRect(rc.right, rc.top + TEXT_HEIGHT, pt))
		OnRename();
	if(InEditRect(rc.right, rc.bottom, pt))
		OnEdit();
}

void PathStyleMapCtrl::LeftDouble(Point pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(IsCursor() && i == cursor)
		WhenLeftDouble();
}

void PathStyleMapCtrl::LeftUp(Point pt, dword keyflags)
{
}

void PathStyleMapCtrl::RightDown(Point pt, dword keyflags)
{
	int i = ClientToIndex(pt);
	if(IsValid(i))
		SetCursor(i);
	SetWantFocus();
	MenuBar::Execute(this, WhenBar, UPP::GetMousePos());
}

void PathStyleMapCtrl::SetCursor(int c)
{
	RefreshItem(cursor);
	RefreshItem(cursor = c);
	if(IsCursor() && IsOpen() && IsVisible())
	{
		Rect rc = IndexToClient(c) + Size(0, scroll);
		rc.Inflate(GAP);
		scroll.ScrollInto(rc.top, rc.Height());
	}
}

void PathStyleMapCtrl::RefreshItem(int i)
{
	if(i >= 0 && i < map->map.GetCount())
		Refresh(IndexToClient(i));
}

void PathStyleMapCtrl::ToolLocal(Bar& bar)
{
	ToolNew(bar);
	ToolEdit(bar);
	ToolRename(bar);
	ToolCopy(bar);
	bar.Separator();
	ToolExport(bar);
	ToolImport(bar);
	bar.Separator();
	ToolRemove(bar);
}

void PathStyleMapCtrl::ToolNew(Bar& bar)
{
	bar.Add("Nový", THISBACK(OnNew));
}

void PathStyleMapCtrl::OnNew()
{
	PathStyle new_style;
	new_style <<= PathStyle::solid();
	String px = "Nový styl";
	if(RunDlgPathEditor(new_style, px))
	{
		String n = map->GetUniqueName(px);
		map->Set(n, new_style);
		map->Touch();
		Layout();
		SetCursor(n);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolEdit(Bar& bar)
{
	bar.Add(IsCursor(), "Upravit", THISBACK(OnEdit));
}

void PathStyleMapCtrl::OnEdit()
{
	if(IsCursor())
	{
		int c = GetCursor();
		String n = map->GetSortName(c);
		PathStyle edited_style;
		edited_style <<= map->GetSortStyle(c);
		if(RunDlgPathEditor(edited_style, "Upravit styl '" + n + "'"))
		{
			map->Set(n, edited_style);
			map->Touch();
			RefreshItem(c);
		}
	}
}

void PathStyleMapCtrl::ToolRename(Bar& bar)
{
	bar.Add(IsCursor(), "Pøejmenovat", THISBACK(OnRename));
}

void PathStyleMapCtrl::OnRename()
{
	if(!IsCursor())
	{
		BeepExclamation();
		return;
	}
	int i = GetCursor();
	Rect rc = IndexToClient(i) + Size(0, scroll);
	scroll.ScrollInto(rc.top, rc.Height());
	rc = IndexToClient(i) + GetScreenView().TopLeft();
	rc.bottom = rc.top + 2 * IGAP + TEXT_HEIGHT;
	rc.Deflate(IGAP);
	WithPopupEdit<DocEdit> pedit;
	pedit <<= map->GetSortName(i);
	while(pedit.PopUp(this, rc))
	{
		String n = ~pedit;
		if(map->FindSortName(n, i) >= 0)
		{ // duplicate name
			Exclamation("Styl èáry [* \1" + n + "\1] již v systému existuje. Zadejte prosím jiný název.");
			continue;
		}
		int x = map->GetSort(i);
		map->Rename(x, n);
		map->Touch();
		Refresh();
		SetCursor(n);
		break;
	}
}

void PathStyleMapCtrl::ToolCopy(Bar& bar)
{
	bar.Add("Duplikovat", THISBACK(OnCopy));
}

void PathStyleMapCtrl::OnCopy()
{
	if(IsCursor())
	{
		int c = GetCursor();
		String prefix = map->GetSortName(c);
		int l = prefix.GetLength();
		if(l > 0 && IsDigit(prefix[l - 1]))
		{
			while(l > 0 && IsDigit(prefix[l - 1]))
				l--;
			if(l > 0 && prefix[l - 1] == ' ')
				prefix.Trim(l - 1);
		}
		PathStyle new_style;
		new_style <<= map->GetSortStyle(c);
		String q = map->GetUniqueName(prefix);
		map->Set(q, new_style);
		map->Touch();
		Layout();
		SetCursor(q);
		OnRename();
	}
}

void PathStyleMapCtrl::ToolExport(Bar& bar)
{
	bar.Add("Export", THISBACK(OnExport))
		.Help("Exportovat styly èar do souboru");
}

String recent;

void PathStyleMapCtrl::OnExport()
{
	if(map->IsEmpty())
	{
		PromptOK("V systému nebyly nalezeny žádné styly èar.");
		return;
	}
	FileSelector fsel;
	fsel.Type("Styly èar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteSaveAs("Exportovat styly èar..."))
		return;
	String exp = map->Export();
	if(!SaveFile(~fsel, exp))
		PromptOK("Chyba pøi zápisu do souboru [* \1" + ~fsel + "\1].");
}

void PathStyleMapCtrl::ToolImport(Bar& bar)
{
	bar.Add("Import", THISBACK(OnImport))
		.Help("Importovat styly èar ze souboru...");
}

void PathStyleMapCtrl::OnImport()
{
	FileSelector fsel;
	fsel.Type("Styly èar (*.wml)", "*.wml")
		.DefaultExt("wml");
	fsel <<= recent;
	if(!fsel.ExecuteOpen("Importovat styly èar"))
		return;
	String imp = LoadFile(recent = ~fsel);
	if(imp.IsVoid())
	{
		PromptOK("Nelze naèíst soubor [* \1" + ~fsel + "\1].");
		return;
	}
	int old_count = map->GetCount();
	String old_name = map->GetName();
	try
	{
		map->Import(imp);
	}
	catch(Exc e)
	{
		PromptOK("Chyba pøi importu souboru [* \1" + ~fsel + "\1]: \1" + e + "\1.");
	}
	map->Touch();
	if((old_count > 0 && !IsNull(old_name)) || IsNull(map->GetName()))
		map->SetName(old_name);
	if(map->GetCount() > old_count)
		SetCursor(map->map.GetKey(old_count));
	WhenRename();
}

void PathStyleMapCtrl::ToolRemove(Bar& bar)
{
	bar.Add("Smazat", THISBACK(OnRemove))
		.Help("Odstranit styl èáry ze systému");
}

void PathStyleMapCtrl::OnRemove()
{
	if(IsCursor())
	{
		int c = cursor;
		map->Remove(map->GetSortName(c));
		map->Touch();
		c = min(c, map->GetCount() - 1);
		Layout();
		SetCursor(c);
	}
}

class PathStyleMapLayoutEx : public WithPathStyleMapLayout<TopWindow>
{
public:
	PathStyleMapCtrl browser;
};

//////////////////////////////////////////////////////////////////////
// DlgPathStyleMap::

class DlgPathStyleMap
{
public:
	typedef DlgPathStyleMap CLASSNAME;
	DlgPathStyleMap();

	bool                 Run(PathStyleMap& map, String& style, bool editor = false, bool read_only = false);

private:
//	void                 UpdateMaps();
//	void                 OnTab();

private:
	PathStyleMapLayoutEx dialog;
	PathStyleMap        *map;
//	const Vector<PathStyleMap *> *maps;
//	Vector<int> map_index;
};

bool RunDlgPathStyleMap(PathStyleMap& map, String& style, bool editor, bool read_only)
{ return DlgPathStyleMap().Run(map, style, editor, read_only); }

RegisterHelpTopicObjectTitle(DlgPathStyleMap, "Mapa stylù")

DlgPathStyleMap::DlgPathStyleMap()
{
	map = 0;
//	HelpLayoutOKCancel(dialog, this);
	CtrlLayoutOKCancel(dialog, DlgPathStyleMapHelpTitle());
	dialog.HelpTopic("DlgPathStyleMap");
	dialog.Sizeable().MaximizeBox();
//	dialog.browser.WhenRename = THISBACK(UpdateMaps);
	dialog.tab.Hide();
//	dialog.tab <<= THISBACK(OnTab);
}

bool DlgPathStyleMap::Run(PathStyleMap& m, String& style, bool editor, bool read_only)
{
	if(editor) {
		dialog.ok.Hide();
		dialog.cancel.SetLabel(t_("Close"));
	}

	map = &m;
//	ASSERT(!maps->IsEmpty());
//	UpdateMaps();

	dialog.browser.Set(map);
	dialog.browser.SetCursor(Nvl(style, ".solid"));
	while(dialog.Run() == IDOK) {
		int c = dialog.browser.GetCursor();
		if(c >= 0) {
			style = map->GetSortName(c);
			if(style == ".solid") style = Null;
			return true;
		}
		BeepExclamation();
	}
	return false;
}


/*
void DlgPathStyleMap::UpdateMaps()
{
	int tab = dialog.tab.Get();
	tab = (tab >= 0 && tab < map_index.GetCount() ? map_index[tab] : -1);
	map_index = GetSortOrder(*maps, XRelation<const PathStyleMap *>(StdCsNumSort, XDeref(XField(&PathStyleMap::name))));
	dialog.tab.Clear();
	int new_tab = -1;
	for(int i = 0; i < map_index.GetCount(); i++)
	{
		int x = map_index[i];
		dialog.tab.Add(Nvl((*maps)[x]->GetName(), "(bez názvu)"));
		if(x == tab)
			new_tab = i;
	}
	dialog.tab.Set(new_tab);
}
*/

/*
void DlgPathStyleMap::OnTab()
{
	int x = dialog.tab.Get();
	if(x >= 0 && x < map_index.GetCount())
		dialog.browser.Set((*maps)[map_index[x]]);
}
*/

PathStyleCtrl::PathStyleCtrl()
: path_map(0)
{
	SetDisplay(display);
}

void PathStyleCtrl::DoAction()
{
	String s = GetData();
	if(RunDlgPathStyleMap(*path_map, s, false, false))
		SetDataAction(s);
}

}
