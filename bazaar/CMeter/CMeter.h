#ifndef __METER_H
#define __METER_H

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

class CMeter : public Ctrl
{
	typedef CMeter CLASSNAME;
	
public:
	struct Style : public ChStyle<Style>
	{
		Color backcol;
		Color textcol;
		Color forecol;
		Color forecol2;

		int cmarksize;

		enum MODES
		{
			STATIC = 0, //static rect
			GRADIENT,
			LINES,
		};
		int mode; //one of the MODES
	};

	class CVMark 
		: Moveable<CVMark>
	{
	public:
		typedef CVMark CLASSNAME;
		
		CVMark(double v = 0.0, String name = "", Color col = LtRed() );

	public:
	
		Color			col;
		String			name;
		double			v;
	};

	enum
	{
		IMGCACHE = 0,
		STATIC,
		GRADIENT,
		DYNGRADIENT,	
	};

	CMeter(double min = 0.0, double max = 1.0, bool vertical = false, bool reverse = false);
	virtual ~CMeter() {}
	
public:

	void CreateBack();

	virtual Value GetData() const { return Get(); }
	virtual void SetData(const Value& data) { Set(data); }
	Value GetMin() const { return Min(); }
	Value GetMax() const { return Max(); }

	CMeter & Set(double val);
	double Get() const { return val; }

	CMeter & Val(double val) { return Set(val); }
	double Val() const { return val; }

	CMeter & Percent(double percent) { Set(min + percent * (max-min) / 100.0); }
	double Percent() const { return 100.0 * (val-min)/(max-min); }

	CMeter & Min(double min);
	double Min() const { return min; }

	CMeter & Max(double max);
	double Max() const { return max; }

	CMeter & MinMax(double min = 0.0, double max = 1.0);

	CMeter & Reverse(bool reverse = true);
	bool IsReverse() const { return reverse; }

	CMeter & Vertical(bool vertical = true);
	bool IsVertical() const { return vertical; }

	CMeter & Stepping(bool stepping = true);
	bool IsStepping() const { return stepping; }

	CMeter & StepLinear(bool steplin = true);
	bool IsStepLinear() { return steplin; }

	CMeter & IncDecToStep(bool b = true) { incdectostep = b; return *this; }
	bool IsIncDecToStep() { return incdectostep; }

	CMeter & Steps(const Vector<double> & vsteps, bool reverse = false);
	void GetSteps(Vector<double> & vstep) { vsteps <<= this->vsteps; }

	CMeter & ShowValue(bool showvalue = true);
	CMeter & HideValue() { return ShowValue(false); }
	bool IsShowValue() { return showvalue; }

	CMeter & ShowPercent(bool showpercent = true);
	bool IsShowPercent() { return showpercent; }

	CMeter & SetMode(int mode);
	int GetMode() const { return drawmode; }

	void Inc();
	void Dec();

	CMeter& SetInc(double _inc = 0.1) { inc = _inc; return *this; }
	double GetInc() const { return inc; }

	CMeter& SetStyle(const Style& s);
	static const Style& StyleDefault();

	CMeter& SetFont(Font f) {
		font = f;
		Update();
		Layout();
		return *this;
	}
	Font GetFont() const { return font; }

protected:
	const Style *style;

	virtual void Layout();
	virtual void Paint(Draw& w);
	virtual void Update();
	
	virtual void LeftRepeat(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);			
	virtual void LeftUp(Point p, dword keyflags) ;	
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	virtual void GotFocus();
	virtual void LostFocus();

	virtual bool Key(dword key, int rep);
	bool Key0(dword key, int rep);

	int FindStep(double v) const;
	
	virtual int ValToPix(double _val);
	virtual double PointToVal(Point & p);

	bool SetFollow(double _val);
	bool SetFollow(Point p, dword keyflags);
	bool SetDirect(Point p, dword keyflags);
	double CalcIncV();

	void PaintBarGradient(Painter & sw, bool staticgradient = true);
	void PaintBarStatic(Painter & sw);
	void PaintBarImage(Painter & sw);
	void PaintCVMarks(Painter & sw);
	void PaintValue(Painter & sw);
	
	void DoPaint0(Painter& sw);
	void DoPaint(Painter& sw);

public:

	Vector<CVMark> cvmarks; //here we can set some marking

protected:
	
	//cached data
	
	int px; //some offsets from top left, used as offset where to start drawinf bar, no matter horz or vert
	int py;

	int _px; //some helpers to draw cvmarks, are extension of the cvmarks
	int _py;
	
	double pp; //cached [0:1] of drawn size
	int dx, lx, dy, ly; //some cache for painting, d barsize, l emtpy size

	int increment; //a state helper for repeating counts
	double incv; //for value following if not stepping
	int inci; //for value following if stepping
	double __val; //cached desired value for inc compare against oszillation when stepping
	int __ii; //cached current step index

	String s; //the printed value
	Size tlen; //text len

	//vital data

	double min;
	double max;
	double val;

	double inc; //for keys
	
	bool vertical:1;
	bool reverse:1;
	bool stepping:1;
	bool steplin:1;
	bool showvalue:1;
	bool showpercent:1;
	bool clickincdec:1;
	bool incdectostep:1;

	int drawmode;


	Font font;
	Image draw;
	Vector<double> vsteps;
};

#endif