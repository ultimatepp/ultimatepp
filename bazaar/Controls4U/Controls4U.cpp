#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "Controls4U/Controls4U.h"

#define IMAGECLASS Controls4UImg
#define IMAGEFILE <Controls4U/Controls4U.iml>
#include <Draw/iml.h>

void EditFileFolder::Init() {
	EditString::AddFrame(ButBrowse);
	ButBrowse.SetImage(CtrlImg::right_arrow());
	ButBrowse <<= THISBACK(DoBrowse);
	EditString::AddFrame(ButGo);
	ButGo.SetImage(CtrlImg::SmallRight());
	ButGo <<= THISBACK(DoGo);
	isFile = isLoad = true;
	fs.Asking(!isLoad);
}

void EditFileFolder::DoBrowse() {
	if (isFile && isLoad) {
		if (fs.ExecuteOpen(title)) {
			SetData(~fs);
			WhenChange();
		}
	} else if (isFile && !isLoad)  {
		if (fs.ExecuteSaveAs(title)) {
			SetData(~fs);
			WhenChange();
		}
	} else if (!isFile) {
		if (fs.ExecuteSelectDir(title)) {
			SetData(~fs);
			WhenChange();
		}
	}
}

void EditFileFolder::DoGo()
{
	Set(GetData());			// Write Edit to FileSel
	WhenChange();
}

bool EditFileFolder::Key(dword key, int rep) {
	if (key == K_ENTER) {	// Catch the ENTER
		DoGo();
		return false;
	} else
		return EditField::Key(key, rep);
}

void StaticImage::Paint(Draw& w) {
	Size sz = GetSize();

	w.DrawRect(sz, background);
	if (!origImage) 
		return;
	if (sz.cx == 0 || sz.cy == 0) 
		return;
	Size imagesize = origImage.GetSize();	
	if (imagesize.cx == 0 || imagesize.cy == 0) 
		return;
	
	Image *imageView;
	if (angle == Angle_0)
		imageView = &origImage;
	else {
		imageView = &image;
		imagesize = imageView->GetSize();
	}
	switch (fit) {
	case BestFit:
		w.DrawImage(FitInFrame(sz, imagesize), *imageView);
		break;
	case FillFrame:
		w.DrawImage(0, 0, sz.cx, sz.cy, *imageView);
		break;
	case NoScale:
		w.DrawImage(0, 0, *imageView);
		break;		
	case Background:
		{
			Ctrl *p = GetParent();
			if (p->GetFirstChild() != this) {
				p->RemoveChild(this);
				p->AddChildBefore(this, p->GetFirstChild());
			}
		}
		SizePos();
	case RepeatToFill:
		for (int left = 0; left < sz.cx; left += imagesize.cx) 
			for (int top = 0; top < sz.cy; top += imagesize.cy) 
				w.DrawImage(left, top, *imageView);
		break;	
	}
}

bool StaticImage::Set(String _fileName) {
	fileName = _fileName;
    origImage.Clear();	
	origImage = StreamRaster::LoadFileAny(_fileName);
	if (angle != Angle_0)
		SetAngle(angle);
	Refresh();
	return !IsNull(origImage);
}

bool StaticImage::Set(Image _image) {
	origImage.Clear();
	origImage = _image;
	if (angle != Angle_0)
		SetAngle(angle);
	fileName = "";
	Refresh();
	return !IsNull(origImage);
}

StaticImage& StaticImage::SetAngle(int _angle) {
	angle = _angle; 
	if (origImage) {
		switch (angle) {
		case Angle_90:	image = RotateClockwise(origImage);
						break;
		case Angle_180:	image = Rotate180(origImage);
						break;
		case Angle_270:	image = RotateAntiClockwise(origImage);
						break;
		}
		Refresh(); 
	}
	return *this;
}	
	
StaticImage::StaticImage() {
	Transparent();
	NoWantFocus();

	origImage = Null;
	background = Null;
	angle = Angle_0;
	fit = BestFit;
}

void StaticRectangle::Paint(Draw& w) {
	Size sz = GetSize();
	ImageBuffer ib(sz);
	MyBufferPainter sw(ib);	

	sw.Clear(RGBAZero());
	double wid;
	if (background)
		wid = width*2;	
	else
		wid = width;
	sw.Rectangle(wid/2, wid/2, sz.cx-wid, sz.cy-wid).Stroke(wid, color).Fill(background);
	
	w.DrawImage(0, 0, ib);
}

StaticRectangle::StaticRectangle() {
	Transparent();
	NoWantFocus();

	color = Black();
	background = Null;
	width = 1;
}

void StaticEllipse::Paint(Draw& w) {
	Size sz = GetSize();
	ImageBuffer ib(sz);
	MyBufferPainter sw(ib);	

	sw.Clear(RGBAZero());
	double wid;
	if (background)
		wid = width*2;	
	else
		wid = width;	
	sw.Ellipse(sz.cx/2, sz.cy/2, sz.cx/2-wid/2, sz.cy/2-wid/2).Stroke(wid, color).Fill(background);
	
	w.DrawImage(0, 0, ib);
}

StaticEllipse::StaticEllipse() {
	Transparent();
	NoWantFocus();

	color = Black();
	background = Null;
	width = 1;
}

StaticFrame::StaticFrame() {
	Transparent();
	NoWantFocus();
	
	SetFrame(InsetFrame());
}

void StaticLine::FramePaint(Draw& w, const Rect& rr) {
	int off = (int)(0.25*max(rr.GetWidth(), rr.GetHeight()));
	
	ImageBuffer ib(rr.GetWidth()+2*off, rr.GetHeight()+2*off);
	MyBufferPainter sw(ib);	
	Rect r = rr;
	r.Offset(off, off);

	sw.Clear(RGBAZero());
	if (orientation == OrVert)
		sw.DrawLine((r.right+r.left)/2, r.top, (r.right+r.left)/2, r.bottom, width, color);
	else if (orientation == OrHor)
		sw.DrawLine(r.left, (r.bottom+r.top)/2, r.right, (r.bottom+r.top)/2, width, color);
	else if (orientation == OrSW_NE)
		sw.DrawLine(r.left, r.bottom, r.right, r.top, width, color);
	else 
		sw.DrawLine(r.left, r.top, r.right, r.bottom, width, color);	
	
	w.DrawImage(-off, -off, ib);
}

StaticLine& StaticLine::SetOrientation(String o) {
	if (o == "|")
		return SetOrientation(OrVert);
	else if (o == "-")
		return SetOrientation(OrHor);
	else if (o == "\\")
		return SetOrientation(OrNW_SE);
	else
		return SetOrientation(OrSW_NE);
}

StaticLine::StaticLine() {
	SetFrame(*this);
	Transparent();
	NoWantFocus();

	color = Black();
	width = 1;
	orientation = OrVert;
}

void PaintArrowEnd(Painter &sw, double x0, double y0, double x1, double y1, int width, Color &color, bool direction) {
	double alen, awidth;
	double wd = width;
	
	if (width == 1) {
		alen = 16;
		awidth = 2;
	} else {				
		alen = 9*wd;
		awidth = 1.5*wd;
	}
	if (!direction)
		alen = -alen;
	if (x0 == x1) {
		sw.Move(x0, y0).Line(x0 + awidth, y0 + alen).Line(x0 - awidth, y0 + alen).Line(x0, y0);
		sw.Fill(color);
	} else if (y0 == y1) {
		sw.Move(x0, y0).Line(x0 + alen, y0 - awidth).Line(x0 + alen, y0 + awidth).Line(x0, y0);
		sw.Fill(color);
	} else {
		double t = atan((y1-y0)/(double)(x1-x0));
		double xa = alen*cos(t);
		double ya = alen*sin(t);
		double xb = awidth*sin(t);
		double yb = awidth*cos(t);
		sw.Move(x0, y0).Line(x0+xa+xb, y0+ya-yb).Line(x0+xa-xb, y0+ya+yb).Line(x0, y0);
		sw.Fill(color);
	}
}

void StaticArrow::FramePaint(Draw& w, const Rect& rr) {
	int off = (int)(0.25*max(rr.GetWidth(), rr.GetHeight()));
	
	ImageBuffer ib(rr.GetWidth()+2*off, rr.GetHeight()+2*off);
	MyBufferPainter sw(ib);	
	Rect r = rr;
	r.Offset(off, off);
		
	Size sz = rr.GetSize();
	
	sw.Clear(RGBAZero());
	sw.LineCap(LINECAP_BUTT);
	
	double x0, y0, x1, y1, middle;

	if (orientation == OrVert) {
		x0 = x1 = (r.right+r.left)/2;
		y0 = r.top;
		y1 = r.bottom;
	} else if (orientation == OrHor) {
		x0 = r.left;
		y0 = y1 = (r.bottom+r.top)/2;
		x1 = r.right;
	} else if (orientation == OrSW_NE) {
		x0 = r.left;
		y0 = r.bottom;
		x1 = r.right;
		y1 = r.top;
	} else if (orientation == OrNW_SE_HVH) {
		x0 = r.left;
		y0 = r.top + 2*width;
		x1 = r.right;
		y1 = r.bottom - 2*width;
		middle = (r.left+r.right)/2;
	} else if (orientation == OrSW_NE_HVH) {
		x0 = r.left;
		y0 = r.bottom - 2*width;
		x1 = r.right;
		y1 = r.top + 2*width;
		middle = (r.left + r.right)/2;
	} else if (orientation == OrNW_SE_VHV) {
		x0 = r.left + 2*width;
		y0 = r.top;
		x1 = r.right - 2*width;
		y1 = r.bottom;
		middle = (r.top + r.bottom)/2;
	} else if (orientation == OrSW_NE_VHV) {
		x1 = r.left  + 2*width;
		y1 = r.bottom;
		x0 = r.right - 2*width;
		y0 = r.top;
		middle = (r.top + r.bottom)/2;
	} else {	// OrNW_SE
		x0 = r.left;
		y0 = r.top;
		x1 = r.right;
		y1 = r.bottom;
	}
	// Arrow len is 9*width, but 8 joins better arrow end with line
	if (ends == EndLeft || ends == EndLeftRight) {
		if (x0 == x1 || orientation == OrVert || orientation == OrNW_SE_VHV || orientation == OrSW_NE_VHV) {
			PaintArrowEnd(sw, x0, y0, x0, y1, width, color, true);
			y0 += 8*width;
		} else if (y0 == y1 || orientation == OrHor || orientation == OrNW_SE_HVH || orientation == OrSW_NE_HVH) {
			PaintArrowEnd(sw, x0, y0, x1, y0, width, color, true);
			x0 += 8*width;
		} else { 
			PaintArrowEnd(sw, x0, y0, x1, y1, width, color, true);
			double t = atan((y1-y0)/(double)(x1-x0));
			x0 += 8*width*cos(t);
			y0 += 8*width*sin(t);
		}
	}
	if (ends == EndRight || ends == EndLeftRight) {	// Same as other but swapping points
		if (x0 == x1 || orientation == OrVert || orientation == OrNW_SE_VHV || orientation == OrSW_NE_VHV) {
			PaintArrowEnd(sw, x1, y1, x1, y0, width, color, false);
			y1 -= 8*width;
		} else if (y0 == y1 || orientation == OrHor || orientation == OrNW_SE_HVH || orientation == OrSW_NE_HVH) {
			PaintArrowEnd(sw, x1, y1, x0, y1, width, color, false);
			x1 -= 8*width;
		} else { 
			PaintArrowEnd(sw, x1, y1, x0, y0, width, color, false);
			double t = atan((y1-y0)/(double)(x1-x0));
			x1 -= 8*width*cos(t);
			y1 -= 8*width*sin(t);			
		}
	}	
	if (orientation == OrVert) 
		sw.DrawLine(x0, y0, x1, y1, width, color);
	else if (orientation == OrHor) 
		sw.DrawLine(x0, y0, x1, y1, width, color);
	else if (orientation == OrSW_NE) 
		sw.DrawLine(x0, y0, x1, y1, width, color);
	else if (orientation == OrNW_SE_HVH) {
		middle = (r.left+r.right)/2;
		sw.DrawLine(x0, y0, middle, y0, width, color);
		sw.DrawLine(middle, y0, middle, y1, width, color);
		sw.DrawLine(middle, y1, x1,     y1, width, color);
	} else if (orientation == OrSW_NE_HVH) {
		middle = (r.left + r.right)/2;
		sw.DrawLine(x0,     y0, middle, y0, width, color);
		sw.DrawLine(middle, y0, middle, y1, width, color);
		sw.DrawLine(middle, y1, x1,     y1, width, color);
	} else if (orientation == OrNW_SE_VHV) {
		middle = (r.top + r.bottom)/2;
		sw.DrawLine(x0, y0,     x0, middle, width, color);
		sw.DrawLine(x0, middle, x1, middle, width, color);
		sw.DrawLine(x1, middle, x1, y1,     width, color);
	} else if (orientation == OrSW_NE_VHV) {
		middle = (r.top + r.bottom)/2;
		sw.DrawLine(x0, y0,     x0, middle, width, color);
		sw.DrawLine(x0, middle, x1, middle, width, color);
		sw.DrawLine(x1, middle, x1, y1,     width, color);
	} else {
		sw.DrawLine(x0, y0, x1, y1, width, color);				
	}
	w.DrawImage(-off, -off, ib);
}

StaticArrow& StaticArrow::SetOrientation(String o) {
	if (o == "|")
		return SetOrientation(OrVert);
	else if (o == "-")
		return SetOrientation(OrHor);
	else if (o == "\\")
		return SetOrientation(OrNW_SE);
	else if (o == "/")
		return SetOrientation(OrSW_NE);
	else if (o == "┐_") 
		return SetOrientation(OrNW_SE_HVH);
	else if (o == "_┌") 
		return SetOrientation(OrSW_NE_HVH);
	else if (o == "└┐") 
		return SetOrientation(OrNW_SE_VHV);
	else //if (o == "┌┘") {
		return SetOrientation(OrSW_NE_VHV);
}

StaticArrow& StaticArrow::SetEnds(String e) {
	if (e == "<-")
		return SetEnds(EndLeft);
	else if (e == "->")
		return SetEnds(EndRight);
	else if (e == "<->")
		return SetEnds(EndLeftRight);
	else
		return SetEnds(NoEnd);	
}
	
StaticArrow::StaticArrow() {
	SetFrame(*this);	
	Transparent();
	NoWantFocus();

	color = Black();
	width = 1;
	orientation = OrVert;
	ends = EndLeft;
}

void StaticClock::PaintPtr(MyBufferPainter &w, double cmx, double cmy, double pos, double m, double d, Color color, double cf) {
	double dx = m * sin(pos * 2 * M_PI);
	double dy = m * cos(pos * 2 * M_PI);

	double sx = cmx - dx * 35 / 2.0;
	double sy = cmy + dy * 35 / 2.0;
	double ex = cmx + dx * cf;
	double ey = cmy - dy * cf;

	w.DrawLine(sx, sy, ex, ey, d, color);
}

void StaticClock::Paint(Draw& ww) {
	Size sz = GetSize();
	ImageBuffer ib(sz);
	MyBufferPainter w(ib);	

	w.Clear(RGBAZero());
	w.LineCap(LINECAP_BUTT);
		
	Rect r = GetRect();
	Rect ro = GetRect();
	
	Color letterColor, background;;
	if (colorType == WhiteType) {
		background = White();
		letterColor = Black();
	} else {
		background = Black();
		letterColor = White();
	}	
	double hs = 20;
	int width = min(r.right - r.left, r.bottom - r.top);
	double bigF = width/200.;
	if (image)
		w.DrawImage(0, 0, width, width, image);
	else
		w.Circle(width/2, width/2, width/2).Fill(background);
	
	double cmx = width / 2;
	double cmy = width / 2;
	double cf = min(cmy, cmx) - 2;
	
	if (hourType != StaticClock::No) {
		for(int i = 1; i <= 60; i++) {
			double x = cmx + (0.95 * sin(i * M_PI / 30.0) * cf);
			double y = cmy - (0.95 * cos(i * M_PI / 30.0) * cf);
			if (hourType == StaticClock::Square) {
				if(i % 5 == 0)
					w.DrawRect(x, y, 3*bigF, 3*bigF, letterColor);
				else
					w.DrawRect(x, y, 2*bigF, 2*bigF, letterColor);
			} else if (hourType == StaticClock::Rectangle) {
				if(i % 5 == 0) {
					double x2 = cmx + (0.7 * sin(i * M_PI / 30.0) * cf);
					double y2 = cmy - (0.7 * cos(i * M_PI / 30.0) * cf);
					w.DrawLine(x, y, x2, y2, 4*bigF, Gray());
					w.DrawLine(x, y, x2, y2, 2*bigF, letterColor);
				} else {
					double x2 = cmx + (0.8 * sin(i * M_PI / 30.0) * cf);
					double y2 = cmy - (0.8 * cos(i * M_PI / 30.0) * cf);
					w.DrawLine(x, y, x2, y2, 1*bigF, Gray());
				}
			}
		}
	}
	if (numberType != StaticClock::NoNumber) {
		double numberPos;
		if (hourType == StaticClock::No) 
			numberPos = 1;
		else if (hourType == StaticClock::Square) 
			numberPos = 0.96;
		else if (hourType == StaticClock::Rectangle) 
			numberPos = 0.75;
		double numberd;
		Font fnt, fnt4;
		if (numberType == StaticClock::Small)  {
			numberd = numberPos - 0.2;	
			fnt4 = Arial((int)(14*bigF));
			fnt = Arial((int)(14*bigF));
		} else if (numberType == StaticClock::Big)  {
			numberd = numberPos - 0.2;
			fnt4 = Arial((int)(20*bigF));
			fnt = Arial((int)(20*bigF));
		} else if (numberType == StaticClock::BigSmall)  {
			numberd = numberPos - 0.2;
			fnt4 = Arial((int)(20*bigF));
			fnt = Arial((int)(14*bigF));
		} else if (numberType == StaticClock::Big4)  {
			numberd = numberPos - 0.2;
			fnt4 = Arial((int)(20*bigF));
		}
		for(int i = 1; i <= 12; i++) {
			double x = cmx + (numberd * sin(i * M_PI / 6.0) * cf);
			double y = cmy - (numberd * cos(i * M_PI / 6.0) * cf);
			if (i % 3 == 0)
				w.DrawCenterText(x, y, FormatInt(i), fnt4, letterColor);
			else if (numberType != StaticClock::Big4)
				w.DrawCenterText(x, y, FormatInt(i), fnt, letterColor);
		}
	}
	double handleF = bigF;
	double tm = t.hour * 3600 + t.minute * 60 + t.second;
	PaintPtr(w, cmx, cmy, tm / 3600 / 12, 0.5, 4*handleF, Color(200, 200, 200), cf);
	PaintPtr(w, cmx, cmy, tm / 3600 / 12, 0.5, 3*handleF, letterColor, cf);
	PaintPtr(w, cmx, cmy, tm / 3600, 0.6, 3*handleF, Color(200, 200, 200), cf);
	PaintPtr(w, cmx, cmy, tm / 3600, 0.6, 2*handleF, letterColor, cf);
	if (seconds)
		PaintPtr(w, cmx, cmy, tm / 60, 0.75, 1*handleF, Color(200, 200, 200), cf);
	
	int wm = width;
	if (colorType == WhiteType) {
		w.Begin();
			w.BeginMask();
				w.Move(0, 0).Line(wm, 0)
				  .Line(wm, wm).Line(0, wm)
				  .Fill(wm/4, wm/4, Black(), 2*wm, Color(220, 220, 220));
			w.End();
			w.Circle(wm/2, wm/2, wm/2).Fill(Black());
		w.End();
	} else {
		w.Begin();
			w.BeginMask();
				w.Ellipse(wm/2, wm/4, wm/3, wm/4).Fill(Color(60, 60, 60));
			w.End();
			w.Ellipse(wm/2, wm/4, wm/3, wm/4).Fill(White());
		w.End();		
	}
	ww.DrawImage(0, 0, ib);
}

StaticClock& StaticClock::SetImage(String _fileName) {
	image = StreamRaster::LoadFileAny(_fileName);
	Refresh();
	return *this;
}

void StaticClock::SetData(const Value& v) {
	SetTime((Time) v);
}

void StaticClock::SetTime(const Time& tm) {
	if (!IsNull(tm) && tm.IsValid()) {
		t = tm;
		Refresh();
	}
}

void StaticClock::SetTime(int h, int n, int s) {
	t.hour = h;
	t.minute = n;
	t.second = s;
	Refresh();
}

struct StaticClocks;
void StaticClockThread(StaticClocks *gui);

struct StaticClocks {
	Array <StaticClock *> clocks;
	volatile Atomic running, kill;
	
	friend void StaticClockThread(StaticClocks *gui);
	
	StaticClocks() {
		running = kill = 0;
	}
	~StaticClocks() {
		AtomicInc(kill);	
		while (running)
			Sleep(10);
	}
	void Add(StaticClock *clock) {
		clocks.Add(clock);
		if (!running) {
			AtomicInc(running);
			Thread().Run(callback1(StaticClockThread, this));
		}
	}
	void Remove(StaticClock *clock) {
		for (int i = 0; i < clocks.GetCount(); ++i) {
			if (clocks[i] == clock) {
				clocks.Remove(i);
				break;
			}
		}
		if (clocks.GetCount() == 0 && running) {	
			AtomicInc(kill);	
			while (running)
				Sleep(10);
			AtomicDec(kill);	
		}
	}
	void SetTimeRefresh(int i) {
		clocks[i]->SetTimeRefresh();
	}
	int GetCount() {return clocks.GetCount();}
};

StaticClocks clocks;

void StaticClockThread(StaticClocks *gui) {
	while (true) {
		if (gui->kill) {
			AtomicDec(gui->running);
			return;		
		}
		for (int i = 0; i < gui->GetCount(); ++i) 
			PostCallback(callback(gui->clocks[i], &StaticClock::SetTimeRefresh));
		Sleep(200);
	}
}

StaticClock& StaticClock::SetAuto(bool mode) {
	autoMode = mode;
	if (!mode) 
		clocks.Remove(this);
	else 
		clocks.Add(this);
	return *this;
}

StaticClock::StaticClock() {
	Transparent();
	NoWantFocus();

	hourType = StaticClock::Square;
	numberType = StaticClock::Small;
	image = Null;
	colorType = WhiteType;
	seconds = true;
	
	t = GetSysTime();
	autoMode = false;
}

StaticClock::~StaticClock() {
}

void Meter::PaintMarks(MyBufferPainter &w, double cx, double cy, double R, double ang0, 
			double ang1, int direction, double step, double bigF, Color color) {
	if (direction == -1) 
		Swap(ang0, ang1);
	ang0 = ToRad(ang0);
	ang1 = ToRad(ang1);
	step = ToRad(step);
	if (ang0 > ang1)
		ang1 += 2*M_PI;
	double width = 6*bigF;
	for (double i = ang0; i <= ang1+0.1; i += step) {
		double x0 = cx + R*cos(i);
		double y0 = cy - R*sin(i);
		double x1 = cx + 0.93*R*cos(i);
		double y1 = cy - 0.93*R*sin(i);
		w.DrawLine(x0, y0, x1, y1, width, color);
	}
}

double Meter::AngAdd(double ang, double val)
{
	ang += val;
	while (ang >= 360)
		ang -= 360;
	while (ang < 0)
		ang += 360;
	return ang;
}

void Meter::PaintNumbers(MyBufferPainter &w, double cx, double cy, double R, double a0, 
	double step, int direction, double minv, double maxv, double stepv, double bigF, Color color)
{
	a0 = ToRad(a0);
	step = ToRad(step);
	Font fnt = Arial((int)(13*bigF));
	while (minv <= maxv) {
		double x = cx + 0.8*R*cos(a0);
		double y = cy - 0.8*R*sin(a0);
		w.DrawCenterText(x, y, FormatDouble(minv), fnt, color);
		a0 += step*direction;
		minv += stepv;
	}
}

void Meter::PaintHand(MyBufferPainter &w, double cx, double cy, double R, double val, 
					  double bigF, int colorType)
{
	Color letterColor;
	if (colorType == WhiteType) {
		letterColor = Black();
	} else {
		letterColor = White();
	}
	val = ToRad(val);
	double x0 = cx + 0.90*R*cos(val);
	double y0 = cy - 0.90*R*sin(val);
	double x1 = cx;
	double y1 = cy;
	w.DrawLine(x0, y0, x1, y1, 5*bigF, LtGray());
	double x2 = cx + 0.4*R*cos(val);
	double y2 = cy - 0.4*R*sin(val);
	w.DrawLine(x0, y0, x2, y2, 4*bigF, letterColor);
	double x3 = cx - 0.3*R*cos(val);
	double y3 = cy + 0.3*R*sin(val);
	w.DrawLine(x1, y1, x3, y3, 5*bigF, letterColor);
	
	w.DrawCircle(cx, cy, bigF*18, LtGray());
	w.DrawCircle(cx, cy, bigF*15, Black());
}

void Meter::Paint(Draw& ww) {
	Size sz = GetSize();
	ImageBuffer ib(sz);
	MyBufferPainter w(ib);	

	w.Clear(RGBAZero());
	w.LineCap(LINECAP_BUTT);
	
	Rect r(0, 0, sz.cx, sz.cy);

	Color backColor, letterColor;
	if (colorType == WhiteType) {
		backColor = White();
		letterColor = Black();
	} else {
		backColor = Black();
		letterColor = White();
	}
	w.DrawRect(r, backColor);
	int width = r.right - r.left;
	int height = r.bottom - r.top;
	double bigF = width/200.;
	double bigH = height/100.;
	
	int direction;
	if (clockWise)
		direction = -1;
	else
		direction = 1;
	double a = angleBegin;
	double b = angleEnd;
	double cosa = cos(ToRad(a));
	double cosb = cos(ToRad(b));
	double sina = sin(ToRad(a));
	double sinb = sin(ToRad(b));
	double minx = cosa;
	double maxx = cosa;
	double miny = sina;
	double maxy = sina;
	double angminx = a;
	double angminy = a;
	if (cosb < minx) {
		minx = cosb;
		angminx = b;
	}
	if (cosb > maxx) 
		maxx = cosb;
	if (sinb > miny) {
		miny = sinb;
		angminy = b;
	}
	if (sinb < maxy)
		maxy = sinb;
	int maxgrad = 0;
	double angmaxx = 0;
	for (double ang = a; ang != b; ang = AngAdd(ang, direction)) {
		maxgrad++;
		if (ang == 180) {
			minx = -1;
			angminx = 180;
		} else if (ang == 0) {
			maxx = 1;
			angmaxx = 0; 
		} else if (ang == 90) {
			miny = 1;
			angminy = 90;
		} else if (ang == 270)
			maxy = -1;
	}
	double rate = width/(maxx - minx);
	double cx = -minx*rate;
	double R = 0.92*fabs(rate*minx/cos(ToRad(angminx)));
	double cy;
	if (fabs(miny) >= fabs(maxy))
		cy = 1.08*R*miny;
	else
		cy = r.bottom + 1.08*R*maxy;
	
	w.DrawArc(cx, cy, R, a, b, direction, fceil(0.8*bigF), letterColor);
	
	if (peak < max && peak > min) {
		double valpk = peak*maxgrad/(max-min);
		for (double i = 0.93; i < 0.98; i+= 0.004)
			w.DrawArc(cx, cy, i*R, a + valpk*direction, b, direction, (int)(2*bigF), LtRed());
		double fsize = 7*Upp::min(bigF, bigH);
		Font fnt = Arial((int)fsize);
		double txtx = cx + R*cos(ToRad(b))/2;
		double txty = cy - R*sin(ToRad(b))/2;
		w.DrawCenterText(txtx, txty, "  PEAK", fnt, Gray());
		Image light;
		if (value < peak)
			light = Controls4UImg::LightOff();
		else
			light = Controls4UImg::LightOn();
		w.DrawImage((int)(txtx - fsize*2/3.), (int)(txty + fsize), (int)(1.8*fsize), (int)(1.8*fsize), light);	
	}
	double stepa = step*maxgrad/(max-min);	
	PaintMarks(w, cx, cy, R, a, b, direction, stepa, bigF, letterColor);
	if (number)
		PaintNumbers(w, cx, cy, R, a, stepa, direction, min, max, step, bigF, letterColor);
		             
	Font fnt = Arial((int)(20*Upp::min(bigF, bigH)));
	double angtxt = ToRad(a + maxgrad*direction/2);
	double txtx = cx + R*cos(angtxt)/2;
	double txty = cy - R*sin(angtxt)/2;
	w.DrawCenterText(txtx, txty, text, fnt, letterColor);
	
	double vala = value*maxgrad/(max-min);
	PaintHand(w, cx, cy, R, a + vala*direction, bigF, colorType);	

	int wm = width;
	if (colorType == WhiteType) {
		w.Begin();
			w.BeginMask();
				w.Rectangle(0, 0, wm, wm)
				  .Fill(wm/4, wm/4, Black(), wm, Color(100, 100, 100));
			w.End();
			w.Rectangle(0, 0, wm, wm).Fill(Black());
		w.End();
	} else {
		w.Begin();
			w.BeginMask();
				w.Rectangle(0, 0, wm, height).Fill(Color(70, 70, 70));
			w.End();
			w.Rectangle(0, 0, wm, height/2).Fill(White());
		w.End();		
	}
	ww.DrawImage(0, 0, ib);
}

void MeterThread(Meter *gui, double newValue) {
	double delta = Sign(newValue-gui->value)*(gui->max - gui->min)/gui->sensibility;
	long deltaT = labs((long)(1000*gui->speed/delta));
	int maxi = (int)(fabs((newValue-gui->value)/delta));
	
	long t0 = GetTickCount();
	for (int i = 0; i < maxi; ++i, gui->value += delta) {
		if (gui->kill) {
			AtomicDec(gui->running);
			return;		
		}
		PostCallback(callback(gui, &Meter::RefreshValue));
		Sleep(deltaT);
	}
	gui->value = newValue;
	PostCallback(callback(gui, &Meter::RefreshValue));
	AtomicDec(gui->running);
}

void Meter::SetData(const Value& v)	{
	double val = v;
	if (running) {	// Stop movement before changing value
		AtomicInc(kill);	
		while (running)
			Sleep(10);
		AtomicDec(kill);	
	}
	AtomicInc(running);
	Thread().Run(callback2(MeterThread, this, val));
}

Meter::~Meter()
{
	AtomicInc(kill);
	while (running)
		Sleep(10);
}

Meter::Meter() {
	Transparent();
	NoWantFocus();

	min = value = 0;
	max = 100;
	peak = 80;
	step = 20;	
	angleBegin = 120;
	angleEnd = 40;
	clockWise = false;
	number = false;
	colorType = WhiteType;
	sensibility = 10;
	speed = 1;
	running = 0;
	kill = 0;
}
