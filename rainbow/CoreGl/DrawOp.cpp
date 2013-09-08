#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

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
		if(!cloff[i].exclude)
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

void SystemDraw::StencilClip(const Rect& r, int mode, bool exclude)
{
	float vtx[12];
	SetVtx(vtx, (float) r.left, (float) r.top, (float) r.right, (float) r.bottom);
	
	glVertexPointer(projection_mode ? 3 : 2, GL_FLOAT, 0, vtx);
	
	int prev_cn = cn;
	
	glColorMask(0, 0, 0, 0);
	
	if(mode == 0)
	{
		if(exclude)
		{
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 255, ~0);
		}
		else
		{
			++cn;
			glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
			glStencilFunc(GL_GEQUAL, cn, ~0);
		}
	}
	else
	{
		if(exclude)
		{
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, cn, ~0);
		}
		else
		{
			--cn;
			glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
			glStencilFunc(GL_ALWAYS, cn, ~0);
		}
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	if(exclude)
		glStencilFunc(GL_GREATER, 255, ~0);
	else
		glStencilFunc(GL_EQUAL, cn, ~0);

	glColorMask(1, 1, 1, 1);
}

void SystemDraw::SetClip(const Rect& r, int mode, bool exclude)
{
	SetClipRect(r);
#if CLIP_MODE == 0
	ScissorClip(clip);
#elif CLIP_MODE == 1
	PlaneClip(clip);
#elif CLIP_MODE == 2
	StencilClip(clip, mode, exclude);
#endif
}

void SystemDraw::BeginOp()
{
	Cloff& w = cloff[ci++];
	w.clipping = false;
	w.exclude = false;
	w.org = drawing_offset;
	w.drawing_clip = drawing_clip;
}

void SystemDraw::EndOp()
{
	ASSERT(ci);
#if CLIP_MODE == 2
	if(cloff[ci - 1].clipping)
		SetClip(drawing_clip, 1, cloff[ci - 1].exclude);
#endif
	Cloff& w = cloff[--ci];
	drawing_offset = w.org;
	drawing_clip = w.drawing_clip;
#if CLIP_MODE != 2
	if(cloff[ci].clipping)
		SetClip(drawing_clip);
#endif
}

void SystemDraw::OffsetOp(Point p)
{
	GuiLock __;
	BeginOp();
	drawing_offset += p;
}

bool SystemDraw::ClipOp(const Rect& r)
{
	GuiLock __;
	BeginOp();
	cloff[ci - 1].clipping = true;
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip);
	return true;
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	GuiLock __;
	BeginOp();
	cloff[ci - 1].clipping = true;
	drawing_clip = r + drawing_offset;
	drawing_offset += r.TopLeft();
	SetClip(drawing_clip);
	return true;
}

bool SystemDraw::ExcludeClipOp(const Rect& r)
{
	GuiLock __;
	ASSERT(ci > 0);
	cloff[ci - 1].clipping = true;
	cloff[ci - 1].exclude = true;
	drawing_clip = r + drawing_offset;
	SetClip(drawing_clip, 0, true);
	return true;
}

bool SystemDraw::IntersectClipOp(const Rect& r)
{
	GuiLock __;
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
	GuiLock __;
	return drawing_clip;
}

void SystemDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	GuiLock __;
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

	#ifdef COLOR_ALPHA	
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), color.GetA() >= 255 ? (int) alpha : color.GetA());
	#else
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	#endif
	
	float vtx[12];
	SetVtx(vtx, sx, sy, dx, dy);
	
	glVertexPointer(projection_mode ? 3 : 2, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	#ifdef COLOR_ALPHA
	glColor4ub(255, 255, 255, 255);
	#else
	glColor4ub(255, 255, 255, (int) alpha);
	#endif
}

void SystemDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	GuiLock __;
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
	
	const TextureResource& t = resources.Bind(img, Resources::AUTO_ATLAS | Resources::NEAREST_FILTERING);

	float tw = 1.f / (float) t.realWidth;
	float th = 1.f / (float) t.realHeight;

	tl = (tl + t.x + TEXEL_OFFSET) * tw;
	tr = (tr + t.x - TEXEL_OFFSET) * tw;
	tt = (tt + t.y + TEXEL_OFFSET) * th;
	tb = (tb + t.y - TEXEL_OFFSET) * th;

	if(image_coloring)
	{
		if(IsNull(color))
			glColor4ub(255, 255, 255, (int) alpha);
		else
		{
			#ifdef COLOR_ALPHA
			glColor4ub(color.GetR(), color.GetG(), color.GetB(), color.GetA() >= 255 ? (int) alpha : color.GetA());
			#else
			glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
			#endif
		}
	}
	else
		glColor4ub(255, 255, 255, 255);
	
	float vtx[12];
	SetVtx(vtx, sx, sy, dx, dy);

	float crd[] = {
		tl, tb,
		tl, tt,
		tr, tb,
		tr, tt
	};

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, crd);
	glVertexPointer(projection_mode ? 3 : 2, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void SystemDraw::DrawTextureOp(const RectF& r, int textureId, int width, int height, const RectF& src)
{
	GuiLock __;
	float sx = r.left + drawing_offset.x;
	float sy = r.top + drawing_offset.y;
	float dx = sx + r.GetWidth();
	float dy = sy + r.GetHeight();

	float tl = src.left;
	float tr = src.right;
	float tt = src.top;
	float tb = src.bottom;
	
	resources.Bind(textureId, Resources::NEAREST_FILTERING);

	float tw = 1.f / (float) (width);
	float th = 1.f / (float) (height);

	tl = (tl + TEXEL_OFFSET) * tw;
	tr = (tr - TEXEL_OFFSET) * tw;
	tt = (tt + TEXEL_OFFSET) * th;
	tb = (tb - TEXEL_OFFSET) * th;
	
	float vtx[12];
	
	SetVtx(vtx, sx, sy, dx, dy);
	
	float crd[] = {
		tl, tt,
		tl, tb,
		tr, tt,
		tr, tb
	};

	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, crd);
	glVertexPointer(projection_mode ? 3 : 2, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
}

void SystemDraw::DrawImageOp(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, const Image& img, const Rect& src, Color color)
{
	GuiLock __;
	float tl = (float) src.left;
	float tr = (float) src.right;
	float tt = (float) src.top;
	float tb = (float) src.bottom;
	
	float sw = (float) src.GetWidth();
	float sh = (float) src.GetHeight();

	const TextureResource& t = resources.Bind(img, Resources::AUTO_ATLAS | Resources::NEAREST_FILTERING);

	float tw = 1.f / (float) t.realWidth;
	float th = 1.f / (float) t.realHeight;

	tl = (tl + t.x + TEXEL_OFFSET) * tw;
	tr = (tr + t.x - TEXEL_OFFSET) * tw;
	tt = (tt + t.y + TEXEL_OFFSET) * th;
	tb = (tb + t.y - TEXEL_OFFSET) * th;

	if(image_coloring)
	{
		if(IsNull(color))	
			glColor4ub(255, 255, 255, (int) alpha);
		else
		{
			#ifdef COLOR_ALPHA
			glColor4ub(color.GetR(), color.GetG(), color.GetB(), color.GetA());
			#else
			glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
			#endif
		}
	}
	
	glEnable(GL_TEXTURE_2D);

	float vtx[] = {
		x3, y3, z3,
		x0, y0, z0,
		x2, y2, z2,
		x1, y1, z1
	};

	float crd[] = {
		tl, tb,
		tl, tt,
		tr, tb,
		tr, tt
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, crd);
	glVertexPointer(3, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
}

void SystemDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	GuiLock __;
	#ifdef COLOR_ALPHA
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), color.GetA());
	#else
	glColor4ub(color.GetR(), color.GetG(), color.GetB(), (int) alpha);
	#endif
	glLineWidth((GLfloat) width);
	glBegin(GL_LINES);
		glVertex2i(x1 + drawing_offset.x, y1 + drawing_offset.y);
		glVertex2i(x2 + drawing_offset.x, y2 + drawing_offset.y);
	glEnd();
}

void SystemDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count, const int *counts, int count_count, int width, Color color, Color doxor)
{
	GuiLock __;
}

void SystemDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count, const int *subpolygon_counts, int scc, const int *disjunct_polygon_counts, int dpcc, Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	GuiLock __;
}

void SystemDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	GuiLock __;
}

void SystemDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	GuiLock __;
}

void SystemDraw::SaveCurrentColor()
{
	GuiLock __;
	glGetFloatv(GL_CURRENT_COLOR, current_color);
}

void SystemDraw::RestoreLastColor()
{
	GuiLock __;
	glColor4f(current_color[0], current_color[1], current_color[2], current_color[3]);
}

RectF SystemDraw::UnProject(const RectF& r, float& depth)
{
	GuiLock __;
	double projMat[16];
	double modelMat[16];
	int viewPort[4];
	
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	RectF fr;
	double x, y, z;
	gluUnProject(r.left, r.top, 0.99f, modelMat, projMat, viewPort, &x, &y, &z);
	depth = (float) z;
	fr.left = (float) x;
	fr.top = (float) y;
	gluUnProject(r.right, r.bottom, 0.99f, modelMat, projMat, viewPort, &x, &y, &z);
	fr.right = (float) x;
	fr.bottom = (float) y;
	return fr;
}

void SystemDraw::UnProject(float* vtx, float sx, float sy, float dx, float dy)
{
	GuiLock __;
	double projMat[16];
	double modelMat[16] = {
		1, 0, 0, 0, 
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	int viewPort[4];
	
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
	glGetIntegerv(GL_VIEWPORT, viewPort);
	
	double xs, ys, xd, yd, z;

	gluUnProject(sx, drawing_size.cy - sy, 0.99, modelMat, projMat, viewPort, &xs, &ys, &z);
	gluUnProject(dx, drawing_size.cy - dy, 0.99, modelMat, projMat, viewPort, &xd, &yd, &z);

	vtx[0] = (float) xs;
	vtx[1] = (float) yd;
	vtx[2] = (float) z;
	vtx[3] = (float) xs;
	vtx[4] = (float) ys;
	vtx[5] = (float) z;
	vtx[6] = (float) xd;
	vtx[7] = (float) yd;
	vtx[8] = (float) z;
	vtx[9] = (float) xd;
	vtx[10] = (float) ys;
	vtx[11] = (float) z;
}

void SystemDraw::UnProject(float& x, float& y, float &z)
{
	GuiLock __;
	double projMat[16];
	double modelMat[16] = {
		1, 0, 0, 0, 
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	int viewPort[4];
	
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	glGetIntegerv(GL_VIEWPORT, viewPort);
	
	double xd, yd, zd;

	gluUnProject(x, drawing_size.cy - y, 0.99, modelMat, projMat, viewPort, &xd, &yd, &zd);
	x = (float) xd;
	y = (float) yd;
	z = (float) zd;
}

void SystemDraw::SetVtx(float* vtx, float sx, float sy, float dx, float dy)
{
	if(projection_mode == 1)
	{
		UnProject(vtx, sx, sy, dx, dy);
	}
	else
	{
		vtx[0] = sx;
		vtx[1] = dy;
		vtx[2] = sx;
		vtx[3] = sy;
		vtx[4] = dx;
		vtx[5] = dy;
		vtx[6] = dx;
		vtx[7] = sy;
	}
}

void SystemDraw::SetProjectionMode(int mode)
{
	if(mode != projection_mode)
	{
		projection_mode = mode;
		if(projection_mode)
			PerspectiveView(false);
		else
			OrthogonalView(false);
	}
}

END_UPP_NAMESPACE

#endif