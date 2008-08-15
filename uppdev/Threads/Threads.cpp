/***********************************************************************************************
	
	Ultimate++ Multi Threading example
	
	Copyright (c) 2007, Artur Bac
	 ______ ____           _____        __ _   
	|  ____|  _ \   /\    / ____|      / _| |  
	| |__  | |_) | /  \  | (___   ___ | |_| |_ 
	|  __| |  _ < / /\ \  \___ \ / _ \|  _| __|
	| |____| |_) / ____ \ ____) | (_) | | | |_ 
	|______|____/_/    \_\_____/ \___/|_|  \__|

	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

    	* Redistributions of source code must retain the above copyright notice, 
    	  this list of conditions and the following disclaimer.
    	* Redistributions in binary form must reproduce the above copyright notice, 
    	  this list of conditions and the following disclaimer in the documentation 
    	  and/or other materials provided with the distribution.
    	* Neither the name of the EBASoft nor the names of its contributors may be used 
    	  to endorse or promote products derived from this software without specific 
    	  prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
	CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************************/

#include "Threads.h"
#include <cmath>
#define TFILE <Threads/Threads.t>
#include <Core/t.h>

Threads::Threads()
	: threads(0)
	, area(-2.0,-1.1, 0.60, 1.1)
	, densityR (1)
	, densityC (1)
	, rows (1)
	, cols (1)
	, julia_c(Complex(-0.4,0.6))
	, posOnClick(0,0)
	, moveShift(0,0)
	, moving(false)
{
	SetLanguage(SetLNGCharset(GetSystemLNG(), CHARSET_UTF8));
	CtrlLayout(*this, t_("Threads"));
	inp_col.SetData(4);
	inp_row.SetData(3);
	inp_iter.SetData(500);
	inp_threads.SetData(3);
	edb_re.SetData(-0.4);
	edb_im.SetData(0.6);
	AddFrame(statusBar);
	
	statusBar.AddFrame(time.Right(140));
	
	BackPaint();
	Zoomable().Sizeable();
	picbox.onPaint = callback(this, &Threads::picbox_Paint);
	picbox.onLeftDown = callback(this, &Threads::picbox_LeftDown);
	picbox.onLeftUp = callback(this, &Threads::picbox_LeftUp);
	picbox.onMouseMove= callback(this, &Threads::picbox_MouseMove);
	
	cbx_fractal_set.Add( t_("Mandelbrot"));
	cbx_fractal_set.Add( t_("Qubic Mandelbrot"));
	cbx_fractal_set.Add( t_("Quadric Mandelbrot"));
	cbx_fractal_set.Add( t_("Julia Set"));
	cbx_fractal_set.Add( t_("Qubic Julia Set"));
	cbx_fractal_set.Add( t_("Quadric Julia Set"));
	cbx_fractal_set.Add( t_("Phoenix Set"));
	cbx_fractal_set.Add( t_("BacPhoenix Set"));
	cbx_fractal_set.Add( t_("Bać Set - my own :) discovered by mistake..."));
	
	cbx_fractal_set.WhenAction = THISBACK(SetSelected);
	btn_recalculate.WhenAction = THISBACK(DoJob);
	cbx_fractal_set.SetIndex(0);
}

void Threads::picbox_Paint(Upp::Draw & g )
{
	int i,r,c,w,h;

	if (picbox.GetSize().cx == 0 || picbox.GetSize().cy == 0)
		return;
	
	DrawingDraw d(cols,rows);
	d.DrawRect(0,0,rows,cols, White);
	
	INTERLOCKED_(job_lock) {
		for (i=0; i< active.GetCount(); i++)
		{
			r = active.GetKey(i) / cols;
			c = active.GetKey(i) % cols;
			d.DrawRect(c,r,1,1,Color(255,255,0));
		}
	}
	g.Offset(moveShift);
	g.DrawDrawing(0,0,densityC * cols, densityR * rows,d);
	
	INTERLOCKED_(image_lock) {
		for (i=0; i < rendered.GetCount(); i++)
		{
			r = rendered.GetKey(i) / cols;
			c = rendered.GetKey(i) % cols;
			w = picbox.GetSize().cx / cols;
			h = picbox.GetSize().cy / rows;
			g.DrawImage(c * w, r *h, w, h, rendered[i]);
		}
	}
	DrawAxis(g);
	g.End();
}



void Threads::ThreadRenderJob(int r, int c, const FractalJobHandeler & job, int)
{
	if (densityR == 0 || densityC==0) return;
	
	double UNITC = area.Width() / (densityC * cols);
	double UNITR = area.Height() / (densityR * rows);
	int iter, y0, x0;
	RGBA *l;
	
	ImageBuffer  ib(densityC, densityR);
	
	for (y0 = 0; y0< densityR; y0++)
	{
		l = ib[y0];
		for (x0 = 0; x0< densityC; x0++)
		{
			Complex tmp(area.left + UNITC *( c*densityC + x0), area.top + UNITR*( y0 + r*densityR));
			job(tmp, &iter);
			
			l->a = 0xFF;
			l->r = ( iter == maxIter ) ? 0  : 255 - (int)(250 * log10((double)iter) / logmax);//255 - iter * 120 / maxIter ;
			l->g = ( iter == maxIter ) ? 0  : 20 ;//iter * 125 / maxIter;
			l->b = ( iter == maxIter ) ? 0  : 40 + (int)(205 * log10((double)iter) / logmax) ;//120 + iter * 125 / maxIter ;
			l++;
		}
	}
	
	AtomicDec(threads);
	INTERLOCKED_(job_lock) 
		active.RemoveKey(r * cols + c);
	INTERLOCKED_(image_lock) 
		rendered.Add(r * cols + c, ib);
	waitforfinish.Release();
}

void Threads::DrawAxis(Upp::Draw & g)
{
	const int DENS = 1000;
	double re_step = area.Width() * 0.05;
	double im_step = area.Height() * 0.05;
	int side = 1;
	Complex re_value(area.left, area.top + area.Height()*0.5);
	Complex im_value(area.left + area.Width()*0.5, area.top);
	
	DrawingDraw axis(DENS<<1,DENS<<1);
	axis.DrawLine(0,DENS,DENS<<1,DENS,1,White);
	axis.DrawLine(DENS,0,DENS,DENS<<1,1,White);
	
	for (int i=0; i< (DENS<<1); i += DENS/10)
	{
		side *= -1;
		re_value.SetRe(re_value.Re() + re_step );
		im_value.SetIm(im_value.Im() + im_step );
		axis.DrawLine(i,DENS - DENS/50,i,DENS+DENS/50,1,White);
		axis.DrawLine(DENS - DENS/50,i,DENS+DENS/50,i,1,White);
		axis.DrawText(i,DENS + side * DENS/30, - side *900,ToString(re_value),Arial(12*(DENS<<1)/ picbox.GetSize().cy),White);
		axis.DrawText(DENS + DENS/30,i,ToString(im_value),Arial(12*(DENS<<1)/ picbox.GetSize().cx),White);
		
		for (int j=DENS/100; j< (DENS/10); j += DENS/100){
			axis.DrawLine(i + j,DENS - DENS/120,i +j,DENS+DENS/120,1,Color(0xAA,0xAA,0xAA));
			axis.DrawLine(DENS - DENS/120,i + j,DENS+DENS/120,i +j,1,Color(0xAA,0xAA,0xAA));
		}
	}
	g.DrawDrawing(0,0,picbox.GetSize().cx, picbox.GetSize().cy,axis);
}

String FormatTS (int ms) { return Format("Render time %d:%02.2d.%03d",(ms/1000/60), (ms / 1000) % 60, ms % 1000 );}

void Threads::Process(const FractalJobHandeler & job)
{
	ProgressInfo statTotal(statusBar);

	rows = inp_row.GetData();
	cols = inp_col.GetData();
	densityR = picbox.GetSize().cy / rows;
	densityC = picbox.GetSize().cx / cols;
	maxIter = inp_iter.GetData();
	logmax= log10((double)maxIter);
	max_threads = inp_threads.GetData();
	
	statTotal.Set(0,rows*cols);
	
	TimeStop startTime;
	
	for (int r = 0; r < rows; r++){
		for (int c = 0; c < cols; c++)
		{
			while(threads >= max_threads)
				waitforfinish.Wait();
			
			AtomicInc(threads);
			Thread().Run(callback4(this, &Threads::ThreadRenderJob, r, c, job, 0));
			
			INTERLOCKED_(job_lock)
				active.Add((r * cols + c), 0);
			
			statTotal.Text(Format("Invoking calculations for sector R:%02d  C:%02d",r,c));
			time.Set(FormatTS(startTime.Elapsed()));
			picbox.Refresh();
			statTotal.Set(r * cols + c,rows*cols);
			ProcessEvents();
		}
	}
	
	while(threads > 0)
		waitforfinish.Wait();

	time.Set(FormatTS(startTime.Elapsed()));
	picbox.Refresh();
}

void Threads::SetSelected()
{
	switch (cbx_fractal_set.GetIndex())
	{
		case 0:
			area = Rect_<double>(-2.0,-1.1, 0.60, 1.1);
			break;
		case 1:
			area = Rect_<double>(-1.3,-1.3, 1.3, 1.3);
			break;
		case 2:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			break;
		case 3:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			edb_re.SetData(-0.4);
			edb_im.SetData(0.6);
			break;
		case 4:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			break;
		case 5:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			break;
		case 6:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			edb_re.SetData(-0.4);
			edb_im.SetData(0.6);
			break;
		case 7:
			area = Rect_<double>(-1.5,-1.0, 1.5, 1.0);
			break;
		case 8:
			area = Rect_<double>(-1.25,-1.2, 1.75, 1.2);
			edb_re.SetData(-0.4);
			edb_im.SetData(0.5);
			break;
	}
	DoJob();
}


void Threads::DoJob()
{
	rendered.Clear();
	FractalJobHandeler job;
	
	switch (cbx_fractal_set.GetIndex())
	{
		case 0:
			job = FractalJobHandeler(callback(this,&Threads::FnMandelbrot));
			break;
		case 1:
			job = FractalJobHandeler(callback(this,&Threads::FnQubicMandelbrot));
			break;
		case 2:
			job = FractalJobHandeler(callback(this,&Threads::FnQuadricMandelbrot));
			break;
		case 3:
			job = FractalJobHandeler(callback(this,&Threads::FnJulia));
			julia_c = Complex(~edb_re, ~edb_im);
			break;
		case 4:
			job = FractalJobHandeler(callback(this,&Threads::FnQubicJulia));
			julia_c = Complex(~edb_re, ~edb_im);
			break;
		case 5:
			job = FractalJobHandeler(callback(this,&Threads::FnQuadricJulia));
			julia_c = Complex(~edb_re, ~edb_im);
			break;
		case 6:
			job = FractalJobHandeler(callback(this,&Threads::FnPhoenix));
			break;
		case 7:
			job = FractalJobHandeler(callback(this,&Threads::FnBacPhoenix));
			julia_c = Complex(~edb_re, ~edb_im);
			break;
		case 8:
			job = FractalJobHandeler(callback(this,&Threads::FnBac));
			julia_c = Complex(~edb_re, ~edb_im);
			break;
	}
	Process(job);
}

void Threads::FnMandelbrot(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp + cpx;
	*res = iter;
}
void Threads::FnQubicMandelbrot(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp * tmp + cpx;
	*res = iter;
}
void Threads::FnQuadricMandelbrot(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp * tmp * tmp + cpx;
	*res = iter;
}
void Threads::FnJulia(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp + julia_c;
	*res = iter;
}
void Threads::FnQubicJulia(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp * tmp + julia_c;
	*res = iter;
}
void Threads::FnQuadricJulia(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmp(cpx);
	for (iter = 0; tmp.Abs2() < 4  &&  iter < maxIter ;iter++)
		tmp = tmp * tmp * tmp * tmp + julia_c;
	*res = iter;
}
void Threads::FnPhoenix(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmpp;
	Complex tmp;
	Complex tmpn(cpx);
	for (iter = 0; tmpn.Abs2() < 4  &&  iter < maxIter ;iter++){
		tmp = tmpn;
		tmpn = tmp * tmp + julia_c.Re() + Complex(0,julia_c.Im()) * tmpp;
		tmpp = tmp;
	}
	*res = iter;
}
//My Own Sets made by mistake
void Threads::FnBacPhoenix(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmpp;
	Complex tmp;
	Complex tmpn(cpx);
	for (iter = 0; tmpn.Abs2() < 4  &&  iter < maxIter ;iter++){
		tmp = tmpn;
		tmpn = tmp * tmp + julia_c.Re() + julia_c.Im() + tmpp;
		tmpp = tmp;
	}
	*res = iter;
}

void Threads::FnBac(const Complex & cpx, int *res) const
{
	int iter =0;
	Complex tmpp;
	Complex tmp;
	Complex tmpn(cpx);
	for (iter = 0; tmpn.Abs2() < 4  &&  iter < maxIter ;iter++){
		tmp = tmpn;
		tmpn = tmp * tmp + tmpp.Re() + tmpp.Im() + julia_c;
		tmpp = tmp;
	}
	*res = iter;
}
void Threads::picbox_LeftDown(Point p, dword keyflags )
{
		if (keyflags & K_CTRL)
		{
			RectTracker tr(picbox);
			Rect s_xy = RectC(p.x, p.y, 0, 0);
			tr.Solid().Animation();
			s_xy = tr.Track(s_xy, ALIGN_RIGHT, ALIGN_BOTTOM);
			area = Rect_<double>( area.left + (double)s_xy.left * area.Width() / picbox.GetSize().cx
									, area.top + (double)s_xy.top * area.Height() / picbox.GetSize().cy
									, area.left + (double)s_xy.right * area.Width() / picbox.GetSize().cx 
									, area.top + (double)s_xy.bottom * area.Height() / picbox.GetSize().cy );
			DoJob();
		}
		else
			moving = true;
		
	posOnClick = p;
}


void Threads::picbox_LeftUp(Point p, dword keyflags )
{
	if (moving)
	{
		moveShift = p - posOnClick;
		double dx = (moveShift.cx* 1.0 / picbox.GetSize().cx) *area.Width();
		double dy = (moveShift.cy* 1.0 / picbox.GetSize().cy) *area.Height();
		area = Rect_<double>(area.left - dx, area.top - dy, area.right - dx,area.bottom - dy);
	}
	moving = false;
	moveShift = Size(0, 0);
	DoJob();
}

void Threads::picbox_MouseMove(Point p, dword keyflags )
{
	if (moving)
	{
		moveShift = p - posOnClick;
		picbox.Refresh();
	}
}




