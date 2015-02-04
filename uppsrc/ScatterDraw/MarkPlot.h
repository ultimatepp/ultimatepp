#ifndef _ScatterDraw_MarkPlot_h_
#define _ScatterDraw_MarkPlot_h_

using namespace Upp;

class MarkPlot {
public:
	virtual ~MarkPlot() {};	
	virtual void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const {};
	virtual void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const {};
	template<class T>
	static void Register(const String& name)
	{
		classMap().FindAdd(name, __Create<T>);
		typeMap().FindAdd(typeid(T).name(), name);
	}
	static void Unregister(const String& name)
	{
		int i = TypeIndex(name);
		ASSERT(i >= 0);
		classMap().Remove(i);
		typeMap().Remove(i);
	}
	static String         TypeName(int i)               {return typeMap()[i];}
	static int            TypeIndex(const String& name) {return typeMap().Find(name);}
	static int            GetCount()                    {return classMap().GetCount();}
	static MarkPlot*   	  Create(int i)                 {return classMap()[i]();}
	static Vector<String> GetTypes()                    {return clone(typeMap()).PickValues();}
	int GetType() const {return typeMap().Find(typeid(*this).name());}
	MarkPlot* Copy()const;
	       	
protected:
	typedef MarkPlot* (*CreateFunc)();
	template<class T>	
	static MarkPlot*                      __Create() {return new T;}
	static VectorMap<String, CreateFunc>& classMap() {static VectorMap<String, CreateFunc> cMap; return cMap;}
	static VectorMap<String, String>&     typeMap()  {static VectorMap<String, String> tMap; 	 return tMap;}
};

class CircleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int radius = fround(scale*size);		
		int radius2 = radius/2;
		w.DrawEllipse(cp.x - radius2, cp.y - radius2, radius, radius, markColor, 1, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class SquareMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x - side2l, cp.y - side2l) << Point(cp.x + side2r, cp.y - side2l)
		  << Point(cp.x + side2r, cp.y + side2r) << Point(cp.x - side2l, cp.y + side2r)
		  << Point(cp.x - side2l, cp.y - side2l);
		w.DrawPolygon(p, markColor, scale/2, markColor); 	
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class TriangleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int xl = int((size*scale)/2.);
		int xr = int(size*scale - xl);
		int yt = int(size*scale*2/3.);
		int yb = int(size*scale - yt);
		p << Point(cp.x - xl, cp.y + yb) << Point(cp.x + xr, cp.y + yb)
		  << Point(cp.x, cp.y - yt) << Point(cp.x - xl, cp.y + yb);
		w.DrawPolygon(p, markColor, scale/2, markColor); 	
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

class CrossMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y, cp.x + side2r, cp.y, scale, markColor);
		w.DrawLine(cp.x, cp.y - side2l, cp.x, cp.y + side2r, scale, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class XMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y - side2l, cp.x + side2r, cp.y + side2r, scale, markColor);
		w.DrawLine(cp.x + side2r, cp.y - side2l, cp.x - side2l, cp.y + side2r, scale, markColor);
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class RhombMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x, cp.y - side2l) << Point(cp.x + side2r, cp.y)
		  << Point(cp.x, cp.y + side2r) << Point(cp.x - side2l, cp.y)
		  << Point(cp.x, cp.y - side2l);
		w.DrawPolygon(p, markColor, scale/2, markColor); 
	}

public:
	void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	

#endif
