const dword COMPLEX_V   = 20;

struct Complex : std::complex<double>
{
	typedef std::complex<double> C;

	Complex()                               {}
	Complex(double r) : C(r)                {}
	Complex(double r, double i) : C(r,i)    {}
	Complex(const C& c) : C(c)              {}

	Complex(const Nuller&) : C(DOUBLE_NULL) {}
	operator Value() const                  { return RichValue<Complex>(*this); } 
	Complex(const Value& v) : C(IsNumber(v) ? C((double)v) : RichValue<Complex>::Extract(v)) {}

	bool operator==(const Complex& c) const { return (const C&)(*this) == (const C&)c; }
	bool operator!=(const Complex& c) const { return (const C&)(*this) != (const C&)c; }
};

template<> inline bool IsNull(const Complex& r) { return r.real() < DOUBLE_NULL_LIM || r.imag() < DOUBLE_NULL_LIM; }
template<> inline unsigned GetHashValue(const Complex& x) { return CombineHash(x.real(), x.imag()); }
template<> inline String AsString(const Complex& x) { return String().Cat() << "(" << x.real() << "," << x.imag() << ")"; }

template<> inline Stream& operator%(Stream& s, Complex& c)
{
	double r,i;
	if(s.IsStoring()) { r = c.real(); i = c.imag(); }
	s % r % i;
	if(s.IsLoading()) { c = Complex(r,i); }
	return s;
}

template<> inline dword ValueTypeNo(const Complex*) { return COMPLEX_V; }

inline const Complex& Nvl(const Complex& a, const Complex& b)  { return IsNull(a) ? b : a; }

inline bool IsPolyEqual(const Complex& x, const Value& v) {
	return IsNumber(v) && x.imag() == 0 && x.real() == (double)v;
}

VALUE_COMPARE(Complex)
NTL_MOVEABLE(Complex)
