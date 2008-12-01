#ifndef _aggtest_aggtest_h
#define _aggtest_aggtest_h

// Original sourcecode from Maxim Shemanarev, 2002-2006
// For Upp rewritten by Jan Dolinár dolik.rce@seznam.cz

#include <CtrlLib/CtrlLib.h>
#include <AggCtrl/AggCtrl.h>
#include <agg24/agg24.h>

using namespace Upp;

class AggTestCtrl:public AggCtrl{
public:
	AggTestCtrl();
	virtual void onDraw(Upp::Draw& dest);
	virtual void onInit();
	virtual void onResize(int width,int height);
	void parse_lion();
	agg::pixfmt_bgr24 pixf;
	agg::renderer_base<agg::pixfmt_bgr24> rb;
	agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgr24> > r;
	agg::scanline_u8 sl;
	agg::path_storage path;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::rgba8        colors[100];
	unsigned          path_idx[100];
	unsigned          npaths;
	double            x1,y1,x2,y2;
	double            base_dx,base_dy,scale;
};

#define LAYOUTFILE <aggtest/aggtest.lay>
#include <CtrlCore/lay.h>

class aggtest : public WithaggtestLayout<TopWindow>{
public:
	typedef aggtest CLASSNAME;
	aggtest();
};

#endif

