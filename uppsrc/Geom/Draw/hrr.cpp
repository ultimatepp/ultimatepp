//////////////////////////////////////////////////////////////////////
// hrr: high resolution raster.

#include "GeomDraw.h"

#include "hrr.h"

#include <plugin/jpg/jpg.h>
#include <plugin/gif/gif.h>
#include <plugin/png/png.h>

namespace Upp {

#define LLOG(x) // LOG(x)

void CalcBitMasks(int bpp, const dword *in_cmasks, dword cmask[], int cshift24[], int cbits[])
{
	static const dword cmasks16[] = { 0xF800, 0x07E0, 0x001F };
	static const dword cmasks32[] = { 0xFF0000, 0x00FF00, 0x0000FF };
	if(!in_cmasks)
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
	else if((in_cmasks[0] | in_cmasks[1] | in_cmasks[2]) == 0) {
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
	}
	for(int i = 0; i < 3; i++) {
		dword cm = in_cmasks[i];
		cmask[i] = cm;
		int shift = 0;
		if(!(cm & 0xFFFF0000)) {
			cm <<= 16;
			shift += 16;
		}
		if(!(cm & 0xFF000000)) {
			cm <<= 8;
			shift += 8;
		}
		if(!(cm & 0xF0000000)) {
			cm <<= 4;
			shift += 4;
		}
		if(!(cm & 0xC0000000)) {
			cm <<= 2;
			shift += 2;
		}
		if(!(cm & 0x80000000)) {
			cm <<= 1;
			shift += 1;
		}
		int width = 0;
		dword mask = 0x80000000;
		while((cm & mask) && width < 8) {
			mask >>= 1;
			width++;
		}
		cshift24[i] = shift;
		cbits[i] = width;
	}
}

static void PixelZStreamInfo(Stream& stream, Size& size, Size& dot_size, Point& hotspot, int& raw_bpp, bool& mono)
{
	enum { VERSION = 1 };
	int version = VERSION;
	stream.Magic(('P' << 24) | ('X' << 16) | ('A' << 8) | 'R');
	stream / version;
	if(version > VERSION) {
		stream.SetError();
		return;
	}
	Pack16(stream, raw_bpp, size.cx, size.cy, hotspot.x, hotspot.y);
	stream % mono % dot_size;
}

struct ZImageDirItem
{
	Size  size;
	Size  dot_size;
	Point hotspot;
	int   raw_bpp;
	bool  mono;
	int   alpha_bpp;

	void  Serialize(Stream& stream)
	{
		PixelZStreamInfo(stream, size, dot_size, hotspot, raw_bpp, mono);
		Pack16(stream, alpha_bpp);
	}
};

class ZImageRaster : public StreamRaster {
public:
	ZImageRaster() {}

	virtual bool    Create();
	virtual Size    GetSize();
	virtual Info    GetInfo();
	virtual Line    GetLine(int line);
	virtual const RGBA *GetPalette();
	virtual const RasterFormat *GetFormat();

private:
	ZImageDirItem   item;
	Vector<Color>   palette;
	dword           cmask[3];
	int             cshift24[3];
	int             cbits[3];
	int             pixel_row_bytes;
	int             alpha_row_bytes;
	int             pixel_block_size;
	int             alpha_block_size;
	int             line_number;
	String          block;
	int             block_offset;
	Vector<byte>    pixel;
	Vector<byte>    alpha;
	Vector<RGBA>    rgba_palette;
	RasterFormat    format;
};

bool ZImageRaster::Create()
{
	static const unsigned MAGIC_TAG = 'Z' * 0x1000000 + 'I' * 0x10000 + 'M' * 0x100 + 'G';
	static const int VERSION = 1;
	
	format.SetRGBA();
	
	Stream& stream = GetStream();
	
	stream.Magic(MAGIC_TAG);
	if(stream.IsError())
		return false;
	int version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("ImageZStreamInfo: version error: " << version);
		stream.SetError();
		return false;
	}
	Array<ZImageDirItem> dir;
	int count = 0;
	stream % count;
	enum { MAX_COUNT = 1000 };
	if(count < 0 || count > MAX_COUNT) {
		LLOG("ImageZStreamInfo: image count error: " << count);
		stream.SetError();
		return false;
	}
	if(stream.IsLoading())
		dir.SetCount(count);
	for(int i = 0; i < count && !stream.IsError(); i++)
		stream % dir[i];
	if(stream.IsError() || dir.IsEmpty())
		return false;
	item = dir[0];
	
	Buffer<int> offsets(dir.GetCount() + 1);
	for(int i = 0; i <= dir.GetCount(); i++)
		stream % offsets[i];
	int64 base = stream.GetPos();
	stream.Seek(base + offsets[0]);
	
	if(item.size.cx <= 0 || item.size.cy <= 0)
		return false;

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version error: " << version);
		stream.SetError();
		return false;
	}
	if(item.raw_bpp) {
		int bpp = (item.raw_bpp < 0 ? 24 : item.raw_bpp);
		if(bpp <= 8) {
			stream % palette;
			rgba_palette.SetCount(1 << bpp, RGBAZero());
			for(int i = 0; i < palette.GetCount() && i < rgba_palette.GetCount(); i++)
				rgba_palette[i] = (RGBA)palette[i];
		}
		else if(bpp == 16 || bpp == 32) {
			stream % cmask[0] % cmask[1] % cmask[2];
			CalcBitMasks(bpp, cmask, cmask, cshift24, cbits);
		}
		pixel_block_size = 0;
		Pack16(stream, pixel_block_size);
		if(pixel_block_size <= 0) {
			LLOG("ZImageRaster::Create -> block size error: " << pixel_block_size);
			stream.SetError();
			return false;
		}
		pixel_row_bytes = ((item.size.cx * bpp + 31) >> 5) << 2;
		pixel.SetCount(item.size.cy * pixel_row_bytes, 0);
		for(int i = 0; i < item.size.cy;) {
			int e = min(item.size.cy, i + pixel_block_size);
			String part;
			stream % part;
			String dpart = ZDecompress(part);
			if(dpart.IsVoid()) {
				LLOG("PixelZStreamData -> decompress error @ row " << i << " (source size = " << part.GetLength() << ")");
				stream.SetError();
				return false;
			}
			int x = 0;
			memcpy(pixel.GetIter(i * pixel_row_bytes), dpart, min(pixel_row_bytes * (e - i), dpart.GetLength()));
			i = e;
		}
	}

	version = VERSION;
	stream / version;
	if(version > VERSION) {
		LLOG("PixelZStreamData -> version error: " << version);
		stream.SetError();
		return false;
	}
/*
	if(item.alpha_bpp) {
		alpha_row_bytes = ((item.size.cx + 31) >> 5) << 2;
		alpha.SetCount(item.size.cy * alpha_row_bytes, 0);
		Vector<Color> mask_palette;
		stream % mask_palette;
		Pack16(stream, alpha_block_size);
		if(alpha_block_size <= 0) {
			LLOG("PixelZStreamData -> block size error: " << alpha_block_size);
			stream.SetError();
			return false;
		}
	
		for(int i = 0; i < item.size.cy;) {
			int e = min(item.size.cy, i + alpha_block_size);
			String part;
			stream % part;
			String dpart = ZDecompress(part);
			if(dpart.IsVoid()) {
				LLOG("PixelZStreamData -> decompress error @ row " << i << " (source size = " << part.GetLength() << ")");
				stream.SetError();
				return false;
			}
			memcpy(alpha.GetIter(i * alpha_row_bytes), dpart, min((e - i) * alpha_row_bytes, dpart.GetLength()));
			i = e;
		}
	}
*/	
	return true;
}

Size ZImageRaster::GetSize()
{
	return item.size;
}

Raster::Info ZImageRaster::GetInfo()
{
	Info info;
	if(item.raw_bpp > 0 && item.raw_bpp <= 8)
		info.colors = 1 << item.raw_bpp;
	info.bpp = (item.raw_bpp < 0 ? 24 : item.raw_bpp);
	info.dots = item.dot_size;
	return info;
}

Raster::Line ZImageRaster::GetLine(int ln)
{
	RGBA *line = new RGBA[item.size.cx];
/*
	if(item.alpha_bpp) {
		const byte *ao = alpha.GetIter(ln * alpha_row_bytes);
		byte active = *ao++;
		byte avail = 8;
		RGBA *out = line;
		for(int width = item.size.cx; --width >= 0; out++) {
			if(!avail) {
				active = *ao++;
				avail = 8;
			}
			--avail;
			out->a = (active & 0x80 ? 0 : 255);
			active <<= 1;
		}
	}
	else */ {
		RGBA bg;
		bg.r = bg.g = bg.b = 0;
		bg.a = 255;
		Fill(line, bg, item.size.cx);
	}
	if(item.raw_bpp) {
		const byte *po = pixel.GetIter(ln * pixel_row_bytes);
		RGBA *out = line;
		if(item.raw_bpp == -3) {
			for(int width = item.size.cx; --width >= 0; out++, po += 3) {
				out->b = po[0];
				out->g = po[1];
				out->r = po[2];
			}
		}
		else if(item.raw_bpp <= 8) {
			byte shift = item.raw_bpp;
			byte per_byte = 8 / item.raw_bpp;
			byte active = 0;
			byte avail = 0;
			RGBA zero = RGBAZero();
			for(int width = item.size.cx; --width >= 0; out++) {
				if(!avail) {
					active = *po++;
					avail = per_byte;
				}
				--avail;
				int index = (active << shift) >> 8;
				active <<= shift;
				RGBA value = (index < rgba_palette.GetCount() ? rgba_palette[index] : zero);
				out->r = value.r;
				out->g = value.g;
				out->b = value.b;
			}
		}
		else if(item.raw_bpp == 16 || item.raw_bpp == 24 || item.raw_bpp == 32) {
			byte bshift = cshift24[2];
			byte bmask = (-256 >> cbits[2]) & 0xFF;
			byte gshift = cshift24[1];
			byte gmask = (-256 >> cbits[1]) & 0xFF;
			byte rshift = cshift24[0];
			byte rmask = (-256 >> cbits[0]) & 0xFF;

			if(item.raw_bpp == 16) {
				for(int width = item.size.cx; --width >= 0; out++, po += 2) {
					uint16 w = Peek16le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
			else {
				for(int width = item.size.cx; --width >= 0; out++, po += 4) {
					uint32 w = Peek32le(po);
					out->r = byte((w << rshift) >> 24) & rmask;
					out->g = byte((w << gshift) >> 24) & gmask;
					out->b = byte((w << bshift) >> 24) & bmask;
				}
			}
		}
		else {
			RLOG("ZImageRaster::GetLine: invalid pixel BPP = " << item.raw_bpp);
		}
	}
	return Raster::Line(line, true);
}

const RGBA * ZImageRaster::GetPalette()
{
	return NULL;
}

const RasterFormat * ZImageRaster::GetFormat()
{
	return &format;
}

void RasterCopy(RasterEncoder& dest, Raster& src, const Rect& src_rc)
{
	dest.Start(src_rc.Size());
	for(int y = src_rc.top; y < src_rc.bottom; y++)
		dest.WriteLine((const RGBA *)src.GetLine(y) + src_rc.left);
}

void ImageWriter::Open(ImageBuffer& output_, Point pos_, Rect clip_, bool merge_)
{
	format.SetRGBA();
	output = &output_;
	pos = pos_;
	clip = clip_;
	merge = merge_;
}

void ImageWriter::Start(Size sz)
{
	src_size = sz;
	line = 0;
	left = max(pos.x, clip.left);
	width = max(min(pos.x + src_size.cx, clip.right) - left, 0);
	offset = (width > 0 ? left - pos.x : 0);
}

void ImageWriter::WriteLineRaw(const byte *s)
{
	if(line >= src_size.cy || width <= 0)
		return;
	int y = line++ + pos.y;
	if(y >= clip.top && y < clip.bottom) {
		const RGBA *l = (const RGBA *)s;
		if(merge)
			AlphaBlend(&(*output)[y][left], l + offset, width);
		else
			memcpy(&(*output)[y][left], l + offset, width * sizeof(RGBA));
	}
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_)
: buffer(buffer_)
{
	crop = buffer.GetSize();
}

ImageBufferRaster::ImageBufferRaster(const ImageBuffer& buffer_, const Rect& crop_)
: buffer(buffer_)
{
	crop = crop_ & Rect(buffer.GetSize());
}

Size ImageBufferRaster::GetSize()
{
	return crop.Size();
}

Raster::Info ImageBufferRaster::GetInfo()
{
	Info info;
	info.bpp = 32;
	info.colors = 0;
	info.dots = Null;
	info.hotspot = Null;
	info.kind = buffer.GetKind();
	return info;
}

Raster::Line ImageBufferRaster::GetLine(int line)
{
	return Line(buffer[line + crop.top] + crop.left, false);
}

inline Stream& operator % (Stream& strm, Color& color)
{
	dword dw = color.GetRaw();
	strm % dw;
	if(strm.IsLoading())
		color = Color::FromRaw(dw);
	return strm;
}

inline Stream& operator % (Stream& strm, Rectf& rc)
{
	strm % rc.left % rc.top % rc.right % rc.bottom;
	return strm;
}

static int64 Unpack64(dword i)
{
	if(!(i & 0x80000000))
		return i;
	return int64(i & 0x7fffffff) << 8;
}

static dword CeilPack64(int64 i)
{
	if(i < 0x7fffffff)
		return (dword)i;
	if(i < I64(0x3fffffff00))
		return (dword)((i + I64(0x80000000ff)) >> 8);
	return 0xffffffff;
}

One<StreamRaster> HRRInfo::GetDecoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGRaster;
	case METHOD_GIF: return new GIFRaster;
	case METHOD_PNG: return new PNGRaster;
	case METHOD_ZIM: return new ZImageRaster;
	default:              return 0;
	}
}

One<StreamRasterEncoder> HRRInfo::GetEncoder() const
{
	switch(method) {
	case METHOD_JPG: return new JPGEncoder(quality);
	case METHOD_GIF: return new GIFEncoder;
	case METHOD_PNG: return new PNGEncoder;
	default:              return 0;
	}
}

/*
One<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
{
	switch(info.GetMethod())
	{
	case HRRInfo::METHOD_JPG: return new JpgEncoder(info.GetQuality());
	case HRRInfo::METHOD_GIF: return new GifEncoder;
	case HRRInfo::METHOD_RLE: return new RleEncoder;
//	case HRRInfo::METHOD_ZIM: return new ZImageEncoder;
#ifndef flagNOHRRPNG
	case HRRInfo::METHOD_PNG: return new PngEncoder;
#endif
	default:              return 0;
	}
}
*/

Vector<int> HRRInfo::EnumMethods()
{
	Vector<int> out;
	out << METHOD_JPG << METHOD_GIF /* << METHOD_RLE*/ << METHOD_PNG; // << METHOD_ZIM;
	return out;
}

/*
enum { wAlphaBlend = 200 };

static void Mask1Blt(byte *dest, const byte *src, const byte *mask, int count)
{
	while(count >= 4)
	{
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
		if(mask[1]) { dest[3] = src[3]; dest[4] = src[4]; dest[5] = src[5]; }
		if(mask[2]) { dest[6] = src[6]; dest[7] = src[7]; dest[8] = src[8]; }
		if(mask[3]) { dest[9] = src[9]; dest[10] = src[10]; dest[11] = src[11]; }
		dest += 12;
		src += 12;
		mask += 4;
		count -= 4;
	}
	if(count & 2)
	{
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
		if(mask[1]) { dest[3] = src[3]; dest[4] = src[4]; dest[5] = src[5]; }
		dest += 6;
		src += 6;
		mask += 2;
	}
	if(count & 1)
		if(mask[0]) { dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; }
}
*/

/*
static void Mask1Copy(PixelArray& dest, const PixelArray& src, const PixelArray& mask)
{
	ASSERT(mask.bpp == 8 && src.bpp == 24 && dest.bpp == 24);
	Size size = dest.GetSize();
	ASSERT(src.GetSize() == size && mask.GetSize() == size);
	for(int i = 0; i < size.cy; i++)
		Mask1Blt(dest.GetUpScan(i), src.GetUpScan(i), mask.GetUpScan(i), size.cx);
}
*/

/*
static void StreamAlphaBlend(Stream& stream, Rect& dest, Rect& src, int& alpha,
							 AlphaArray& image, Color& blend_bgnd)
{
	int version = 2;
	stream / version / alpha;
	Pack16(stream, dest);
	Pack16(stream, src);
	stream % image;
	if(version >= 2)
		stream % blend_bgnd;
	else if(stream.IsLoading())
	{
		alpha = tabs(alpha);
		blend_bgnd = Null;
	}
}
*/

/*
static void DrawAlphaBlend(Draw& draw, Rect dest, Rect src, int alpha, AlphaArray& image, Color blend_bgnd)
{
	ASSERT(alpha >= 0);

	Rect clip = draw.GetClip(), dclip = dest & clip, dclip0 = dclip.Size();
	if(dclip.IsEmpty() || alpha == 0)
		return;

	Color c0 = (image.pixel.palette.GetCount() >= 1 ? image.pixel.palette[0] : Color(Null));
	Color c1 = (image.pixel.palette.GetCount() >= 2 ? image.pixel.palette[1] : Color(Null));
	bool mono_pixel = (image.pixel.IsMono() && (IsNull(c0) || IsNull(c1)));
	if(mono_pixel && IsNull(c0) && IsNull(c1))
		return;

	if(draw.IsDrawing())
	{
		StreamAlphaBlend(draw.DrawingOp(wAlphaBlend), dest, src, alpha, image, blend_bgnd);
		return;
	}

	PixelArray in_blend;
	if(alpha < 100 && IsNull(blend_bgnd))
	{
#ifdef PLATFORM_WIN32
		in_blend = ImageToPixelArray(DrawToImage(draw, dest), draw, -3);
#else
		in_blend = DrawableToPixelArray(draw.GetDrawable(), dest, false, -3, 4);
#endif
	}
	bool resize = (src.Size() != dest.Size() || (dest.Size() != dclip.Size() && draw.Dots()));

	if(mono_pixel)
	{
		if(resize)
		{
			PixelArray new_data = PixelArray::Mono(dclip.Size(), 8);
			PixelCopyAntiAliasMaskOnly(new_data, dest - dclip.TopLeft(), image.pixel, src, false, false, dclip0);
			new_data.palette = image.pixel.palette;
			image.pixel = new_data;
			src = dclip0;
			dest = dclip;
		}
		if(!in_blend.IsEmpty())
		{
			PixelArray copy_blend;
			copy_blend <<= in_blend;
			PixelKillMask(copy_blend, image.pixel, Nvl(c0, c1), !IsNull(c0));
			PixelAlphaBlend(copy_blend, src, in_blend, Point(0, 0), alpha);
			copy_blend.Paint(draw, src, dest);
		}
		else
			image.pixel.Paint(draw, src, dest, c0, c1);
		return;
	}

	if(resize)
	{ // scale image offhand
		if(image.pixel.GetBPP() > 8)
			PixelSetConvert(image.pixel, -3);
		if(!image.HasAlpha())
		{
			PixelArray new_data(dclip.Size(), -3);
			PixelCopyAntiAlias(new_data, dest - dclip.TopLeft(), image.pixel, src, dclip0);
			image.pixel = new_data;
		}
		else
		{
			AlphaArray new_image(dclip.Size(), -3);
			PixelCopyAntiAliasMaskOut(new_image, dest - dclip.TopLeft(), image, src, false, false, dclip0);
			image = new_image;
		}
		src = dclip0;
		dest = dclip;
	}
	if(!in_blend.IsEmpty())
	{ // blend with display contents
		if(image.HasAlpha())
		{
			PixelSetConvert(image.pixel, -3);
			Mask1Copy(image.pixel, in_blend, image.alpha);
		}
		PixelAlphaBlend(image.pixel, src, in_blend, Point(0, 0), alpha);
		image.pixel.Paint(draw, src, dest);
	}
	else {
		if(alpha < 100)
			PixelAlphaBlend(image.pixel, blend_bgnd, alpha, src);
		if(image.HasAlpha())
			image.Paint(draw, src, dest);
		else
			image.pixel.Paint(draw, src, dest);
	}
//	RTIMING("DrawAlphaBlend (raw)");
}
*/

/*
static void wsAlphaBlend(Draw& draw, Stream& stream, const DrawingPos& pos)
{
	Rect src, dest;
	int alpha;
	AlphaArray image;
	Color blend_bgnd;
	StreamAlphaBlend(stream, dest, src, alpha, image, blend_bgnd);
	DrawAlphaBlend(draw, pos(dest), src, alpha, image, blend_bgnd);
}
*/

//static DrawerRegistrator MK__s(wAlphaBlend, wsAlphaBlend);

static int GetMaskInfo(const RGBA *rgba, int count)
{
	if(count == 0)
		return 0;
	if(rgba->a == 255) {
		for(; count > 0 && rgba->a == 255; count--, rgba++)
			;
		return (count ? 2 : 1);
	}
	else if(rgba->a == 0) {
		for(; count > 0 && rgba->a == 0; count--, rgba++)
			;
		return (count ? 2 : 0);
	}
	return 2;
}

static String EncodeMask(const ImageBuffer& mask, bool write_size)
{
	StringBuffer out;
	if(write_size) {
		char temp[4];
		Poke16le(temp + 0, mask.GetWidth());
		Poke16le(temp + 2, mask.GetHeight());
		out.Cat(temp, 4);
	}
	int full = out.GetLength();
	Size size = mask.GetSize();
	for(int i = 0; i < size.cy; i++) {
		const RGBA *p = mask[size.cy - i - 1], *e = p + size.cx;
		int start = out.GetLength();
		while(p < e) {
			bool init0 = false;
			if(p->a < 128)
			{ // full part
				const RGBA *b = p;
				while(++p < e && p->a < 128)
					;
				int n = p - b;
				while(n > 253) {
					out.Cat(255);
					out.Cat(2);
					n -= 253;
				}
				if(n > 0)
					out.Cat(n + 2);
			}
			else
				init0 = true;
			if(p < e) {
				const RGBA *b = p;
				while(++p < e && p->a >= 128)
					;
				if(p < e) {
					if(init0)
						out.Cat(2);
					int n = p - b;
					while(n > 253) {
						out.Cat(255);
						out.Cat(2);
						n -= 253;
					}
					if(n > 0)
						out.Cat(n + 2);
				}
			}
		}
		if(out.GetLength() > start)
			full = out.GetLength();
		out.Cat(1);
	}
	if(full < out.GetLength())
		out.SetLength(full);
	return out;
}

static void DecodeMask(ImageBuffer& mask, String s, bool read_size)
{
	Size size = mask.GetSize();
	const byte *p = s;
	if(read_size) {
		size.cx = Peek16le(p);
		size.cy = Peek16le(p + 2);
		p += 4;
	}
	for(int i = 0; i < size.cy && *p; i++) {
		RGBA *d = mask[size.cy - i - 1], *e = d + size.cx;
		while(*p >= 2 && d < e) {
			int n1 = *p++ - 2;
			if(e - d <= n1) {
				while(d < e)
					d++->a = 0;
				break;
			}
			RGBA *dd = d + n1;
			while(d < dd)
				d++->a = 0;
			if(*p >= 2) {
				n1 = *p++ - 2;
				if(e - d <= n1)
					break;
				d += n1;
			}
		}
		while(*p >= 2)
			p++;
		if(*p)
			p++;
	}
}

/*
static String EncodeMask(const RawImage& mask)
{
	ASSERT(mask.bpp == 8);
	String out;
	int full = 0;
	Size size = mask.GetSize();
	for(int i = 0; i < size.cy; i++)
	{
		const byte *p = mask.GetUpScan(i), *e = p + size.cx;
		int start = out.GetLength();
		while(p < e)
		{
			const byte *b = p;
			while(++p < e && *p)
				;
			if(p >= e)
				break;
			int n = p - b;
			while(n > 253)
			{
				out.Cat(255);
				out.Cat(2);
				n -= 253;
			}
			out.Cat(n + 2);
			b = p;
			while(++p < e && !*p)
				;
			n = p - b;
			while(n > 253)
			{
				out.Cat(255);
				out.Cat(2);
				n -= 253;
			}
			if(n > 0 || p < e)
				out.Cat(n + 2);
		}
		if(out.GetLength() > start)
			full = out.GetLength();
		out.Cat(1);
	}
	if(full < out.GetLength())
		out.Trim(full);
	return out;
}

static void DecodeMask(RawImage& mask, const String& s)
{
	ASSERT(mask.bpp == 8);
	Size size = mask.GetSize();
	mask.Set(1);
	const byte *p = s;
	for(int i = 0; i < size.cy && *p; i++)
	{
		byte *d = mask.GetUpScan(i), *e = d + size.cx;
		while(*p >= 2 && d < e)
		{
			int n1 = *p++ - 2;
			if(e - d <= n1)
				break;
			d += n1;
			if(*p < 2)
				break;
			n1 = *p++ - 2;
			if(e - d <= n1)
			{
				memset(d, 0, e - d);
				break;
			}
			memset(d, 0, n1);
			d += n1;
		}
		while(*p >= 2)
			p++;
		if(*p)
			p++;
	}
}
*/

HRRInfo::HRRInfo()
: levels(0)
, log_rect(0, 0, 0, 0)
, map_rect(0, 0, 0, 0)
, background(White)
, method(METHOD_JPG)
, quality(50)
, mono(false)
, mono_black(Black)
, mono_white(White)
{
}

HRRInfo::HRRInfo(const Rectf& log_rect_, const Rectf& map_rect_,
	int levels_, Color background_, int method_, int quality_,
	bool mono_, Color mono_black_, Color mono_white_)
: log_rect(log_rect_)
, map_rect(map_rect_)
, levels(levels_)
, background(background_)
, method(method_)
, quality(quality_)
, mono(mono_)
, mono_black(mono_black_)
, mono_white(mono_white_)
{
	if(IsNull(map_rect)) {
		double wadd = log_rect.Height() - log_rect.Width();
		map_rect = log_rect;
		if(wadd >= 0)
			map_rect.right += wadd;
		else
			map_rect.top += wadd;
	}
}

void HRRInfo::Serialize(Stream& stream)
{
	int outver = (stream.IsStoring() && !mono && method != METHOD_ZIM && method != METHOD_BZM ? 4 : 5);
	int version = StreamHeading(stream, outver, 2, 5, "HRRInfo");
	if(version >= 2)
		stream / levels % background % log_rect % map_rect;
	if(version >= 3)
		stream / method;
	else if(stream.IsLoading())
		method = METHOD_JPG;
	if(version >= 4)
		stream / quality;
	else if(stream.IsLoading())
		quality = 0;
	if(version >= 5)
		stream % mono % mono_black % mono_white;
	else if(stream.IsLoading())
	{
		mono = false;
		mono_black = Null;
		mono_white = Null;
	}
}

Vector<int> HRRInfo::EnumQualities(int method)
{
	Vector<int> out;
	switch(method)
	{
	case METHOD_JPG:
		{
			for(int i = 10; i <= 100; i += 10)
				out << i;
		}
		break;

	case METHOD_GIF:
	case METHOD_RLE:
	case METHOD_PNG:
	case METHOD_ZIM:
	case METHOD_BZM:
		out << 0;
		break;

	default:
		NEVER();
		break;
	}
	return out;
}

VectorMap<int, String> HRRInfo::GetPackMap()
{
	VectorMap<int, String> out;
	Vector<int> methods = EnumMethods();
	for(int m = 0; m < methods.GetCount(); m++)
	{
		Vector<int> qualities = EnumQualities(methods[m]);
		if(qualities.IsEmpty())
			qualities.Add(0);
		for(int q = 0; q < qualities.GetCount(); q++)
			out.FindAdd(Pack(methods[m], qualities[q]), GetName(methods[m], qualities[q]));
	}
	return out;
}

String HRRInfo::GetName(int method, int quality)
{
	String out;
	switch(method)
	{
	case METHOD_JPG:
		out << "JPEG " << (quality ? quality : DFLT_JPG_QUALITY) << "%";
		break;

	case METHOD_GIF:
		out << "GIF";
		break;
/*
	case METHOD_RLE:
		out << "RLE";
		break;
*/
	case METHOD_PNG:
		out << "PNG";
		break;

/*
	case METHOD_ZIM:
		out << "ZIM";
		break;

	case METHOD_BZM:
		out << "BZM";
		break;
*/
	default:
		out << "?? (" << method << ")";
	}
	return out;
}

double HRRInfo::GetEstimatedFileSize(int _levels, int method, int quality)
{
	int images = 0;
	for(int i = 0; i < _levels; i++)
		images += 1 << (2 * i);
	int dir_size = images * sizeof(int) // offset table
		+ 256; // estimated heading size
	double data_size = images * double(UNIT * UNIT);
	switch(method)
	{
	case METHOD_JPG:
		data_size *= (quality ? quality : DFLT_JPG_QUALITY) / 400.0; // guessed JPEG size
		break;

	case METHOD_GIF:
		data_size /= 2;
		break;

	case METHOD_RLE:
		data_size /= 1.5;
		break;

	case METHOD_PNG:
		data_size /= 1.6;
		break;

	case METHOD_ZIM:
		data_size /= 1.6;
		break;

	case METHOD_BZM:
		data_size /= 1.8;
		break;

	default:
		NEVER();
		break;
	}
	return data_size;
}

//////////////////////////////////////////////////////////////////////
// HRR::Block::

void HRR::Block::Init(Size s, RGBA color)
{
//	static TimingInspector ti("HRR::Block::Init");
//	ti.Start();
	size = s;
	block.Create(size);
	Fill(~block, color, block.GetLength());
}

//////////////////////////////////////////////////////////////////////
// HRR::

One<StreamRaster> (*HRR::CreateDecoder)(const HRRInfo& info) = &HRR::StdCreateDecoder;
One<StreamRasterEncoder> (*HRR::CreateEncoder)(const HRRInfo& info) = &HRR::StdCreateEncoder;

static const Size SUNIT(HRRInfo::UNIT, HRRInfo::UNIT);
static const Rect RUNIT(0, 0, HRRInfo::UNIT, HRRInfo::UNIT);

HRR::HRR()
{
	cache_sizeof_limit = DEFAULT_CACHE_SIZEOF_LIMIT;
}

HRR::HRR(const char *path, bool read_only)
{
	Open(path, read_only);
}

One<StreamRaster> HRR::StdCreateDecoder(const HRRInfo& info)
{
	switch(info.GetMethod()) {
		case HRRInfo::METHOD_GIF: return new GIFRaster;
		case HRRInfo::METHOD_PNG: return new PNGRaster;
		case HRRInfo::METHOD_JPG: return new JPGRaster;
		case HRRInfo::METHOD_ZIM: return new ZImageRaster;
//		case HRRInfo::METHOD_BMP: return new BMPRaster;
	}
	return NULL;
}

One<StreamRasterEncoder> HRR::StdCreateEncoder(const HRRInfo& info)
{
	switch(info.GetMethod()) {
		case HRRInfo::METHOD_GIF: return new GIFEncoder;
		case HRRInfo::METHOD_PNG: return new PNGEncoder;
		case HRRInfo::METHOD_JPG: return new JPGEncoder(info.GetQuality());
//		case HRRInfo::METHOD_BMP: return new BMPEncoder;
	}
	return NULL;
}

bool HRR::Open(const char *path, bool read_only)
{
	Close();
	if(!path || !*path || !stream.Open(path, read_only ? stream.READ : stream.READWRITE))
		return false;
	stream.SetLoading();
	Serialize();
	if(stream.IsError() || info.levels <= 0
		|| info.map_rect.Width() <= 0 || info.map_rect.Height() <= 0)
	{
		Close();
		return false;
	}
	return true;
}

void HRR::Close()
{
	stream.Close();
	pixel_directory_offset.Clear();
	mask_directory_offset.Clear();
//	pixel_directory.Clear();
//	mask_directory.Clear();
	image_cache.Clear();
	directory_sizeof = 0;
	cache_sizeof = 0;
	info = HRRInfo();
}

static int GetImageSize(Size sz)
{
	return sizeof(Image) + 32 + sz.cx * sz.cy * sizeof(RGBA);
}

inline static int GetImageSize(const Image& im) { return GetImageSize(im.GetSize()); }

void HRR::FlushCache(int limit)
{
	while(!image_cache.IsEmpty() && cache_sizeof > limit) {
		cache_sizeof -= GetImageSize(image_cache[0]);
		image_cache.Remove(0);
	}
}

void HRR::ClearCache()
{
	image_cache.Clear();
	cache_sizeof = 0;
}

static Size GetLogBlockSize(Rectf box_rc, Rectf map_rc)
{
	Size part_size(HRRInfo::UNIT, HRRInfo::UNIT);
	if(box_rc.left >= map_rc.right)
		return Size(0, 0);
	if(box_rc.right >= map_rc.right)
		part_size.cx = fround(part_size.cx * (map_rc.right - box_rc.left) / box_rc.Width());
	if(box_rc.bottom <= map_rc.top)
		return Size(0, 0);
	else if(box_rc.top < map_rc.top)
		part_size.cy = fround(part_size.cy * (box_rc.bottom - map_rc.top) / box_rc.Height());
	return part_size;
}

static Color BlendColor(Color a, int percent, Color b)
{
	return Color(
		b.GetR() + iscale(a.GetR() - b.GetR(), percent, 100),
		b.GetG() + iscale(a.GetG() - b.GetG(), percent, 100),
		b.GetB() + iscale(a.GetB() - b.GetB(), percent, 100));
}

static int StopMsec(int start = 0)
{
	return GetTickCount() - start;
}

static void DrawAlphaImage(Draw& draw, Rect dest, Image img, Rect src, int alpha)
{
	if(alpha <= 0)
		return;
	alpha += alpha >> 7;
	if(alpha >= 256) {
		draw.DrawImage(dest, img, src);
		return;
	}
	Size outsz = min(src.Size(), dest.Size());
	ImageBuffer temp(outsz);
	Rescale(ImageWriter(temp, false), outsz, ImageRaster(img), src);
	byte conv[256];
	for(int i = 0; i < 256; i++)
		conv[i] = (i * alpha) >> 8;
	for(RGBA *p = ~temp, *e = ~temp + temp.GetLength(); p < e; p++) {
//		int a = (p->a + (p->a >> 7)) * alpha;
		p->r = conv[p->r];
		p->g = conv[p->g];
		p->b = conv[p->b];
		p->a = conv[p->a];
	}
//	temp.SetKind(IMAGE_PREMULTIPLIED);
	draw.DrawImage(dest, Image(temp));
}

void HRR::Paint(Draw& draw, Rect dest, Rectf src,
	int alpha, int max_pixel, Color mono_black, Color mono_white, Color blend_bgnd)
{
	LLOG("HRR::Paint: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", dest = " << dest << ", src = " << src << BeginIndent);
	draw.Clip(dest);
	Swap(dest.top, dest.bottom);
	Paint(draw, MatrixfScale(src, dest), Null, alpha, max_pixel, mono_black, mono_white, blend_bgnd);
	draw.End();
	LLOG(EndIndent << "// HRR::Paint");
}

HRR::Cursor::Cursor(HRR& owner_, const Rectf& extent_, double measure_,
	int alpha_, Color mono_black_, Color mono_white_, Color blend_bgnd_)
: owner(owner_)
, extent(extent_)
, measure(measure_)
, alpha(alpha_)
, mono_black(mono_black_)
, mono_white(mono_white_)
, blend_bgnd(blend_bgnd_)
{
	bool use_pixel = (IsNull(mono_black) && IsNull(mono_white));

	if(owner.info.IsMono() && use_pixel) {
		mono_black = owner.info.GetMonoBlack();
		mono_white = owner.info.GetMonoWhite();
		use_pixel = (IsNull(mono_black) && IsNull(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::Paint, null color, empty case");
			return;
		}
	}

//	bool use_bg = (alpha < 100 && IsNull(blend_bgnd) || do_transform);
//	bool use_alpha = !use_pixel || IsNull(info.background);
	bool is_bw = (!IsNull(mono_black) && !IsNull(mono_white));
//	bool out_pixel = (use_pixel || is_bw);
//	bool out_alpha = (use_pixel ? IsNull(info.background) : !is_bw);

//	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
//		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	double r = HRRInfo::UNIT / owner.info.GetMapRect().Width();
//	if(draw.Dots())
//		r /= 5; // ad hoc conversion from screen space to dot resolution
	level = 0;
	for(; level < owner.info.GetLevels() - 1 && r < measure; r *= 2, level++)
		;
//	DUMP(level);

	if(!IsNull(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(owner.info.GetBackground(), White));
	if(!IsNull(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(owner.info.GetBackground(), White));

	// calculate interest area in Q-tree blocks
	total = 1 << level;
	Rectf blocks = (extent - owner.info.GetMapRect().BottomLeft()) / owner.info.GetMapRect().Size() * double(total);
	rc = Rect(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= Rect(0, 0, total, total);

	// prepare clipping & image loader
	if(!owner.info.IsMono()) {
		raster = CreateDecoder(owner.info);
		if(!raster) {
			LLOG(EndIndent << "//HRR:x: decoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.BottomLeft() * scale;
//	scale *= Sizef(1, -1) * info.map_rect.Size() / double(1 << level);

#ifdef _DEBUG
//	int ti = 0;
#endif

	block = rc.TopLeft();
	block.x--;
}

bool HRR::Cursor::Fetch(Rectf& part)
{
	for(;;) {
		cimg = -1;
		if(++block.x >= rc.right) {
			block.x = rc.left;
			if(++block.y >= rc.bottom)
				return false;
		}
		LLOG("[" << StopMsec(ticks) << "] block = [" << x << ", " << y << "]");
		int layer_offset = 4 * (block.x + block.y * total);
		int pixel_offset = 0, mask_offset = 0;
		if(level >= 0 && level < owner.pixel_directory_offset.GetCount()) {
			owner.stream.Seek(owner.pixel_directory_offset[level] + layer_offset);
			pixel_offset = owner.stream.Get32le();
		}
		if(level >= 0 && level < owner.mask_directory_offset.GetCount()) {
			owner.stream.Seek(owner.mask_directory_offset[level] + layer_offset);
			mask_offset = owner.stream.Get32le();
		}
		Point pixel_mask(pixel_offset, mask_offset);
		if(!pixel_offset && !mask_offset)
			continue;
		if((cimg = owner.image_cache.Find(pixel_mask)) < 0) {
			ImageBuffer new_image;
			if(pixel_offset) {
				owner.stream.Seek(Unpack64(pixel_offset));
				new_image = raster->Load(owner.stream);
				if(new_image.IsEmpty()) {
					RLOG(NFormat("Failed to load block [%d, %d].", block.x, block.y));
					continue;
				}
//				PixelSetConvert(new_image.pixel, -3);
			}
			if(mask_offset) {
				owner.stream.Seek(Unpack64(mask_offset));
				int len = owner.stream.GetIL();
				ASSERT(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
				StringBuffer databuf(len);
				owner.stream.Get(databuf, len);
				String data = databuf;
				if(owner.version < 5) {
					Size sz(0, 0);
					if(cimg >= 0)
						sz = new_image.GetSize();
					else if(pixel_offset) {
						int csize = owner.size_cache.Find(pixel_offset);
						if(csize < 0) {
							if(owner.size_cache.GetCount() >= 10000)
								owner.size_cache.Clear();
							int64 pixpos = Unpack64(pixel_offset);
							if(pixpos > owner.stream.GetSize())
								owner.stream.SetSize(pixpos);
	//								stream.Seek(pixpos);
							csize = owner.size_cache.GetCount();
	//								Stream64Stream pixel_stream(stream, pixpos);
							owner.stream.Seek(pixpos);
							raster->Open(owner.stream);
							owner.size_cache.Add(pixel_offset, raster->GetSize());
						}
						sz = owner.size_cache[csize];
					}
					if(sz.cx <= 0 || sz.cy <= 0)
						continue;
//					new_image.alpha = PixelArray::Mono(sz);
				}
				DecodeMask(new_image, data, owner.version >= 5);
			}
			int new_len = new_image.GetLength() * sizeof(RGBA);
			owner.FlushCache(owner.cache_sizeof_limit - new_len);
			owner.cache_sizeof += new_len;
			cimg = owner.image_cache.GetCount();
			owner.image_cache.Add(pixel_mask) = new_image;
		}
		if(cimg >= 0) {
			part = owner.GetLogBlockRect(level, RectC(block.x, block.y, 1, 1));
			Size sz = owner.image_cache[cimg].GetSize();
			part.right = part.left + part.Width() * sz.cx / HRRInfo::UNIT;
			part.top = part.bottom - part.Height() * sz.cy / HRRInfo::UNIT;
			return true;
		}
	}
}

Image HRR::Cursor::Get()
{
	ASSERT(cimg >= 0);
	return owner.image_cache[cimg];
}

void HRR::Paint(Draw& draw, const Matrixf& trg_pix, GisTransform transform,
	int alpha, int max_pixel, Color mono_black, Color mono_white, Color blend_bgnd)
{
	LLOG("HRR::Paint: alpha = " << alpha
		<< ", max_pixel = " << max_pixel << ", mono_black = " << mono_black
		<< ", mono_white = " << mono_white << ", blend_bgnd = " << blend_bgnd
		<< ", trg_pix = " << trg_pix << BeginIndent);

	int ticks = StopMsec();
	ASSERT(alpha >= 0);

	if(alpha == 0 || info.IsEmpty() || !IsOpen()) {
		LLOG(EndIndent << "//HRR::Paint, empty case");
		return;
	}

	bool do_transform = !transform.IsIdentity();
	bool is_straight = !do_transform && fabs(trg_pix.x.y) <= 1e-10 && fabs(trg_pix.y.x) <= 1e-10;
	bool use_pixel = (IsNull(mono_black) && IsNull(mono_white));

	if(info.mono && use_pixel) {
		mono_black = info.mono_black;
		mono_white = info.mono_white;
		use_pixel = (IsNull(mono_black) && IsNull(mono_white));
		if(use_pixel) {
			LLOG(EndIndent << "//HRR::Paint, null color, empty case");
			return;
		}
	}

	bool use_bg = (alpha < 100 && IsNull(blend_bgnd) || do_transform);
	bool use_alpha = !use_pixel || IsNull(info.background);
	bool is_bw = (!IsNull(mono_black) && !IsNull(mono_white));
	bool out_pixel = (use_pixel || is_bw);
	bool out_alpha = (use_pixel ? IsNull(info.background) : !is_bw);

	LLOG("[" << StopMsec(ticks) << "] use_bg = " << use_bg << ", use_pixel = " << use_pixel << ", use_alpha = " << use_alpha
		<< ", is_bw = " << is_bw << ", out_pixel = " << out_pixel << ", out_alpha = " << out_alpha);

	Matrixf pix_trg = MatrixfInverse(trg_pix);
	Rect clip = draw.GetPaintRect(); //draw.GetClip();
	Rectf csrc = info.log_rect & transform.SourceExtent(Rectf(clip) * pix_trg);
//	Pointf scale = Sizef(1, -1) * Sizef(dest.Size()) / Sizef(src.Size());
//	Pointf delta = Pointf(dest.TopLeft()) - src.BottomLeft() * scale;
//	Rectf  csrc  = src & info.log_rect;
	Rect   cdest = RectfToRect(transform.TargetExtent(csrc) * trg_pix) & clip;
//	Rect   cdest = RectfToRect(csrc * scale + delta);
//	Swap(cdest.top, cdest.bottom);
//	DrawRectMinusRect(draw, dest, cdest, info.background);
	if(cdest.IsEmpty())
	{ // intersection is less than 1 pixel wide / high
		LLOG(EndIndent << "//HRR::Paint: empty destination, exiting");
		return;
	}
	double r = fpmax(Sizef(cdest.Size()) * Sizef(info.map_rect.Size()) / csrc.Size()) / info.UNIT;
	if(draw.Dots())
		r /= 5; // ad hoc conversion from screen space to dot resolution
	int level = 0;
	for(; level < info.levels - 1 && r > max_pixel; r /= 2, level++)
		;
//	DUMP(level);

	if(!IsNull(mono_black))
		mono_black = BlendColor(mono_black, alpha, Nvl(info.background, White));
	if(!IsNull(mono_white))
		mono_white = BlendColor(mono_white, alpha, Nvl(info.background, White));

	ImageBuffer out_blend;
	if(use_bg) {
		out_blend.Create(cdest.Size());
		Fill(out_blend, info.background, out_blend.GetLength());
	}
	LOG("out blend: " << out_blend.GetSize());

	// calculate interest area in Q-tree blocks
	int total = 1 << level;
	Rectf blocks = (csrc - info.map_rect.BottomLeft()) / info.map_rect.Size() * double(total);
	Rect rc(ffloor(blocks.left), ffloor(-blocks.bottom), fceil(blocks.right), fceil(-blocks.top));
	rc &= Rect(0, 0, total, total);

	// prepare clipping & image loader
	draw.Clip(cdest);
	One<StreamRaster> decoder;
	if(!info.mono) {
		decoder = CreateDecoder(info);
		if(decoder.IsEmpty()) {
			draw.DrawText(cdest.left, cdest.top,
				String().Cat() << "Unsupported HRR encoding: " << info.GetMethod(), StdFont());
			draw.End();
			LLOG(EndIndent << "//HRR:x: encoder not found, exiting");
			return;
		}
	}

	// adjust transform parameters to convert from Q-tree space to device coords
//	delta += info.map_rect.BottomLeft() * scale;
//	scale *= Sizef(1, -1) * info.map_rect.Size() / double(1 << level);

#ifdef _DEBUG
//	int ti = 0;
#endif

	SegmentTreeInfo seginfo;
	seginfo.src_trg = transform;
	seginfo.trg_pix = trg_pix;
	seginfo.trg_pix.a -= cdest.TopLeft();
	seginfo.antialias = true;
	seginfo.branch = 0;
	seginfo.max_depth = HRRInfo::HALF_BITS - 1;
	double trg_dv = 2 / sqrt(fabs(Determinant(trg_pix)));
	Rect rclip = clip - cdest.TopLeft();
	Font err_font = StdFont();
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++)
		{
			LLOG("[" << StopMsec(ticks) << "] block = [" << x << ", " << y << "]");
			seginfo.img_src = GetPixMapMatrix(level, x, y);
			seginfo.img_src.x.x /= HRRInfo::UNIT;
			seginfo.img_src.y.y /= HRRInfo::UNIT;
			Matrixf src_img = MatrixfInverse(seginfo.img_src);
			Rect src = RectfToRect((RUNIT * seginfo.img_src & csrc) * src_img).Inflated(2) & RUNIT;
			Rectf map = src * seginfo.img_src;
			Rect dest = (transform.TargetExtent(map) * trg_pix).Inflated(1) & Rectf(clip);
//			Rect dest = RectfToRect(Rectf(x, y, x + 1, y + 1) * scale + delta);
//			Rect clip = dest & draw.GetClip();
			Rect rdest = (dest & cdest) - cdest.TopLeft();
			if(rdest.IsEmpty())
				continue;
			LinearSegmentTree tleft, ttop, tright, tbottom;
			PlanarSegmentTree tplanar;
			if(!is_straight) {
				seginfo.max_deviation = trg_dv;
				tleft = CreateLinearTree(src.TopLeft(), src.BottomLeft(), seginfo);
				ttop = CreateLinearTree(src.TopLeft(), src.TopRight(), seginfo);
				tright = CreateLinearTree(src.TopRight(), src.BottomRight(), seginfo);
				tbottom = CreateLinearTree(src.BottomLeft(), src.BottomRight(), seginfo);
				tplanar = CreatePlanarTree(tleft, ttop, tright, tbottom, seginfo);
			}
//			Rect src = (clip - dest.TopLeft()) * SUNIT / dest.Size();
//			src.Inflate(2);
//			src &= RUNIT;
			int layer_offset = 4 * (x + y * total);
			stream.Seek(pixel_directory_offset[level] + layer_offset);
			int pixel_offset = stream.Get32le();
			int mask_offset = 0;
			if(!mask_directory_offset.IsEmpty()) {
				stream.Seek(mask_directory_offset[level] + layer_offset);
				mask_offset = stream.Get32le();
			}
//			int pixel_offset = pixel_directory[level][x + y * total];
			Point pixel_mask(pixel_offset, mask_offset);
			if(!pixel_offset && !mask_offset)
				continue;
			bool newimg = false;
			if(image_cache.Find(pixel_mask) < 0) {
//				Stream64Stream pixel_stream(stream, Unpack64(pixel_offset));
				ImageBuffer new_image;
				if(pixel_offset) {
					stream.Seek(Unpack64(pixel_offset));
					new_image = decoder->Load(stream);
					if(new_image.IsEmpty()) {
						String warn = NFormat("Failed to load block [%d, %d].", x, y);
						Size sz = GetTextSize(warn, err_font);
						draw.DrawRect(Rect(dest.CenterPoint(), Size(1, 1)).Inflated(sz + 2), Color(255, 192, 192));
						draw.DrawText((dest.left + dest.right - sz.cx) >> 1, (dest.top + dest.bottom - sz.cy) >> 1,
							warn, StdFont(), Black);
						continue;
					}
				}
				if(mask_offset && use_alpha) {
					stream.Seek(Unpack64(mask_offset));
					int len = stream.GetIL();
					StringBuffer data(len);
					ASSERT(len >= 0 && len < HRRInfo::UNIT * (HRRInfo::UNIT + 1) + 1);
					stream.Get(data, len);
					if(version < 5) {
						Size sz(0, 0);
						if(pixel_offset)
							sz = new_image.GetSize();
						else {
							int csize = size_cache.Find(pixel_offset);
							if(csize < 0) {
								if(size_cache.GetCount() >= 10000)
									size_cache.Clear();
								int64 pixpos = Unpack64(pixel_offset);
								if(pixpos > stream.GetSize())
									stream.SetSize(pixpos);
	//								stream.Seek(pixpos);
								csize = size_cache.GetCount();
	//								Stream64Stream pixel_stream(stream, pixpos);
								stream.Seek(pixpos);
								Size sz = 0;
								if(decoder->Open(stream))
									sz = decoder->GetSize();
								size_cache.Add(pixel_offset, sz);
							}
							sz = size_cache[csize];
						}
						if(sz.cx <= 0 || sz.cy <= 0)
							continue;
						DecodeMask(new_image, data, version >= 5);
					}
				}
				FlushCache(cache_sizeof_limit - GetImageSize(new_image.GetSize()));
				cache_sizeof += GetImageSize(new_image.GetSize());
				image_cache.Add(pixel_mask) = new_image;
			}
			int cimg = image_cache.Find(pixel_mask);
			if(cimg < 0)
				continue;
/*
			if(cimg < 0) {
				LLOG("[" << StopMsec(ticks) << "] pixel off, mask off");
				if(!is_straight && !IsNull(info.background))
					AlphaTransformPaint(out_blend, Image(), tplanar, tleft, ttop, tright, tbottom, seginfo, info.background);
				else if(use_pixel)
					draw.DrawRect(dest, info.background);
			}
			else 
*/
			{
				const Image& img = image_cache[cimg];
				if(!use_bg) {
					LLOG("[" << StopMsec(ticks) << "] !use_bg -> direct mask blend");
					if(alpha >= 100)
						draw.DrawImage(dest, img, src);
					else
						DrawAlphaImage(draw, dest, img, src, minmax(alpha * 256 / 100, 0, 255));
//					DrawAlphaBlend(draw, dest, src, 100, out_part, blend_bgnd);
				}
				else if(!is_straight) {
					LLOG("[" << StopMsec(ticks) << "] use_bg -> twisted mask blend");
					AlphaTransformPaint(out_blend, img,
						tplanar, tleft, ttop, tright, tbottom, seginfo, LtRed());
				}
				else {
					LLOG("[" << StopMsec(ticks) << "] use_bg -> buffered colored mask blend");
					ImageWriter writer(out_blend, rdest.TopLeft(), rclip);
					Rescale(writer, rdest.Size(), ImageRaster(img), src);
				}
			}
		}

	if(use_bg) {
		if(alpha < 100) {
			int coef = alpha * 255 / 100;
			byte conv[256];
			for(int i = 0; i < 256; i++)
				conv[i] = (i * coef) >> 8;
			for(RGBA *p = ~out_blend, *e = p + out_blend.GetLength(); p < e; p++) {
				p->r = conv[p->r];
				p->g = conv[p->g];
				p->b = conv[p->b];
				p->a = conv[p->a];
			}
		}
		draw.DrawImage(cdest, out_blend);
	}
	draw.End();
	LLOG(EndIndent << "[" << StopMsec(ticks) << "] //HRR::Paint");
}

int HRR::GetProgressCount(int levels, bool downscale)
{
	ASSERT(levels > 0);
	int images = 0;
	if(downscale)
		images = 1 << (2 * (levels - 1));
	else
		for(int i = 0; i < levels; i++)
			images += 1 << (2 * i);
	return images;
}

bool HRR::Create(const HRRInfo& _info, const char *path)
{
	ASSERT(_info.levels > 0);
	Close();
	if(!stream.Open(path, stream.CREATE))
		return false;
	info = _info;
	map_offset = 0;
	Serialize();
	return true;
}

static void StreamHRRString(Stream& stream, String& string)
{
	int version = 1, len = string.GetLength();
	stream / version / len;
	if(version > 1 || stream.IsLoading() && (unsigned)len > stream.GetLeft())
	{
		stream.SetError();
		return;
	}
	if(stream.IsStoring())
		stream.SerializeRaw((byte *)(const char *)string, len);
	else {
		StringBuffer stringbuf(len);
		stream.SerializeRaw((byte *)~stringbuf, len);
		string = stringbuf;
	}
}

void HRR::Serialize()
{
	int outver = (stream.IsStoring() && (info.mono || info.method >= info.METHOD_ZIM) ? 5 : 4);
	version = StreamHeading(stream, outver, 1, 5, "\r\n"
		"High-Resolution Raster\r\n"
		"Copyright ©1999 Cybex Development, spol. s r.o.\r\n");
	if(version >= 1)
		stream % info;
	if(version >= 2)
		stream % map_offset;
	else
		map_offset = 0;
	pixel_directory_offset.SetCount(info.levels);
	if(version >= 1) {
		if(version <= 3 || !info.mono)
			for(int l = 0; l < info.levels; l++) {
				int c = 1 << (2 * l);
				int byte_size = 4 * c;
				pixel_directory_offset[l] = stream.GetPos();
				if(stream.IsStoring() && stream.GetLeft() < byte_size)
					stream.Put(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)pixel_directory[l].Begin(),
//					sizeof(pixel_directory[0][0]) * pixel_directory[l].GetCount());
			}
		if(version >= 3 && (IsNull(info.background) || info.mono)) {
			mask_directory_offset.SetCount(info.levels);
			for(int m = 0; m < info.levels; m++) {
				int c = 1 << (2 * m);
				int byte_size = 4 * c;
				mask_directory_offset[m] = stream.GetPos();
				if(stream.IsStoring() && stream.GetLeft() < byte_size)
					stream.Put(0, byte_size);
				else
					stream.SeekCur(byte_size);
//				stream.SerializeRaw((byte *)mask_directory[m].Begin(),
//					sizeof(mask_directory[0][0]) * mask_directory[m].GetCount());
			}
		}
	}
	if(map_offset && version > 3) {
		int64 mappos = Unpack64(map_offset);
		if(stream.IsStoring() && stream.GetSize() < mappos) {
			stream.Seek(stream.GetSize());
			stream.Put(0, (int)(mappos - stream.GetSize()));
		}
		if(stream.IsStoring() || mappos >= 0 && mappos < stream.GetSize()) {
			stream.Seek(mappos);
			int count = map.GetCount();
			stream / count;
			for(int i = 0; i < count; i++)
			{
				String key;
				String val;
				if(stream.IsStoring())
				{
					key = map.GetKey(i);
					val = map[i];
				}
				StreamHRRString(stream, key);
				StreamHRRString(stream, val);
				if(stream.IsLoading())
					map.Add(key, val);
			}
		}
	}
	else
		map.Clear();
}

void HRR::Write(Writeback drawback, bool downscale)
{
	ASSERT(stream.IsOpen());
	if(map_offset > 0)
	{
		int64 mu = Unpack64(map_offset);
		stream.Seek(mu);
		stream.SetSize(mu);
		map_offset = 0;
	}
	One<StreamRasterEncoder> encoder = CreateEncoder(info);
	if(!encoder)
		throw Exc(String().Cat() << "Unsupported HRR encoding: " << info.GetMethod());

	bool abort = false;
	try
	{
		Write(drawback, downscale, 0, 0, 0, *encoder, 0);
	}
	catch(AbortExc)
	{
		abort = true;
	}
	map_offset = CeilPack64(stream.GetPos());
	stream.Seek(0);
	Serialize(); // update header
	if(abort)
		throw AbortExc();
}

Matrixf HRR::GetPixMapMatrix(int level, int x, int y) const
{
	double fac = 1 << level;
	double xx = info.map_rect.Width() / fac, yy = -info.map_rect.Height() / fac;
	return Matrixf(xx, 0, 0, yy, info.map_rect.left + xx * x, info.map_rect.bottom + yy * y);
}

int64 HRR::GetFileWriteSize() const
{
	ASSERT(stream.IsOpen());
	return stream.GetSize();
}

Rectf HRR::GetLogBlockRect(int level, const Rect& rc) const
{
	return Rectf(rc) * GetPixMapMatrix(level, 0, 0);
/*	Rectf r(rc);
	double fac = 1.0 / (1 << level);
	r = r * fac;
	double t = r.bottom; r.bottom = 1 - r.top; r.top = 1 - t;
	return r * info.map_rect.Size() + info.map_rect.TopLeft();
*/
}

bool HRR::Write(Writeback drawback, bool downscale, int level, int px, int py,
				StreamRasterEncoder& format, Block *put)
{
	static const Size SUNIT(info.UNIT, info.UNIT);
	Block block(*this);
//	TIMING("HRR::Write");

	if(level >= info.levels - info.LCOUNT)
	{ // generate all at once
//		TIMING("HRR::Write(short step)");
//		static TimingInspector __part("HRR::Write(part)");
//		__part.Start();
		int count = info.levels - level - 1;
		// step & render individual images
		block.Init(SUNIT << count, info.background);
//		__part.End();
		block.level = level + count;
		block.area = RectC(px << count, py << count, 1 << count, 1 << count);
		block.log_area = GetLogBlockRect(block.level, block.area);
		bool done = drawback(block);
		if(!done && downscale)
			return false;

		while(count >= 0)
		{
			int n = 1 << count;
			for(Size a(0, 0); a.cy < n; a.cy++)
				for(a.cx = 0; a.cx < n; a.cx++)
				{
					Point src = a * info.UNIT;
					Size part_size = GetLogBlockSize(GetLogBlockRect(level + count, RectC(a.cx, a.cy, 1, 1)), info.log_rect);
					if(part_size.cx <= 0 || part_size.cy <= 0)
						continue;
					ImageBuffer part(part_size);
					RasterCopy(ImageWriter(part, false), ImageBufferRaster(block.block), Rect(src, part_size));
					int lin = (int)((px << count) + a.cx + (((py << count) + a.cy) << (count + level)));
//					TIMING("HRR::Write / save (direct)");
					if(info.mono || IsNull(info.background)) {
						int kind = GetMaskInfo(~part, part.GetLength());
						if(kind && !info.mono) {
							int pixoff = CeilPack64(stream.GetPos());
							stream.Seek(pixel_directory_offset[level + count] + 4 * lin);
							stream.Put32le(pixoff);
							stream.SeekEnd();
							int64 pixpos = Unpack64(pixoff);
							if(stream.GetSize() < pixpos)
								stream.Put(0, (int)(pixpos - stream.GetSize()));
							stream.Seek(pixpos);
//							Stream64Stream pixstream(stream, pixpos);
							format.Save(stream, ImageBufferRaster(part));
						}
						if(kind == 2 || (kind == 1 && info.mono)) {
							String s = EncodeMask(part, version >= 5);
							ASSERT(s.GetLength() >= 4);
							int maskoff = CeilPack64(stream.GetPos());
							stream.Seek(mask_directory_offset[level + count] + 4 * lin);
							stream.Put32le(maskoff);
							stream.SeekEnd();
//							mask_directory[level + count][lin] = maskoff;
							int64 maskpos = Unpack64(maskoff);
							if(stream.GetSize() < maskpos)
								stream.Put(0, (int)(maskpos - stream.GetSize()));
							stream.Seek(maskpos);
							stream.PutIL(s.GetLength());
							stream.Put(s, s.GetLength());
						}
					}
					else {
						int pixoff = CeilPack64(stream.GetPos());
						stream.Seek(pixel_directory_offset[level + count] + 4 * lin);
						stream.Put32le(pixoff);
						stream.SeekEnd();
//						pixel_directory[level + count][lin] = pixoff;
						int64 pixpos = Unpack64(pixoff);
						if(stream.GetSize() < pixpos)
							stream.Put(0, (int)(pixpos - stream.GetSize()));
						stream.Seek(pixpos);
//						Stream64Stream pixstream(stream, pixpos);
						format.Save(stream, ImageBufferRaster(part));
					}
				}
			if(--count >= 0) // reduce image
				if(downscale) {
					Size sz = SUNIT << count;
					ImageBuffer new_data(sz);
					Rescale(ImageWriter(new_data, false), sz, ImageBufferRaster(block.block), block.size);
					block.block = new_data;
				}
				else {
					block.Init(SUNIT << count, info.background);
					block.level = level + count;
					block.area = RectC(px << count, py << count, 1 << count, 1 << count);
					drawback(block);
				}
		}
	}
	else
	{ // too big - bisect to generate higher level
//		TIMING("HRR::Write (long step)");
		Block *ptr = 0;
		if(downscale) {
			Size part_size = GetLogBlockSize(GetLogBlockRect(level, RectC(px, py, 1, 1)), info.log_rect);
			if(part_size.cx <= 0 || part_size.cy <= 0)
				return false;
			block.Init(part_size, info.background);
			ptr = &block;
		}
		bool done = Write(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 0, format, ptr);
		done     |= Write(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 0, format, ptr);
		done     |= Write(drawback, downscale, level + 1, 2 * px + 0, 2 * py + 1, format, ptr);
		done     |= Write(drawback, downscale, level + 1, 2 * px + 1, 2 * py + 1, format, ptr);
		if(!done && downscale)
			return false;
		if(!downscale) {
			block.Init(SUNIT, info.background);
			block.level = level;
			block.area = RectC(px, py, 1, 1);
			block.log_area = GetLogBlockRect(block.level, block.area);
			drawback(block);
		}
		int lin = px + (py << level);
//		TIMING("HRR::Write / save (indirect)");
		if(info.mono || IsNull(info.background)) {
			int kind = GetMaskInfo(block.block, block.block.GetLength());
			if(kind && !info.mono) {
				int pixoff = CeilPack64(stream.GetPos());
				stream.Seek(pixel_directory_offset[level] + 4 * lin);
				stream.Put32le(pixoff);
				stream.SeekEnd();
//				pixel_directory[level][lin] = pixoff;
				int64 pixpos = Unpack64(pixoff);
				if(stream.GetSize() < pixpos)
					stream.Put(0, (int)(pixpos - stream.GetSize()));
				stream.Seek(pixpos);
				//Stream64Stream pixstream(stream, pixpos);
				format.Save(stream, ImageBufferRaster(block.block));
			}
			if(kind == 2 || (kind == 1 && info.mono)) {
				String s = EncodeMask(block.block, version >= 5);
				ASSERT(s.GetLength() >= 4);
				int maskoff = CeilPack64(stream.GetPos());
				stream.Seek(mask_directory_offset[level] + 4 * lin);
				stream.Put32le(maskoff);
				stream.SeekEnd();
//				mask_directory[level][lin] = maskoff;
				int64 maskpos = Unpack64(maskoff);
				if(stream.GetSize() < maskpos)
					stream.Put(0, (int)(maskpos - stream.GetSize()));
				stream.Seek(maskpos);
				stream.PutIL(s.GetLength());
				stream.Put(s, s.GetLength());
			}
		}
		else {
			int pixoff = CeilPack64(stream.GetPos());
			stream.Seek(pixel_directory_offset[level] + 4 * lin);
			stream.Put32le(pixoff);
			stream.SeekEnd();
//			pixel_directory[level][lin] = pixoff;
			int64 pixpos = Unpack64(pixoff);
			while(stream.GetSize() < pixpos)
				stream.Put(0, (int)min<int64>(pixpos - stream.GetSize(), 1 << 24));
			stream.Seek(pixpos);
			//Stream64Stream pixstream(stream, pixpos);
			format.Save(stream, ImageBufferRaster(block.block));
		}
	}
	if(put) {
//		TIMING("HRR::Write / put");
		Rect org = RectC((px & 1) << info.HALF_BITS, (py & 1) << info.HALF_BITS,
			1 << info.HALF_BITS, 1 << info.HALF_BITS);
		Rescale(ImageWriter(put->block, org.TopLeft(), false), org.Size(), ImageBufferRaster(block.block), RUNIT);
	}
	return true;
}

void HRR::SetMap(String key, String value)
{
	if(IsNull(value))
	{
		int i = map.Find(key);
		if(i >= 0)
			map.Remove(i);
	}
	else
		map.GetAdd(key) = value;
}

void HRR::FlushMap()
{
	ASSERT(stream.IsOpen());
	if(map_offset == 0)
		map_offset = CeilPack64(stream.GetSize());
	stream.Seek(0);
	stream.SetStoring();
	Serialize();
}

int HRR::SizeOfInstance() const
{
	return sizeof(*this) + directory_sizeof + cache_sizeof;
}

}
