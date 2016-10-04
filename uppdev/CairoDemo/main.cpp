
#include <CtrlLib/CtrlLib.h>
#include <plugin/cairo/CairoCtrl.h>
using namespace Upp;

#define LAYOUTFILE <CairoDemo/CairoDemo.lay>
#include <CtrlCore/lay.h>

//http://cgit.freedesktop.org/cairo/plain/perf/pythagoras-tree.c
static void	add_rectangle (Cairo &c, double size)
{
	if (size < 2)
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
	c.Rotate(M_PI/4);
	add_rectangle(c, size / M_SQRT2);
	c.Restore();
	
	c.Save();
	c.Translate(size/2., size);
	c.MoveTo(p.x, p.y);
	c.Rotate(-M_PI/4);
	add_rectangle(c, size / M_SQRT2);
	c.Restore();
}

class CairoDemo : public WithCairoDemoLayout<TopWindow> 
{
	typedef CairoDemo CLASSNAME;
	
public:
	CairoDemo()
	{
		CtrlLayout(*this, "Window title");
		_cairo.WhenCairoPaint = THISBACK(CairoPaint);
		//_cairo.SetDoubleBuffering(false);
	}
	
protected:
	void CairoPaint(Cairo &c)
	{
		c.Clear(White());
		
		CairoRadialGradient gradient(c.GetWidth() / 2.0, c.GetHeight() / 2.0, 0, 
		                             c.GetWidth() / 2.0, c.GetHeight() / 2.0, 500);
		//CairoLinearGradient gradient(0.0, 0.0, c.GetWidth(), c.GetHeight());
		gradient.AddColorStopRGB(0.0, Red());
		gradient.AddColorStopRGB(0.3, White());
		gradient.AddColorStopRGB(0.6, White());
		gradient.AddColorStopRGB(1.0, Blue());
		c.SetSource(gradient);
		c.Rectangle(0, 0, c.GetWidth(), c.GetHeight());
		c.Fill();
		
		double size = 128;
		
		c.Save();
		c.Translate(0, c.GetHeight());
		c.Scale(1, -1);
		
		c.MoveTo(c.GetWidth()/2, size/2);
		add_rectangle(c, size);
		c.SetSourceRGB(Black());
		c.Fill();
		c.Restore();
	}
};

GUI_APP_MAIN
{
	CairoDemo().Run();
}

