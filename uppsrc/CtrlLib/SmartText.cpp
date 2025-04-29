#include "CtrlLib.h"

namespace Upp {


Size GetSmartTextSize(const char *text, Font font, int cx) {
	if(*text == '\1') {
		Size sz;
		RichText txt = ParseQTF(text + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		sz.cx = min(cx, txt.GetWidth());
		sz.cy = txt.GetHeight(Zoom(1, 1), sz.cx);
		return sz;
	}
	return GetTLTextSize(ToUnicode(text, CHARSET_DEFAULT), font);
}

int GetSmartTextHeight(const char *s, int cx, Font font) {
	if(*s == '\1') {
		Size sz;
		RichText txt = ParseQTF(s + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		return txt.GetHeight(Zoom(1, 1), cx);
	}
	int cy = font.Info().GetHeight();
	int h = cy;
	while(*s) {
		if(*s == '\n')
			h += cy;
		s++;
	}
	return h;
}

void DrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Font font, Color ink,
                   int accesskey, Color qtf_ink, int dark_theme) {
	if(*text == '\1') {
		RichText txt = ParseQTF(text + 1, accesskey);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		PaintInfo pi;
		pi.darktheme = !IsNull(dark_theme) ? dark_theme : IsDarkTheme();
		pi.textcolor = qtf_ink;
		txt.Paint(draw, x, y, cx, pi);
		return;
	}
	DrawTLText(draw, x, y, cx, ToUnicode(text, CHARSET_DEFAULT), font, ink, accesskey);
}

namespace detail {

struct OriDraw : public DrawProxy {
	bool  clockwise;
	Point origin;

	void  Transform(int& x, int& y) const;
	void  Transform(int& x, int& y, int& cx, int& cy) const;

	Event<Point&> transform_point;
	Event<int&>   transform_angle;
	Event<Image&> transform_image;
	
	void OffsetOp(Point p) override;
	bool ClipOp(const Rect& r) override;
	bool ClipoffOp(const Rect& r) override;
	bool IsPaintingOp(const Rect& r) const override { return true; }

	void DrawRectOp(int x, int y, int cx, int cy, Color color) override;
	void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color) override;
	void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor) override;
	void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		            Color ink, int n, const int *dx) override;
};

force_inline
void OriDraw::Transform(int& x, int& y) const
{
	if(clockwise) {
		int y0 = y;
		y = origin.y + x;
		x = origin.x - y0;
	}
	else {
		int y0 = y;
		y = origin.y - x;
		x = origin.x + y0;
	}
}

force_inline
void OriDraw::Transform(int& x, int& y, int& cx, int& cy) const
{
	Transform(x, y);
	Swap(cx, cy);
	if(clockwise)
		x -= cx;
	else
		y -= cy;
}

void OriDraw::OffsetOp(Point p)
{
	if(clockwise)
		ptr->OffsetOp(Point(-p.y, p.x));
	else
		ptr->OffsetOp(Point(p.y, -p.x));
}

bool OriDraw::ClipOp(const Rect& r)
{
	Begin();
	return true;
}

bool OriDraw::ClipoffOp(const Rect& r)
{
	OffsetOp(r.TopLeft());
	return true;
}

void OriDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	Transform(x, y, cx, cy);
	ptr->DrawRectOp(x, y, cx, cy, color);
}

void OriDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	Transform(x, y, cx, cy);
	Image m = MakeImage(img, clockwise ? RotateClockwise : RotateAntiClockwise);
	ptr->DrawImageOp(x, y, cx, cy, m, Size(cx, cy), color);
}

void OriDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	int x = r.left;
	int y = r.top;
	int cx = r.GetWidth();
	int cy = r.GetHeight();
	Transform(x, y, cx, cy);
	ptr->DrawEllipseOp(RectC(x, y, cx, cy), color, pen, pencolor);
}

void OriDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	Transform(x, y);
	if(clockwise)
		angle -= 900;
	else
		angle += 900;
	ptr->DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

}

void DrawSmartText(Draw& w, int x, int y, int cx, const char *text, int orientation,
                   Font font, Color ink, int accesskey, Color qtf_ink, int dark_theme)
{
	if(orientation == ORIENTATION_NORMAL) {
		DrawSmartText(w, x, y, cx, text, font, ink, accesskey, qtf_ink, dark_theme);
		return;
	}
	
	detail::OriDraw ow;
	ow.SetTarget(&w);
	ow.origin = Point(x, y);
	ow.clockwise = orientation == ORIENTATION_CLOCKWISE;
	DrawSmartText(ow, 0, 0, cx, text, font, ink, accesskey, qtf_ink, dark_theme);
}

String DeAmp(const char *s)
{
	String out;
	for(; *s; out.Cat(*s++))
		if(*s == '&')
			out.Cat('&');
	return out;
}

bool CompareAccessKey(int accesskey, dword key)
{
	return accesskey && dword(ToUpper(accesskey & 255) - 'A' + K_ALT_A) == key;
}

int  ExtractAccessKey(const char *s, String& label)
{
	byte akey = 0;
	int  pos = 0;
	String text;
	bool qtf = *s == '\1';
	while(*s)
		if((*s == '&' && !qtf || *s == '\b') && s[1] && s[1] != '&') {
			akey = ToAscii(ToUpper(s[1]));
			pos = text.GetLength() + 1;
			s++;
		}
		else
			text.Cat(*s++);
	text.Shrink();
	label = text;
	return MAKELONG(akey, pos);
}

int  ChooseAccessKey(const char *text, dword used)
{
	for(const char *s = text; *s; s++) {
		byte ac = *s;
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return MAKELONG(ac, s - text + 1);
	}
	for(const char *s = text; *s; s++) {
		dword ac = ToUpper(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && ac != 'I' && ac != 'L' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	for(const char *s = text; *s; s++) {
		dword ac = ToUpper(*s);
		if(ac < 128 && ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	return 0;
}

}