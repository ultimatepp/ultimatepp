#include "Draw.h"

namespace Upp {

Vector<ImageIml> UnpackImlData(const void *ptr, int len)
{
	Vector<ImageIml> img;
	String data = ZDecompress(ptr, len);
	const char *s = data;
	while(s + 6 * 2 + 1 <= data.End()) {
		ImageIml& m = img.Add();
		ImageBuffer ib(Peek16le(s + 1), Peek16le(s + 3));
		m.flags = byte(*s) & 0x3f;
		ib.SetResolution(decode(byte(*s) >> 6, 0, IMAGE_RESOLUTION_STANDARD,
		                                       1, IMAGE_RESOLUTION_UHD,
		                                       IMAGE_RESOLUTION_NONE));
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
		m.image = ib;
	}
	return img;
}

Vector<ImageIml> UnpackImlData(const String& d)
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

ImageIml Iml::GetRaw(int mode, int i)
{
	Mutex::Lock __(sImlLock);
	if(data[mode].GetCount()) {
		int ii = 0;
		for(;;) {
			const Data& d = data[mode][ii];
			if(i < d.count) {
				static const char *cached_data[4];
				static Vector<ImageIml> cached[4];
				if(cached_data[mode] != d.data) { // cache single .iml
					cached_data[mode] = d.data;
					cached[mode] = UnpackImlData(d.data, d.len);
					if(premultiply)
						for(int i = 0; i < cached[mode].GetCount(); i++)
							cached[mode][i].image = Premultiply(cached[mode][i].image);
				}
				return cached[mode][i];
			}
			i -= d.count;
			ii++;
		}
	}
	return ImageIml();
}

ImageIml Iml::GetRaw(int mode, const String& id)
{
	ASSERT(mode >= 0 && mode < 4);
	int ii = -1;
	if(mode == 0)
		ii = map.Find(id);
	else {
		ii = ex_name[mode - 1].Find(id);
	}
	return ii >= 0 ? GetRaw(mode, ii) : ImageIml();
}

Image Iml::Get(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		Mutex::Lock __(sImlLock);
		if(!m.loaded) {
			int mode = IsUHDMode() * GUI_MODE_UHD + IsDarkTheme() * GUI_MODE_DARK;
			String id = GetId(i);
			if(mode == GUI_MODE_NORMAL)
				m.image = GetRaw(GUI_MODE_NORMAL, i).image;
			else {
				auto Mode = [&](dword m, const char *s) { return mode & m ? String(s) : String(); };
				m.image = GetRaw(GUI_MODE_NORMAL, id + Mode(GUI_MODE_UHD, "__UHD") + Mode(GUI_MODE_DARK, "__DARK")).image;
				if(IsNull(m.image))
					m.image = GetRaw(mode, id).image;
				if(IsNull(m.image)) {
					ImageIml im;
					dword f = im.flags | global_flags;
					if(mode & GUI_MODE_DARK) {
						im = GetRaw(0, id + "__DARK");
						if(IsNull(im.image))
							im = GetRaw(GUI_MODE_DARK, id);
						if(IsNull(im.image)) {
							im = GetRaw(GUI_MODE_NORMAL, id);
							if(!(f & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_COLORS)))
								im.image = DarkTheme(im.image);
						}
					}
					else
						im = GetRaw(GUI_MODE_NORMAL, id);
					if((mode & GUI_MODE_UHD) && !(f & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE)))
						im.image = Upscale2x(im.image);
					m.image = im.image;
				}
				if(!IsNull(m.image) && (mode & GUI_MODE_UHD)) // this is to support legacy code mostly
					SetResolution(m.image, IMAGE_RESOLUTION_UHD);
			}
			m.loaded = true;
		}
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
	premultiply = true;
	Init(n);
}

void Iml::AddData(const byte *s, int len, int count, int mode)
{
	Data& d = data[mode].Add();
	d.data = (const char *)s;
	d.len = len;
	d.count = count;
	data[mode].Shrink();
}

void Iml::AddId(int mode1, const char *name)
{
	DDUMP(name);
	DDUMP(mode1);
	ex_name[mode1].Add(name);
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

}
