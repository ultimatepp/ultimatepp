#ifndef _rainbow_CoreGl_ControlPanel_h_
#define _rainbow_CoreGl_ControlPanel_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

struct ValueSlider : Ctrl
{
	bool shaded;
	bool immediate;
	Color src;
	Color dst;
		
	float pos;
	float minValue;
	float maxValue;
	String text;
	
	typedef ValueSlider CLASSNAME;
	
	ValueSlider();

	virtual void Paint(Draw &w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	
	void SetPos(float p, float minValue, float maxValue);
	float GetPos();
	
	Callback WhenLeftUp;
};

struct SlimButton : Ctrl
{
	Color fg;
	Color bg;
	float alpha;
	String label;
	virtual void Paint(Draw &w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	void SetLabel(const char* s);
	SlimButton();
};

struct InfoPanel : Ctrl
{
	bool init;
	Ctrl* parent;
	
	ValueSlider alphaSlider;
	ValueSlider angleSlider;
	ValueSlider scaleSlider;

	float GetAlpha();
	float GetAngle();
	float GetScale();
	
	void Init(Ctrl& parent, float angle = 0.f, float scale = 1.f, float alpha = 255.f);
	void Show(bool b = true);
	virtual void Paint(Draw& w);
	typedef InfoPanel CLASSNAME;
	InfoPanel();
};

END_UPP_NAMESPACE

#endif
