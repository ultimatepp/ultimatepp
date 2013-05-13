#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x)    // DLOG(x)

static StaticCriticalSection sMakeImage;

// SystemDraw image cache can get destructed later than maker cache invoking SysImageReleased
static bool sFinished;  

struct ImageMakeCacheClass : LRUCache<Image> {
	~ImageMakeCacheClass() { sFinished = true; }
};

static LRUCache<Image>& sImageCache()
{
	static ImageMakeCacheClass m;
	return m;
}

struct scImageMaker : LRUCache<Image>::Maker {
	const ImageMaker *m;
	bool  paintonly;

	virtual String Key() const {
		return m->Key();
	}
	virtual int    Make(Image& object) const {
		object = m->Make();
		LLOG("ImageMaker " << object.GetSerialId() << ", size " << object.GetSize() << ", paintonly: " << paintonly);
		if(paintonly && !IsNull(object))
			SetPaintOnly__(object);
		return object.GetLength() + 100;
	}
};

static int sMaxSize;
static int sMaxSizeMax = 1000000;

int ImageSizeAdjuster(const Image& img)
{
	return ~img ? img.GetLength() + 100 : 100;
}

void SysImageRealized(const Image& img)
{ // CtrlCore created handle for img, no need to keep pixels data in cache if it is paintonly kind
	Mutex::Lock __(sMakeImage);
	if(sFinished)
		return;
	LLOG("SysImageRealized " << img.GetSize());
	if(img.data && img.data->paintonly) {
		LLOG("Dropping PAINTONLY image " << img.GetSerialId() << " pixels, cache size: " << sImageCache().GetSize() << ", img " << img.GetLength());
		DropPixels___(img.data->buffer);
		sImageCache().AdjustSize(ImageSizeAdjuster);
		LLOG("After drop, cache size: " << sImageCache().GetSize());
	}
}

struct ImageRemover {
	int64 serial_id;
	
	bool operator()(const Image& img) const {
		if(serial_id == img.GetSerialId()) {
			LLOG("ImageCache Removing " << serial_id);
			return true;
		}
		return false;
	}
};

void SysImageReleased(const Image& img)
{ // CtrlCore removed handle for img, have to remove paintonly
	Mutex::Lock __(sMakeImage);
	if(sFinished)
		return;
	if(!~img) {
		ImageRemover ir;
		ir.serial_id = img.GetSerialId();
		LLOG("SysImageReleased " << img.GetSerialId() << ", cache size: " << sImageCache().GetSize() << ", count " << sImageCache().GetCount());
		int n = sImageCache().RemoveOne(ir);
		IGNORE_RESULT(n); // suppress warning about unused 'n'
		LLOG("SysImageReleased count: " << n);
		LLOG("SysImageReleased done cache size: " << sImageCache().GetSize() << ", count " << sImageCache().GetCount());
	}
}

void ClearMakeImageCache()
{
	Mutex::Lock __(sMakeImage);
	sImageCache().Clear();
}

void SetMakeImageCacheMax(int m)
{
	Mutex::Lock __(sMakeImage);
	sMaxSizeMax = m;
}

void  SetMakeImageCacheSize(int m)
{
	Mutex::Lock __(sMakeImage);
	sMaxSize = m;
}

void SweepMkImageCache()
{
	Mutex::Lock __(sMakeImage);
	LRUCache<Image>& cache = sImageCache();
	cache.ClearCounters();
}

Image MakeImage__(const ImageMaker& m, bool paintonly)
{
	Mutex::Lock __(sMakeImage);
	Image result;
	LRUCache<Image>& cache = sImageCache();
	scImageMaker cm;
	cm.m = &m;
	cm.paintonly = paintonly;
	LLOG("MakeImage before shrink: " << cache.GetSize() << ", count " << cache.GetCount());
	cache.Shrink(sMaxSize, 2000);
	LLOG("MakeImage before make: " << cache.GetSize() << ", count " << cache.GetCount());
	result = cache.Get(cm);
	LLOG("MakeImage after make: " << cache.GetSize() << ", count " << cache.GetCount());
	int q = min(3 * (cache.GetFoundSize() + cache.GetNewSize()), sMaxSizeMax);
	if(q > sMaxSize)
		sMaxSize = q;
	return result;
}

Image MakeImage(const ImageMaker& m)
{
	return MakeImage__(m, false);
}

Image MakeImagePaintOnly(const ImageMaker& m)
{
	return MakeImage__(m, true);
}

class SimpleImageMaker : public ImageMaker {
	Image (*make)(const Image& image);
	Image image;

public:
	virtual String Key() const;
	virtual Image  Make() const;

	SimpleImageMaker(const Image& image, Image (*make)(const Image& image))
	:	make(make),image(image) {}
};

String SimpleImageMaker::Key() const
{
	String key;
	RawCat(key, image.GetSerialId());
	RawCat(key, make);
	return key;
}

Image SimpleImageMaker::Make() const
{
	return (*make)(image);
}

Image MakeImage(const Image& image, Image (*make)(const Image& image))
{
	return MakeImage(SimpleImageMaker(image, make));
}

struct sCachedRescale : public ImageMaker
{
	Rect  src;
	Size  sz;
	Image img;
	int   filter;

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, src.left);
		RawCat(h, src.top);
		RawCat(h, src.right);
		RawCat(h, src.bottom);
		RawCat(h, sz.cx);
		RawCat(h, sz.cy);
		RawCat(h, img.GetSerialId());
		RawCat(h, filter);
		return h;
	}

	virtual Image Make() const {
		return IsNull(filter) ? Rescale(img, sz, src) : RescaleFilter(img, sz, src, filter);
	}
};

Image CachedRescale(const Image& m, Size sz, const Rect& src, int filter)
{
	if(m.GetSize() == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	cr.filter = filter;
	return MakeImage(cr);
}

Image CachedRescale(const Image& m, Size sz, int filter)
{
	return CachedRescale(m, sz, m.GetSize(), filter);
}

Image CachedRescalePaintOnly(const Image& m, Size sz, const Rect& src, int filter)
{
	if(m.GetSize() == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	cr.filter = filter;
	return MakeImagePaintOnly(cr);
}

Image CachedRescalePaintOnly(const Image& m, Size sz, int filter)
{
	return CachedRescalePaintOnly(m, sz, m.GetSize(), filter);
}


END_UPP_NAMESPACE
