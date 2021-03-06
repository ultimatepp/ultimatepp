#ifndef _ScatterDraw_MarkPlot_h_
#define _ScatterDraw_MarkPlot_h_

namespace Upp {
	
class MarkPlot {
public:
	MarkPlot() : multiPlot(false), type(0) {}
	virtual ~MarkPlot() noexcept {};	
	virtual void Paint(Draw &p, const double& , const Point& cp, const double& size, 
		const Color& markColor, const double& markBorderWidth, const Color& markBorderColor) const = 0;
	virtual void Paint(Draw &, const double& , int , int , const Vector<int>& , 
		const Vector<int>& , const Vector<double>& , const double& , 
		const Color& , const double& , const Color& ) const {NEVER();};
	virtual void Paint(Painter &p, const double& , const Point& cp, const double& size, 
		const Color& markColor, const double& markBorderWidth, const Color& markBorderColor) const = 0;
	virtual void Paint(Painter &, const double& , int , int , const Vector<int>& , 
		const Vector<int>& , const Vector<double>& , const double& , 
		const Color& , const double& , const Color& ) const {NEVER();};
	template<class T>
	static void Register(const String& name)
	{
		classMap().FindAdd(name, __Create<T>);
		typeMap().FindAdd(typeid(T).name(), name);
		T dummy;
		typeNumber().Add(dummy.GetTypeCount());
		typeString().Add(dummy.TypeString());
	}
	static void Unregister(const String& name)
	{
		int i = TypeIndex(name);
		ASSERT(i >= 0);
		classMap().Remove(i);
		typeMap().Remove(i);
		typeNumber().Remove(i);
		typeString().Remove(i);
	}
	static String         TypeName(int i)               {return typeMap()[i];}
	static int            TypeIndex(const String& name) {return classMap().Find(name);}
	static int            GetCount()                    {return classMap().GetCount();}
	static MarkPlot*   	  Create(int i)                 {return classMap()[i]();}
	static Vector<String> GetTypes()                    {return clone(typeMap()).PickValues();}
	int GetType() const {return typeMap().Find(typeid(*this).name());}
	MarkPlot* Copy()const;
	bool IsMultiPlot() const 	{return multiPlot;}

	static int GetTypeCount(int iM)                 	{return typeNumber()[iM];}
	static String TypeString(int iM, int iT)			{return typeString()[iM][iT];}
	virtual int GetTypeCount()							{return 0;}
	virtual const char **TypeString()					{return NULL;}
	
	void SetTypeType(int _type) 						{this->type = _type;}
	int GetTypeType() 									{return type;}
 
protected:
	typedef MarkPlot* (*CreateFunc)();
	template<class T>	
	static MarkPlot*                      __Create() {return new T;}
	static VectorMap<String, CreateFunc>& classMap() {static VectorMap<String, CreateFunc> cMap; return cMap;}
	static VectorMap<String, String>&     typeMap()  {static VectorMap<String, String> tMap; 	 return tMap;}
	
	bool multiPlot;
	int type;
	
	static Vector<int>& typeNumber()				 {static Vector<int> typeNumber; 			return typeNumber;}
	static Vector<const char **>& typeString()		 {static Vector<const char **> typeString; 	return typeString;}
};

class CircleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor, 
		const double& markBorderWidth, const Color& markBorderColor) const
	{
		int radius = fround(scale*size);		
		int radius2 = radius/2;
		w.DrawEllipse(cp.x - radius2, cp.y - radius2, radius, radius, markColor, fround(markBorderWidth), markBorderColor);
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const  
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const  
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
};

class SquareMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x - side2l, cp.y - side2l) << Point(cp.x + side2r, cp.y - side2l)
		  << Point(cp.x + side2r, cp.y + side2r) << Point(cp.x - side2l, cp.y + side2r)
		  << Point(cp.x - side2l, cp.y - side2l);
		w.DrawPolygon(p, markColor, fround(markBorderWidth), markBorderColor); 	
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const  
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const  
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
};

class TriangleMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const
	{
		Vector <Point> p;
		int xl = int((size*scale)/2.);
		int xr = int(size*scale - xl);
		int yt = int(size*scale*2/3.);
		int yb = int(size*scale - yt);
		p << Point(cp.x - xl, cp.y + yb) << Point(cp.x + xr, cp.y + yb)
		  << Point(cp.x, cp.y - yt) << Point(cp.x - xl, cp.y + yb);
		w.DrawPolygon(p, markColor, fround(markBorderWidth), markBorderColor); 	
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const 
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& markBorderWidth, const Color& markBorderColor) const  
	{
		DoPaint(p, scale, cp, size, markColor, markBorderWidth, markBorderColor);
	}
};

class CrossMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y, cp.x + side2r, cp.y, fround(scale), markColor);
		w.DrawLine(cp.x, cp.y - side2l, cp.x, cp.y + side2r, fround(scale), markColor);
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class XMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		w.DrawLine(cp.x - side2l, cp.y - side2l, cp.x + side2r, cp.y + side2r, fround(scale), markColor);
		w.DrawLine(cp.x + side2r, cp.y - side2l, cp.x - side2l, cp.y + side2r, fround(scale), markColor);
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const  
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	
	
class RhombMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		Vector <Point> p;
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		p << Point(cp.x, cp.y - side2l) << Point(cp.x + side2r, cp.y)
		  << Point(cp.x, cp.y + side2r) << Point(cp.x - side2l, cp.y)
		  << Point(cp.x, cp.y - side2l);
		w.DrawPolygon(p, markColor, fround(scale/2.), markColor); 
	}

public:
	void Paint(Draw &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	void Paint(Painter &p, const double& scale, const Point& cp, const double& size, const Color& markColor,
		const double& , const Color& ) const 
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};	

//void debug_h();			// Dummy function used to debug .h files

class RangePlot : public MarkPlot {
public:
	enum RangeType {ALL, MIN_MAX, MIN_AVG_MAX};
	RangePlot(RangeType rangeType = ALL) {
		type = rangeType;
		multiPlot = true;
	}
	virtual int GetTypeCount()			{return 3;}
	virtual const char **TypeString()	{
		static const char *names[] = {"ALL", "MIN_MAX", "MIN_AVG_MAX"};
		return names;
	}
	
private:
	template <class T>
	void DoPaint(T& w, const double& scale, int x, int y, const Vector<int>& dataX, 
		const Vector<int>& dataY, const double& size, const Color& markColor) const
	{
		int side2l = int((size*scale)/2.);
		int side2r = int(size*scale - side2l);
		const Vector<int> *pdata;
		if (!dataY.IsEmpty()) 
			pdata = &dataY;
		else
			pdata = &dataX;
		if (pdata->GetCount() == 0)
			return;
		int avg = 0;
		int min = INT_MAX;
		int max = INT_MIN;
		for (int i = 0; i < pdata->GetCount(); ++i) {
			if (min > (*pdata)[i])
				min = (*pdata)[i];
			if (max < (*pdata)[i])
				max = (*pdata)[i];
		}
		if (type == MIN_AVG_MAX) {
			for (int i = 0; i < pdata->GetCount(); ++i) 
				avg += (*pdata)[i];
			avg /= pdata->GetCount();
		}
		if (!dataY.IsEmpty()) {	
			if (type == ALL) {
				for (int i = 0; i < dataY.GetCount(); ++i) 
					w.DrawLine(x - side2l, dataY[i], x + side2r, dataY[i], fround(scale), markColor);
			} else {
				w.DrawLine(x - side2l, min, x + side2r, min, fround(scale), markColor);			
				w.DrawLine(x - side2l, max, x + side2r, max, fround(scale), markColor);			
				if (type == MIN_AVG_MAX) 
					w.DrawLine(x - side2l, avg, x + side2r, avg, fround(scale*4), markColor);			
			}
			w.DrawLine(x, min, x, max, fround(scale), markColor);
		} else {
			if (type == ALL) {
				for (int i = 0; i < dataX.GetCount(); ++i) 
					w.DrawLine(dataX[i], y - side2l, dataX[i], y + side2r, fround(scale), markColor);
			} else {
				w.DrawLine(min, y - side2l, min, y + side2r, fround(scale), markColor);
				w.DrawLine(max, y - side2l, max, y + side2r, fround(scale), markColor);
				if (type == MIN_AVG_MAX) 
					w.DrawLine(avg, y - side2l, avg, y + side2r, fround(scale*4), markColor);
			}
			w.DrawLine(min, y, max, y, fround(scale), markColor);
		}
	}
	
public:
	virtual void Paint(Draw &p, const double& scale, int x, int y, const Vector<int>& dataX, 
		const Vector<int>& dataY, const Vector<double>& , const double& size, 
		const Color& markColor, const double& , const Color& ) const
	{
		DoPaint(p, scale, x, y, dataX, dataY, size, markColor);
	}
	virtual void Paint(Painter &p, const double& scale, int x, int y, const Vector<int>& dataX, 
		const Vector<int>& dataY, const Vector<double>& , const double& size, 
		const Color& markColor, const double& , const Color& ) const 
	{
		DoPaint(p, scale, x, y, dataX, dataY, size, markColor);
	}
	void Paint(Draw &, const double& , const Point& , const double& , const Color& ,
		const double& , const Color& ) const 
	{}
	void Paint(Painter &, const double& , const Point& , const double& , const Color& ,
		const double& , const Color& ) const  
	{}
};	

class BubblePlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const double& scale, int x, int y, const Vector<double>& dataFixed, const double& , 
		const Color& markColor, const double& markBorderWidth, const Color& markBorderColor) const
	{
		if (dataFixed.IsEmpty())
			return;
		int diameter = int(scale*dataFixed[0]);
		int radius = int(scale*dataFixed[0]/2.);		
		w.DrawEllipse(x - radius, y - radius, diameter, diameter, markColor, fround(markBorderWidth), markBorderColor);
	}
	
public:
	BubblePlot() {multiPlot = true;}
	virtual void Paint(Draw &p, const double& scale, int x, int y, const Vector<int>& , 
		const Vector<int>& , const Vector<double>& dataFixed, const double& size, 
		const Color& markColor, const double& markBorderWidth, const Color& markBorderColor) const
	{
		DoPaint(p, scale, x, y, dataFixed, size, markColor, markBorderWidth, markBorderColor);
	}
	virtual void Paint(Painter &p, const double& scale, int x, int y, const Vector<int>& , 
		const Vector<int>& , const Vector<double>& dataFixed, const double& size, 
		const Color& markColor, const double& markBorderWidth, const Color& markBorderColor) const 
	{
		DoPaint(p, scale, x, y, dataFixed, size, markColor, markBorderWidth, markBorderColor);
	}
	void Paint(Draw &, const double& , const Point& , const double& , const Color& ,
		const double& , const Color& ) const 
	{}
	void Paint(Painter &, const double& , const Point& , const double& , const Color& ,
		const double& , const Color& ) const  
	{}
};	
					
}

#endif
