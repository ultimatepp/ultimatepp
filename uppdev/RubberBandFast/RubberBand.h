#ifndef _RubberBand_RubberBand_h
#define _RubberBand_RubberBand_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <RubberBandFast/RubberBand.lay>
#include <CtrlCore/lay.h>

class RubberBandClass : public LocalLoop
{
public:
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags)    	{ EndLoop(); }
	virtual void RightUp(Point p, dword keyflags)   	{ EndLoop(); }

	const Vector<Point> &	GetPoints()					{ return points; }
	const Drawing GetImage () 							{ return resultpaint; }
	void Clear()										{ points.Clear(); resultpaint.Clear(); resultpaint = DrawingDraw(0, 0);}
	RubberBandClass () {resultpaint = DrawingDraw(0, 0);}
private:
	Vector<Point> points;
	Drawing resultpaint;
};

class RubberBand : public WithRubberBandLayout<TopWindow> {
public:
	virtual void LeftDown(Point p, dword keyflags);
	virtual void Paint(Draw& w);

	typedef RubberBand CLASSNAME;
	RubberBand();
	RubberBandClass band;	
};

#endif
