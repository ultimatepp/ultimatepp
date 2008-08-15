#ifndef _Threads_complex_h_
#define _Threads_complex_h_

#include <Core/Core.h>

template<typename T>
class Complex_ 
{
	public:
		
	T 				Re(void) const 							{ return re; }
	void 			SetRe(T value ) 						{ re = value; }
	T 				Im(void) const 							{ return im; }
	void 			SetIm(T value ) 						{ im = value; }
	
	explicit Complex_ (void) : re(0), im(0) 				{}
	explicit Complex_ (T re) : re(re), im(0) 				{}
	Complex_ (T re, T im) : re(re), im(im)				 	{}
	
	Complex_ operator + (const Complex_ & other) const  	{ return Complex_(re + other.re, im + other.im ); }
	Complex_ operator - (const Complex_ & other) const  	{ return Complex_(re - other.re, im - other.im ); }
	Complex_ operator * (const Complex_ & other) const  	{ return Complex_(re * other.re - im * other.im, re * other.im + im * other.re ); }
	
	T Abs2() const { return re*re+im*im; }
	private:
	
	T re;
	T im;
};


typedef Complex_<double> Complex;

String ToString(const Complex & c);

template<typename T, typename U>
Complex_<T> operator + (const Complex_<T> & a, U b) 		{ return Complex_<T>(a.Re() + b,  a.Im() ); }
template<typename T, typename U>
Complex_<T> operator + (U b, const Complex_<T> & a) 		{ return Complex_<T>(a.Re() + b,  a.Im() ); }

template<typename T, typename U>
Complex_<T> operator - (const Complex_<T> & a, U b) 		{ return Complex_<T>(a.Re() - b,  a.Im() ); }
template<typename T, typename U>
Complex_<T> operator - (U b, const Complex_<T> & a) 		{ return Complex_<T>(a.Re() - b,  a.Im() ); }

template<typename T, typename U>
Complex_<T> operator * (const Complex_<T> & a, U b) 		{ return Complex_<T>(a.Re() * b,  a.Im() * b ); }
template<typename T, typename U>
Complex_<T> operator * (U b, const Complex_<T> & a) 		{ return Complex_<T>(a.Re() * b,  a.Im() * b ); }

#endif
