#include <CtrlCore/CtrlCore.h>

#ifdef GUI_TURTLE

NAMESPACE_UPP

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

Vector<int> SystemDraw::ImageSysData::free_handle;
int         SystemDraw::ImageSysData::handle_count;

int SystemDraw::ImageSysData::AllocImageHandle()
{
	if(free_handle.GetCount())
		return free_handle.Pop();
	return handle_count++;
}

void SystemDraw::ImageSysData::FreeImageHandle(int handle)
{
	free_handle.Add(handle);
}

void SystemDraw::ImageSysData::Init(const Image& m)
{
	img = m;
	handle = AllocImageHandle();
	LLOG("SetImage " << handle << ", size: " << img.GetSize() << ", cache size: " << SystemDraw::cache.GetSize());
	Ctrl::Put8(Ctrl::SETIMAGE);
	Ctrl::Put16(handle);
	Ctrl::Put(img.GetSize());
	Image um = Unmultiply(img);
	const RGBA *end = ~um + um.GetLength();
	for(const RGBA *s = ~um; s < end; s++) {
		Ctrl::Put8(s->r);
		Ctrl::Put8(s->g);
		Ctrl::Put8(s->b);
		Ctrl::Put8(s->a);
	}
	Ctrl::stat_setimage++;
	Ctrl::stat_setimage_len += img.GetLength() * sizeof(RGBA);
	SysImageRealized(img);
}

SystemDraw::ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	FreeImageHandle(handle);
}

struct ImageSysDataMaker : LRUCache<SystemDraw::ImageSysData, int64>::Maker {
	Image img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(SystemDraw::ImageSysData& object) const { object.Init(img); return img.GetLength(); }
};

LRUCache<SystemDraw::ImageSysData, int64> SystemDraw::cache;

void SystemDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	LLOG("Ctrl::PutImage " << p << ", size: " << img.GetSize() << ", src: " << src << ", id: " << img.GetSerialId());
	Ctrl::stat_putimage++;
	ImageSysDataMaker m;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	ImageSysData& sd = cache.Get(m);
	if(Rect(img.GetSize()) == src) {
		Point dp = p - pos;
		if(abs(dp.x) < 256 && abs(dp.y) < 256) {
			Ctrl::Put8(dp.x < 0 ? dp.y < 0 ? Ctrl::IMAGENN : Ctrl::IMAGENP
			                    : dp.y < 0 ? Ctrl::IMAGEPN : Ctrl::IMAGEPP);
			Ctrl::Put8(abs(dp.x));
			Ctrl::Put8(abs(dp.y));
			Ctrl::Put16(sd.handle);
			pos = p;
			cache.Shrink(25000000, 5000); // Cache must be after Paint because of PaintOnly!
			return;
		}
	}
	Ctrl::Put8(Ctrl::IMAGE);
	Ctrl::Put16(sd.handle);
	Ctrl::Put(p);
	Ctrl::Put(src);
	pos = p;
	cache.Shrink(25000000, 5000); // Cache must be after Paint because of PaintOnly!
}

void SystemDraw::ResetI()
{
	cache.Clear();
	ImageSysData::free_handle.Clear();
	ImageSysData::handle_count = 0;
}

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	GuiLock __;
	// Empty as CanSetSurface is false
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
	GuiLock __;
	// Empty as CanSetSurface is false
}

#define IMAGECLASS FBImg
#define IMAGEFILE <Turtle/FB.iml>
#include <Draw/iml_source.h>

#define STD_CURSOR(name, sdl) \
Image Image::name() { static Image img; ONCELOCK { img = FBImg::name(); img.SetAuxData(sdl); } return img; }

STD_CURSOR(Arrow, 1)
STD_CURSOR(Wait, 2)
STD_CURSOR(IBeam, 3)
STD_CURSOR(No, 4)
STD_CURSOR(SizeAll, 5)
STD_CURSOR(SizeHorz, 6)
STD_CURSOR(SizeVert, 7)
STD_CURSOR(SizeTopLeft, 8)
STD_CURSOR(SizeTop, 9)
STD_CURSOR(SizeTopRight, 10)
STD_CURSOR(SizeLeft, 11)
STD_CURSOR(SizeRight, 12)
STD_CURSOR(SizeBottomLeft, 13)
STD_CURSOR(SizeBottom, 14)
STD_CURSOR(SizeBottomRight, 15)
STD_CURSOR(Hand, 16)

END_UPP_NAMESPACE

#endif
