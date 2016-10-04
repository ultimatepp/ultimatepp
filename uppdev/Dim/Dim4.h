#ifndef _Dim_Dim4_h_
#define _Dim_Dim4_h_

template <class T> class Dim4;
template <class T, class R> class Dim4y;
template <class T, class R> class Dim4z;

template <class T, class R>
class Dim4x {
protected:
	typename Dim4<T> *dim;
	int   x;
	friend typename Dim4<T>;
	friend typename Dim4z<T, R>;

public:
	Dim4y<T, R> operator[](int i)       { Dim4y<T, R> h; (Dim4x<T, R>&)h = *this; h.y = i; return h; }
};

template <class T, class R>
class Dim4y : protected Dim4x<T, R> {
protected:
	int   y;
	friend typename Dim4x<T, R>;
	
public:
	Dim4z<T, R> operator[](int i)       { Dim4z<T, R> h; (Dim4y<T, R>&)h = *this; h.z = i; return h; }
};

template <class T, class R>
class Dim4z : Dim4y<T, R> {
	int   z;
	friend typename Dim4y<T, R>;
	
public:
	R& operator[](int i)                { return dim->At(x, y, z, i); }
};

template <class T>
class Dim4 {
	T  *data;
	int cx, cy, cz, cu;
	
	T&  At0(int x, int y, int z, int u) const;

public:
	void Create(int cx, int cy, int cz, int cu);

	T&       At(int x, int y, int z, int u)                 { return At0(x, y, z, u); }
	const T& At(int x, int y, int z, int u) const           { return At0(x, y, z, u); }
	T&       operator()(int x, int y, int z, int u)         { return At0(x, y, z, u); }
	const T& operator()(int x, int y, int z, int u) const   { return At0(x, y, z, u); }
	
	void Clear();
	void Set(const Dim4& src);
	
	Dim4& operator=(const Dim4& src);
	
	Dim4(const Dim4& src)                            { data = NULL; Set(src); }
	Dim4()                                           { data = NULL; }
	Dim4(int cx, int cy, int cz, int cu)             { data = NULL; Create(cx, cy, cz, cu); }
	~Dim4()                                          { Clear(); }
	
	Dim4x<T, T>       operator[](int i)          { Dim4x<T, T> h; h.dim = this; h.x = i; return h; }
	Dim4x<T, const T> operator[](int i) const    { Dim4x<T, const T> h; h.dim = const_cast<Dim4<T> *>(this); h.x = i; return h; }
};

#endif
