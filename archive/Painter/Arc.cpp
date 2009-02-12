#include "Painter.h"

NAMESPACE_UPP

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all coM_PIes. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// Arc generator. Produces at most 4 consecutive cubic bezier curves, i.e., 
// 4, 7, 10, or 13 vertices.
//
//----------------------------------------------------------------------------

// Recycled for U++ by Miroslav Fidler 2008

void sSubArc(double cx, double cy, double rx, double ry, 
             double start_angle, double sweep_angle,
             double *px, double *py)
{
	double x0 = cos(sweep_angle / 2.0);
	double y0 = sin(sweep_angle / 2.0);
	double tx = (1.0 - x0) * 4.0 / 3.0;
	double ty = y0 - tx * x0 / y0;
	double x[4], y[4];
	x[0] =  x0;
	y[0] = -y0;
	x[1] =  x0 + tx;
	y[1] = -ty;
	x[2] =  x0 + tx;
	y[2] =  ty;
	x[3] =  x0;
	y[3] =  y0;
	
	double sn = sin(start_angle + sweep_angle / 2.0);
	double cs = cos(start_angle + sweep_angle / 2.0);
	
	unsigned i;
	for(i = 0; i < 4; i++) {
		px[i] = cx + rx * (x[i] * cs - y[i] * sn);
		py[i] = cy + ry * (x[i] * sn + y[i] * cs);
	}
}

Painter& Painter::Arc(double x,  double y, double rx, double ry, 
                  double start_angle,  double sweep_angle, bool startline)
{
	const double bezier_arc_angle_epsilon = 0.01;
	start_angle = fmod(start_angle, 2.0 * M_PI);
	if(sweep_angle >=  2.0 * M_PI) sweep_angle =  2.0 * M_PI;
	if(sweep_angle <= -2.0 * M_PI) sweep_angle = -2.0 * M_PI;
	if(fabs(sweep_angle) < 1e-10) {
		double px = x + rx * cos(start_angle);
		double py = y + ry * sin(start_angle);
		if(startline)
			Line(px, py);
		else
			Move(px, py);
		Line(x + rx * cos(start_angle + sweep_angle), y + ry * sin(start_angle + sweep_angle));
		return *this;
	}
	double total_sweep = 0.0;
	double local_sweep = 0.0;
	double prev_sweep;
	bool done = false;
	bool first = true;
	for(int i = 0; !done && i < 4; i++) {
		if(sweep_angle < 0.0) {
			prev_sweep  = total_sweep;
			local_sweep = -M_PI * 0.5;
			total_sweep -= M_PI * 0.5;
			if(total_sweep <= sweep_angle + bezier_arc_angle_epsilon) {
			    local_sweep = sweep_angle - prev_sweep;
			    done = true;
			}
		}
		else {
			prev_sweep  = total_sweep;
			local_sweep =  M_PI * 0.5;
			total_sweep += M_PI * 0.5;
			if(total_sweep >= sweep_angle - bezier_arc_angle_epsilon) {
			    local_sweep = sweep_angle - prev_sweep;
	    		done = true;
			}
		}
		double px[4];
		double py[4];
		sSubArc(x, y, rx, ry, start_angle, local_sweep, px, py);
		if(first)
			if(startline)
				Line(px[0], py[0]);
			else
				Move(px[0], py[0]);
		first = false;
		Cubic(px[1], py[1], px[2], py[2], px[3], py[3]);
		start_angle += local_sweep;
	}
	return *this;
}

END_UPP_NAMESPACE
