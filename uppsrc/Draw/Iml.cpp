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

void iml_ReplaceAll(Image& tgt, const Image& src)
{ // this very special function replaces all unmodified instances of Image with new content
	Image h = src; // make sure src has refcount 1, basically 'clone'
	ImageBuffer ib = h;
	h = ib;
	if(tgt.GetSize() == h.GetSize() && tgt.data && src.data) {
		tgt.data->buffer = h.data->buffer;
		tgt.data->NewSerial();
	}
	else
		tgt = src;
}

void Iml::Init(int n)
{
	for(int i = 0; i < n; i++)
		map.Add(name[i]);
	flags.Alloc(map.GetCount(), IML_IMAGE_FLAGS_UNKNOWN);
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

void Iml::AddData(const byte *s, int len, int count)
{
	Data& d = data.Add();
	d.data = (const char *)s;
	d.len = len;
	d.count = count;
	data.Shrink();
}

void Iml::AddId(int mode1, const char *name)
{
	map.Add(name);
}

static StaticMutex sImlLock;

ImageIml Iml::GetRaw(int i)
{
	int i0 = 0;
	for(const Data& d : data) {
		if(i >= i0 && i < i0 + d.count) {
			ImageIml m = MakeValue(
				[&] {
					String key;
					RawCat(key, d.data); // all is static
					return key;
				},
				[&](Value& v) {
					Vector<ImageIml>& m = CreateRawValue<Vector<ImageIml>>(v);
					m = UnpackImlData(d.data, d.len);
					int sz = 0;
					int ii = i0;
					for(ImageIml& img : m) {
						sz += img.image.GetLength();
						if(premultiply)
							img.image = Premultiply(img.image);
						if(version == 0) // cleanup some bits that are set for legacy reasons
							img.flags &= 0x3f;
						flags[ii++] = img.flags;
					}
					return sz;
				}
			).To<Vector<ImageIml>>()[i - i0];
			m.flags |= global_flags;
			return m;
		}
		i0 += d.count;
	}
	return ImageIml();
}

void Iml::Set(int i, const Image& img)
{ // TODO: MT
	IImage& m = map[i];
	iml_ReplaceAll(m.image, img);
	m.loaded = true;
}

Image MakeImlImage(const String& id, Event<int, dword&, String&> GetRawFlags, Function<ImageIml(int)> GetRaw)
{
	int  scale = GetDPIScale();
	bool dark = IsDarkTheme();
	int best_i  = -1;
	int exact_scale_i = -1;
	int best_dark = 10; // minimize this
	int best_scale = -1; // maximize this
	int ii = 0;
	for(;;) {
		dword    flags;
		String   iid;
		GetRawFlags(ii, flags, iid);
		if(IsNull(iid))
			break;
		int q = iid.Find("__");
		if(q > 0)
			iid.Trim(q);
		if(iid == id) {
			bool isdark = flags & IML_IMAGE_FLAG_DARK;
			int  iscale = ImlFlagsToDPIScale(flags);
			
			if(isdark == dark && iscale == scale) // found perfect match
				return GetRaw(ii).image;
			
			if(iscale == scale)
				exact_scale_i = ii;
			
			int  idark = !!isdark - dark;
			if(CombineCompare(best_dark, idark)(iscale, best_scale) > 0) { // prioritize color, then find highest scale
				best_i = ii;
				best_dark = idark;
				best_scale = iscale;
			}
		}
		ii++;
	}
	
	auto AdjustColor0 = [&](const Image& img, dword flags) { // flip light to dark if needed
		if(dark && !(flags & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_COLORS|IML_IMAGE_FLAG_DARK)))
			return DarkTheme(img);
		return img;
	};

	if(best_dark && exact_scale_i >= 0) { // dark color version not found, but we have exact scale
		ImageIml m = GetRaw(exact_scale_i);
		return AdjustColor0(m.image, m.flags);
	}
	
	if(best_i < 0)
		return Null;
	
	ImageIml best = GetRaw(best_i);

	auto AdjustColor = [&](const Image& m) { // flip light to dark if needed
		return best_dark ? AdjustColor0(m, best.flags) : m;
	};
	
	if(best.flags & (IML_IMAGE_FLAG_FIXED|IML_IMAGE_FLAG_FIXED_SIZE))
		return AdjustColor(best.image);
	
	if(best_scale == DPI_600) {
		if(scale == DPI_100)
			return AdjustColor(Downscale6x(best.image));
		if(scale == DPI_150)
			return AdjustColor(Downscale2x(DownSample2x(best.image)));
		if(scale == DPI_200)
			return AdjustColor(DownSample3x(best.image));
		if(scale == DPI_300)
			return AdjustColor(DownSample2x(best.image));
	}
	
	return DPISmartRescale(best.image, scale * best.image.GetSize() / best_scale);
}

Image MakeImlImage(const String& id, Event<int, ImageIml&, String&> GetRaw)
{
	return MakeImlImage(id,
				[&](int i, dword& flags, String& id) {
					ImageIml m;
					GetRaw(i, m, id);
					flags = m.flags;
				},
				[&](int i) -> ImageIml {
					ImageIml m;
					String   id;
					GetRaw(i, m, id);
					return m;
				}
			);

}

Image Iml::Get(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		Mutex::Lock __(sImlLock);
		if(!m.loaded) {
			Image h = MakeImlImage(GetId(i),
				[&](int i, dword& rflags, String& id) {
					id.Clear();
					rflags = 0;
					if(i < GetCount()) {
						id = map.GetKey(i);
						if(flags[i] != IML_IMAGE_FLAGS_UNKNOWN)
							rflags = flags[i];
						else
							rflags = GetRaw(i).flags;
					}
				},
				[&](int i) -> ImageIml {
					return GetRaw(i);
				}
			);
			iml_ReplaceAll(m.image, h);
			m.loaded = true;
		}
	}
	return m.image;
}

int ImlFlagsToDPIScale(int imlflags) {
	int scale = DPI_100;
	if(imlflags & IML_IMAGE_FLAG_UHD)
		scale = DPI_200;
	if(imlflags & IML_IMAGE_FLAG_S3)
		scale *= 3;
	if(imlflags & IML_IMAGE_FLAG_QHD)
		scale++;
	return scale;
}

int DPIScaleToImlFlags(int dpiscale) {
	return dpiscale == DPI_600 ? IML_IMAGE_FLAG_S3|IML_IMAGE_FLAG_UHD
	: get_i(dpiscale, 0, 0,
		0, // DPI_100
		IML_IMAGE_FLAG_QHD, // DPI_150
		IML_IMAGE_FLAG_UHD, // DPI_200
		IML_IMAGE_FLAG_UHD|IML_IMAGE_FLAG_QHD, // 'DPI_250' - not used
		IML_IMAGE_FLAG_S3 // DPI_300
	);
}

Iml::Iml(const char **name, int n)
:	name(name)
{
	premultiply = true;
	Init(n);
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
