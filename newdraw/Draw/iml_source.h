//#BLITZ_APPROVE

//$

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
	static UPP::Image::Init init[] = {
	#define IMAGE_PACKED(n, d) { COMBINE(COMBINE(IMAGECLASS, _), n##__scans__), __countof(COMBINE(COMBINE(IMAGECLASS, _), n##__scans__)), d },

		#include IMAGEFILE

		{ NULL }
	#undef  IMAGE_PACKED
	};

	static const char *name[IMAGECLASS::COUNT] = {
	#define IMAGE_PACKED(n, d) #n,
	#undef  IMAGE_ID
	#define IMAGE_ID(n) #n,

		#include IMAGEFILE

	#undef  IMAGE_PACKED
	#undef  IMAGE_ID
	#define IMAGE_ID(n)
	#define IMAGE_PACKED(n, d)
	};
	static UPP::Iml iml(init, name, COUNT);
	static bool imlinit;
	if(!imlinit) {
		imlinit = true;

	#undef  IMAGE_BEGIN_DATA
	#undef  IMAGE_DATA
	#undef  IMAGE_END_DATA
	#define IMAGE_BEGIN_DATA { static const UPP::byte data[] = {
	#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)\
	                   a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf,

	#define IMAGE_END_DATA(n, c)   }; iml.AddData(data, n, c); }

		#include IMAGEFILE

	#undef IMAGE_BEGIN_DATA
	#undef IMAGE_END_DATA
	#undef IMAGE_DATA
	#define IMAGE_BEGIN_DATA
	#define IMAGE_END_DATA(n, c)
	#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)

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
