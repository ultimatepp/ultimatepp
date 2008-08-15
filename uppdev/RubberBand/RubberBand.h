#ifndef _RubberBand_RubberBand_h
#define _RubberBand_RubberBand_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <RubberBand/RubberBand.lay>
#include <CtrlCore/lay.h>
class RubberBandClass : public LocalLoop
{
public:
	virtual void MouseMove(Point p, dword keyflags) 	{ points.Add(p); GetMaster().Refresh(); }
	virtual void LeftUp(Point p, dword keyflags)    	{ EndLoop(); }
	virtual void RightUp(Point p, dword keyflags)   	{ EndLoop(); }

	const Vector<Point> &	GetPoints()					{ return points; }
	void 					Clear()						{ points.Clear(); }
private:
	Vector<Point> points;
};

class RubberBand : public WithRubberBandLayout<TopWindow> {
public:
	virtual void RubberBand::LeftDown(Point p, dword keyflags);
	virtual void Paint(Draw& w);
	typedef RubberBand CLASSNAME;
	RubberBand();
	RubberBandClass band;	
};

#endif

