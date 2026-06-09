#ifndef _Mandelbrot_Mandelbrot_h
#define _Mandelbrot_Mandelbrot_h

// Run this example in release mode (-O3)

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Mandelbrot/Mandelbrot.lay>
#include <CtrlCore/lay.h>

class Mandelbrot : public WithMainLayout<TopWindow> {
public:
	Mandelbrot();
	void Reset();
	void LeftDown(Point pt, dword keyflags) override;
	void LeftUp(Point pt, dword keyflags) override;
	void MouseMove(Point pt, dword keyflags) override;
	void MouseWheel(Point pt, int zdelta, dword keyflags) override;
	
private:
	Image  DrawScalar(int itermax);
	Image  DrawSIMD(int itermax);
	void   Update();
	Rectf  crect = { -2.0f, -1.5f, 1.0f, 1.5f };
	bool   rendering = false;
	Point  panpos = { -1, -1 };
};

#endif
