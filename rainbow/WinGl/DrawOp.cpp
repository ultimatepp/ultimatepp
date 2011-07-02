#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL

NAMESPACE_UPP

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

#define LLOG(x)      // LOG(x)
#define LTIMING(x)   // RTIMING(x)

void SystemDraw::PlaneEquation(double eq[4], float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	eq[0] = y1 * (z2 - z3) + 
	        y2 * (z3 - z1) + 
	        y3 * (z1 - z2);

	eq[1] = z1 * (x2 - x3) + 
	        z2 * (x3 - x1) +
	        z3 * (x1 - x2);
	        
	eq[2] = x1 * (y2 - y3) +
	        x2 * (y3 - y1) +
	        x3 * (y1 - y2);
	        
	eq[3] = -(x1 * (y2 * z3 - y3 * z2) + 
	          x2 * (y3 * z1 - y1 * z3) +
	          x3 * (y1 * z2 - y2 * z1));
}

void SystemDraw::SetClipRect(const Rect& r)
{
	clip = r;
#if CLIP_MODE != 2
	for(int i = 0; i < ci; i++)
		clip &= cloff[i].drawing_clip;
#endif
}

void SystemDraw::ScissorClip(const Rect& r)
{
	glScissor(r.left, drawing_size.cy - r.top - r.Height(), r.Width(), r.Height());
}

void SystemDraw::PlaneClip(const Rect& r)
{
	float cl = (float) r.left;
	float ct = (float) r.top;
	float cr = (float) r.right;
	float cb = (float) r.bottom;
	
	double eq[4];
	
	PlaneEquation(eq, cl, ct, 0, cl, cb, 0, cl, cb, +1.0f);	
	glClipPlane(GL_CLIP_PLANE0, eq);
	
	PlaneEquation(eq, cr, ct, 0, cr, cb, 0, cr, cb, -1.0f);	
	glClipPlane(GL_CLIP_PLANE1, eq);

	PlaneEquation(eq, cl, ct, 0, cr, ct, 0, cr, ct, -1.0f);	
	glClipPlane(GL_CLIP_PLANE2, eq);

	PlaneEquation(eq, cl, cb, 0, cr, cb, 0, cr, cb, +1.0f);	
	glClipPlane(GL_CLIP_PLANE3, eq);
}

void SystemDraw::SetVec(float* v, float sx, float sy, float dx, float dy)
{
	v[0] = sx; v[1] = dy;
	v[2] = sx; v[3] = sy;
	v[4] = dx; v[5] = dy;
	v[6] = dx; v[7] = sy;
}

void SystemDraw::SetVec(float* v, int sx, int sy, int dx, int dy)
{
	v[0] = (float) sx; v[1] = (float) dy;
	v[2] = (float) sx; v[3] = (float) sy;
	v[4] = (float) dx; v[5] = (float) dy;
	v[6] = (float) dx; v[7] = (float) sy;
}

void SystemDraw::StencilClip(const Rect& r, int mode)
{
	float vtx[] = {
		(float) r.left, (float) r.bottom,
		(float) r.left, (float) r.top,
		(float) r.right, (float) r.bottom,
		(float) r.right, (float) r.top
	};
	
	//SetVec(vtx, r.left, r.top, r.right, r.bottom);
	glVertexPointer(2, GL_FLOAT, 0, vtx);
	
	glColorMask(0, 0, 0, 0);
	if(mode == 0)
	{
		++cn;
		glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
		glStencilFunc(GL_GEQUAL, cn, ~0);	
	}
	else
	{
		--cn;
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
		glStencilFunc(GL_ALWAYS, cn, ~0);
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glStencilFunc(GL_LEQUAL, cn, ~0);
	glColorMask(1, 1, 1, 1);
	
	/*
	if(mode == 0)
	{
		glColorMask(0, 0, 0, 0);
		glStencilOp(GL_KEEP, GL_INCR_WRAP, GL_INCR_WRAP);
		glStencilFunc(GL_EQUAL, cn, ~0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glStencilFunc(GL_LEQUAL, ++cn, ~0);
		glColorMask(1, 1, 1, 1);
		//cn = cd;
	}
	else
	{
		glStencilFunc(GL_LEQUAL, --cn, ~0);
	}
	*/			
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void SystemDraw::SetClip(const Rect& r, int mode)
{
	//glColor4ub(255, 0, 0, 10);
	//glRecti(r.left, r.top, r.right, r.bottom);
	SetClipRect(r);

#if CLIP_MODE == 0
	ScissorClip(clip);
#elif CLIP_MODE == 1
	PlaneClip(clip);
#elif CLIP_MODE == 2
	StencilClip(clip, mode);
#endif
}

void SystemDraw::BeginOp()
{
	Cloff& w = cloff[ci++];
	w.clipping = true;
	w.org = drawing_offset;
	w.drawing_clip = drawing_clip;	
}

void SystemDraw::EndOp()
{
	ASSERT(ci);
#if CLIP_MODE == 2
	if(cloff[ci - 1].clipping)
		SetClip(drawing_clip, 1);
#endif
	Cloff& w = cloff[--ci];
	drawing_offset = w.org;
	drawing_clip = w.drawing_clip;
#if CLIP_MODE != 2
	if(cloff[ci].clipping)
		SetClip(drawing_clip, 1);
#endif
}

void SystemDraw::OffsetOp(Point p)
{
	BeginOp();
	cloff[ci - 1].clipping = false;
	drawing_offset += p;
}

bool SystemDraw::ClipOp(const Rect& r)
{
	BeginOp();
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip);
	return true;
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	BeginOp();
	drawing_clip = r + drawing_offset;
	drawing_offset += r.TopLeft();
	SetClip(drawing_clip);
	return true;
}

bool SystemDraw::ExcludeClipOp(const Rect& r)
{
	return true;
}

bool SystemDraw::IntersectClipOp(const Rect& r)
{
	Cloff& w = cloff[ci];
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip);
	return true;
}

bool SystemDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

Rect SystemDraw::GetPaintRect() const
{
	return drawing_clip;
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(IsNull(color))
		return;
	
	if(cx <= 0 || cy <= 0) return;

	float sx = (float) x + drawing_offset.x;
	float sy = (float) y + drawing_offset.y;
	float dx = sx + cx;
	float dy = sy + cy;

#if CLIP_MODE == 3
	float cl = (float) clip.left;
	float ct = (float) clip.top;
	float cr = (float) clip.right;
	float cb = (float) clip.bottom;

	if(sx > cr || sy > cb)
		return;

	if(dx < cl || dy < ct)
		return;
	
	if(sx < cl)
		sx = cl;
	if(sy < ct)
		sy = ct;
	if(dx > cr)
		dx = cr;
	if(dy > cb)
		dy = cb;
#endif
	
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	
	float vtx[] = {
		sx, dy,
		sx, sy,
		dx, dy,
		dx, sy
	};
	
	//SetVec(vtx, sx, sy, dx, dy);
	
	
	glVertexPointer(2, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glColor4ub(255, 255, 255, (int) alpha);
}

void SystemDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	if(cx <= 0 || cy <= 0) return;

	float sx = (float) x + drawing_offset.x;
	float sy = (float) y + drawing_offset.y;
	float dx = sx + cx;
	float dy = sy + cy;

#if CLIP_MODE == 3
	if(sx > clip.right || sy > clip.bottom)
		return;

	if(dx < clip.left || dy < clip.top)
		return;
#endif
	
	float tl = (float) src.left;
	float tr = (float) src.right;
	float tt = (float) src.top;
	float tb = (float) src.bottom;
	
	float sw = (float) src.GetWidth();
	float sh = (float) src.GetHeight();

#if CLIP_MODE == 3
	float cl = (float) clip.left;
	float ct = (float) clip.top;
	float cr = (float) clip.right;
	float cb = (float) clip.bottom;

	if(sx < cl)
	{
		float dl = cl - sx;
		tl += dl * sw / (float) cx;
		sx = cl;
	}
	
	if(sy < ct)
	{
		float dt = ct - sy;
		tt += dt * sh / (float) cy;
		sy = ct;
	}
	
	if(dx > cr)
	{
		float dr = dx - cr;
		tr -= dr * sw / (float) cx;
		dx = cr;
	}
	
	if(dy > cb)
	{
		float db = dy - cb;
		tb -= db * sh / (float) cy;
		dy = cb;
	}
#endif
	
	Resources::Bind(img);

	float tw = 1.f / (float) img.GetWidth();
	float th = 1.f / (float) img.GetHeight();

	tl *= tw;
	tr *= tw;
	tt *= th;
	tb *= th;

	if(IsNull(color))	
		glColor4ub(255, 255, 255, (int) alpha);
	else
		glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	//if(!IsNull(color))
	//	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	
	glEnable(GL_TEXTURE_2D);

	float vtx[] = {
		sx, dy,
		sx, sy,
		dx, dy,
		dx, sy
	};

	float crd[] = {
		tl, tb,
		tl, tt,
		tr, tb,
		tr, tt
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, crd);
	glVertexPointer(2, GL_FLOAT, 0, vtx);
	//SetVec(vtx, sx, sy, dx, dy);
	//SetVec(crd, tl, tt, tr, tb);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	glLineWidth((GLfloat) width);
	glBegin(GL_LINES);
		glVertex2i(x1 + drawing_offset.x, y1 + drawing_offset.y);
		glVertex2i(x2 + drawing_offset.x, y2 + drawing_offset.y);
	glEnd();
}

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
}

void SystemDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
}

void SystemDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
}

void SystemDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
}

void SystemDraw::SaveCurrentColor()
{
	glGetFloatv(GL_CURRENT_COLOR, current_color);
}

void SystemDraw::RestoreLastColor()
{
	glColor4f(current_color[0], current_color[1], current_color[2], current_color[3]);
}

END_UPP_NAMESPACE

#endif