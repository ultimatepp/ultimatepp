#include "Mandelbrot.h"

// Run this example in release mode (-O3)

force_inline RGBA GetFractalColor(int iter, int maxiter)
{
	float hue = (float) iter / maxiter;
	float val = (iter < maxiter) ? 1.0f : 0.0f;
	return HsvColorf(hue, 1.0f, val);
}

Mandelbrot::Mandelbrot()
{
	CtrlLayout(*this, "Mandelbrot Viewer");
#ifndef CPU_SIMD
	simd.Hide();
#endif
	for(int i : { 250, 500, 1000 })
		maxiter.Add(i);
	maxiter.SetIndex(1);
	auto cb = [this] { Update(); };
	render << [this] { Reset(); Update(); };
	maxiter << cb;
	simd << cb;
	render.WhenAction();
	view.IgnoreMouse();
}

void Mandelbrot::Reset()
{
	rendering = true;
	crect = { -2.0f, -1.5f, 1.0f, 1.5f };
	maxiter.SetIndex(1);
	simd = true;
	rendering = false;
}

void Mandelbrot::Update()
{
	if(rendering)
		return;

	rendering = true;
	TimeStop ts;
	Image img = ~simd ?	DrawSIMD(~maxiter)  : DrawScalar(~maxiter);
	timing.SetText(Format("Generated in %02.0f milliseconds...", ts.Elapsed() / 1000.0));
	view.SetImage(img);
	rendering = false;
}

Image Mandelbrot::DrawScalar(int itermax)
{
	const Size sz = view.GetSize();
	const float dx = crect.GetWidth() / sz.cx;
	const float dy = crect.GetHeight() / sz.cy;
	ImageBuffer ib(sz);
	RGBA* pix = ib;

	for(int py = 0; py < sz.cy; py++) {
		const float cy = crect.top + py * dy;
		for(int px = 0; px < sz.cx; px++) {
			float cx = crect.left + px * dx;
			float zx = 0.0f, zy = 0.0f;
			int iter = 0;
			for(int k = 0; k < itermax; k++) {
				float zx2 = zx * zx;
				float zy2 = zy * zy;
				if(zx2 + zy2 >= 4.0f)
					break;
				float zxy = zx * zy;
				zx = zx2 - zy2 + cx;
				zy = zxy + zxy + cy;
				iter++;
			}
			pix[py * sz.cx + px] = GetFractalColor(iter, itermax);
		}
		ProcessEvents();
	}

	return ib;
}

Image Mandelbrot::DrawSIMD(int itermax)
{
#ifdef CPU_SIMD
	const Size sz = view.GetSize();
	const float dx = crect.GetWidth() / sz.cx;
	const float dy = crect.GetHeight() / sz.cy;
	ImageBuffer ib(sz);
	RGBA* pix = ib;
	const int block = 4;

	const float offsets[block] = { 0.0f, dx, 2.0f * dx, 3.0f * dx };
	f32x4 xoffsets; xoffsets.Load(offsets);

	for(int py = 0; py < sz.cy; py++) {
		f32x4 cy4 = f32all(crect.top + py * dy);
		for(int px = 0; px < sz.cx; px += block) {
			f32x4 cx4 = f32all(crect.left + px * dx) + xoffsets;
			f32x4 zx = f32all(0.0f), zy = f32all(0.0f);
			i32x4 iter = i32all(0);
			for(int k = 0; k < itermax; k++) {
				f32x4 zx2 = zx * zx;
				f32x4 zy2 = zy * zy;
				f32x4 mag2 = zx2 + zy2;
				f32x4 mask = mag2 < f32all(4.0f);
				if(!AnyTrue(mask))
					break;
				f32x4 zxy = zx * zy;
				zx = zx2 - zy2 + cx4;
				zy = zxy + zxy + cy4;
				iter = IncrementIf(iter, mask);
			}
			alignas(16) int tmp[block];
			iter.Store(tmp);
			for(int i = 0; i < block; i++) {
				if(px + i >= sz.cx) break;
				pix[py * sz.cx + px + i] = GetFractalColor(tmp[i], itermax);
			}
		}
		ProcessEvents();
	}

	return ib;
#else
	return DrawScalar(itermax);
#endif
}

void Mandelbrot::LeftDown(Point pt, dword keyflags)
{
	if(!rendering) {
		panpos = pt;
		SetCapture();
	}
}

void Mandelbrot::LeftUp(Point pt, dword keyflags)
{
	if(panpos.x != -1) {
		panpos = Point(-1, -1);
		ReleaseCapture();
	}
}

void Mandelbrot::MouseMove(Point pt, dword keyflags)
{
	if(rendering || panpos.x == -1)
		return;
	
	const Size sz = view.GetSize();
	const Sizef csz = crect.GetSize();
    const double dx = (double)(pt.x - panpos.x) / sz.cx * csz.cx;
    const double dy = (double)(pt.y - panpos.y) / sz.cy * csz.cy;
    crect.Offset((float) -dx, (float) -dy);
	panpos = pt;
	Update();
}

void Mandelbrot::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	if(rendering)
		return;

	const Size sz = view.GetSize();
	const double scale = (zdelta > 0) ? 0.8 : 1.25;
	const Sizef csz = crect.GetSize();
	crect = Rectf(crect.TopLeft() + (Pointf(pt) / Sizef(sz)) * csz * (1.0 - scale), csz * scale);
	Update();
}

GUI_APP_MAIN
{
	Mandelbrot().Run();
}