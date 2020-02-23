#ifndef _STEM4U_Polynomial_h_
#define _STEM4U_Polynomial_h_

namespace Upp {
	
template<class T>
class Polynomial : public Moveable<Polynomial<T>> {
public:
	Polynomial() {}
	Polynomial(const Polynomial &p) {
		c = clone(p.c);
	}
	template<typename... Args>
	Polynomial(T val, Args... args) {
		Append(val);
    	Append(args...);
	}
	void Set(T val) {
		Append(val);
	}
	template<typename... Args>
	void Set(T val, Args... args) {
		Append(val);
    	Append(args...);
	}
	void Append(T val) {
		c << val;
	}
	template<typename... Args>
	void Append(T val, Args... args) {
		c << val;
    	Append(args...);
	}
	T &operator[](int i) {
		if (i >= c.GetCount())
			c.SetCount(i+1);
		return c[i];		
	}
	Polynomial Diff() const {
  		Polynomial res;
  		int num = GetCount();
  		if (num == 0)
			return res;
  		res.c.SetCount(num-1);
		for (int i = 1; i < num; ++i) 
			res.c[i-1] = c[i]*i;
		return res;
	}
	Polynomial Order(int delta) const {
		Polynomial res;
		int num = c.GetCount();
		if (num == 0)
			return res;
		res.c.SetCount(num+delta);
		int init = max(0, -delta);
		for (int i = init; i < num; ++i)
			res.c[i+delta] = c[i];
		return res;
	}
	T y(const T val) {
		T res = 0;
		int num = c.GetCount();
		if (num == 0)
			return res;
		res = c[0];
		T vval = 1;
		for (int i = 1; i < num; ++i) {
			vval *= val;
			res += c[i]*vval;
		}
		return res;
	}
	int Order() const		{return c.GetCount() - 1;}
	int GetCount() const	{return c.GetCount();}
	T Coeff(int i) const	{return c[i];}
	
	const Polynomial& operator=(const T& l) {
		int num = l.GetCount();
		c.SetCount(num);	
		for (int i = 0; i < num; ++i)
			c[i] = l.Coeff[i];
		return *this;
	}

    Polynomial operator+(const Polynomial& rhs) const {
  		Polynomial res;
  		int num = max(GetCount(), rhs.GetCount());
  		res.c.SetCount(num);
  		for (int i = 0; i < GetCount(); ++i) 
  			res.c[i] = c[i];
  		for (int i = 0; i < rhs.GetCount(); ++i) 
  			res.c[i] += rhs.c[i];
  		return res;	  
    }
    Polynomial operator-(const Polynomial& rhs) const {
  		Polynomial res;
  		int num = max(GetCount(), rhs.GetCount());
  		res.c.SetCount(num);
  		for (int i = 0; i < GetCount(); ++i) 
  			res.c[i] = c[i];
  		for (int i = 0; i < rhs.GetCount(); ++i) 
  			res.c[i] -= rhs.c[i];
  		return res;	  
    }
    Polynomial operator*(const Polynomial& rhs) const {
  		Polynomial res;
  		int num = Order() + rhs.Order() + 1;
  		res.c.SetCount(num, 0);
  		for (int i = 0; i < GetCount(); ++i)
  			for (int j = 0; j < rhs.GetCount(); ++j)
  				res.c[i + j] += c[i] * rhs.c[j];
  		return res;	  
    }
    Polynomial operator*(const T &rhs) const {
        Polynomial res;
  		int num = GetCount();
  		res.c.SetCount(num);
  		for (int i = 0; i < GetCount(); ++i)
  			res.c[i] = c[i] * rhs;
  		return res;
    }
    String ToString() const {
        String ret;
     
		for (int i = GetCount()-1; i >= 0; --i) {
			bool isneg = c[i] < 0;
			String str;
			str << abs(c[i]);
			if (str != "0") {
				if (i < GetCount()-1) {
					if (!isneg) 
						ret << " + ";
					else
						ret << " - ";
				} else {
					if (isneg)
						ret << "-";
				}
				if (str != "1") 
					ret << str;
				if (i > 1)
					ret << "x^" << i;
				else if (i == 1)
					ret << "x";
			}
		}
        return ret;
    }
    
private:
	Upp::Vector<T> c;
};

template<typename T>
Polynomial<T> operator*(const T &lhs, const Polynomial<T> &rhs) {
    Polynomial<T> res;
	int num = rhs.GetCount();
	res.c.SetCount(num);
	for (int i = 0; i < num; ++i)
		res.c[i] = rhs.c[i] * lhs;
	return res;
}

}

#endif
