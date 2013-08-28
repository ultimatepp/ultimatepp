#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

void DDRect(RGBA *t, int dir, const byte *pattern, int pos, int count)
{
	while(count-- > 0) {
		byte p = pattern[7 & pos++];
		t->r ^= p;
		t->g ^= p;
		t->b ^= p;
		t += dir;
	}
}

void DrawLine(const Vector<Rect>& clip, int x, int y, int cx, int cy, bool horz, const byte *pattern, int animation)
{
	if(cx <= 0 || cy <= 0)
		return;

/*	Vector<Rect> rr = Intersection(clip, RectC(x, y, cx, cy));
	for(int i = 0; i < rr.GetCount(); i++) {
		Rect r = rr[i];
		if(horz)
			for(int y = r.top; y < r.bottom; y++)
				DDRect(framebuffer[y] + r.left, 1, pattern, r.left + animation, r.GetWidth());
		else
			for(int x = r.left; x < r.right; x++)
				DDRect(framebuffer[r.top] + x, framebuffer.GetWidth(), pattern, r.top + animation, r.GetHeight());
	}*/

	glColor4ub(255, 0, 0, 255);
	//glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x + cx, y + cy);
		//glVertex2i(10, 10);
		//glVertex2i(100, 100);
	glEnd();
	//glDisable(GL_LINE_STIPPLE);
}

void DragRectDraw0(const Vector<Rect>& clip, const Rect& rect, int n, const byte *pattern, int animation)
{
	int hn = min(rect.GetHeight(), n);
	int vn = min(rect.GetWidth(), n);
	DrawLine(clip, rect.left, rect.top, rect.GetWidth(), hn, true, pattern, animation);
	DrawLine(clip, rect.left, rect.top + hn, vn, rect.GetHeight() - hn, false, pattern, animation);
	DrawLine(clip, rect.right - vn, rect.top + hn, vn, rect.GetHeight() - hn, false, pattern, animation);
	DrawLine(clip, rect.left + vn, rect.bottom - hn, rect.GetWidth() - 2 * vn, hn, true, pattern, animation);
}

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n, Color color, int type, int animation)
{
	static byte solid[] =  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	static byte normal[] = { 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00 };
	static byte dashed[] = { 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
	Point p = Ctrl::screenRect.TopLeft();
	Vector<Rect> pr;
	if(type & Ctrl::DRAWDRAGRECT_SCREEN) {
		pr.Add(Rect(Ctrl::screenRect.GetSize()));
		type &= ~Ctrl::DRAWDRAGRECT_SCREEN;
		p = Point(0, 0);
	}
	else
		//pr = Intersection(screenRect, clip.Offseted(p));
		pr.Add(clip.Offseted(p));
	const byte *pattern = type == DRAWDRAGRECT_DASHED ? dashed :
	                      type == DRAWDRAGRECT_NORMAL ? normal : solid;
	DragRectDraw0(pr, rect1.Offseted(p), n, pattern, animation);
	DragRectDraw0(pr, rect2.Offseted(p), n, pattern, animation);
}

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
}

void FinishDragRect(Ctrl& q)
{
}

END_UPP_NAMESPACE

#endif
