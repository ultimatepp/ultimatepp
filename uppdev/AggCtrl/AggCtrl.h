#ifndef _AggCtrl_AggCtrl_h_
#define _AggCtrl_AggCtrl_h_

/*
This package implements Ctrl which allows user to use Agg software rendering engine in Upp
Autor: Jan Dolinár <dolik.rce@seznam.cz>
Usage: Just define onDraw method in your source and put in there the drawing stuff
       The Ctrl has pre-initialized rendering_buffer rbuf which is rendered in Paint(...) function
       You also can connect it to higher level Agg classes, or even draw other buffer 
       with PaintAgg(...)
Bugs:  Currently only X11 is supported
       Doesn't work properly when used pixelformat has different bpp then system (on my system
       only 32bit pixel formats work...)
*/

#include <CtrlLib/CtrlLib.h>
//#include <Core/Core.h>
#include <agg24/agg_rendering_buffer.h>
#include <agg24/agg_basics.h>
#include <agg24/agg_color_conv_rgb8.h>
#include "PixFmts.h"

#ifdef PLATFORM_X11
#include <X11/Xlib.h>
#elif defined(PLATFORM_WIN32)
#include <windows.h>
#include <string.h>
#include <agg24/agg_win32_bmp.h>
#include <agg24/agg_color_conv_rgb16.h>
#endif

class AggCtrl:public Upp::Ctrl{
public:
	typedef AggCtrl CLASSNAME;
	AggCtrl(agg::pix_format_e format=agg::pix_format_bgra32);
	~AggCtrl();
	virtual void Paint(Upp::Draw& draw);
	virtual void onDraw(Upp::Draw& dest);
	virtual void onInit();
	virtual void onResize(int width,int height);
	void PaintAgg(Upp::Draw& dest);
	void PaintAgg(const agg::rendering_buffer* src,Upp::Draw& dest);
	void Resized();
	void SetPixFmt(agg::pix_format_e format);
	agg::rendering_buffer rbuf;
	bool flip_y; 
private:
	unsigned bpp,sysbpp;
	agg::pix_format_e pixformat,sysformat;
	int cx,cy;
#ifdef PLATFORM_X11
public:
	unsigned char* buf;
private:
	XImage* ximg;
	int byte_order;
	unsigned long r_mask,g_mask,b_mask;
#elif defined(PLATFORM_WIN32)
	void CreatePmap(unsigned width,unsigned height,agg::rendering_buffer* wnd);
	agg::pixel_map pmap;
#endif
};


#endif
