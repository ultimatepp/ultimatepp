#include "SDraw.h"

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
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

const double bezier_arc_angle_epsilon = 0.01;

void sSubArc(double cx, double cy, double rx, double ry, 
          double start_angle, double sweep_angle,
          double* curve)
{
	double x0 = cos(sweep_angle / 2.0);
	double y0 = sin(sweep_angle / 2.0);
	double tx = (1.0 - x0) * 4.0 / 3.0;
	double ty = y0 - tx * x0 / y0;
	double px[4];
	double py[4];
	px[0] =  x0;
	py[0] = -y0;
	px[1] =  x0 + tx;
	py[1] = -ty;
	px[2] =  x0 + tx;
	py[2] =  ty;
	px[3] =  x0;
	py[3] =  y0;
	
	double sn = sin(start_angle + sweep_angle / 2.0);
	double cs = cos(start_angle + sweep_angle / 2.0);
	
	unsigned i;
	for(i = 0; i < 4; i++) {
	curve[i * 2]     = cx + rx * (px[i] * cs - py[i] * sn);
	curve[i * 2 + 1] = cy + ry * (px[i] * sn + py[i] * cs);
	}
}

void Arc(SDraw& sw, double x,  double y, double rx, double ry, 
                    double start_angle,  double sweep_angle)
{
	start_angle = fmod(start_angle, 2.0 * M_PI);
	if(sweep_angle >=  2.0 * M_PI) sweep_angle =  2.0 * M_PI;
	if(sweep_angle <= -2.0 * M_PI) sweep_angle = -2.0 * M_PI;

	if(fabs(sweep_angle) < 1e-10) {
		sw.MoveTo(x + rx * cos(start_angle), 
		m_num_vertices = 4;
		m_cmd = path_cmd_line_to;
		m_vertices[0] = ;
		m_vertices[1] = y + ry * sin(start_angle);
		m_vertices[2] = x + rx * cos(start_angle + sweep_angle);
		m_vertices[3] = y + ry * sin(start_angle + sweep_angle);
		return;
	}

        double total_sweep = 0.0;
        double local_sweep = 0.0;
        double prev_sweep;
        m_num_vertices = 2;
        m_cmd = path_cmd_curve4;
        bool done = false;
        do
        {
            if(sweep_angle < 0.0)
            {
                prev_sweep  = total_sweep;
                local_sweep = -pi * 0.5;
                total_sweep -= pi * 0.5;
                if(total_sweep <= sweep_angle + bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }
            else
            {
                prev_sweep  = total_sweep;
                local_sweep =  pi * 0.5;
                total_sweep += pi * 0.5;
                if(total_sweep >= sweep_angle - bezier_arc_angle_epsilon)
                {
                    local_sweep = sweep_angle - prev_sweep;
                    done = true;
                }
            }

            arc_to_bezier(x, y, rx, ry, 
                          start_angle, 
                          local_sweep, 
                          m_vertices + m_num_vertices - 2);

            m_num_vertices += 6;
            start_angle += local_sweep;
        }
        while(!done && m_num_vertices < 26);
    }
