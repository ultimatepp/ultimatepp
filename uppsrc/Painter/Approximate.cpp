#include "Painter.h"

namespace Upp {

static void sQuadratic(LinearPathConsumer& t, const Pointf& p1, const Pointf& p2, const Pointf& p3,
                       double qt, int lvl)
{
	if(lvl < 16) {
		PAINTER_TIMING("Quadratic approximation");
		Pointf d = p3 - p1;
		double q = Squared(d);
		if(q > 1e-30) {
			Pointf pd = p2 - p1;
			double u = (pd.x * d.x + pd.y * d.y) / q;
			if(u <= 0 || u >= 1 || SquaredDistance(u * d, pd) > qt) {
				Pointf p12 = Mid(p1, p2);
				Pointf p23 = Mid(p2, p3);
				Pointf div = Mid(p12, p23);
				sQuadratic(t, p1, p12, div, qt, lvl + 1);
				sQuadratic(t, div, p23, p3, qt, lvl + 1);
				return;
			}
		}
	}
	t.Line(p3);
}

void ApproximateQuadratic(LinearPathConsumer& t, const Pointf& p1, const Pointf& p2, const Pointf& p3,
                          double tolerance)
{
	sQuadratic(t, p1, p2, p3, tolerance * tolerance, 0);
//	t.Line(p3);
}

static void sCubic(LinearPathConsumer& t,
                   const Pointf& p1, const Pointf& p2, const Pointf& p3, const Pointf& p4,
                   double qt, int lvl)
{
	if(lvl < 16) {
		PAINTER_TIMING("Cubic approximation");
		Pointf d = p4 - p1;
		double q = d.x * d.x + d.y * d.y;
		if(q >= 1e-30) {
			Pointf d2 = p2 - p1;
			Pointf d3 = p3 - p1;
			double u1 = (d2.x * d.x + d2.y * d.y) / q;
			double u2 = (d3.x * d.x + d3.y * d.y) / q;
			if(u1 <= 0 || u1 >= 1 || u2 <= 0 || u2 >= 1 ||
			   SquaredDistance(u1 * d, d2) > qt || SquaredDistance(u2 * d, d3) > qt) {
				Pointf p12 = Mid(p1, p2);
				Pointf p23 = Mid(p2, p3);
				Pointf p34 = Mid(p3, p4);
				Pointf p123 = Mid(p12, p23);
				Pointf p234 = Mid(p23, p34);
				Pointf div = Mid(p123, p234);
				sCubic(t, p1, p12, p123, div, qt, lvl + 1);
				sCubic(t, div, p234, p34, p4, qt, lvl + 1);
				return;
			}
		}
	}
	t.Line(p4);
}

void ApproximateCubic(LinearPathConsumer& t,
                      const Pointf& p1, const Pointf& p2, const Pointf& p3, const Pointf& p4,
                      double tolerance)
{
	sCubic(t, p1, p2, p3, p4, tolerance * tolerance, 0);
	t.Line(p4);
}

}
