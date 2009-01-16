#include "bench.h"

//http://cgit.freedesktop.org/cairo/plain/perf/pythagoras-tree.c
void AddRect(Cairo &c, double size)
{
	if (size < 1)
		return;
	
	Pointf p = c.GetCurrentPoint();
	
	c.RelMoveTo(-size/2., -size/2.);
	c.RelLineTo(size, 0);
	c.RelLineTo(0, size);
	c.RelLineTo(-size, 0);
	c.ClosePath();
	
	c.Save();
	c.Translate(-size/2., size);
	c.MoveTo(p.x, p.y);
	c.Rotate(M_PI/4.);
	AddRect(c, size / M_SQRT2);
	c.Restore();
	
	c.Save();
	c.Translate(size/2., size);
	c.MoveTo(p.x, p.y);
	c.Rotate(-M_PI/4.);
	AddRect(c, size / M_SQRT2);
	c.Restore();
	
	c.Fill();

}

void DoRect(Painter &sw, double size)
{
	if(size < 1)
		return;
	
	sw.Move(0, 0);
	sw.Line(0, size);
	sw.Line(size, size);
	sw.Line(size, 0);
	sw.Fill(Blue());
	
	sw.Begin();
	sw.Translate(0, size);
	sw.Rotate(M_PI/4.);
	DoRect(sw, size / M_SQRT2);
	sw.End();

	sw.Begin();
	sw.Translate(size / 2, 1.5 * size);
	sw.Rotate(-M_PI/4.);
	DoRect(sw, size / M_SQRT2);
	sw.End();
}

void DrawPythagorasTree(Size sz, Painter *sw, Cairo *ca)
{
	double size = 128;
	
	if (sw)
	{
		sw->Begin();
		sw->Translate(sz.cx / 2 - size / 2, sz.cy);
		sw->Scale(1, -1);
		
		DoRect(*sw, size);
		sw->End();
	}
	
	if (ca)
	{
		ca->Save();
		ca->Translate(0, sz.cy);
		ca->Scale(1, -1);
		
		ca->MoveTo(sz.cx/2, size/2);
		AddRect(*ca, size);
		ca->SetSourceRGB(Black());
		ca->Restore();
	}
}