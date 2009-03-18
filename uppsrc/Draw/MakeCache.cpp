#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

static StaticCriticalSection sMakeImage;

static LRUCache<Image>& sImageCache()
{
	static LRUCache<Image> m;
	return m;
}

struct scImageMaker : LRUCache<Image>::Maker {
	const ImageMaker *m;
	bool  paintonly;

	virtual String Key() const {
//		String q = m->Key();
		return m->Key();
	}
	virtual int    Make(Image& object) const {
		object = m->Make();
		if(paintonly) {
			SetPaintOnly___(object);
			return object.GetLength() + 20000;
		}
		return object.GetLength() + 100;
	}
};

static int sMaxSize;
static int sMaxSizeMax = 4000000;

void ClearMakeImageCache()
{
	sImageCache().Clear();
}

void SetMakeImageCacheMax(int m)
{
	sMaxSizeMax = m;
}

void  SetMakeImageCacheSize(int m)
{
	sMaxSize = m;
}

void SweepMkImageCache()
{
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		cache.ClearCounters();
	}
}

Image MakeImage__(const ImageMaker& m, bool paintonly)
{
	Image result;
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		scImageMaker cm;
		cm.m = &m;
		cm.paintonly = paintonly;
		result = cache.Get(cm);
		int q = min(cache.GetFoundSize() + cache.GetNewSize(), sMaxSizeMax);
		if(q > sMaxSize) {
			sMaxSize = q;
			LLOG("ImageCache: Increasing maxsize to " << sMaxSize);
		}
		cache.Shrink(sMaxSize);
	}
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
	int64 k = image.GetSerialId();
	key.Cat((const char *)&k, sizeof(int64));
	key.Cat((const char *)&make, sizeof(make));
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

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, src.left);
		RawCat(h, src.top);
		RawCat(h, src.right);
		RawCat(h, src.bottom);
		RawCat(h, sz.cx);
		RawCat(h, sz.cy);
		RawCat(h, img.GetSerialId());
		return h;
	}

	virtual Image Make() const {
		return Rescale(img, sz, src);
	}

};

Image CachedRescale(const Image& m, Size sz, const Rect& src)
{
	if(m.GetSize() == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	return MakeImage(cr);
}

Image CachedRescale(const Image& m, Size sz)
{
	return CachedRescale(m, sz, m.GetSize());
}

Image CachedRescalePaintOnly(const Image& m, Size sz, const Rect& src)
{
	if(m.GetSize() == sz)
		return m;
	sCachedRescale cr;
	cr.sz = sz;
	cr.src = src;
	cr.img = m;
	return MakeImagePaintOnly(cr);
}

Image CachedRescalePaintOnly(const Image& m, Size sz)
{
	return CachedRescalePaintOnly(m, sz, m.GetSize());
}


END_UPP_NAMESPACE
