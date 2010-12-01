#ifndef _PlotCtrl_PlotCtrl_h
#define _PlotCtrl_PlotCtrl_h

#include <CtrlLib/CtrlLib.h>
#include <PlotLib/PlotLib.h>

#define  LAYOUTFILE <PlotCtrl/PlotCtrl.lay>
#include <CtrlCore/lay.h>

using namespace Upp;

class PlotCtrl : public Ctrl, public Plot{
public:
	typedef PlotCtrl CLASSNAME;
	PlotCtrl();
	PlotCtrl& Zoom(double rx,double ry,Pointf C=Null);
	PlotCtrl& ZoomIn(double ratio=2,const Pointf& C=Null);
	PlotCtrl& ZoomOut(double ratio=2,const Pointf& C=Null);
	PlotCtrl& ZoomAll(bool visibleonly=false);
	virtual void Layout();
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point pt,dword keyflags);
	virtual void LeftUp(Point pt,dword keyflags);
	virtual void RightDown(Point pt,dword keyflags);
	virtual void MouseMove(Point pt, dword keyflags);
	virtual void MiddleDown(Point pt,dword keyflags);
	virtual void MiddleUp(Point pt,dword keyflags);
	virtual void MouseLeave();
	virtual void MouseWheel(Point pt, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	Callback WhenSync;
protected:
	RectTracker tr;
	Image img;
	Pointf mpos;
	Rectf lim;
private:
	enum{PC_CROSS,PC_ARROW,PC_PAN,PC_WAIT,PC_LOCAL};
	int cursor;
	Image cimg;
	void TrackerSync(Rect r);
};

class LegendCtrl : public Ctrl, public Legend{
public:
	typedef LegendCtrl CLASSNAME;
	LegendCtrl(Plot& p);
	Callback WhenSync;
	virtual void Paint(Draw& w);
	void SetSize(const Size& sz);
	Size GetSize()const;
protected:
	unsigned hash;
	Image img;
};

class PlotStyleDlg : public WithStyleDlg<TopWindow>{
public:
	typedef PlotStyleDlg CLASSNAME;
	PlotStyleDlg(PlotCtrl& parent,int index=0);
private:
	PlotCtrl& p;
	void Change();
	void UpdateFields();
	void Rename();
	class RenameDlg:public WithRenameDlg<TopWindow>{
		PlotCtrl& p;
		int idx;
	public:
		typedef RenameDlg CLASSNAME;
		RenameDlg(PlotCtrl& plot,int index);
		void Update();
	};
};

#endif
