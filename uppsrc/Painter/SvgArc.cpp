#include "Painter.h"

NAMESPACE_UPP

void Painter::DoArc(const Pointf& c, const Pointf& r, double angle, double sweep, double xangle)
{
	int n_segs = int(ceil(fabs(sweep / (M_PI * 0.5 + 0.001))));
	for (int i = 0; i < n_segs; i++) {
		double th0 = angle + i * sweep / n_segs;
		double th1 = angle + (i + 1) * sweep / n_segs;
		double thHalf = 0.5 * (th1 - th0);
		double t = (8.0 / 3.0) * sin(thHalf * 0.5) * sin(thHalf * 0.5) / sin(thHalf);
		double x3 = c.x + cos(th1);
		double y3 = c.y + sin(th1);
		Xform2D m = Xform2D::Rotation(xangle);
		m.x *= r;
		m.y *= r;
		Cubic(m.Transform(c.x + cos(th0) - t * sin(th0), c.y + sin(th0) + t * cos(th0)),
		      m.Transform(x3 + t * sin(th1), y3 - t * cos(th1)),
		      m.Transform(x3, y3));
	}
}

// the arc handling code underneath is from XSVG (BSD license)
/*
 * Copyright  2002 USC/Information Sciences Institute
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Information Sciences Institute not be used in advertising or
 * publicity pertaining to distribution of the software without
 * specific, written prior permission.  Information Sciences Institute
 * makes no representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * INFORMATION SCIENCES INSTITUTE DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL INFORMATION SCIENCES
 * INSTITUTE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

void Painter::DoSvgArc(const Pointf& rr, double xangle, int large, int sweep,
                       const Pointf& p1, const Pointf& p0)
{
	Pointf r(fabs(rr.x), fabs(rr.y));

	Xform2D m = Xform2D::Rotation(-xangle);
	Pointf d1 = m.Transform((p0 - p1) / 2);
	Pointf Pr = r * r; 
	Pointf P = d1 * d1;
	double check = P.x / Pr.x + P.y / Pr.y;
	if(check > 1)
		r *= sqrt(check);
	m.x /= r.x;
	m.y /= r.y;
	Pointf q0 = m.Transform(p0);
	Pointf q1 = m.Transform(p1);
	double d = SquaredDistance(q1, q0);
	double sfactor_sq = 1.0 / d - 0.25;
	if(sfactor_sq < 0) sfactor_sq = 0;
	double sfactor = sqrt(sfactor_sq);
	if (sweep == large) sfactor = -sfactor;
	Pointf c(0.5 * (q0.x + q1.x) - sfactor * (q1.y - q0.y),
	         0.5 * (q0.y + q1.y) + sfactor * (q1.x - q0.x));
	double th0 = Bearing(q0 - c);
	double th_sweep = Bearing(q1 - c) -  th0;
	if(th_sweep < 0 && sweep)
		th_sweep += 2 * M_PI;
	else
	if(th_sweep > 0 && !sweep)
		th_sweep -= 2 * M_PI;

	DoArc(c, r, th0, th_sweep, xangle);
}

#if 0
void Painter::DoSvgArc(double rx, double ry,
                       double x_axis_rotation, int large_arc_flag, int sweep_flag,
                       double x, double y, double curx, double cury)
{
    double sin_th, cos_th;
    double a00, a01, a10, a11;
    double x0, y0, x1, y1, xc, yc;
    double d, sfactor, sfactor_sq;
    double th0, th1, th_arc;
    int i, n_segs;
    double dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;

    rx = fabs(rx);
    ry = fabs(ry);

    sin_th = sin(x_axis_rotation);
    cos_th = cos(x_axis_rotation);

    dx = (curx - x) / 2.0;
    dy = (cury - y) / 2.0;
    dx1 =  cos_th * dx + sin_th * dy;
    dy1 = -sin_th * dx + cos_th * dy;
    Pr1 = rx * rx;
    Pr2 = ry * ry;
    Px = dx1 * dx1;
    Py = dy1 * dy1;
    /* Spec : check if radii are large enough */
    check = Px / Pr1 + Py / Pr2;
    if(check > 1) {
        rx = rx * sqrt(check);
        ry = ry * sqrt(check);
    }

    a00 =  cos_th / rx;
    a01 =  sin_th / rx;
    a10 = -sin_th / ry;
    a11 =  cos_th / ry;
    x0 = a00 * curx + a01 * cury;
    y0 = a10 * curx + a11 * cury;
    x1 = a00 * x + a01 * y;
    y1 = a10 * x + a11 * y;
    /* (x0, y0) is current point in transformed coordinate space.
       (x1, y1) is new point in transformed coordinate space.

       The arc fits a unit-radius circle in this space.
    */
    d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
    sfactor_sq = 1.0 / d - 0.25;
    if (sfactor_sq < 0) sfactor_sq = 0;
    sfactor = sqrt(sfactor_sq);
    if (sweep_flag == large_arc_flag) sfactor = -sfactor;
    xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
    yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);
    /* (xc, yc) is center of the circle. */

    th0 = atan2(y0 - yc, x0 - xc);
    th1 = atan2(y1 - yc, x1 - xc);

    th_arc = th1 - th0;
    if (th_arc < 0 && sweep_flag)
        th_arc += 2 * M_PI;
    else if (th_arc > 0 && !sweep_flag)
        th_arc -= 2 * M_PI;
    
    n_segs = int(ceil(fabs(th_arc / (M_PI * 0.5 + 0.001))));

    for (i = 0; i < n_segs; i++)
        ArcSegment(xc, yc,
                    th0 + i * th_arc / n_segs,
                    th0 + (i + 1) * th_arc / n_segs,
                    rx, ry, x_axis_rotation);
}
#endif

END_UPP_NAMESPACE
