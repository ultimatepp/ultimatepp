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
	x = fabs(x);
	return x < 1.5 ? 1 : 0;
}

double MagicKernel(double x)
{
	x = fabs(x);
	return x < 0.5 ? 0.75 - x * x :
	       x < 1.5 ? 0.5 * (x - 1.5) * (x - 1.5) :
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

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;
	
	DropList kernel;
	DropList method;
	Option   fast;
	
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtGray());
	//	w.DrawImage(0, 0, img);
	//	w.DrawImage(0, 100, Rescale2(img, 480, 84));
	
		Image (*rescale)(const Image& _img, int cx, int cy, double (*kernel)(double x), int a, int method);
		
		if(fast)
			rescale = RescaleWithKernel;
		else
			rescale = RescaleWithKernelE;
 
		double (*k)(double) = (double (*)(double))(int64)~kernel;
		int ka = k == Lanczos3 ? 3 : k == Lanczos4 ? 4 : k == Lanczos5 ? 5 : k == LinearKernel ? 1 : 2;

		for(int i = 0; i < TestImg().GetCount(); i++) {
			w.DrawImage(250 * i, 0, TestImg().Get(i));
			w.DrawImage(250 * i, 200, rescale(TestImg().Get(i), 84, 84, k, ka, ~method));
			w.DrawImage(250 * i, 300, rescale(TestImg().Get(i), 250, 250, k, ka, ~method));
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
		kernel.Add((int64)BiCubicKernel2, "BiCubic");
		kernel.Add((int64)MagicKernel, "Magic");
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
		method <<= DOWNSCALE_SIMPLE;

		Add(fast.TopPos(0, STDSIZE).RightPos(400, 200));
		fast <<= THISBACK(Sync);
		fast.SetLabel("Integer");
		fast <<= true;
		
		Maximize();
	}
};

GUI_APP_MAIN
{
	DUMP((int)Saturate255(5000));
	DUMP((int)Saturate255(500));
	DUMP((int)Saturate255(-1));
	DUMP((int)Saturate255(-10000));
	for(double x = -2; x <= 2; x += 0.1)
		LOG(x << ' ' << Lanczos2(x) << ' ' << MagicKernel(x) << ' ' << LinearKernel(x));

//	MyApp().Run();
#ifndef _DEBUG
	Image img = TestImg::img1();
	for(int i = 0; i < 100; i++) {
		{
			RTIMING("Rescale");
			Rescale(img, 84, 84);
		}
		{
			RTIMING("Rescale Linear integer");
			RescaleWithKernel(img, 84, 84, LinearKernel, 1);
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
}
