#ifndef _STEM4U_Rational_h_
#define _STEM4U_Rational_h_

namespace Upp {
	
class Rational : public Moveable<Rational> {
public:
	Rational() : num(0), den(1) {}
	Rational(const Rational& d) {
		num = d.num;
		den = d.den;
	}
	template <typename T>
	Rational(T v) {
		*this = v;
	}
	template <typename T>
	Rational(T n, T d) {
		num = n;
		den = d;
	}
	Rational(double n, double d) {
		*this = Rational(n)/Rational(d);
	}
	Rational operator-() const {
		Rational ret = *this;
		ret.num = -ret.num;
		return ret;
	}
	bool operator<(const Rational& right) const {
		intInf nd, right_nd;
		nd = num/den;
		right_nd = right.num/right.den;
		if (nd < right_nd)
			return true;
		if (nd == right_nd)
			return num%den < right.num%right.den;
		return false;
	}
	template <typename T>
	bool operator<(const T& right) const {
		return num < den*intInf(right);
	}
	bool operator>(const Rational& right) const {
		intInf nd, right_nd;
		nd = num/den;
		right_nd = right.num/right.den;
		if (nd > right_nd)
			return true;
		if (nd == right_nd)
			return num%den > right.num%right.den;
		return false;
	}
	template <typename T>
	bool operator>(const T& right) const {
		return num > den*intInf(right);
	}
	template <typename T>
  	const Rational& operator=(T d) {
  		num = d;
  		den = 1;
  		return *this;
  	}
  	const Rational& operator=(double d) {
		int exp;
		double man = normalize(d, exp);
		long long lman = static_cast<long long>(man*1E17);
		num = lman;
		den = 1;
		if (exp > 0) 
			num *= intInf(("1" + String('0', exp)).Begin());
		else
			den *= intInf(("1" + String('0', -exp)).Begin());
		den *= intInf(("1" + String('0', 17)).Begin());
  		return *this;
  	}
  	void operator++() {
  		num++;
  	}
  	void operator--() {
  		num--;
  	}
  	void operator+=(const Rational& right) {
  		if (den == right.den) 
  			num += right.num;
  		else {
  			num = num*right.den + den*right.num;
  			den *= right.den;
  		}
  	}
  	void operator-=(const Rational& right) {
  		if (den == right.den) 
  			num -= right.num;
  		else {
  			num = num*right.den - den*right.num;
  			den *= right.den;
  		}
  	}
  	void operator*=(const Rational& right) {
  		num *= right.num; 	
  		den *= right.den; 	
  	}
  	void operator/=(const Rational& right) {
  		if (&right == this) {
  			num = den = 1;
  		} else {
	  		den *= right.num; 	
	  		num *= right.den;
  		}
  	}
  	bool operator==(Rational& right) const {
  		return num == right.num && den == right.den;
  	}
  	template <typename T>
  	bool operator==(T right) const {
  		if (den == 1)
  			return num == intInf(right);
  		else 
			return T(num/den) == right;
  	}
    bool operator!=(Rational& right) const {return !operator==(right);}
  	template <typename T>
  	bool operator!=(T right) const 		 {return !operator==(right);}
  	    
  	template <typename T>
	operator T() {
		if (den == 0)
			throw std::domain_error("Division by zero");
		if (den == 1)	
			return T(num);
		Rational ret = T(num)/T(den);
		return ret;
	}
	operator double() {
		if (den == 0)
			throw std::domain_error("Division by zero");
		if (den == 1)	
			return double(num.toLongLong());
		Rational ret = double(num.toLongLong())/double(num.toLongLong());
		return ret;		
	}
  	Rational Simplify(bool full = false);
  	String ToString() const {return FormatRational(*this, Null);}
  	
private:
	intInf num, den;
	
	template <typename T>
	void SimplifyVal(T val) {
		intInf vval = val;
		while (num%vval == 0 && den%vval == 0) {
			num /= vval;
			den /= vval;
		}
	}
	
	friend Rational abs(const Rational &l);
	friend Rational pow(const Rational &l, int e);
	friend Rational sqrt(const Rational &l);
	friend String FormatRational(const Rational &d, int numDec = Null);
	
	template <typename T>
	friend Rational operator+(T left, const Rational &right);
	template <typename T>
	friend Rational operator+(const Rational& left, T right);
	friend Rational operator+(const Rational& left, const Rational& right);
	template <typename T>
	friend Rational operator-(T left, const Rational &right);
	template <typename T>
	friend Rational operator-(const Rational& left, T right);
	friend Rational operator-(const Rational& left, const Rational& right);
	template <typename T>
	friend Rational operator*(T left, const Rational &right);
	template <typename T>
	friend Rational operator*(const Rational& left, T right);
	friend Rational operator*(const Rational& left, const Rational& right);
	template <typename T>
	friend Rational operator/(T left, const Rational &right);
	template <typename T>
	friend Rational operator/(const Rational& left, T right);
	friend Rational operator/(const Rational& left, const Rational& right);
};
  	
Rational abs(const Rational &l);
Rational pow(const Rational &l, int e);
Rational sqrt(const Rational &l);
String FormatRational(const Rational &d, int numDec);

template <typename T>
Rational operator+(T left, const Rational &right) {
	Rational ret;
	ret.num = right.den*left + right.num;
	ret.den = right.den; 	
	return ret;
}

template <typename T>
Rational operator+(const Rational& left, T right) {
	Rational ret;
	ret.num = left.num + right; 
	ret.den = left.den;	
	return ret;
}

template <typename T>
Rational operator-(T left, const Rational &right) {
	Rational ret;
	ret.num = right.den*left - right.num;
	ret.den = right.den; 	
	return ret;
}

template <typename T>
Rational operator-(const Rational& left, T right) {
	Rational ret;
	ret.num = left.num - left.den*right; 
	ret.den = left.den;	
	return ret;
}

template <typename T>
Rational operator*(T left, const Rational& right) {
	Rational ret;
	ret.num = right.num*intInf(left); 	
	ret.den = right.den;
	return ret;
}

template <typename T>
Rational operator*(const Rational& left, T right) {
	Rational ret;
	ret.num = left.num*intInf(right); 	
	ret.den = left.den;
	return ret;
}

template <typename T>
Rational operator/(T left, const Rational& right) {
	Rational ret;
	ret.num = right.den*left;
	ret.den = right.num; 	
	return ret;
}

template <typename T>
Rational operator/(const Rational& left, T right) {
	Rational ret;
	ret.num = left.num;
	ret.den = left.den*right; 	
	return ret;
}

}
  
#endif
