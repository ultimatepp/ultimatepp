#include "GLDraw.h"

namespace Upp {

void DrawGL::MoveOp(const Pointf& p_, bool rel)
{
	if(path_done) {
		path.Clear();
		path_done = false;
	}
	Pointf p = p_;
	if(rel)
		p = prev + p;
	path.Add().Add(Off(p));
	prev = p;
}

void DrawGL::LineOp(const Pointf& p_, bool rel)
{
	if(path_done) {
		path.Clear();
		path_done = false;
	}
	if(path.GetCount() == 0)
		path.Add();
	Pointf p = p_;
	if(rel)
		p = prev + p;
	path.Top().Add(Off(p));
	prev = p;
}

void DrawGL::OpacityOp(double o)
{
	dd.alpha *= o;
}

void DrawGL::CloseOp()
{
	if(path.GetCount() && path.Top().GetCount())
		if(path.Top().Top() != path.Top()[0])
			path.Add(path[0]);
}

void DrawGL::StrokeOp(double width, const RGBA& rgba)
{
	if(width > 0) {
		GLVertexData data;
		if(dash.GetCount()) {
			Vector<Vector<Pointf>> r;
			for(auto& l : path)
				DashPolyline(r, l, dash);
			GLPolylines(data, r);
		}
		else
			GLPolylines(data, path);
		GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, rgba);
		path_done = true;
	}
}

void DrawGL::FillOp(const RGBA& color)
{
	GLVertexData data;
	GLPolygons(data, path);
	GLDrawPolygons(dd, Pointf(0, 0), data, Sizef(1, 1), color);
	CloseOp();
	path_done = true;
}

void DrawGL::DashOp(const Vector<double>& dash_, double start)
{
	dash = clone(dash_);
	dash_start = start;
}

};