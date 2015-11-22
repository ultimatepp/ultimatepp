#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
}

void DrawDragLine(SystemDraw& w, bool horz, int x, int y, int len, int n, const int *pattern, Color color, int animation)
{
	if(len <= 0)
		return;
	if(horz)
		w.Clip(x, y, len, n);
	else
		w.Clip(x, y, n, len);
	
	(horz ? x : y) -= animation;
	len += animation;
	bool ch = false;
	while(len > 0) {
		int segment = pattern[ch];
		int d = segment + pattern[2];
		if(horz) {
			w.DrawRect(x, y, segment, n, color);
			x += d;
		}
		else {
			w.DrawRect(x, y, n, segment, color);
			y += d;
		}
		len -= d;
		ch = !ch;
	}
	w.End();
}

void DrawDragFrame(SystemDraw& w, const Rect& r, int n, const int *pattern, Color color, int animation)
{
	DrawDragLine(w, true, r.left, r.top, r.GetWidth(), n, pattern, color, animation);
	DrawDragLine(w, false, r.left, r.top + n, r.GetHeight() - 2 * n, n, pattern, color, animation);
	DrawDragLine(w, false, r.right - n, r.top + n, r.GetHeight() - 2 * n, n, pattern, color, animation);
	DrawDragLine(w, true, r.left, r.bottom - n, r.GetWidth(), n, pattern, color, animation);
}

void DrawDragRect(Ctrl& q, const DrawDragRectInfo& f)
{
	ViewDraw w(&q);
	w.Clip(f.clip);
	static int dashes[3][3] = {
		{ 32, 32, 0 },
		{ 1, 1, 1 },
		{ 5, 1, 2 },
	};
	const int *dash = dashes[minmax(f.type, 0, 2)];
	Color color = InvertColor;
	DrawDragFrame(w, f.rect1, f.n, dash, color, f.animation);
	DrawDragFrame(w, f.rect2, f.n, dash, color, f.animation);
	w.End();
}

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
	Ctrl *top = q.GetTopCtrl();
	if(top && top->top) {
		Rect sv = q.GetScreenView();
		Rect tv = top->GetScreenView();
		Point off = sv.TopLeft() - tv.TopLeft();
		DrawDragRectInfo& f = top->top->dr.Create();
		f.rect1 = rect1 + off;
		f.rect2 = rect2 + off;
		f.clip = (clip & q.GetSize()) + off;
		f.n = n;
		f.color = color;
		f.type = type;
		f.animation = animation;
		DrawDragRect(*top, f);
	}
}

void FinishDragRect(Ctrl& q)
{
	Ctrl *top = q.GetTopCtrl();
	if(top && top->top && top->top->dr) {
		DrawDragRect(*top, *top->top->dr);
		top->top->dr.Clear();
	}
}

GdkRect::GdkRect(const Rect& r)
{
	x = r.left;
	y = r.top;
	width = r.GetWidth();
	height = r.GetHeight();
}

Image ImageFromPixbufUnref(GdkPixbuf *pixbuf)
{
	Image img;
	if(pixbuf) {
		if(gdk_pixbuf_get_n_channels (pixbuf) == 4 &&
		   gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB &&
		   gdk_pixbuf_get_bits_per_sample (pixbuf) == 8 && 
		   gdk_pixbuf_get_has_alpha (pixbuf)) {
			Size sz(gdk_pixbuf_get_width (pixbuf), gdk_pixbuf_get_height(pixbuf));
			ImageBuffer m(sz);
			int stride = gdk_pixbuf_get_rowstride(pixbuf);
			byte *l = (byte *)gdk_pixbuf_get_pixels(pixbuf);
			for(int y = 0; y < sz.cy; y++) {
				RGBA *s = m[y];
				const RGBA *e = s + sz.cx;
				const byte *t = l;
				while(s < e) {
					s->r = *t++;
					s->g = *t++;
					s->b = *t++;
					s->a = *t++;
					s++;
				}
				l += stride;
			}
			img = m;
		}
		g_object_unref(pixbuf);
	}
	return img;
}

GdkAtom GAtom(const String& id)
{
	GuiLock __;
	static VectorMap<String, GdkAtom> m;
	int q = m.Find(id);
	return q >= 0 ? m[q] : (m.Add(id) = gdk_atom_intern(~id, FALSE));
}

int rmsecs()
{
	static int msecs0 = msecs();
	return msecs(msecs0);
}
 
END_UPP_NAMESPACE

#endif
