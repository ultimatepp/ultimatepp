#ifndef _Dim_Dim_h_
#define _Dim_Dim_h_

template <class T> class Dim3;
template <class T, class R> class Dim3y;

template <class T, class R>
class Dim3x {
protected:
	typename Dim3<T> *dim;
	int   x;
	friend typename Dim3<T>;

public:
	Dim3y<T, R> operator[](int i)       { Dim3y<T, R> h; (Dim3x<T, R>&)h = *this; h.y = i; return h; }
};

template <class T, class R>
class Dim3y : Dim3x<T, R> {
	int   y;
	friend typename Dim3x<T, R>;
	
public:
	R& operator[](int i)                { return dim->At(x, y, i); }
};

template <class T>
class Dim3 {
	T  *data;
	int cx, cy, cz;
	
	T&  At0(int x, int y, int z) const;

public:
	void Create(int cx, int cy, int cz);

	T&       At(int x, int y, int z)                 { return At0(x, y, z); }
	const T& At(int x, int y, int z) const           { return At0(x, y, z); }
	T&       operator()(int x, int y, int z)         { return At0(x, y, z); }
	const T& operator()(int x, int y, int z) const   { return At0(x, y, z); }
	
	void Clear();
	void Set(const Dim3& src);
	
	Dim3& operator=(const Dim3& src);
	
	Dim3(const Dim3& src)                        { data = NULL; Set(src); }
	Dim3()                                       { data = NULL; }
	Dim3(int cx, int cy, int cz)                 { data = NULL; Create(cx, cy, cz); }
	~Dim3()                                      { Clear(); }
	
	Dim3x<T, T>       operator[](int i)          { Dim3x<T, T> h; h.dim = this; h.x = i; return h; }
	Dim3x<T, const T> operator[](int i) const    { Dim3x<T, const T> h; h.dim = const_cast<Dim3<T> *>(this); h.x = i; return h; }
};


#endif
