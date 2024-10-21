//#BLITZ_APPROVE

#define IMAGE_META(k, v)

#define PREMULTIPLIED
#define IMAGE_ID(n)
#define IMAGE_BEGIN_DATA
#define IMAGE_END_DATA(n, c)
#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)

#define IMAGE_BEGIN(n) const char *COMBINE(COMBINE(IMAGECLASS, _), n##__scans__)[] = {
#define IMAGE_SCAN(s)  s,
#define IMAGE_PACKED(n, d) };
#define IMAGE_DATA_BEGIN

#include IMAGEFILE

#undef  IMAGE_BEGIN
#undef  IMAGE_SCAN
#undef  IMAGE_PACKED

// -----------------------

#define IMAGE_SCAN(s)
#define IMAGE_BEGIN(n)

// -----------------------


UPP::Iml& IMAGECLASS::Iml() {
	static const char *name[IMAGECLASS::COUNT] = {
	#define IMAGE_PACKED(n, d) #n,
	#undef IMAGE_ID
	#define IMAGE_ID(n) #n,
		#include IMAGEFILE
	};

	static UPP::Iml iml(name, COUNT);
	
	#ifdef FIXED_COLORS
	#undef FIXED_COLORS
	iml.GlobalFlag(IML_IMAGE_FLAG_FIXED_COLORS);
	#endif

	#ifdef FIXED_SIZE
	#undef FIXED_SIZE
	iml.GlobalFlag(IML_IMAGE_FLAG_FIXED_SIZE);
	#endif

	#undef IMAGE_PACKED
	#undef IMAGE_ID
	#define IMAGE_ID(n)
	#define IMAGE_PACKED(n, d)

	ONCELOCK {

	#undef IMAGE_BEGIN_DATA
	#undef IMAGE_DATA
	#undef IMAGE_END_DATA
	#define IMAGE_BEGIN_DATA { static const UPP::byte data[] = {
	#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)\
	                   a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf,

	#define IMAGE_END_DATA(n, c)   }; iml.AddData(data, n, c); }

	#include IMAGEFILE

	#ifdef IMAGEFILE_DARK
		#undef  IMAGE_ID
		#undef  IMAGE_END_DATA
		#define IMAGE_ID(n)            iml.AddId(0, #n);
		#define IMAGE_END_DATA(n, c)   }; iml.AddData(data, n, c, 1); }
		#include IMAGEFILE_DARK
	#endif
	
	#ifdef IMAGEFILE_UHD
		#undef IMAGE_ID
		#undef IMAGE_END_DATA
		#define IMAGE_ID(n)            iml.AddId(1, #n);
		#define IMAGE_END_DATA(n, c)   }; iml.AddData(data, n, c, 2); }
		#include IMAGEFILE_UHD
	#endif

	#ifdef IMAGEFILE_DARK_UHD
		#undef IMAGE_ID
		#undef IMAGE_END_DATA
		#define IMAGE_ID(n)            iml.AddId(2, #n);
		#define IMAGE_END_DATA(n, c)   }; iml.AddData(data, n, c, 3); }
		#include IMAGEFILE_DARK_UHD
	#endif

	#undef IMAGE_BEGIN_DATA
	#undef IMAGE_END_DATA
	#undef IMAGE_DATA
	#undef IMAGE_ID
	#define IMAGE_BEGIN_DATA
	#define IMAGE_END_DATA(n, c)
	#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)
	#define IMAGE_ID(n)

	#undef PREMULTIPLIED
	#define PREMULTIPLIED iml.Premultiplied();
		#include IMAGEFILE
	#undef PREMULTIPLIED
	#define PREMULTIPLIED

	}
	return iml;
}

UPP::Image IMAGECLASS::Get(int i)
{
	return Iml().Get(i);
}

UPP::Image IMAGECLASS::Get(const char *s)
{
	return Iml().Get(Find(s));
}

UPP::Image IMAGECLASS::Get(const UPP::String& s)
{
	return Iml().Get(Find(s));
}

int   IMAGECLASS::Find(const UPP::String& s)
{
	return Iml().Find(s);
}

int   IMAGECLASS::Find(const char *s)
{
	return Iml().Find(s);
}

void  IMAGECLASS::Set(int i, const UPP::Image& m)
{
	Iml().Set(i, m);
}

void  IMAGECLASS::Set(const char *s, const UPP::Image& m)
{
	Iml().Set(Find(s), m);
}

struct COMBINE(IMAGECLASS, __Reg) {
	COMBINE(IMAGECLASS, __Reg()) {
		IMAGECLASS::Register__();
	}
};

static COMBINE(IMAGECLASS, __Reg) COMBINE(IMAGECLASS, ___Reg);

#undef  IMAGE_BEGIN
#undef  IMAGE_ID

#define IMAGE_BEGIN(n) UPP::Image IMAGECLASS::n() { return Get(I_##n); }
#define IMAGE_ID(n) UPP::Image IMAGECLASS::n() { return Get(I_##n); }
#include IMAGEFILE
#undef  IMAGE_BEGIN
#undef  IMAGE_ID

#undef  IMAGE_BEGIN_DATA
#undef  IMAGE_DATA
#undef  IMAGE_END_DATA
#undef  IMAGE_PACKED
#undef  IMAGE_ID

#undef  IMAGE_SCAN
#undef  IMAGE_BEGIN

#ifndef IMAGE_KEEP
#undef  IMAGECLASS
#undef  IMAGEFILE
#endif

#undef  IMAGE_META

#ifdef IMAGEFILE_UHD
#undef IMAGEFILE_UHD
#endif

#ifdef IMAGEFILE_DARK
#undef IMAGEFILE_DARK
#endif

#ifdef IMAGEFILE_DARK_UHD
#undef IMAGEFILE_DARK_UHD
#endif
