#include "GLDraw.h"

namespace Upp {

void DrawGL::MoveOp(const Pointf& p_, bool rel)
{
	if(path_done) {
		path.Clear();
		path_done = close_path = false;
		path_dash = clone(dash);
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
		path_done = close_path = false;
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
	close_path = true;
}

void DrawGL::StrokeOp(double width, const RGBA& color)
{
	if(width > 0 && color.a) {
		GLVertexData data;
		if(path_dash.GetCount()) {
			Vector<Vector<Pointf>> r;
			for(auto& l : path)
				DashPolyline(r, l, path_dash);
			for(const auto& p : r)
				Polyline(*this, p, width, color, dd.alpha, close_path);
		}
		else
			for(const auto& p : path)
				Polyline(*this, p, width, color, dd.alpha, close_path);
	}
	path_done = true;
}

void DrawGL::FillOp(const RGBA& color)
{
	if(color.a)
		DoDrawPolygons(path, color);
	CloseOp();
	path_done = true;
}

void DrawGL::DashOp(const Vector<double>& dash_, double start)
{
	(path.GetCount() ? path_dash : dash) = clone(dash_);
	dash_start = start;
}

};