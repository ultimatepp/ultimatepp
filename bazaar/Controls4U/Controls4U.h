#ifndef _Controls4U_Controls4U_h_
#define _Controls4U_Controls4U_h_

#include <Painter/Painter.h>
#include "Functions4U/Functions4U.h"

class MyBufferPainter : public BufferPainter {
public:
	MyBufferPainter(ImageBuffer& ib, int mode = MODE_ANTIALIASED) : BufferPainter(ib, mode) {};
	void DrawLine(double x1, double y1, double x2, double y2, double width, Color color) {
		Move(x1, y1).Line(x2, y2).Stroke(width, color);
	}
	void DrawRect(double x, double y, double cx, double cy, Color color) {
		Move(x, y).Line(x + cx, y).Line(x + cx, y + cy).Line(x, y + cy).Close();
		Fill(color);
	}
	void DrawRect(const Rect &r, Color color) {
		DrawRect(r.left, r.top, r.right-r.left, r.bottom-r.top, color);
	}	
	void DrawEllipse(double x, double y, double rx, double ry, Color color) {
		Ellipse(x, y, rx, ry);
		Fill(color);
	}
	void DrawEllipse(const Rect& r, Color color) {
		Sizef sz = r.GetSize();
		Ellipse(r.left + sz.cx / 2, r.top + sz.cy / 2, sz.cx / 2, sz.cy / 2);
		Fill(color);
	}
	void DrawCircle(double x, double y, double r, Color color) {
		Ellipse(x, y, r, r);
		Fill(color);
	}
	void DrawCenterText(double x, double y, String text, Font fnt, Color color) {
		Size sz = GetTextSize(text, fnt);
		DrawText((int)(x - sz.cx / 2.), (int)(y - sz.cy / 2.), text, fnt, color);
	}
	void DrawArc(double cx, double cy, double R, double ang0, double ang1, int direction, 
					double width, Color color) {
		if (direction == -1) {
			double c = ang0;
			ang0 = ang1;
			ang1 = c;
		}
		ang0 = ToRad(ang0);
		ang1 = ToRad(ang1);
		double delta = ToRad(1);
		if (ang0 > ang1)
			ang1 += 2*M_PI;
		double x0, y0, x1, y1;
		for (double i = ang0; i < ang1; i += delta) {
			if (i == ang0) {
				x0 = cx + R*cos(i);
				y0 = cy - R*sin(i);
			} else {
				x0 = x1;
				y0 = y1;
			}
			x1 = cx + R*cos(i + delta);
			y1 = cy - R*sin(i + delta);
			DrawLine(x0, y0, x1, y1, width, color);
		}
	}
};

class EditFileFolder : public EditString {
typedef EditFileFolder CLASSNAME;
protected:
	FrameLeft<Button> ButBrowse;
	FrameRight<Button> ButGo;
	bool Key(dword key, int rep);
	
	FileSel fs;
	bool isFile, isLoad;
	String title;
	
public:
	EditFileFolder() 								{Init();};
	void Init();
	void DoBrowse();
	void DoGo();
	void Type(const char *name, const char *ext)	{fs.Type(name, ext);};
	void AllFilesType()								{fs.AllFilesType();};
	void ActiveDir(const String& d) 				{fs.ActiveDir(d);};
	void MkDirOption(bool b)						{fs.MkDirOption(b);};
	String Get() const                           	{return fs.Get();};	
	String operator~() const                     	{return Get();};
	operator String() const                      	{return Get();}
	void Set(const String& s)						{fs.Set(s); SetData(s);};
	void operator<<=(const String& s)            	{Set(s);};
	void operator=(const String& s)              	{Set(s);};
	EditFileFolder &NotNull(bool b)					{EditString::NotNull(b);  return *this;};
	EditFileFolder &SelLoad(bool load) 				{isLoad = load; return *this;};
	EditFileFolder &SetTitle(const char *_title)	{title = _title; return *this;};
	
	Callback WhenChange;
};

class EditFile : public EditFileFolder {
public:
	EditFile() {isFile = true;		EditFileFolder();};
};

class EditFolder : public EditFileFolder {
public:
	EditFolder() {isFile = false;	EditFileFolder();};
};

class StaticImage : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	
	enum ImageAngle {Angle_0, Angle_90, Angle_180, Angle_270};
	enum ImageFit   {BestFit, FillFrame, NoScale, RepeatToFill, Background};

protected:
	String fileName;
	Image image, origImage;
	Color background;
	int angle, fit;

public:
	bool  Set(String fileName);
	bool  Set(Image image);

	void  operator=(String fileName)      	{Set(fileName);}
	void  operator=(Image image)       		{Set(image);}	

	StaticImage& SetAngle(int _angle);
	StaticImage& SetFit(int _fit)			{fit = _fit; Refresh(); return *this;}
	StaticImage& SetBackground(Color c) 	{background = c; Refresh(); return *this;}

	StaticImage();
};

class StaticRectangle : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	
protected:
	Color background;
	Color color;
	int width;

public:
	StaticRectangle& SetWidth(int w) 		{width = w; Refresh(); return *this;}
	StaticRectangle& SetColor(Color c) 		{color = c; Refresh(); return *this;}
	StaticRectangle& SetBackground(Color c) {background = c; Refresh(); return *this;}

	StaticRectangle();
};

class StaticEllipse : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	
protected:
	Color background;
	Color color;
	int width;

public:
	StaticEllipse& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticEllipse& SetColor(Color c) 		{color = c; Refresh(); return *this;}
	StaticEllipse& SetBackground(Color c) 	{background = c; Refresh(); return *this;}

	StaticEllipse();
};

class StaticFrame : public Ctrl {
public:
	StaticFrame();
};

class StaticLine : public Ctrl, public CtrlFrame {
public:
	virtual void FrameAddSize(Size& sz) {}
	virtual void FrameLayout(Rect& r) {}
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual int OverPaint() const { return 20; }
	
protected:
	String orientation;
	Color color;
	int width;

public:
	StaticLine& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticLine& SetColor(Color c) 			{color = c; Refresh(); return *this;}
	StaticLine& SetOrientation(String o) 	{orientation = o; Refresh(); return *this;}

	StaticLine();
};

class StaticArrow : public Ctrl, public CtrlFrame  {
public:
	virtual void FrameAddSize(Size& sz) {}
	virtual void FrameLayout(Rect& r) {}
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual int OverPaint() const { return 20; }
	
protected:
	String orientation;
	String ends;
	Color color;
	int width;

public:
	StaticArrow& SetWidth(int w) 			{width = w; Refresh(); return *this;}
	StaticArrow& SetColor(Color c) 			{color = c; Refresh(); return *this;}
	StaticArrow& SetOrientation(String o) 	{orientation = o; Refresh(); return *this;}
	StaticArrow& SetEnds(String e) 			{ends = e; Refresh(); return *this;}

	StaticArrow();
};

class StaticClock : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	
	enum HourType 	{No, Square, Rectangle};
	enum NumberType {NoNumber, Small, Big, BigSmall, Big4};
	enum ColorType 	{WhiteType, BlackType};

protected:
	void PaintPtr(MyBufferPainter &w, double cmx, double cmy, double pos, double m, double d, 
					Color color, double cf);
	int hourType;
	int numberType;
	Image image;
	bool seconds;
	int colorType;

	Time t;		

public:
	StaticClock& SetImage(String fileName);
	StaticClock& SetImage(Image _image)	{image = _image; Refresh(); return *this;}

	StaticClock& SetHourType(int type) 	{hourType = type; Refresh(); return *this;}
	StaticClock& SetNumberType(int type){numberType = type; Refresh(); return *this;}
	StaticClock& SetColorType(int c)	{colorType = c; Refresh(); return *this;}
	StaticClock& Seconds(bool b) 		{seconds = b; Refresh(); return *this;}	
	
	void SetData(const Value& v);
	void SetTime(const Time& tm = GetSysTime());
	void SetTime(int h, int n, int s);

	StaticClock();	
};

class Meter : public Ctrl {
public:
	virtual void   Paint(Draw& draw);
	
	enum ColorType {WhiteType, BlackType};
	
	friend void MeterThread(Meter *gui, double value);

protected:
	double value;
	double min;
	double max;
	double peak;
	double step;	
	double angleBegin;
	double angleEnd;
	String text;
	bool clockWise;
	bool number;
	int colorType;
	int speed;
	int sensibility;
	volatile Atomic running, kill;
	
	void PaintMarks(MyBufferPainter &w, double cx, double cy, double R, double ang0, 
					double ang1, int direction, double step, double bigF, Color color);
	double AngAdd(double ang, double val);
	void PaintNumbers(MyBufferPainter &w, double cx, double cy, double R, double a0, 
			double step, int direction, double minv, double maxv, double stepv, double bigF, 
			Color color);
	void PaintHand(MyBufferPainter &w, double cx, double cy, double R, double val, double bigF,
					int colorType);
	void RefreshValue() {Refresh();};
	
public:
	Meter& SetMin(double v)			{min = value = v; Refresh(); return *this;}
	Meter& SetMax(double v)			{max  = v; Refresh(); return *this;}
	Meter& SetPeak(double v)		{peak = v; Refresh(); return *this;}
	Meter& SetStep(double v) 		{step = v; Refresh(); return *this;}
	Meter& SetAngleBegin(double v) 	{angleBegin = v; Refresh(); return *this;}
	Meter& SetAngleEnd(double v)	{angleEnd = v; Refresh(); return *this;}
	Meter& SetText(String s)		{text = s; Refresh(); return *this;}
	Meter& ClockWise(bool v)		{clockWise = v; Refresh(); return *this;}
	Meter& SetNumber(bool v)		{number = v; Refresh(); return *this;}
	Meter& SetColorType(int c)		{colorType = c; Refresh(); return *this;}
	Meter& SetSpeed(int s)			{speed = s; Refresh(); return *this;} 
	Meter& SetSensibility(int s)	{sensibility = s; Refresh(); return *this;}

	void SetData(const Value& v);

	Meter();
	~Meter();
};

/*
class FileBrowser : public StaticRect {
public: 
 	bool  SetBrowseFiles(bool files);
 	
};
*/

#endif
