template <class T>
void Dim2<T>::Create(int cx_, int cy_)
{
	cx = cx_; cy = cy_;
	ASSERT(cx >= 0 && cy >= 0);
	if(data)
		delete[] data;
	data = new T[cx * cy];
}

template <class T>
T& Dim2<T>::At0(int x, int y) const
{
	ASSERT(x >= 0 && y >= 0 && x < cx && y < cy);
	return data[x * cy + y];
}

template <class T>
void Dim2<T>::Clear()
{
	if(data)
		delete[] data;
	cx = cy = 0;
}

template <class T>
void Dim2<T>::Set(const Dim2& src)
{
	Create(src.cx, src.cy);
	Copy(data, src.data, src.data + cx * cy);
}

template <class T>
Dim2<T>& Dim2<T>::operator=(const Dim2& src)
{
	if(&src == this) return *this;
	Set(src);
	return *this;
}

template <class T>
void Dim3<T>::Create(int cx_, int cy_, int cz_)
{
	cx = cx_; cy = cy_; cz = cz_;
	ASSERT(cx >= 0 && cy >= 0 && cz >= 0);
	if(data)
		delete[] data;
	data = new T[cx * cy * cz];
}

template <class T>
T& Dim3<T>::At0(int x, int y, int z) const
{
	ASSERT(x >= 0 && y >= 0 && z >= 0 && x < cx && y < cy && z < cz);
	return data[cz * (x * cy + y) + z];
}

template <class T>
void Dim3<T>::Clear()
{
	if(data)
		delete[] data;
	cx = cy = cz = 0;
}

template <class T>
void Dim3<T>::Set(const Dim3& src)
{
	Create(src.cx, src.cy, src.cz);
	Copy(data, src.data, src.data + cx * cy * cz);
}

template <class T>
Dim3<T>& Dim3<T>::operator=(const Dim3& src)
{
	if(&src == this) return *this;
	Set(src);
	return *this;
}

template <class T>
void Dim4<T>::Create(int cx_, int cy_, int cz_, int cu_)
{
	cx = cx_; cy = cy_; cz = cz_; cu = cu_;
	ASSERT(cx >= 0 && cy >= 0 && cz >= 0 && cy >= 0);
	if(data)
		delete[] data;
	data = new T[cx * cy * cz * cu];
}

template <class T>
T& Dim4<T>::At0(int x, int y, int z, int u) const
{
	ASSERT(x >= 0 && y >= 0 && z >= 0 && u >= 0 && x < cx && y < cy && z < cz && u < cu);
	return data[cu * (cz * (x * cy + y) + z) + u];
}

template <class T>
void Dim4<T>::Clear()
{
	if(data)
		delete[] data;
	cx = cy = cz = cu = 0;
}

template <class T>
void Dim4<T>::Set(const Dim4& src)
{
	Create(src.cx, src.cy, src.cz, src.cu);
	Copy(data, src.data, src.data + cx * cy * cz * cu);
}

template <class T>
Dim4<T>& Dim4<T>::operator=(const Dim4& src)
{
	if(&src == this) return *this;
	Set(src);
	return *this;
}

template <class T>
void Dim5<T>::Create(int cx_, int cy_, int cz_, int cu_, int cv_)
{
	cx = cx_; cy = cy_; cz = cz_; cu = cu_; cv = cv_;
	ASSERT(cx >= 0 && cy >= 0 && cz >= 0 && cu >= 0 && cv >= 0);
	if(data)
		delete[] data;
	data = new T[cx * cy * cz * cu * cv];
}

template <class T>
T& Dim5<T>::At0(int x, int y, int z, int u, int v) const
{
	ASSERT(x >= 0 && y >= 0 && z >= 0 && u >= 0 && v >= 0 && x < cx && y < cy && z < cz && u < cu && v < cv);
	return data[cv * (cu * (cz * (x * cy + y) + z) + u) + v];
}

template <class T>
void Dim5<T>::Clear()
{
	if(data)
		delete[] data;
	cx = cy = cz = cu = cv = 0;
}

template <class T>
void Dim5<T>::Set(const Dim5& src)
{
	Create(src.cx, src.cy, src.cz, src.cu, src.cv);
	Copy(data, src.data, src.data + cx * cy * cz * cu * cv);
}

template <class T>
Dim5<T>& Dim5<T>::operator=(const Dim5& src)
{
	if(&src == this) return *this;
	Set(src);
	return *this;
}
