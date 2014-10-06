#ifndef _Dim_Dim2_h_
#define _Dim_Dim2_h_

template <class T> class Dim2;
template <class T, class R> class Dim2y;

template <class T, class R>
class Dim2x {
protected:
	typename Dim2<T> *dim;
	int   x;
	friend typename Dim2<T>;

public:
	R& operator[](int i)                      { return dim->At(x, i); }
};

template <class T>
class Dim2 {
	T  *data;
	int cx, cy;
	
	T&  At0(int x, int y) const;

public:
	void Create(int cx, int cy);

	T&       At(int x, int y)                 { return At0(x, y); }
	const T& At(int x, int y) const           { return At0(x, y); }
	T&       operator()(int x, int y)         { return At0(x, y); }
	const T& operator()(int x, int y) const   { return At0(x, y); }
	
	void Clear();
	void Set(const Dim2& src);
	
	Dim2& operator=(const Dim2& src);
	
	Dim2(const Dim2& src)                        { data = NULL; Set(src); }
	Dim2()                                       { data = NULL; }
	Dim2(int cx, int cy, int cz)                 { data = NULL; Create(cx, cy, cz); }
	~Dim2()                                      { Clear(); }
	
	Dim2x<T, T>       operator[](int i)          { Dim2x<T, T> h; h.dim = this; h.x = i; return h; }
	Dim2x<T, const T> operator[](int i) const    { Dim2x<T, const T> h; h.dim = const_cast<Dim2<T> *>(this); h.x = i; return h; }
};

#endif
