#ifndef _PointCtrl_PointCtrl_h
#define _PointCtrl_PointCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Gen/Gen.h>

typedef Pointf XY;

class PointCtrl : public Ctrl {
public:
	typedef PointCtrl CLASSNAME;
	PointCtrl();

	struct Style : public ChStyle<Style> {
		Color backcol;
		Color linecol;
		Color pointcol;
		int linesize;
		int pointsize;
	};

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftRepeat(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseEnter(Point p, dword keyflags) { xp = p; }
	virtual void MouseLeave() { xp.SetNull(); Refresh(); }
	
	virtual void Updated(); //from draw domain to value domain
	virtual void UpdatedBack(); //from value domain to draw domain
	virtual void Layout();

	static const Style& StyleDefault();
	void SetStyle(const Style& s) { style = &s; Refresh(); }

	virtual void SetData(const Value& v) { if(!v.Is<XY>()) return; vxp = v; UpdatedBack(); Refresh(); }
	virtual Value GetData() const { return vxp; }

	void SetDataX(double v) { vxp.x = minmax(v, vsx.Min(), vsx.Max()); UpdatedBack(); Refresh(); }
	void SetDataY(double v) { vxp.y = minmax(v, vsy.Min(), vsy.Max()); UpdatedBack(); Refresh(); }
	void SetDataXY(double vx, double vy) { vxp.x = minmax(vx, vsx.Min(), vsx.Max()); vxp.y = minmax(vy, vsy.Min(), vsy.Max()); UpdatedBack(); Refresh(); }
	double GetDataX() const { return vxp.x; }
	double GetDataY() const { return vxp.y; }
	
	void SetMinMaxX(double mn, double mx) { vsx.MinMax(mn, mx); vxp.x = minmax(vxp.x, vsx.Min(), vsx.Max()); UpdatedBack(); }
	void SetMinMaxY(double mn, double mx) { vsy.MinMax(mn, mx);  vxp.y = minmax(vxp.y, vsy.Min(), vsy.Max()); UpdatedBack(); }
	void SetMinMaxXY(double mnx, double mxx, double mny, double mxy) { vsx.MinMax(mnx, mxx); vsy.MinMax(mny, mxy); vxp.x = minmax(vxp.x, vsx.Min(), vsx.Max()); vxp.y = minmax(vxp.y, vsy.Min(), vsy.Max()); UpdatedBack(); }
	double GetMinX() const { return vsx.Min(); }
	double GetMaxX() const { return vsx.Max(); }
	double GetMinY() const { return vsy.Min(); }
	double GetMaxY() const { return vsy.Max(); }

	struct PData : Moveable<PData>
	{
		String name;
		Color col;
		int size;
		int shape;
		PData() : col(LtGreen()), size(4), shape(0) {}
	};

	Vector<XY> vp;
	Vector<PData> vpd;
	
	Callback WhenAddPoint;
	Callback1<int> WhenRemovePoint;
	Callback1<int> WhenMovePoint;
	
private:
	//cache helper
	Size sz;
	Point xp; //when moving
	Point sxp; //drawing representation of valx
	bool pressed;
	bool moving;
	int di; //dragging point index

	//vital
	XY vxp;
	Scaler<double> vsx, psx, vsy, psy;
	const Style* style;
};

#endif

