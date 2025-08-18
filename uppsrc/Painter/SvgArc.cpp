#include "Painter.h"

namespace Upp {

void Painter::DoArc0(double theta, double th_sweep, const Xform2D& m, double scale)
{
	int nsegs = int(ceil(fabs(th_sweep / (M_PI * 0.5 + 0.001))));
	for(int i = 0; i < nsegs; i++) {
		double th0 = theta + i * th_sweep / nsegs;
		double th1 = theta + (i + 1) * th_sweep / nsegs;
		double thHalf = 0.5 * (th1 - th0);
		double t = (8.0 / 3.0) * sin(thHalf * 0.5) * sin(thHalf * 0.5) / sin(thHalf);
		double x3 = cos(th1);
		double y3 = sin(th1);
		Cubic(m.Transform(cos(th0) - t * sin(th0), sin(th0) + t * cos(th0)),
		      m.Transform(x3 + t * sin(th1), y3 - t * cos(th1)),
		      m.Transform(x3, y3));
	}
}

void Painter::DoArc(const Pointf& c, const Pointf& r, double angle, double sweep, double xangle, double scale)
{
	Xform2D m = Xform2D::Scale(r.x, r.y);
	m = m * Xform2D::Translation(c.x, c.y);
	Line(m.Transform(cos(angle), sin(angle)));
	DoArc0(angle, sweep, m, scale);
}

void Painter::DoSvgArc(const Pointf& rr, double xangle, int large, int sweep,
                       const Pointf& p1, const Pointf& p0, double scale)
{
	Pointf r(fabs(rr.x), fabs(rr.y));
	Xform2D m = Xform2D::Rotation(-xangle);
	Pointf d1 = m.Transform(0.5 * (p0 - p1));
	Pointf pr = r * r;
	Pointf p = d1 * d1;
	double check = p.x / pr.x + p.y / pr.y;
	if(check > 1)
		r *= sqrt(check);
	m.x /= r.x;
	m.y /= r.y;
	Pointf q0 = m.Transform(p0);
	Pointf q1 = m.Transform(p1);
	double d = SquaredDistance(q0, q1);
	double sfactor_sq = 1.0 / d - 0.25;
	if(sfactor_sq < 0)
		sfactor_sq = 0;
	double sfactor = sqrt(sfactor_sq);
	if(sweep == large)
		sfactor = -sfactor;
	Pointf c(0.5 * (q0.x + q1.x) - sfactor * (q1.y - q0.y),
	         0.5 * (q0.y + q1.y) + sfactor * (q1.x - q0.x));
	double theta = Bearing(q0 - c);
	double th_sweep = Bearing(q1 - c) -  theta;
	if(th_sweep < 0 && sweep)
		th_sweep += 2 * M_PI;
	else
	if(th_sweep > 0 && !sweep)
		th_sweep -= 2 * M_PI;
	m = Xform2D::Rotation(xangle);
	m.x *= r;
	m.y *= r;
	m = Xform2D::Translation(c.x, c.y) * m;
	DoArc0(theta, th_sweep, m, scale);
}

}
