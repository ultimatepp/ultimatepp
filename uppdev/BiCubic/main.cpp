#include "bicubic.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <BiCubic/image.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <BiCubic/image.iml>
#include <Draw/iml_source.h>

double BiCubicKernel2(double x)
{
	double r = 0;

	double xp2 = x + 2;
	double xp1 = x + 1;
	double xm1 = x - 1;

	if(xp2 > 0)
		r += xp2 * xp2 * xp2;
	if(xp1 > 0)
		r -= 4 * xp1 * xp1 * xp1;
	if(x > 0)
		r += 6 * x * x * x;
	if(xm1 > 0)
		r -= 4 * xm1 * xm1 * xm1;

	return (1 / 6.0) * r;
}

double BiCubic(double x, double a)
{
	x = fabs(x);
	return x <= 1 ? (a + 2) * x * x * x - (a + 3) * x * x + 1 :
	       x <= 2 ? a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a :
	       0;
}

double BiCubic0(double x)
{
	return BiCubic(x, 0);
}

double BiCubic1(double x)
{
	return BiCubic(x, -0.25);
}

double BiCubic2(double x)
{
	return BiCubic(x, -0.5);
}

double BiCubic3(double x)
{
	return BiCubic(x, -1.0);
}

double BiCubic4(double x)
{
	return BiCubic(x, -1.5);
}

double BiCubic_(double x, double B, double C)
{
	x = fabs(x);
	double x2 = x * x;
	double x3 = x * x * x;
	return
		1 / 6.0 * (x < 1 ? (12 - 9*B - 6*C) * x3 + (-18 + 12*B + 6*C) * x2 + (6 - 2*B) :
		           x < 2 ? (-B - 6*C) * x3 + (6*B + 30*C) * x2 + (-12*B - 48*C) *x + (8*B + 24*C) :
	    	       0);
}

double BiCubic_Bspline(double x)
{
	return BiCubic_(x, 1, 0);
}

double BiCubic_Mitchell(double x)
{
	return BiCubic_(x, 1 / 3.0, 1 / 3.0);
}

double BiCubic_CatmullRom(double x)
{
	return BiCubic_(x, 0, 1 / 2);
}

double Nearest(double x)
{
	return x >= -0.5 && x <= 0.5;
}

double LinearKernel(double x)
{
	x = fabs(x);
	if(x > 1)
		return 0;
	return 1 - fabs(x);
}

double SimpleKernel(double x)
{
	return 1;
}

double MagicKernel(double x)
{
	x = fabs(x);
	return x < 0.5 ? 0.75 - x * x :
	       x < 1.5 ? 0.5 * (x - 1.5) * (x - 1.5) :
	       0;
}

double BSpline(double x)
{
	x = fabs(x);
	return x <= 1 ? (x * x * x) / 2 - x * x + 2.0 / 3 :
	       x <= 2 ? -(x * x * x) / 6 + x * x - 2 * x + 4.0 / 3 :
	       0;
}

double Lanczos(double x, int a)
{
	x = fabs(x);
	if(x < 1e-200)
		return 1;
	if(x >= a)
		return 0;
	return a * sin(M_PI * x) * sin(M_PI * x / a) / (M_PI * M_PI * x * x);
}

double Lanczos2(double x)
{
	return Lanczos(x, 2);
}

double Lanczos3(double x)
{
	return Lanczos(x, 3);
}

double Lanczos4(double x)
{
	return Lanczos(x, 4);
}

double Lanczos5(double x)
{
	return Lanczos(x, 5);
}

Image RescaleWithKernel2(const Image& img, int cx, int cy, double (*kernel)(double x), int a, int method = DOWNSCALE_WIDE)
{
	return RescaleFilter(img, Size(cx, cy), kernel, a);
}

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;
	
	DropList kernel;
	DropList method;
	DropList fn;
	
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtGray());
	//	w.DrawImage(0, 0, img);
	//	w.DrawImage(0, 100, Rescale2(img, 480, 84));
	
		Image (*rescale)(const Image& _img, int cx, int cy, double (*kernel)(double x), int a, int method);
		
		if(~fn == 0)
			rescale = RescaleWithKernelE;
		else
		if(~fn == 1)
			rescale = RescaleWithKernel;
		else
			rescale = RescaleWithKernel2;
 
		double (*k)(double) = (double (*)(double))(int64)~kernel;
		int ka = k == Lanczos3 ? 3 : k == Lanczos4 ? 4 : k == Lanczos5 ? 5 : k == LinearKernel ? 1 : 2;

		TimeStop tm;
		
		for(int i = 0; i < TestImg().GetCount(); i++) {
			Image img = TestImg().Get(i);
			w.DrawImage(250 * i, 0, img);
			w.DrawImage(250 * i, 200, rescale(img, 84, 84, k, ka, ~method));
			w.DrawImage(250 * i + 84, 200, rescale(img, 42, 42, k, ka, ~method));
			w.DrawImage(250 * i + 84 + 52, 200, rescale(img, 21, 21, k, ka, ~method));
			w.DrawImage(250 * i + 84 + 52 + 31, 200, rescale(img, 10, 10, k, ka, ~method));
			w.DrawImage(250 * i + 84 + 52 + 31 + 20, 200, rescale(img, 5, 5, k, ka, ~method));
			w.DrawImage(250 * i + 84 + 52 + 31 + 20 + 20, 200, rescale(img, 1, 1, k, ka, ~method));
			w.DrawImage(250 * i, 300, rescale(img, 250, 250, k, ka, ~method));
			w.DrawImage(250 * i, 550, RescaleFilter(img, Size(250, 250), RectC(10, 10, 100, 100), k, ka));
		}
		
		w.DrawImage(GetSize().cx - 400, 100, rescale(TestImg::img2(), 400, 400, k, ka, ~method));
		w.DrawImage(GetSize().cx - 400, 500, rescale(TestImg::img3(), 400, 400, k, ka, ~method));
		
		w.DrawText(GetSize().cx - 200, 40, String().Cat() << "Time " << tm);

		for(int i = 0; i < TestImg().GetCount(); i++) {
			w.DrawImage(250 * i, GetSize().cy - 84, Rescale(TestImg().Get(i), 84, 84));
		}

/*
		w.DrawImage(0, 0, RescaleBicubic2(rings, 180, 180, k, ka, expand));
		w.DrawImage(200, 0, Rescale(rings, 180, 180));
		
		Size n = rings.GetSize() / 180;
		Image dr = DownScale(rings, n.cx, n.cy);
		w.DrawImage(400, 0, dr);
		w.DrawImage(480, 200, RescaleBicubic2(dr, 180, 180, k, ka, expand));
*/		

		
/*
		w.DrawImage(0, 200, RescaleBicubic2(img, 480, 84, k, ka, ~method));

		int y = 200;
		int x = 200;
		for(int i = 7; i >= 1; i -= 2) {
			Size sz = img.GetSize() * i / 10;
			y += sz.cy;
			w.DrawImage(500 + x, 100, RescaleBicubic2(CtrlImg::copy(), 20 * i, 20 * i, k, ka));
			x += 20 * i;
			sz = photo.GetSize() * i / 3;
			w.DrawImage(500 + x, 0, RescaleBicubic2(photo, sz.cx, sz.cy, k, ka));
			x += 20 * i;
		}
*/		
		


/*
		Image dm = DownScale(img, 2, 2);
		w.DrawImage(0, 800, dm);
		w.DrawImage(0, 900, RescaleBicubic2(dm, 480, 84, k, ka));
		w.DrawImage(480, 900, Rescale(img, 480, 84));
*/
//		w.DrawImage(0, 300, RescaleBicubic(img, Size(480, 84), Rect(img.GetSize()).Deflated(20)));
	//	w.DrawImage(0, 300, RescaleBicubic3(img, 480, 84));
	//	w.DrawImage(0, 400, RescaleBicubic4(img, 480, 84));
//		for(int i = 1; i < 5; i++)
//			w.DrawImage(0, 200 + 100 * i, RescaleBicubic(img, 480 / (i + 3), 84 / (i + 3)));
	//	w.DrawImage(0, 400, RescaleBicubic3(img, 480, 84));
	}
	
	void Sync()
	{
		Refresh();
	}

	MyApp() {
		SetRect(0, 0, 600, 800);
		Sizeable();
		kernel.Add((int64)Nearest, "Nearest");
		kernel.Add((int64)SimpleKernel, "Simple");
		kernel.Add((int64)LinearKernel, "Linear");
		kernel.Add((int64)BiCubicKernel2, "BiCubic old");
		kernel.Add((int64)BiCubic0, "BiCubic0");
		kernel.Add((int64)BiCubic1, "BiCubic1");
		kernel.Add((int64)BiCubic2, "BiCubic2");
		kernel.Add((int64)BiCubic3, "BiCubic3");
		kernel.Add((int64)BiCubic4, "BiCubic4");
		kernel.Add((int64)MagicKernel, "Magic");
		kernel.Add((int64)BSpline, "BSpline");

		kernel.Add((int64)BiCubic_Bspline, "BiCubic_Bspline");
		kernel.Add((int64)BiCubic_Mitchell, "BiCubic_Mitchell");
		kernel.Add((int64)BiCubic_CatmullRom, "BiCubic_CatmullRom");

		kernel.Add((int64)Lanczos2, "Lanczos2");
		kernel.Add((int64)Lanczos3, "Lanczos3");
		kernel.Add((int64)Lanczos4, "Lanczos4");
		kernel.Add((int64)Lanczos5, "Lanczos5");
		kernel <<= (int64)Lanczos2;
		kernel <<= THISBACK(Sync);
		Add(kernel.TopPos(0, STDSIZE).RightPos(0, 200));

		method.Add(DOWNSCALE_SIMPLE, "Simple");
		method.Add(DOWNSCALE_MIPMAP, "MipMap");
		method.Add(DOWNSCALE_WIDE, "Wide");
		method <<= THISBACK(Sync);
		Add(method.TopPos(0, STDSIZE).RightPos(200, 200));
		method <<= DOWNSCALE_WIDE;

		fn.Add(0, "FP");
		fn.Add(1, "Integer");
		fn.Add(2, "Optimized");
		Add(fn.TopPos(0, STDSIZE).RightPos(400, 200));
		fn <<= THISBACK(Sync);
		fn <<= 2;
		
		Zoomable();
		Maximize();
	}
};

GUI_APP_MAIN
{
	Image img = TestImg::img1();
	RescaleWithKernel2(img, 5, 5, LinearKernel, 1);

	DUMP((int)Saturate255(5000));
	DUMP((int)Saturate255(500));
	DUMP((int)Saturate255(-1));
	DUMP((int)Saturate255(-10000));
	for(double x = -2; x <= 2; x += 0.1)
		RLOG(x << ' ' << Lanczos2(x) << ' ' << MagicKernel(x) << ' ' << BiCubic2(x));

	MyApp().Run();
#ifndef _DEBUG
#if 1
	for(int i = 0; i < 100; i++) {
/*		{
			RTIMING("Rescale");
			Rescale(img, 84, 84);
		}
		{
			RTIMING("Rescale Linear integer");
			RescaleWithKernel(img, 84, 84, LinearKernel, 1);
		}
		{
			RTIMING("Rescale Linear integer optimized");
			RescaleWithKernel2(img, 84, 84, LinearKernel, 1);
		}
*/		{
			RTIMING("Rescale UP");
			Rescale(img, 300, 300);
		}
		{
			RTIMING("Rescale Linear integer optimized UP");
			RescaleWithKernel2(img, 300, 300, LinearKernel, 1);
		}
	#if 0
		{
			RTIMING("Rescale Linear double");
			RescaleWithKernelE(img, 84, 84, LinearKernel, 1);
		}
		{
			RTIMING("Rescale Bicubic double");
			RescaleWithKernelE(img, 84, 84, BiCubicKernel2, 2);
		}
		{
			RTIMING("Rescale Bicubic integer");
			RescaleWithKernel(img, 84, 84, BiCubicKernel2, 2);
		}
		{
			RTIMING("Rescale Lanczos2 double");
			RescaleWithKernelE(img, 84, 84, Lanczos2, 2);
		}
		{
			RTIMING("Rescale Lanczos2 integer");
			RescaleWithKernel(img, 84, 84, Lanczos2, 2);
		}
		{
			RTIMING("Rescale Lanczos3 double");
			RescaleWithKernelE(img, 84, 84, Lanczos3, 3);
		}
		{
			RTIMING("Rescale Lanczos3 integer");
			RescaleWithKernel(img, 84, 84, Lanczos3, 3);
		}
	#endif
	}
#endif
#endif
}
