#include "Draw.h"

namespace Upp {

Vector<ImageIml> UnpackImlDataUncompressed(const String& data)
{
	Vector<ImageIml> img;
	const char *s = data;
	while(s + 6 * 2 + 1 <= data.End()) {
		ImageIml& m = img.Add();
		ImageBuffer ib(Peek16le(s + 1), Peek16le(s + 3));
		m.flags = byte(*s);
		ib.SetHotSpot(Point(Peek16le(s + 5), Peek16le(s + 7)));
		ib.Set2ndSpot(Point(Peek16le(s + 9), Peek16le(s + 11)));
		s += 13;
		size_t len = ib.GetLength();
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

Vector<ImageIml> UnpackImlData(const void *ptr, int len)
{
	return UnpackImlDataUncompressed(ZDecompress(ptr, len));
}

Vector<ImageIml> UnpackImlData(const String& d)
{
	return UnpackImlData(~d, d.GetLength());
}

void iml_ReplaceAll(Image& tgt, Image& src)
{ // this very special function replaces all unmodified instances of Image with new content
	if(tgt.GetSize() == src.GetSize() && tgt.data) {
		tgt.data->buffer = src.data->buffer;
		tgt.data->NewSerial();
	}
	else
		tgt = src;
}

void Iml::Init(int n)
{
	for(int i = 0; i < n; i++)
		map.Add(name[i]);
}

void Iml::Reset()
{
	for(IImage& m : map)
		m.loaded = false;
}

void Iml::Skin()
{
	for(int i = 0; i < map.GetCount(); i++)
		if(map[i].loaded) {
			map[i].loaded = false;
			Get(i);
		}
}

static StaticMutex sImlLock;

void Iml::Set(int i, const Image& img)
{ // TODO: MT
	IImage& m = map[i];
	Image h = img; // make sure h has refcount 1, basically 'clone'
	ImageBuffer ib = h;
	h = ib;
	iml_ReplaceAll(m.image, h);
	m.loaded = true;
}

Image Iml::Get(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		Mutex::Lock __(sImlLock);
		if(!m.loaded) {
			Image h = MakeImlImage(GetId(i), [&](int mode, const String& id) { return GetRaw(mode, id); }, global_flags);
			iml_ReplaceAll(m.image, h);
			m.loaded = true;
		}
	}
	return m.image;
}

ImageIml Iml::GetRaw(int mode, int i)
{
	Mutex::Lock __(sImlLock);
	if(data[mode].GetCount()) {
		int ii = 0;
		while(ii < data[mode].GetCount()) {
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
	else
		ii = ex_name[mode - 1].Find(id);
	return ii >= 0 ? GetRaw(mode, ii) : ImageIml();
}

Image MakeImlImage(const String& id, Function<ImageIml(int, const String& id)> GetRaw, dword global_flags)
{
	int mode = IsUHDMode() * GUI_MODE_UHD + IsDarkTheme() * GUI_MODE_DARK;
	
	const static int mode_candidates[4][4] = {
		{ GUI_MODE_NORMAL, GUI_MODE_UHD, -1 },
		{ GUI_MODE_DARK, GUI_MODE_DARK_UHD, GUI_MODE_NORMAL, GUI_MODE_UHD },
		{ GUI_MODE_UHD, GUI_MODE_NORMAL, -1 },
		{ GUI_MODE_DARK_UHD, GUI_MODE_DARK, GUI_MODE_UHD, GUI_MODE_NORMAL }
	};
	
	ImageIml im;
	const int *candidates = mode_candidates[mode];
	
	for(int i = 0; i < 4 && candidates[i] >= 0; i++) {
		int cmode = candidates[i];
		auto Mode = [&](dword m, const char *s) { return cmode & m ? String(s) : String(); };
		im = GetRaw(GUI_MODE_NORMAL, id + Mode(GUI_MODE_UHD, "__UHD") + Mode(GUI_MODE_DARK, "__DARK"));
		if(IsNull(im.image))
			im = GetRaw(cmode, id); // try alternative iml
		if(!IsNull(im.image)) {
			if(im.flags & IML_IMAGE_FLAG_S3)
				im.image = DownSample3x(im.image);
			break;
		}
	}
	
	if(IsNull(im.image))
		return Null;

	if(!(mode & GUI_MODE_UHD) && (im.flags & IML_IMAGE_FLAG_UHD) && !((im.flags | global_flags) & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE)))
		im.image = Downscale2x(im.image);
	if((mode & GUI_MODE_UHD) && !(im.flags & IML_IMAGE_FLAG_UHD) && !((im.flags | global_flags) & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE)))
		im.image = Upscale2x(im.image);
	if((mode & GUI_MODE_DARK) && !(im.flags & IML_IMAGE_FLAG_DARK) && !((im.flags | global_flags) & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_COLORS)))
		im.image = DarkTheme(im.image);

	ScanOpaque(im.image);
	
	return im.image;
}

Iml::Iml(const char **name, int n)
:	name(name)
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
	ex_name[mode1].Add(name);
}

void Iml::ResetAll()
{
	for(int i = 0; i < GetImlCount(); i++)
		GetIml(i).Reset();
}

void Iml::SkinAll()
{
	for(int i = 0; i < GetImlCount(); i++)
		GetIml(i).Skin();
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
