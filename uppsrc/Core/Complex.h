const dword COMPLEX_V   = 20;

struct Complex : std::complex<double>
{
	typedef std::complex<double> C;

	Complex()                               {}
	Complex(double r) : C(r)                {}
	Complex(double r, double i) : C(r,i)    {}
	Complex(const C& c) : C(c)              {}

	Complex(const Nuller&) : C(DOUBLE_NULL, DOUBLE_NULL) {}
	operator Value() const                  { return RichToValue(*this); }
	Complex(const Value& v) : C(IsNumber(v) ? Complex((double)v) : v.Get<Complex>()) {}

	operator Ref()                          { return AsRef(*this); }

	bool operator==(const Complex& c) const { return (const C&)(*this) == (const C&)c; }
	bool operator!=(const Complex& c) const { return (const C&)(*this) != (const C&)c; }
	bool operator==(double x) const         { return (const C&)(*this) == x; }
	bool operator!=(double x) const         { return (const C&)(*this) != x; }
	
	int  Compare(const Complex& c) const    { NEVER(); return 0; }

	void Serialize(Stream& s);
	void Xmlize(XmlIO& xio);
	void Jsonize(JsonIO& jio);
};

template<> inline bool IsNull(const Complex& r) { return IsNull(r.real()) || IsNull(r.imag()); }
template<> inline hash_t GetHashValue(const Complex& x) { return CombineHash(x.real(), x.imag()); }
template<> inline String AsString(const std::complex<double>& x) { return String().Cat() << "(" << x.real() << "," << x.imag() << ")"; }
template<> inline String AsString(const Complex& x) { return AsString((const std::complex<double>&)x); }

template<> inline dword ValueTypeNo(const Complex*) { return COMPLEX_V; }

inline double abs(const Complex& c) { return std::abs(c); }

inline const Complex& Nvl(const Complex& a, const Complex& b)  { return IsNull(a) ? b : a; }

template<>
inline bool IsPolyEqual(const Complex& x, const Value& v) {
	return IsNumber(v) && x.imag() == 0 && x.real() == (double)v;
}

template<>
inline int  PolyCompare(const Complex& a, const Value& b)
{
	NEVER(); return 0;
}

inline bool IsNaN(const Complex& x)        { return IsNaN(x.real()) || IsNaN(x.imag()); }
inline bool IsInf(const Complex& x)        { return IsInf(x.real()) || IsInf(x.imag()); }
inline bool IsFin(const Complex& x)        { return IsFin(x.real()) && IsFin(x.imag()); }

VALUE_COMPARE(Complex)
NTL_MOVEABLE(Complex)
