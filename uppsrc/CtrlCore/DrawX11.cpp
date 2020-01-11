#include "CtrlCore.h"

#ifdef GUI_X11

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#undef Picture
#undef Time
#undef Font
#undef Display

namespace Upp {

#define LLOG(x)     //LOG(x)
#define LTIMING(x)  //TIMING(x)

XDisplay   *Xdisplay;
int         Xscreenno;
Visual     *Xvisual;
Window      Xroot;
Screen     *Xscreen;
Colormap    Xcolormap;
int         Xheight;
int         Xwidth;
int         XheightMM;
int         XwidthMM;
int         Xdepth;
dword       Xblack;
dword       Xwhite;
int         Xconnection;
byte       *Xmapcolor;
byte       *Xunmapcolor;
bool        Xpalette;

dword   (*Xgetpixel)(int r, int g, int b);

EXITBLOCK
{
	if(Xdisplay) {
		XCloseDisplay(Xdisplay);
		LLOG("Xdisplay closed");
		Xdisplay = NULL;
	}
	if(Xmapcolor)
		delete[] Xmapcolor;
	if(Xunmapcolor)
		delete[] Xunmapcolor;
}

void XError()
{
	Panic("X11 error !");
}

void   XError(const char *s)
{
	Panic(String("X11 error:") + s + " !");
}

static int sAcs;

bool sAllocColor(int xr, int xg, int xb)
{
	XColor ce;
	ce.red = xr;
	ce.green = xg;
	ce.blue = xb;
	ce.flags = DoRed | DoGreen | DoBlue;
	if(!XAllocColor(Xdisplay, Xcolormap, &ce)) return false;
	sAcs++;
	return sAcs < 257;
}

void sAllocColors()
{
	int r, g, b;
	for(r = 0; r < 2; r++)
		for(g = 0; g < 2; g++)
			for(b = 0; b < 2; b++)
				if(!sAllocColor(65535 * r, 65535 * g, 65535 * b)) return;
	for(r = 0; r < 3; r++)
		for(g = 0; g < 3; g++)
			for(b = 0; b < 3; b++)
				if((r == 1 || g == 1 || b == 1) &&
				   !sAllocColor((65535 * r) / 2, (65535 * g) / 2, (65535 * b) / 2)) return;
	for(r = 5; r >= 0; r--)
		for(g = 5; g >= 0; g--)
			for(b = 5; b >= 0; b--)
				if((r != 0 && r != 5 || g != 0 && g != 5 || b != 0 && b != 5) &&
				   !sAllocColor((65535 * r) / 5, (65535 * g) / 5, (65535 * b) / 5)) return;
	for(r = 1; r <= 11; r += 2)
		if(!sAllocColor((65535 * r) / 11, (65535 * r) / 11, (65535 * r) / 11)) return;
	for(int r = 255; r >= 0; r--)
		if(!sAllocColor(r << 8, r << 8, r << 8)) return;
}

dword GetPseudoColorPixel(int r, int g, int b)
{
	return Xmapcolor[r * 11 / 255 * (24 * 12) + g * 23 / 255 * 12 + b * 11 / 255];
}

static
struct Xshift {
	dword  mask;
	int    bits;
	int    shift;
	dword  Do(int c) { return (c >> bits << shift) & mask; }
}
Xred, Xgreen, Xblue;

Xshift CalcXShift(dword mask)
{
	Xshift f;
	f.mask = mask;
	f.shift = 0;
	f.bits = 0;
	while((mask & 1) == 0) {
		mask >>= 1;
		f.shift++;
	}
	while((mask & 1) == 1) {
		mask >>= 1;
		f.bits++;
	}
	f.bits = 8 - f.bits;
	if(f.bits < 0) {
		f.shift += f.bits;
		f.bits = 0;
	}
	LLOG("xshift(" << FormatIntHex(mask) << "): mask = "
		<< FormatIntHex(f.mask) << ", bits = " << f.bits << ", shift = " << f.shift);
	return f;
}

dword GetTrueColorPixel(int r, int g, int b)
{
	return Xred.Do(r) | Xgreen.Do(g) | Xblue.Do(b);
}

inline int ssq(int x) { return x * x; }

void InitX11Draw(XDisplay *display)
{
	Xdisplay = display;
	if(!Xdisplay) {
		puts(NFormat("No X11 display, errno = %d, %s", errno, strerror(errno)));
		fflush(stdout);
		XError();
	}
	Xscreenno = DefaultScreen(Xdisplay);
	Xroot = RootWindow(Xdisplay, Xscreenno);
	Xscreen = ScreenOfDisplay(Xdisplay, Xscreenno);
	Xcolormap = DefaultColormap(Xdisplay, Xscreenno);
//	Xcolormap = (Colormap)GDK().gdk_x11_colormap_get_xcolormap(GDK().gdk_rgb_get_colormap());
	Xheight = DisplayHeight(Xdisplay, Xscreenno);
	Xwidth = DisplayWidth(Xdisplay, Xscreenno);
	XheightMM = DisplayHeightMM(Xdisplay, Xscreenno);
	XwidthMM = DisplayWidthMM(Xdisplay, Xscreenno);
	LLOG("Xwidth = " << Xwidth << ", XwidthMM = " << XwidthMM);
	LLOG("Xheight = " << Xheight << ", XheightMM = " << XheightMM);
	Xdepth = DefaultDepth(Xdisplay, Xscreenno);
	Xblack = BlackPixel(Xdisplay, 0);
	Xwhite = WhitePixel(Xdisplay, 0);
	Xconnection = XConnectionNumber(Xdisplay);
	Xvisual = DefaultVisual(Xdisplay, Xscreenno);
	Visual *v = Xvisual;
	if(v->c_class == TrueColor) {
		Xred = CalcXShift(v->red_mask);
		Xgreen = CalcXShift(v->green_mask);
		Xblue = CalcXShift(v->blue_mask);
		Xgetpixel = GetTrueColorPixel;
	}
	else {
		Xpalette = true;
		sAllocColors();
		int colorcount = max(1 << Xdepth, 256);
		Buffer<XColor> cs(colorcount);
		int i;
		for(i = 0; i < colorcount; i++)
			cs[i].pixel = i;
		XQueryColors(Xdisplay, Xcolormap, cs, colorcount);
		Xunmapcolor = new byte[3 * colorcount];
		for(i = 0; i < colorcount; i++)
		{
			Xunmapcolor[3 * i + 0] = cs[i].blue;
			Xunmapcolor[3 * i + 1] = cs[i].green;
			Xunmapcolor[3 * i + 2] = cs[i].red;
		}
		byte *cm = Xmapcolor = new byte[12 * 24 * 12];
		for(int r = 0; r < 12; r++)
			for(int g = 0; g < 24; g++)
				for(int b = 0; b < 12; b++) {
					int mind = INT_MAX;
					int mini = 0;
					for(int i = 0; i < colorcount; i++) {
						int d = ssq(r * 255 / 11 - (cs[i].red >> 8)) +
						        ssq(g * 255 / 23 - (cs[i].green >> 8)) +
						        ssq(b * 255 / 11 - (cs[i].blue >> 8));
						if(d < mind) {
							mini = i;
							mind = d;
						}
					}
					*cm++ = mini;
				}
		Xgetpixel = GetPseudoColorPixel;
	}
//	XFree(v);
}

void InitX11Draw(const char *dispname)
{
	if(!dispname || !*dispname) {
		int f = Environment().Find("DISPLAY");
		dispname = (f >= 0 ? ~Environment()[f] : ":0.0");
	}
	InitX11Draw(XOpenDisplay(dispname));
}

void SetClip(GC gc, XftDraw *xftdraw, const Vector<Rect>& cl)
{
	GuiLock __;
	LTIMING("SetClip");
	Buffer<XRectangle> xr(cl.GetCount());
	LLOG("SetClip");
	for(int i = 0; i < cl.GetCount(); i++) {
		XRectangle& r = xr[i];
		const Rect& cr = cl[i];
		LLOG("[" << i << "] = " << cr);
		r.x = cr.left;
		r.y = cr.top;
		r.width = cr.Width();
		r.height = cr.Height();
	}
	XSetClipRectangles(Xdisplay, gc, 0, 0, xr, cl.GetCount(), Unsorted);
	LLOG("XftDrawSetClipRectangles, # = " << cl.GetCount() << ", xftdraw = " << FormatIntHex(xftdraw));
	XftDrawSetClipRectangles(xftdraw, 0, 0, xr, cl.GetCount());
	LLOG("//XftDrawSetClipRectangles");
}

void SystemDraw::CloneClip()
{
	if(cloff.GetCount() > 1 && cloff.Top().clipi == cloff[cloff.GetCount() - 2].clipi) {
		cloff.Top().clipi = clip.GetCount();
		Vector<Rect>& c = clip.Add();
		c <<= clip[clip.GetCount() - 2];
	}
}

void SystemDraw::SetForeground(Color color)
{
	GuiLock __;
	LTIMING("SetForeground");
	int p = GetXPixel(color.GetR(), color.GetG(), color.GetB());
	if(p == foreground) return;
	LTIMING("XSetForeground");
	LLOG("XSetForeground " << color);
	foreground = p;
	XSetForeground(Xdisplay, gc, foreground);
}

void SystemDraw::SetClip() {
	GuiLock __;
	drawingclip = Rect(-(INT_MAX >> 1), -(INT_MAX >> 1), +(INT_MAX >> 1), +(INT_MAX >> 1));
	if(!clip.IsEmpty()) {
		const Vector<Rect>& topclip = clip.Top();
		drawingclip = Rect(0, 0, 0, 0);
		if(!topclip.IsEmpty()) {
			drawingclip = topclip[0];
			for(int i = 1; i < topclip.GetCount(); i++)
				drawingclip |= topclip[i];
		}
		drawingclip -= actual_offset;
	}
	if(dw == Xroot) return;
	LTIMING("SetClip");
	UPP::SetClip(gc, xftdraw, clip.Top());
}

void SystemDraw::SetLineStyle(int width)
{
	GuiLock __;
	if(width == linewidth) return;
	linewidth = width;
	if(IsNull(width))
		width = 1;
	if(width < PEN_SOLID) {
		static const char dash[] = { 18, 6 };
		static const char dot[] = { 3, 3 };
		static const char dashdot[] = { 9, 6, 3, 6 };
		static const char dashdotdot[] = { 9, 3, 3, 3, 3, 3 };
		static struct {
			const char *dash;
			int   len;
		} ds[] = {
			{ dash, __countof(dash) },
			{ dot, __countof(dot) },
			{ dashdot, __countof(dashdot) },
			{ dashdotdot, __countof(dashdotdot) }
		};
		int i = -(width - PEN_DASH);
		ASSERT(i >= 0 && i < 4);
		XSetDashes(Xdisplay, gc, 0, ds[i].dash, ds[i].len);
	}
	XSetLineAttributes(Xdisplay, gc, max(width, 1),
	                   width < PEN_SOLID ? LineOnOffDash : LineSolid, CapRound, JoinRound);
}

void SystemDraw::Init()
{
	GuiLock __;
	pageSize = Size(Xwidth, Xheight);
	cloff.Clear();
	clip.Clear();
	drawingclip = Rect(-(INT_MAX >> 1), -(INT_MAX >> 1), +(INT_MAX >> 1), +(INT_MAX >> 1));
	foreground = linewidth = Null;
}

void SystemDraw::Init(const Vector<Rect>& _clip, Point _offset)
{
	GuiLock __;
	Init();
	clip.Add() <<= _clip;
	offset.Add(_offset);
	actual_offset = _offset;
	Cloff& f = cloff.Add();
	f.offseti = 0;
	f.clipi = 0;
	SetClip();
}

dword SystemDraw::GetInfo() const
{
	return 0;
}

Size SystemDraw::GetPageSize() const
{
	return pageSize;
}

SystemDraw::SystemDraw()
{
	GuiLock __;
	dw = None;
	gc = None;
	actual_offset = Point(0, 0);
	Init();
}

Size SystemDraw::GetNativeDpi() const
{
	return Size(96, 96);
}

void SystemDraw::BeginNative() {}
void SystemDraw::EndNative() {}

int SystemDraw::GetCloffLevel() const
{
	return cloff.GetCount();
}

Rect SystemDraw::GetClip() const
{
	LLOG("Draw::GetClipOp; #clip = " << clip.GetCount() << ", #cloff = " << cloff.GetCount()
		<< ", clipi = " << cloff.Top().clipi);
	const Vector<Rect>& cl = clip[cloff.Top().clipi];
	Rect box(0, 0, 0, 0);
	if(!cl.GetCount()) return box;
	box = cl[0];
	LLOG("cl[0] = " << box);
	for(int i = 1; i < cl.GetCount(); i++) {
		LLOG("cl[" << i << "] = " << cl[i]);
		box |= cl[i];
	}
	LLOG("out box = " << box << ", actual offset = " << actual_offset);
	return box - actual_offset;
}

SystemDraw::SystemDraw(Drawable _dw, GC _gc, XftDraw *_xftdraw, const Vector<Rect>& _clip)
{
	LLOG("SystemDraw");
	dw = _dw;
	gc = _gc;
	xftdraw = _xftdraw;
	Init(_clip);
}

void BackDraw::Create(SystemDraw& w, int cx, int cy)
{
	GuiLock __;
	LLOG("Creating BackDraw " << cx << "x" << cy);
	Destroy();
	size.cx = cx;
	size.cy = cy;
	dw = XCreatePixmap(Xdisplay, w.GetDrawable(), max(cx, 1), max(cy, 1), Xdepth);
	gc = XCreateGC(Xdisplay, dw, 0, 0);
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
	Vector<Rect> clip;
	clip.Add(RectC(0, 0, cx, cy));
	Init(clip, Point(0, 0));
}

void BackDraw::Put(SystemDraw& w, int x, int y)
{
	GuiLock __;
	LLOG("Putting BackDraw");
	ASSERT(dw != None);
	XCopyArea(Xdisplay, dw, w.GetDrawable(), w.GetGC(), 0, 0, size.cx, size.cy,
	          x + w.GetOffset().x, y + w.GetOffset().y);
}

void BackDraw::Destroy()
{
	GuiLock __;
	if(dw != None) {
		XftDrawDestroy(xftdraw);
		XFreePixmap(Xdisplay, dw);
		XFreeGC(Xdisplay, gc);
	}
}

bool ScreenInPaletteMode()
{
	return Xpalette;
}

}

#endif
