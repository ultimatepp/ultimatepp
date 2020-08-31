#include "Turtle.h"

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {

static int			sHandleCount;
static Vector<int>  sFreeHandles;
static TurtleServer::Draw sTurtleDraw;
static LRUCache<TurtleServer::ImageSysData, int64> sImageCache;

struct ImageSysDataMaker : LRUCache<TurtleServer::ImageSysData, int64>::Maker {
	Image img;
	
	int64  Key() const override
	{
		return img.GetSerialId();
	}
	
	int Make(TurtleServer::ImageSysData& object) const override
	{
		object.Init(img);
		return img.GetLength();
	}
};

static int sAllocImageHandle()
{
	return sFreeHandles.GetCount() ? sFreeHandles.Pop() : sHandleCount++;
}

static void sFreeImageHandle(int handle)
{
	sFreeHandles.Add(handle);
}

TurtleServer::ImageSysData::ImageSysData()
{
}

TurtleServer::ImageSysData::~ImageSysData()
{
	SysImageReleased(image);
	sFreeImageHandle(handle);
}

void TurtleServer::ImageSysData::Init(const Image& img)
{
	image  = img;
	handle = sAllocImageHandle();

	LLOG(Format("SetImage %`, size: %`, cache size: %",
			handle, image.GetSize(), sImageCache.GetSize()));

	TurtleServer::Put8(TurtleServer::SETIMAGE);
	TurtleServer::Put16(handle);
	TurtleServer::Put(image.GetSize());
	Image um = Unmultiply(image);
	const RGBA *end = ~um + um.GetLength();
	for(const RGBA *s = ~um; s < end; s++) {
		TurtleServer::Put8(s->r);
		TurtleServer::Put8(s->g);
		TurtleServer::Put8(s->b);
		TurtleServer::Put8(s->a);
	}
	TurtleServer::stat_setimage++;
	TurtleServer::stat_setimage_len += image.GetLength() * sizeof(RGBA);
	SysImageRealized(image);
}

TurtleServer::Draw::Draw()
{
	pos = { 0, 0 };
	PaintOnly();
	sysdraw.SetTarget(this);
	SDraw::Init(TurtleServer::desktopsize);
}

void TurtleServer::Draw::Init(const Size& sz)
{
	SDraw::Init(sz);
}

void TurtleServer::Draw::PutImage(Point p, const Image& img, const Rect& src)
{
	LLOG(Format("Turtle::Draw::PutImage %`, size: %`, src: %`, id: %",
							p, img.GetSize(), src, img.GetSerialId()));

	TurtleServer::stat_putimage++;
	ImageSysDataMaker m;

	LLOG(Format("SysImage cache pixels %`, count: %",
							sImageCache.GetSize(), sImageCache.GetCount()));

	m.img = img;
	ImageSysData& sd = sImageCache.Get(m);
	if(Rect(img.GetSize()) == src) {
		Point dp = p - pos;
		if(abs(dp.x) < 256
		&& abs(dp.y) < 256) {
			TurtleServer::Put8(dp.x < 0
				? dp.y < 0 ? TurtleServer::IMAGENN : TurtleServer::IMAGENP
			    : dp.y < 0 ? TurtleServer::IMAGEPN : TurtleServer::IMAGEPP);
			TurtleServer::Put8(abs(dp.x));
			TurtleServer::Put8(abs(dp.y));
			TurtleServer::Put16(sd.handle);
			pos = p;
			sImageCache.Shrink(25000000, 5000); // Cache must be after Paint because of PaintOnly!
			return;
		}
	}
	TurtleServer::Put8(TurtleServer::IMAGE);
	TurtleServer::Put16(sd.handle);
	TurtleServer::Put(p);
	TurtleServer::Put(src);
	pos = p;
	sImageCache.Shrink(25000000, 5000); // Cache must be after Paint because of PaintOnly!
}

void TurtleServer::Draw::PutRect(const Rect& r, Color color)
{
	LLOG("TurtleDraw::PutRect " << r << ", color " << color);

	TurtleServer::stat_putrect++;
	
	Point p = r.TopLeft();
	if(color == InvertColor()) {
		TurtleServer::Put8(TurtleServer::INVERTRECT);
		TurtleServer::Put(r);
	}
	else {
		Size sz = r.GetSize();
		Point dp = p - pos;
		if(abs(dp.x) < 256
		&& abs(dp.y) < 256
		&& sz.cx < 256
		&& sz.cy < 256
		&& 0) { // FIXME: ?
			TurtleServer::Put8(dp.x < 0
				? dp.y < 0 ? TurtleServer::RECTNN : TurtleServer::RECTNP
			    : dp.y < 0 ? TurtleServer::RECTPN : TurtleServer::RECTPP);
			TurtleServer::Put8(abs(dp.x));
			TurtleServer::Put8(abs(dp.y));
			TurtleServer::Put8(sz.cx);
			TurtleServer::Put8(sz.cy);
		}
		else {
			TurtleServer::Put8(TurtleServer::RECT);
			TurtleServer::Put(r);
		}
		TurtleServer::Put8(color.GetR());
		TurtleServer::Put8(color.GetG());
		TurtleServer::Put8(color.GetB());
	}
	pos = p;
}

SystemDraw& TurtleServer::BeginDraw()
{
	return sTurtleDraw.sysdraw;
}

void TurtleServer::CommitDraw()
{
	if(recieved_update_serial < update_serial -1) // Falling behind, wait.
		SyncClient();
	Flush();
}

void TurtleServer::SetCanvasSize(const Size& sz)
{
	sTurtleDraw.Init(sz);
}

void TurtleServer::ResetImageCache()
{
	sImageCache.Clear();
	sFreeHandles.Clear();
	sHandleCount = 0;
}

}

