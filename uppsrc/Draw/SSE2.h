#ifdef CPU_SIMD

force_inline
i16x8 LoadRGBA(const RGBA *s)
{
	return Unpack8L(i32x4(*(dword *)s));
}

force_inline
i16x8 LoadRGBA2(const RGBA *s0, const RGBA *s1)
{
	return Unpack8L(i32x4(0, 0, *(dword *)s1, *(dword *)s0));
}

force_inline
i16x8 LoadRGBA2(const RGBA& c)
{
	return LoadRGBA2(&c, &c);
}

force_inline
i16x8 LoadRGBA2(const RGBA *s)
{
	i16x8 h;
	h.Load64(s);
	return Unpack8L(h);
}

force_inline
i16x8 LoadRGBAL(i16x8 x)
{
	return Unpack8L(x);
}

force_inline
i16x8 LoadRGBAH(i16x8 x)
{
	return Unpack8H(x);
}

force_inline
void LoadRGBA4(const RGBA *s, i16x8& l, i16x8& h)
{
	i16x8 t4(s);
	l = LoadRGBAL(t4);
	h = LoadRGBAH(t4);
}

force_inline
i8x16 PackRGBA(i16x8 l, i16x8 h)
{
	return Pack16(l, h);
}

force_inline
void StoreRGBA(RGBA *rgba, i16x8 x)
{
	PackRGBA(x, 0).Store32(rgba);
}

force_inline
void StoreRGBA2(RGBA *rgba, i16x8 x)
{
	PackRGBA(x, 0).Store64(rgba);
}

force_inline
void StoreRGBA4(RGBA *rgba, i16x8 l, i16x8 h)
{
	PackRGBA(l, h).Store(rgba);
}

force_inline
f32x4 LoadRGBAF(const RGBA *s)
{
	return ToFloat(Unpack16L(Unpack8L(i16x8().Load32(s))));
}

force_inline
void StoreRGBAF(RGBA *t, f32x4 s)
{
	Pack16(Pack32(Truncate(s))).Store32(t);
}

force_inline
f32x4 ClampRGBAF(f32x4 p)
{
#ifdef PLATFORM_MACOS
	f32x4 alpha = Broadcast0(p);
#else
	f32x4 alpha = Broadcast3(p);
#endif
	alpha = min(alpha, f32all(255.0));
	return min(p, alpha);
}

#endif
