#ifndef _PlotLib_PlotSymbol_h_
#define _PlotLib_PlotSymbol_h_

#include <Painter/Painter.h>
using namespace Upp;

class PlotSymbol{
private:
	typedef PlotSymbol CLASSNAME;
	typedef PlotSymbol* (*CreateFunc)();
	template<class T>
	static PlotSymbol*                   __Create() {return (PlotSymbol*)new T;}
	static VectorMap<String,CreateFunc>& classMap() {static VectorMap<String,CreateFunc> cMap; return cMap;}
	static VectorMap<String,String>&     typeMap()  {static VectorMap<String,String> tMap; return tMap;}
public:
	template<class T>
	static void Register(const String& name){
		classMap().FindAdd(name, __Create<T>);
		typeMap().FindAdd(typeid(T).name(),name);
	}
	static void Unregister(const String& name){
		int i=TypeIndex(name);
		ASSERT(i>=0);
		classMap().Remove(i);
		typeMap().Remove(i);
	}
	static String         TypeName(int i)               {return typeMap()[i];}
	static int            TypeIndex(const String& name) {return max(classMap().Find(name),0);}
	static int            GetCount()                    {return classMap().GetCount();}
	static PlotSymbol*    Create(int i)                 {return classMap()[i]();}
	static Vector<String> GetTypes()                    {return VectorMap<String,String>(typeMap(),1).PickValues();}
	       int            GetType()const                {return typeMap().Find(typeid(*this).name());}
	       PlotSymbol*    Copy()const;
protected:
	Callback1<Painter&> paint;
	void DoPaint(Painter& sw)const;
public:
	PlotSymbol();
	PlotSymbol(Sizef size,RGBA color,RGBA frame,double thickness);
	
	void Paint(Painter& sw,Pointf pos)const;
	unsigned GetHashValue()const;
	String ToString()const;
	
	virtual void PaintOp(Painter& sw)const{}

	PlotSymbol& SetSize(Sizef size)           {s=size;         return *this;}
	PlotSymbol& SetSize(double cx,double cy)  {s=Sizef(cx,cy); return *this;}
	PlotSymbol& SetColor(RGBA color)          {c=color;        return *this;}
	PlotSymbol& SetFrameColor(RGBA color)     {f=color;        return *this;}
	PlotSymbol& SetThickness(double thickness){t=thickness;    return *this;}
	Sizef GetSize()const                      {return s;}
	RGBA GetColor()const                      {return c;}
	RGBA GetFrameColor()const                 {return f;}
	double GetThickness()const                {return t;}

protected:
	Sizef s;
	RGBA c,f;
	double t;
private:
	mutable unsigned hash;
	mutable Image img;
	void Refresh()const;
};

template <class T>
class WithPlotSymbol:public PlotSymbol{
public:
	WithPlotSymbol(){paint=callback((T*)(this),&T::PaintOp);}
};

// Basic PlotSymbols

struct Cross:public WithPlotSymbol<Cross>{
	virtual void PaintOp(Painter& sw)const{
		sw.Move(0,0.5*s.cy).Line(s.cx,0,true).Stroke(t,c);
		sw.Move(0.5*s.cx,0).Line(0,s.cy,true).Stroke(t,c);
	}
};

struct XCross:public WithPlotSymbol<XCross>{
	virtual void PaintOp(Painter& sw)const{
		sw.Move(0,   0).Line(s.cx, s.cy,true).Stroke(t,c);
		sw.Move(0,s.cy).Line(s.cx,-s.cy,true).Stroke(t,c);
	}
};
struct Circle:public WithPlotSymbol<Circle>{
	virtual void PaintOp(Painter& sw)const{
		sw.Ellipse(0.5*s.cx,0.5*s.cy,0.5*(s.cx-t),0.5*(s.cy-t));
		sw.Fill(c).Stroke(t,f);
	}
};
struct Square:public WithPlotSymbol<Square>{
	virtual void PaintOp(Painter& sw)const{
		sw.Rectangle(t*0.5,t*0.5,s.cx-t,s.cy-t);
		sw.Fill(c).Stroke(t,f);
	}
};
struct Star:public WithPlotSymbol<Star>{
	virtual void PaintOp(Painter& sw)const{
		sw.Move(0,0.5*s.cy).Line(s.cx,0,true).Stroke(t,c);
		sw.Move(0.5*s.cx,0).Line(0,s.cy,true).Stroke(t,c);
		sw.Move(0,   0).Line(s.cx, s.cy,true).Stroke(t,c);
		sw.Move(0,s.cy).Line(s.cx,-s.cy,true).Stroke(t,c);
	}
};
#endif
