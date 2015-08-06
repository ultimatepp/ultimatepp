#include "Draw.h"

NAMESPACE_UPP

Vector<Image> UnpackImlData(const void *ptr, int len)
{
	Vector<Image> img;
	String data = ZDecompress(ptr, len);
	const char *s = data;
	int version = 0;
	while(s + 6 * 2 + 1 <= data.End()) {
		ImageBuffer ib(Peek16le(s + 1), Peek16le(s + 3));
		int q = byte(*s) >> 6;
		ib.SetResolution(decode(q, 0, IMAGE_RESOLUTION_STANDARD, 1, IMAGE_RESOLUTION_UHD, IMAGE_RESOLUTION_NONE));
		ib.SetHotSpot(Point(Peek16le(s + 5), Peek16le(s + 7)));
		ib.Set2ndSpot(Point(Peek16le(s + 9), Peek16le(s + 11)));
		s += 13;
		int len = ib.GetLength();
		RGBA *t = ib;
		const RGBA *e = t + len;
		if(s + 4 * len > data.End())
			break;
		while(t < e) {
			t->a = s[3];
			t->r = s[0];
			t->g = s[1];
			t->b = s[2];
			s += 4;
			t++;
		}
		img.Add() = ib;
	}
	return img;
}

Vector<Image> UnpackImlData(const String& d)
{
	return UnpackImlData(~d, d.GetLength());
}

void Iml::Init(int n)
{
	for(int i = 0; i < n; i++)
		map.Add(name[i]);
}

void Iml::Reset()
{
	int n = map.GetCount();
	map.Clear();
	Init(n);
}

void Iml::Set(int i, const Image& img)
{
	map[i].image = img;
	map[i].loaded = true;
}

static StaticMutex sImlLock;

Image Iml::Get(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		Mutex::Lock __(sImlLock);
		if(data.GetCount()) {
			int ii = 0;
			for(;;) {
				const Data& d = data[ii];
				if(i < d.count) {
					static const char   *cached_data;
					static Vector<Image> cached;
					if(cached_data != d.data) {
						cached_data = d.data;
						cached = UnpackImlData(d.data, d.len);
						if(premultiply)
							for(int i = 0; i < cached.GetCount(); i++)
								cached[i] = Premultiply(cached[i]);
					}
					m.image = cached[i];
					break;
				}
				i -= d.count;
				ii++;
			}
		}
		else
			m.image = Premultiply(Image(img_init[i]));
		m.loaded = true;
	}
	return m.image;
}

#ifdef _DEBUG
int  Iml::GetBinSize() const
{
	int size = 0;
	for(int i = 0; i < map.GetCount(); i++) {
		const Image::Init& init = img_init[i];
		size += (int)strlen(name[i]) + 1 + 24;
		for(int q = 0; q < init.scan_count; q++)
			size += (int)strlen(init.scans[q]);
	}
	return size;
}
#endif

Iml::Iml(const Image::Init *img_init, const char **name, int n)
:	img_init(img_init),
	name(name)
{
#ifdef flagCHECKINIT
	RLOG("Constructing iml " << *name);
#endif
	premultiply = true;
	Init(n);
}

void Iml::AddData(const byte *_data, int len, int count)
{
	Data& d = data.Add();
	d.data = (const char *)_data;
	d.len = len;
	d.count = count;
	data.Shrink();
}

static StaticCriticalSection sImgMapLock;

static VectorMap<String, Iml *>& sImgMap()
{
	static VectorMap<String, Iml *> x;
	return x;
}

void Register(const char *imageclass, Iml& list)
{
#ifdef flagCHECKINIT
	RLOG("Registering iml " << imageclass);
#endif
	INTERLOCKED_(sImgMapLock)
		sImgMap().GetAdd(imageclass) = &list;
}

int GetImlCount()
{
	int q = 0;
	INTERLOCKED_(sImgMapLock)
		q = sImgMap().GetCount();
	return q;
}

Iml& GetIml(int i)
{
	return *sImgMap()[i];
}

String GetImlName(int i)
{
	Mutex::Lock __(sImlLock);
	return sImgMap().GetKey(i);
}

int FindIml(const char *name)
{
	Mutex::Lock __(sImlLock);
	return sImgMap().Find(name);
}

Image GetImlImage(const char *name)
{
	Image m;
	const char *w = strchr(name, ':');
	if(w) {
		Mutex::Lock __(sImlLock);
		int q = FindIml(String(name, w));
		if(q >= 0) {
			Iml& iml = *sImgMap()[q];
			while(*w == ':')
				w++;
			q = iml.Find(w);
			if(q >= 0)
				m = iml.Get(q);
		}
	}
	return m;
}

void SetImlImage(const char *name, const Image& m)
{
	const char *w = strchr(name, ':');
	if(w) {
		Mutex::Lock __(sImlLock);
		int q = FindIml(String(name, w));
		if(q >= 0) {
			Iml& iml = *sImgMap()[q];
			while(*w == ':')
				w++;
			q = iml.Find(w);
			if(q >= 0)
				iml.Set(q, m);
		}
	}
}

END_UPP_NAMESPACE
