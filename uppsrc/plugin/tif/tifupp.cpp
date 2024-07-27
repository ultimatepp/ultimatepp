#include <Draw/Draw.h>
#include "tif.h"
#include <Painter/Painter.h>

#define	tif_int32 int32_t
#define	tif_uint32 uint32_t

#define LLOG(x)  // LOG(x)

// #define DBGALLOC 1

namespace Upp {

#if DBGALLOC
double total_allocated = 0, total_freed = 0;
unsigned alloc_calls = 0, free_calls = 0, realloc_calls = 0;
Index<tsize_t> size_index;
Vector<int> size_alloc_calls, size_free_calls;
int op_id;

void dbgAddAlloc(void *p, tsize_t s)
{
	++op_id;
	total_allocated += s;
	int i = size_index.Find(s);
	if(i >= 0)
		size_alloc_calls[i]++;
	else
	{
		size_index.Add(s);
		size_alloc_calls.Add(1);
		size_free_calls.Add(0);
	}
	LLOG(op_id << " tAlloc(" << s << ") = " << p << ": blks: " << alloc_calls - free_calls << ", alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void dbgAddFree(void *p, tsize_t s)
{
	++op_id;
	total_freed += s;
	int i = size_index.Find(s);
	if(i >= 0)
		size_free_calls[i]++;
	else
	{
		size_index.Add(s);
		size_alloc_calls.Add(0);
		size_free_calls.Add(1);
	}
	LLOG(op_id << " tFree(" << p << ") = " << s << ": blks: " << alloc_calls - free_calls << ", alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void TiffAllocStat()
{
	for(int i = 0; i < size_index.GetCount(); i++)
		if(size_alloc_calls[i] != size_free_calls[i])
			LOG("Alloc/free mismatch: size = " << size_index[i]
			<< ", alloc = " << size_alloc_calls[i] << ", frees = " << size_free_calls[i]);
}
#endif

extern "C" tdata_t _TIFFmalloc(tsize_t s)
{
	byte *p = new byte[s + 16];
	Poke32le(p, s);
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(p, s);
#endif
	return (tdata_t)(p + 16);
}

extern "C" void* _TIFFcalloc(tmsize_t nmemb, tmsize_t siz)
{
    if( nmemb == 0 || siz == 0 )
        return ((void *) NULL);

    return _TIFFmalloc(nmemb * siz);
}

extern "C" void    _TIFFfree(tdata_t p)
{
	if(p) {
		byte *rawp = (byte *)p - 16;
#if DBGALLOC
		free_calls++;
		dbgAddFree(p, Peek32le(rawp));
#endif
		delete[] (rawp);
	}
}

extern "C" tdata_t _TIFFrealloc(tdata_t p, tsize_t s)
{
	int oldsize = (p ? Peek32le((const byte *)p - 16) : 0);
	byte *newptr = new byte[s + 16];
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(newptr, s);
#endif
	if(oldsize) {
		memcpy(newptr + 16, p, min<int>(oldsize, s));
#if DBGALLOC
		free_calls++;
		dbgAddFree(newptr, oldsize);
#endif
		delete[] ((byte *)p - 16);
	}
	Poke32le(newptr, s);
	return (tdata_t)(newptr + 16);
}

extern "C" void _TIFFmemset(void* p, int v, tmsize_t c)           { memset(p, v, c); }
extern "C" void _TIFFmemcpy(void* d, const void *s, tmsize_t c) { memcpy(d, s, c); }
extern "C" int  _TIFFmemcmp(const void *p1, const void *p2, tmsize_t c) { return memcmp(p1, p2, c); }

/*
static void Blt2to4(byte *dest, const byte *src, unsigned count)
{
	byte b;

#define BLT2_4_4(o) \
	b = src[(o)]; \
	dest[2 * (o) + 0] = ((b >> 2) & 0x30) | ((b >> 4) & 0x03); \
	dest[2 * (o) + 1] = ((b << 2) & 0x30) | (b & 0x03);

	for(unsigned rep = count >> 5; rep; rep--) {
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		BLT2_4_4(4) BLT2_4_4(5) BLT2_4_4(6) BLT2_4_4(7)
		dest += 8 * 2;
		src += 8;
	}
	if(count & 16) {
		BLT2_4_4(0) BLT2_4_4(1) BLT2_4_4(2) BLT2_4_4(3)
		dest += 4 * 2;
		src += 4;
	}
	if(count & 8) {
		BLT2_4_4(0) BLT2_4_4(1)
		dest += 2 * 2;
		src += 2;
	}
	if(count & 4) {
		BLT2_4_4(0)
		dest += 2;
		src++;
	}
	switch(count & 3) {
	case 0:
		break;

	case 1:
		*dest = ((*src >> 2) & 0x30);
		break;

	case 2:
		*dest = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		break;

	case 3:
		*dest++ = ((*src >> 2) & 0x30) | ((*src >> 4) & 0x03);
		*dest = (*src << 2) & 0x30;
		break;
	}
}
*/
static void BltPack11(byte *dest, const byte *src, byte bit_shift, unsigned count)
{
	if(bit_shift == 0)
	{ // simple case
#if defined(CPU_IA32)
#define BLT_PACK_11_4(o) *(unsigned *)(dest + (o)) = *(const unsigned *)(src + (o));
#else
#define BLT_PACK_11_4(o) dest[(o) + 0] = src[(o) + 0]; dest[(o) + 1] = src[(o) + 1]; \
	dest[(o) + 2] = src[(o) + 2]; dest[(o) + 3] = src[(o) + 3];
#endif
		for(unsigned rep = count >> 7; rep; rep--)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4) BLT_PACK_11_4(8) BLT_PACK_11_4(12)
			dest += 16;
			src += 16;
		}
		if(count & 0x40)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_PACK_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			dest[0] = src[0]; dest[1] = src[1];
			dest += 2;
			src += 2;
		}
		if(count & 8)
			*dest++ = *src++;
		switch(count & 7)
		{
		case 0: break;
		case 1: *dest = (*src & 0x80) | (*dest | 0x7f); break;
		case 2: *dest = (*src & 0xc0) | (*dest | 0x3f); break;
		case 3: *dest = (*src & 0xe0) | (*dest | 0x1f); break;
		case 4: *dest = (*src & 0xf0) | (*dest | 0x0f); break;
		case 5: *dest = (*src & 0xf8) | (*dest | 0x07); break;
		case 6: *dest = (*src & 0xfc) | (*dest | 0x03); break;
		case 7: *dest = (*src & 0xfe) | (*dest | 0x01); break;
		}
	}
	else
	{
		const byte shift1 = bit_shift, shift2 = 8 - bit_shift;
		byte mask;
		if(count + shift1 <= 8)
		{ // touch just 1 byte
			mask = ((1 << count) - 1) << (8 - count - shift1);
			*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
			return;
		}
		mask = 0xff00 >> shift1;
		*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
		dest++;
		count -= shift2;
#define BLT_SHIFT_11_1(o) dest[(o)] = (src[(o)] << shift2) | (src[(o) + 1] >> shift1);
#define BLT_SHIFT_11_4(o) BLT_SHIFT_11_1((o)) BLT_SHIFT_11_1((o) + 1) BLT_SHIFT_11_1((o) + 2) BLT_SHIFT_11_1((o) + 3)
		for(unsigned rep = count >> 6; rep; rep--)
		{
			BLT_SHIFT_11_4(0) BLT_SHIFT_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_SHIFT_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			BLT_SHIFT_11_1(0) BLT_SHIFT_11_1(1)
			dest += 2;
			src += 2;
		}
		if(count & 8)
		{
			BLT_SHIFT_11_1(0)
			dest++;
			src++;
		}
		if(count &= 7)
		{
			byte data = (count <= shift1 ? src[1] << shift2 : (src[1] << shift2) | (src[2] >> shift1));
			mask = 0xff00 >> count;
			*dest = (*dest & ~mask) | (data & mask);
		}
	}
}

// add support for 2 bpp tif - Massimo Del Fedele
// un-optimized way....
// bit_shift should be shift on destination, NOT source
static void BltPack22(byte *dest, const byte *src, byte bit_shift, unsigned count)
{
	unsigned c2 = count >> 2;
	count &= 0x03;
	byte shift1, shift2;
	byte sMask1, sMask2;
	byte dMask1, dMask2;

	if(!bit_shift) // fast path
	{
		if(c2)
		{
			memcpy(dest, src, c2);
			dest += c2;
			src += c2;
		}
		switch(count)
		{
			default:
			case 0:
				break;

			case 1:
				*dest = (*dest & 0x3f) | (*src & 0x3f);
				break;

			case 2:
				*dest = (*dest & 0x0f) | (*src & 0x0f);
				break;

			case 3:
				*dest = (*dest & 0x03) | (*src & 0x03);
				break;
		} // switch(count)
	}
	else // slow path
	{
		bit_shift <<= 1;
		shift1 = bit_shift;
		shift2 = (8 - bit_shift);
		sMask1 = 0xff << shift1;
		dMask1 = 0xff << shift2;
		sMask2 = 0xff >> shift2;
		dMask2 = 0xff >> shift1;
		while(c2--)
		{
			*dest = (*dest & dMask1) | ((*src & sMask1) >> shift1);
			dest++;
			*dest = (*dest & dMask2) | ((*src & sMask2) << shift2);
			src++;
		}
		switch(count)
		{
			case 0:
			default:
				break;

			case 1:
				*dest = (*dest & ~(0xc0 >> bit_shift)) | ((*src & 0xc0) >> bit_shift);
				break;

			case 2:
				if(bit_shift <= 4)
				{
					*dest = (*dest & ~(0xf0 >> bit_shift)) | ((*src & 0xf0) >> bit_shift);
				}
				else
				{
					*dest = (*dest & ~(0xc0 >> bit_shift)) | ((*src & 0xc0) >> bit_shift);
					dest++;
					*dest = (*dest & 0x3f) | ((*src & 0x30) << 2);
				}
				break;

			case 3:
				if(bit_shift <= 2)
				{
					*dest = (*dest & ~(0xfc >> bit_shift)) | ((*src & 0xfc) >> bit_shift);
				}
				else if(bit_shift <= 4)
				{
					*dest = (*dest & ~(0xf0 >> bit_shift)) | ((*src & 0xf0) >> bit_shift);
					dest++;
					*dest = (*dest & 0x3f) | ((*src & 0x0c) << 4);
				}
				else
				{
					*dest = (*dest & 0xfc) | ((*src & 0xc0) >> 6);
					dest++;
					*dest = (*dest & 0x0f) | ((*src & 0x3c) << 2);
				}
				break;
		} // switch(count)
	} // end slow path

}

static void BltPack44(byte *dest, const byte *src, bool shift, unsigned count)
{
//	RTIMING("BltPack44");
	ASSERT(count > 0);
	if(shift)
	{
		byte c = *src++, d;
		*dest = (*dest & 0xF0) | (c >> 4);
		dest++;
		count--;
		while(count >= 8)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			d = src[2]; dest[2] = (c << 4) | (d >> 4);
			c = src[3]; dest[3] = (d << 4) | (c >> 4);
			src += 4;
			dest += 4;
			count -= 8;
		}
		if(count & 4)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			src += 2;
			dest += 2;
		}
		if(count & 2)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = d;
			src++;
			dest++;
		}
		if(count & 1)
			dest[0] = (dest[0] & 15) | (c << 4);
	}
	else
	{
		unsigned c2 = count >> 1;
		if(c2)
			memcpy(dest, src, c2);
		if(count & 1)
			dest[c2] = (dest[c2] & 15) | (src[c2] & 0xF0);
	}
}
/*
static void BltPack4(byte *dest, const byte *src, unsigned count)
{
#define BLT_PACK_4_4(o) dest[(o)] = src[4 * (o)]; dest[(o) + 1] = src[4 * (o) + 4]; \
	dest[(o) + 2] = src[4 * (o) + 8]; dest[(o) + 3] = src[4 * (o) + 12];
	for(unsigned rep = count >> 4; rep; rep--)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4) BLT_PACK_4_4(8) BLT_PACK_4_4(12)
		dest += 16;
		src += 4 * 16;
	}
	if(count & 8)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4)
		dest += 8;
		src += 4 * 8;
	}
	if(count & 4)
	{
		BLT_PACK_4_4(0)
		dest += 4;
		src += 4 * 4;
	}
	if(count & 2)
	{
		dest[0] = src[0]; dest[1] = src[4];
		dest += 2;
		src += 4 * 2;
	}
	if(count & 1)
		dest[0] = src[0];
}
*/
static tsize_t ReadStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Stream *stream = reinterpret_cast<Stream *>(fd);
	if(!stream->IsOpen())
		return 0;
//	RLOG("TiffStream::TIFRaster::Data & " << (int)wrapper.stream.GetPos() << ", count = " << size
//		<< ", end = " << (int)(wrapper.stream.GetPos() + size));
	return stream->Get(buf, size);
}

static tsize_t WriteStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Stream *stream = reinterpret_cast<Stream *>(fd);
	ASSERT(stream->IsOpen());
	stream->Put(buf, size);
	return stream->IsError() ? 0 : size;
}

static toff_t SeekStream(thandle_t fd, toff_t off, int whence)
{
	Stream *stream = reinterpret_cast<Stream *>(fd);
	ASSERT(stream->IsOpen());
	toff_t size = (toff_t)stream->GetSize();
	toff_t destpos = (toff_t)(off + (whence == 1 ? stream->GetPos() : whence == 2 ? size : 0));
	stream->Seek(destpos);
//	RLOG("TIFRaster::Data::SeekStream -> " << (int)off << ", whence = " << whence << " -> pos = " << (int)wrapper.stream.GetPos());
	return (toff_t)stream->GetPos();
}

static int CloseStream(thandle_t fd)
{
	return 0;
}

static int CloseOwnedStream(thandle_t fd)
{
	delete reinterpret_cast<Stream *>(fd);
	return 0;
}

static toff_t SizeStream(thandle_t fd)
{
	Stream *stream = reinterpret_cast<Stream *>(fd);
	ASSERT(stream->IsOpen());
//	RLOG("TIFRaster::Data::SizeStream -> " << (int)wrapper.stream.GetSize());
	return (toff_t)stream->GetSize();
}

static int MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize)
{
	return 0;
}

static void UnmapStream(thandle_t fd, tdata_t base, toff_t size)
{
}

struct ::tiff *TIFFFileStreamOpen(const char *filename, const char *mode)
{
	One<FileStream> fs = new FileStream;
	int m = _TIFFgetMode(NULL, NULL, mode, "TIFFOpen");
	dword fmode = FileStream::READ;

	switch(m) {
		case O_RDONLY: {
			fmode = FileStream::READ;
			break;
		}
		case O_RDWR: {
			fmode = FileStream::READWRITE;
			break;
		}
		case O_RDWR|O_CREAT:
		case O_RDWR|O_TRUNC:
		case O_RDWR|O_CREAT|O_TRUNC: {
			fmode = FileStream::CREATE;
			break;
		}
	}
	if(!fs->Open(filename, fmode))
		return NULL;
	return TIFFStreamOpen(filename, mode, fs.Detach(), true);
}
/*

struct ::tiff* TIFFWrapOpen(const char *filename, const char *mode){
	return ::TIFFOpen(filename, mode);
}

int TIFFWrapGetField(::tiff* tif_data, uint32 tag, ...){
	va_list ap;
	return ::TIFFGetField(tif_data, tag, ap);
}
*/

struct ::tiff *TIFFStreamOpen(const char *filename, const char *mode, Stream *stream, bool destruct)
{
	return TIFFClientOpen(filename, mode, reinterpret_cast<thandle_t>(stream),
		&ReadStream, &WriteStream,
		&SeekStream,
		destruct ? &CloseOwnedStream : &CloseStream,
		&SizeStream,
		&MapStream, &UnmapStream);
}


struct TIFRaster::Data : public TIFFRGBAImage {
	Data(Stream& stream);
	~Data();

	bool             Create();
	Raster::Info     GetInfo();
	Raster::Line     GetLine(int i, Raster *owner);
	bool             SeekPage(int page);
	bool             FetchPage();

	static void      Warning(const char* module, const char* fmt, va_list ap);
	static void      Error(const char* module, const char* fmt, va_list ap);

	RasterFormat     format;

	Stream&          stream;
	TIFF             *tiff;

	struct Page {
		uint32       width, height;
		uint16       bits_per_sample;
		uint16       samples_per_pixel;
		uint16       photometric;
		uint16       orientation;
		Size         dot_size;
		bool         alpha;
	};
	Array<Page>      pages;
	int              page_index;

	VectorMap<String, Value> attr;

	byte *MapDown(int x, int y, int count);
	byte *MapUp(int x, int y, int count);

	Size size;
	int bpp;
	int row_bytes;
	int cache_size;
	bool alpha;
	bool page_open;
	bool page_fetched;
	bool page_error;
	Vector<byte> imagebuf;
	struct Row {
		Row() {}

		Buffer<byte> mapping;
	};
	enum { MAX_CACHE_SIZE = 50000000 };
	RGBA palette[256];
	int palette_count;
	Buffer<Row> rows;
	int64 mapping_offset;
	int mapping_size;
	Vector<uint32_t> buffer;
	tileContigRoutine contig;
	tileSeparateRoutine separate;
	int skewfac;
//	void (*pack)(TIFFImageHelper *helper, uint32 x, uint32 y, uint32 w, uint32 h);
//	String warnings;
//	String errors;
};

extern "C" {

TIFFErrorHandler _TIFFwarningHandler = TIFRaster::Data::Warning;
TIFFErrorHandler _TIFFerrorHandler   = TIFRaster::Data::Error;

};

static void packTileRGB(TIFRaster::Data *helper, uint32 x, uint32 y, uint32 w, uint32 h)
{
	if(helper->alpha) {
		int x4 = 4 * x, w4 = 4 * w;
	//	byte *dest = helper->dest.GetUpScan(y) + 3 * x;
		const byte *src = (const byte *)helper->buffer.Begin();
	//	unsigned srow = sizeof(uint32) * w; //, drow = helper->dest.GetUpRowBytes();
		for(; h; h--, /*src += srow,*/ /*dest += drow*/ y++) {
			for(byte *dest = helper->MapUp(x4, y, w4), *end = dest + w4; dest < end; dest += 4, src += 4) {
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
				dest[3] = src[3];
			}
		}
	}
	else {
		int x3 = 3 * x, w3 = 3 * w;
	//	byte *dest = helper->dest.GetUpScan(y) + 3 * x;
		const byte *src = (const byte *)helper->buffer.Begin();
	//	unsigned srow = sizeof(uint32) * w; //, drow = helper->dest.GetUpRowBytes();
		for(; h; h--, /*src += srow,*/ /*dest += drow*/ y++) {
			for(byte *dest = helper->MapUp(x3, y, w3), *end = dest + w3; dest < end; dest += 3, src += 4) {
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
			}
		}
	}
}

static void putContig1(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x8 = x >> 3;
	int w8 = ((x + w + 7) >> 3) - x8;
	bool read = !!((x | w) & 7) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 3);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = keep_y ? 1 : -1;
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y += drow /*dest += drow*/, src += srow)
		BltPack11(helper->MapUp(x8, y, w8), src, (byte)(x & 7), w);
}

static void putContig2(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x4 = x >> 2;
	int w4 = ((x + w + 3) >> 2) - x4;
	bool read = !!((x | w) & 3) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 3);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = keep_y ? 1 : -1;
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y += drow /*dest += drow*/, src += srow)
		BltPack22(helper->MapUp(x4, y, w4), src, (byte)(x & 3), w);
}

static void putContig4(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size; //dest.GetSize();
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	int x2 = x >> 1;
	int w2 = ((x + w + 1) >> 1) - x2;
	bool read = !!((x | w) & 1) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 1);
	bool shift = (x & 1);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		BltPack44(helper->MapUp(x2, y, w2), src, shift, w);
}

static void putContig8(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size;
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
//	byte *dest = helper->dest.GetUpScan(y) + x;
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		memcpy(helper->MapUp(x, y, w), src, w);
}

static void putContigRGB(TIFFRGBAImage *img, tif_uint32 *cp, tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size;
	int iw = toskew + w;
	int wh = w * h;
	if(wh > helper->buffer.GetCount())
		helper->buffer.SetCount(wh);
	bool keep_y = (iw >= 0);
	helper->contig(img, (tif_uint32 *)(keep_y ? &helper->buffer[0] : &helper->buffer[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, pp);
	packTileRGB(helper, x, keep_y ? y : y - h + 1, w, h);
}

static void putSeparate(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *r, byte *g, byte *b, byte *a)
{
	TIFRaster::Data *helper = (TIFRaster::Data *)img;
//	Size size = helper->size;
	int wh = w * h;
	if(wh > helper->buffer.GetCount())
		helper->buffer.SetCount(wh);
	int iw = toskew + w;
	bool keep_y = (iw >= 0);
	helper->separate(img, (tif_uint32 *)(keep_y ? &helper->buffer[0] : &helper->buffer[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, r, g, b, a);
	packTileRGB(helper, x, keep_y ? y : y - h + 1, w, h);
}

byte *TIFRaster::Data::MapUp(int x, int y, int count)
{
	return MapDown(x, size.cy - 1 - y, count);
}

byte *TIFRaster::Data::MapDown(int x, int y, int count)
{
	return &imagebuf[row_bytes * y] + x;
}

void TIFRaster::Data::Warning(const char *fn, const char *fmt, va_list ap)
{
	if(!memcmp(fn, "tiff@", 5) && IsDigit(fn[5])) {
		int addr = stou(fn + 5);
		if(addr != -1 && addr != 0) {
//			TIFRaster::Data& wrapper = *reinterpret_cast<TIFRaster::Data *>(addr);
			LLOG("TIF warning: " << VFormat(fmt, ap));
//			RLOG("TiffWrapper::Warning: " << wrapper.errors);
		}
	}
}

void TIFRaster::Data::Error(const char *fn, const char *fmt, va_list ap)
{
	if(!memcmp(fn, "tiff@", 5) && IsDigit(fn[5])) {
		int addr = stou(fn + 5);
		if(addr != -1 && addr != 0) {
//			Data& wrapper = *reinterpret_cast<Data *>(addr);
			LLOG("TIF error: " << VFormat(fmt, ap));
//			RLOG("TiffWrapper::Error: " << wrapper.errors);
		}
	}
}

TIFRaster::Data::Data(Stream& stream)
: stream(stream)
{
	tiff = NULL;
	page_index = 0;
	cache_size = 0;
	page_open = false;
	page_fetched = false;
	page_error = false;
}

TIFRaster::Data::~Data()
{
	if(tiff) {
		if(page_open)
			TIFFRGBAImageEnd(this);
		TIFFClose(tiff);
	}
}

bool TIFRaster::Data::Create()
{
	TIFFSetErrorHandler(NULL);
	TIFFSetErrorHandlerExt(NULL);
	TIFFSetWarningHandler(NULL);
	TIFFSetWarningHandlerExt(NULL);

	tiff = TIFFStreamOpen("tiff@" + Format64((intptr_t)this), "r", &stream);
	if(!tiff)
		return false;

	int count = TIFFNumberOfDirectories(tiff);
	if(count <= 0)
		return false;
	for(int i = 0; i < count; i++) {
		Page& page = pages.Add();
		TIFFSetDirectory(tiff, i);
		TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &page.width);
		TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &page.height);
		float xres, yres;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_YRESOLUTION, &yres);
		uint16 resunit;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_RESOLUTIONUNIT, &resunit);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_BITSPERSAMPLE, &page.bits_per_sample);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_SAMPLESPERPIXEL, &page.samples_per_pixel);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_PHOTOMETRIC, &page.photometric);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_ORIENTATION, &page.orientation);
		double dots_per_unit = (resunit == RESUNIT_INCH ? 600.0 : resunit == RESUNIT_CENTIMETER
			? 600.0 / 2.54 : 0);
		page.dot_size.cx = (xres ? fround(page.width * dots_per_unit / xres) : 0);
		page.dot_size.cy = (yres ? fround(page.height * dots_per_unit / yres) : 0);
		page.alpha = false;
		uint16 extrasamples, *sampletypes;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_EXTRASAMPLES, &extrasamples, &sampletypes);
		for(int e = 0; e < extrasamples; e++)
			if(sampletypes[e] == EXTRASAMPLE_ASSOCALPHA) {
				page.alpha = true;
				break;
			}
		
		if(i == 0) {
			const word TIFFTAG_GEOPIXELSCALE = 33550,
			           TIFFTAG_GEOTIEPOINTS = 33922,
			           TIFFTAG_GEOTRANSMATRIX = 34264,
			           TIFFTAG_GEOKEYDIRECTORY = 34735,
			           TIFFTAG_GEODOUBLEPARAMS = 34736,
			           TIFFTAG_GEOASCIIPARAMS = 34737;

			word count = 0;
			word *geokeys = nullptr;
			bool pixel_is_area = true; // Default is true, usually only DTM/DEM use 'RasterPixelIsPoint' and generally include the GeoKey in such case

			int dpc = 0;
			double *doubleparams = 0;

			int apc = 0;
			char *asciiparams = 0;

			TIFFGetField(tiff, TIFFTAG_GEODOUBLEPARAMS, &dpc, &doubleparams);
			TIFFGetField(tiff, TIFFTAG_GEOASCIIPARAMS, &apc, &asciiparams);

			if(TIFFGetField(tiff, TIFFTAG_GEOKEYDIRECTORY, &count, &geokeys) &&
				count >= 4 && (count % 4) == 0 && geokeys[0] == 1)
				for(int i = 4; i + 3 < count; i += 4) {
					const word GTRasterTypeGeoKey = 1025,
					           RasterPixelIsArea = 1,
					           GeographicTypeGeoKey = 2048,
					           ProjectedCSTypeGeoKey = 3072;

					word value = geokeys[i + 3];

					switch(geokeys[i]) {
					case GTRasterTypeGeoKey:
						pixel_is_area = value == RasterPixelIsArea;
						break;
					case GeographicTypeGeoKey:
					case ProjectedCSTypeGeoKey:
						attr.GetAdd("epsg") = value;
						break;
					}

					switch(geokeys[i+1]){ // by type
						case TIFFTAG_GEOASCIIPARAMS:
							if(geokeys[i+3]<=apc) attr.GetAdd(Format("GK%d",geokeys[i])) = Value(String(&asciiparams[geokeys[i+3]],geokeys[i+2]));
							break;
						case TIFFTAG_GEODOUBLEPARAMS:
							if(geokeys[i+3]<=dpc) attr.GetAdd(Format("GK%d",geokeys[i])) = Value(doubleparams[geokeys[i+3]]);
							break;
						default:
							attr.GetAdd(Format("GK%d",geokeys[i])) = Value(geokeys[i+3]);
							break;
					}
				}

			double geomatrix[6];
	        geomatrix[0] = 0.0;
	        geomatrix[1] = 1.0;
	        geomatrix[2] = 0.0;
	        geomatrix[3] = 0.0;
	        geomatrix[4] = 0.0;
	        geomatrix[5] = 1.0;
	        
	        auto AdjustMatrixOrientation = [&](double dx, double dy) {
				geomatrix[0] += (geomatrix[1] * dx + geomatrix[2] * dy);
				geomatrix[3] += (geomatrix[4] * dx + geomatrix[5] * dy);
				if(dx!=0){
					geomatrix[1] = -geomatrix[1];
					geomatrix[4] = -geomatrix[4];
				}
				if(dy!=0){
					geomatrix[2] = -geomatrix[2];
					geomatrix[5] = -geomatrix[5];
				}
	        };
	        
			auto NormalizeOrientation = [&] {
				if(!pixel_is_area) {
					geomatrix[0] -= (geomatrix[1] * 0.5 + geomatrix[2] * 0.5);
					geomatrix[3] -= (geomatrix[4] * 0.5 + geomatrix[5] * 0.5);
				}
				if(page.orientation>=5){
					Swap(geomatrix[1],geomatrix[2]);
					Swap(geomatrix[4],geomatrix[5]);
				}
				switch(page.orientation){
					case 2:
						AdjustMatrixOrientation(page.width, 0);
						break;
					case 3:
						AdjustMatrixOrientation(page.width, page.height);
						break;
					case 4:
						AdjustMatrixOrientation(0, page.height);
						break;
					case 6:
						AdjustMatrixOrientation(page.height, 0);
						break;
					case 7:
						AdjustMatrixOrientation(page.height, page.width);
						break;
					case 8:
						AdjustMatrixOrientation(0, page.width);
						break;
				}
				// TIFF orientations:
				// 1 = FLIP_MIRROR_VERT,
				// 2 = FLIP_ROTATE_180,
				// 3 = FLIP_MIRROR_HORZ,
				// 4 = FLIP_NONE,
				// 5 = FLIP_ROTATE_CLOCKWISE,
				// 6 = FLIP_TRANSPOSE,
				// 7 = FLIP_ROTATE_ANTICLOCKWISE,
				// 8 = FLIP_TRANSVERSE,
			};
	    
	        double  *data;
	        auto FinishMatrix = [&] {
				ValueArray va;
				for(int i = 0; i < 6; i++)
					va << geomatrix[i];
				attr.GetAdd("geo_matrix") = va;
	        };
	        
			if(TIFFGetField(tiff, TIFFTAG_GEOTRANSMATRIX, &count, &data) && count == 16) {
				geomatrix[0] = data[3];
				geomatrix[1] = data[0];
				geomatrix[2] = data[1];
				geomatrix[3] = data[7];
				geomatrix[4] = data[4];
				geomatrix[5] = data[5];

				NormalizeOrientation();
				FinishMatrix();
			}
			else
			if(TIFFGetField(tiff, TIFFTAG_GEOTIEPOINTS, &count, &data) && count >= 6) {
				if(count>=18){ // 3 tiepoints needed for Xform2D::Map()
					for(int t=0; t<count; t+=6){
						if(page.orientation>=5) Swap(data[t + 0], data[t + 1]);
						switch(page.orientation){
							case 1:
								break;
							case 2:
								data[t + 0] = page.width - data[t + 0];
								break;
							case 3:
								data[t + 0] = page.width - data[t + 0];
								data[t + 1] = page.height - data[t + 1];
								break;
							case 4:
								data[t + 1] = page.height - data[t + 1];
								break;
							case 5:
								break;
							case 6:
								data[t + 0] = page.height - data[t + 0];
								break;
							case 7:
								data[t + 0] = page.height - data[t + 0];
								data[t + 1] = page.width - data[t + 1];
								break;
							case 8:
								data[t + 1] = page.width - data[t + 1];
								break;
						}
					}

					Xform2D xf = Xform2D::Map(Pointf(data[0],data[1]),Pointf(data[6+0],data[6+1]),Pointf(data[12+0],data[12+1]),
												Pointf(data[3],data[4]),Pointf(data[6+3],data[6+4]),Pointf(data[12+3],data[12+4]));

					geomatrix[0] = xf.t.x;
					geomatrix[1] = xf.x.x;
					geomatrix[2] = xf.y.x;
					geomatrix[3] = xf.t.y;
					geomatrix[4] = xf.x.y;
					geomatrix[5] = xf.y.y;

					if(!pixel_is_area) {
						geomatrix[0] -= (geomatrix[1] * 0.5 + geomatrix[2] * 0.5);
						geomatrix[3] -= (geomatrix[4] * 0.5 + geomatrix[5] * 0.5);
					}
					FinishMatrix();
				}
				else{
					double x = data[0];
					double y = data[1];
					double e = data[3];
					double n = data[4];

					if(TIFFGetField(tiff, TIFFTAG_GEOPIXELSCALE, &count, &data) && count >= 2 && data[0] && data[1]) {
						double dx = data[0];
						double dy = -abs(data[1]);

						geomatrix[0] = e - x * dx;
						geomatrix[1] = dx;
						geomatrix[2] = 0;
						geomatrix[3] = n - y * dy;
						geomatrix[4] = 0;
						geomatrix[5] = dy;

						NormalizeOrientation();
						FinishMatrix();
					}
				}
			}

			if(TIFFGetField(tiff, 42113, &count, &data) && // TIFFTAG_GDAL_NODATA = 42113
				count >= 1){
					attr.GetAdd("nodata") = String((const char*)data, count);
				}

		}
	}
	return SeekPage(0);
}

bool TIFRaster::Data::SeekPage(int pgx)
{
	if(page_open) {
		TIFFRGBAImageEnd(this);
		page_open = false;
	}

	ASSERT(pgx >= 0 && pgx < pages.GetCount());
	page_index = pgx;
	page_error = false;
	TIFFSetDirectory(tiff, page_index);

	char emsg[1024];
	if(!TIFFRGBAImageBegin(this, tiff, 0, emsg)) {
		TIFFError(TIFFFileName(tiff), "%s", emsg);
		page_error = true;
		return false;
	}

	page_open = true;
	const Page& page = pages[page_index];

	size = Size(page.width, page.height);
	if(isContig) {
		contig = put.contig;
		put.contig = putContigRGB;
	}
	else {
		separate = put.separate;
		put.separate = putSeparate;
	}
	
	attr.GetAdd("BITSPERSAMPLE") = page.bits_per_sample;
	attr.GetAdd("SAMPLESPERPIXEL") = page.samples_per_pixel;
	attr.GetAdd("PHOTOMETRIC") = page.photometric;
	attr.GetAdd("tiff_orientation") = Value((int)page.orientation);

	alpha = page.alpha;
	if(alpha) {
		format.Set32le(0xFF << 16, 0xFF << 8, 0xFF, 0xFF << 24);
		bpp = 32;
	}
	else {
		format.Set24le(0xFF << 16, 0xFF << 8, 0xFF);
		bpp = 24;
	}
	palette_count = 0;
	if(isContig	&& (photometric == PHOTOMETRIC_PALETTE
	|| photometric == PHOTOMETRIC_MINISWHITE || photometric == PHOTOMETRIC_MINISBLACK)
	&& (bitspersample == 1 || bitspersample == 2 || bitspersample == 4 || bitspersample == 8)) {
		bpp = 8;
		tif_uint32 **ppal = (photometric == PHOTOMETRIC_PALETTE ? PALmap : BWmap);
		ASSERT(ppal);
//		byte rshift = 8 - img.bitspersample;
		palette_count = 1 << min<int>(bitspersample, 8);
		byte mask = (1 << bitspersample) - 1;
		int part_last = 8 / bitspersample - 1;
		int i;
		for(i = 0; i <= mask; i++) {
			uint32 rgba = ppal[i][part_last];
			palette[i].r = (byte)TIFFGetR(rgba);
			palette[i].g = (byte)TIFFGetG(rgba);
			palette[i].b = (byte)TIFFGetB(rgba);
			palette[i].a = 255;
		}
		put.contig = putContig8;
		switch(bitspersample) {
		case 1: bpp = 1; put.contig = putContig1; format.Set1mf(); break;
		case 2: bpp = 2; put.contig = putContig2; format.Set2mf(); break;
		case 4: bpp = 4; put.contig = putContig4; format.Set4mf(); break;
		case 8: format.Set8(); break;
		default: NEVER();
		}
		skewfac = 8 / bitspersample;
	}
	row_bytes = (bpp * width + 31) >> 5 << 2;

	page_fetched = false;
	return true;
}

bool TIFRaster::Data::FetchPage()
{
	if(page_error)
		return false;
	if(page_fetched)
		return true;

	cache_size = 0;
	rows.Clear();
	int64 bytes = row_bytes * (int64)height;
	imagebuf.SetCount(size.cy * row_bytes, 0);
	req_orientation = pages[page_index].orientation;

	bool res = TIFFRGBAImageGet(this, 0, width, height);
	TIFFRGBAImageEnd(this);
	page_open = false;

	page_fetched = true;
	return true;
}

Raster::Info TIFRaster::Data::GetInfo()
{
	const Page& page = pages[page_index];
	Raster::Info out;
	out.kind = (page.alpha ? IMAGE_ALPHA : IMAGE_OPAQUE);
	out.bpp = bpp;
	out.colors = 0;
	out.dots = page.dot_size;
	out.hotspot = Null;
	const int info_orientation[] = { FLIP_NONE, FLIP_MIRROR_VERT, FLIP_ROTATE_180, FLIP_MIRROR_HORZ, FLIP_NONE, FLIP_ROTATE_CLOCKWISE, FLIP_TRANSPOSE, FLIP_ROTATE_ANTICLOCKWISE, FLIP_TRANSVERSE, FLIP_NONE}; // Map from TIFFTAG_ORIENTATION to U++ orientation
	out.orientation = info_orientation[min((int)page.orientation,9)];
	return out;
}

Raster::Line TIFRaster::Data::GetLine(int line, Raster *raster)
{
	if(!page_error && !page_fetched)
		FetchPage();
	if(page_error) {
		byte *tmp = new byte[row_bytes];
		memset(tmp, 0, row_bytes);
		return Raster::Line(tmp, raster, true);
	}
	if(!imagebuf.IsEmpty())
		return Raster::Line(&imagebuf[row_bytes * line], raster, false);
	const byte *data = MapDown(0, line, row_bytes);
	byte *tmp = new byte[row_bytes];
	memcpy(tmp, data, row_bytes);
	return Raster::Line(tmp, raster, true);
}

TIFRaster::TIFRaster()
{
}

TIFRaster::~TIFRaster()
{
}

bool TIFRaster::Create()
{
	data = new Data(GetStream());
	return data->Create();
}

Size TIFRaster::GetSize()
{
	return data->size;
}

Raster::Info TIFRaster::GetInfo()
{
	return data->GetInfo();
}

Raster::Line TIFRaster::GetLine(int line)
{
	return data->GetLine(line, this);
}

int TIFRaster::GetPaletteCount()
{
	return data->palette_count;
}

const RGBA *TIFRaster::GetPalette()
{
	return data->palette;
}

const RasterFormat *TIFRaster::GetFormat()
{
	return &data->format;
}

int TIFRaster::GetPageCount()
{
	return data->pages.GetCount();
}

int TIFRaster::GetActivePage() const
{
	return data->page_index;
}

void TIFRaster::SeekPage(int n)
{
	data->SeekPage(n);
}

Value TIFRaster::GetMetaData(String id)
{
	return data->attr.Get(id, Value());
}

void TIFRaster::EnumMetaData(Vector<String>& id_list)
{
	id_list = clone(data->attr.GetKeys());
}

class TIFEncoder::Data {
public:
	Data(Stream& stream, RasterFormat& format);
	~Data();

	void             Start(Size size, Size dots, int bpp, const RGBA *palette);
	void             WriteLineRaw(const byte *line);

private:
	Stream&          stream;
	TIFF             *tiff;
	Size             size;
	int              bpp;
	const RGBA       *palette;
	Vector<byte>     rowbuf;
	int              linebytes;
	RasterFormat&    format;
	int              line;

	static tsize_t   ReadStream(thandle_t fd, tdata_t buf, tsize_t size);
	static tsize_t   WriteStream(thandle_t fd, tdata_t buf, tsize_t size);
	static toff_t    SeekStream(thandle_t fd, toff_t off, int whence);
	static int       CloseStream(thandle_t fd);
	static toff_t    SizeStream(thandle_t fd);
	static int       MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize);
	static void      UnmapStream(thandle_t fd, tdata_t base, toff_t size);
};

TIFEncoder::Data::Data(Stream& stream, RasterFormat& format)
: stream(stream), format(format)
{
	tiff = NULL;
}

TIFEncoder::Data::~Data()
{
	if(tiff) TIFFClose(tiff);
}

tsize_t TIFEncoder::Data::ReadStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Data& wrapper = *reinterpret_cast<Data *>(fd);
	ASSERT(wrapper.stream.IsOpen());
//	RLOG("TiffStream::ReadStream & " << (int)wrapper.stream.GetPos() << ", count = " << size
//		<< ", end = " << (int)(wrapper.stream.GetPos() + size));
	return wrapper.stream.Get(buf, size);
	return 0;
}

tsize_t TIFEncoder::Data::WriteStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	Data& wrapper = *reinterpret_cast<Data *>(fd);
	ASSERT(wrapper.stream.IsOpen());
//	RLOG("TIFRaster::Data::WriteStream & " << (int)wrapper.stream.GetPos() << ", count = " << (int)size
//		<< ", end = " << (int)(wrapper.stream.GetPos() + size));
	wrapper.stream.Put(buf, size);
	return size;
}

toff_t TIFEncoder::Data::SeekStream(thandle_t fd, toff_t off, int whence)
{
	Data& wrapper = *reinterpret_cast<Data *>(fd);
	ASSERT(wrapper.stream.IsOpen());
	toff_t size = (toff_t)wrapper.stream.GetSize();
	toff_t destpos = (toff_t)(off + (whence == 1 ? wrapper.stream.GetPos() : whence == 2 ? size : 0));
	if(destpos > size) {
		wrapper.stream.Seek(size);
		wrapper.stream.Put((int)0, (int)(destpos - size));
	}
	else
		wrapper.stream.Seek(destpos);
//	RLOG("TIFRaster::Data::SeekStream -> " << (int)off << ", whence = " << whence << " -> pos = " << (int)wrapper.stream.GetPos());
	return (toff_t)wrapper.stream.GetPos();
}

int TIFEncoder::Data::CloseStream(thandle_t fd)
{
	return 0;
}

toff_t TIFEncoder::Data::SizeStream(thandle_t fd)
{
	Data& wrapper = *reinterpret_cast<Data *>(fd);
	ASSERT(wrapper.stream.IsOpen());
//	RLOG("TIFRaster::Data::SizeStream -> " << (int)wrapper.stream.GetSize());
	return (toff_t)wrapper.stream.GetSize();
}

int TIFEncoder::Data::MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize)
{
	return 0;
}

void TIFEncoder::Data::UnmapStream(thandle_t fd, tdata_t base, toff_t size)
{
}

void TIFEncoder::Data::Start(Size sz, Size dots, int bpp_, const RGBA *palette)
{
	size = sz;
	bpp = bpp_;
	line = 0;

	tiff = TIFFClientOpen("tiff@" + Format64((intptr_t)this), "w", reinterpret_cast<thandle_t>(this),
		ReadStream, WriteStream, SeekStream, CloseStream, SizeStream, MapStream, UnmapStream);

	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, size.cx);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, size.cy);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, min<int>(bpp, 8));
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, bpp <= 8 ? PHOTOMETRIC_PALETTE : PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, bpp <= 8 ? 1 : bpp != 32 ? 3 : 4);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	if(bpp == 32) {
		uint16 es = EXTRASAMPLE_ASSOCALPHA;
		TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, &es);
	}
//	TIFFSetField(tiff, TIFFTAG_REFERENCEBLACKWHITE, refblackwhite);
//	TIFFSetField(tiff, TIFFTAG_TRANSFERFUNCTION, gray);
	if(dots.cx && dots.cy) {
		TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, (uint16)RESUNIT_INCH);
		float xres = float(sz.cx * 600.0 / dots.cx);
		TIFFSetField(tiff, TIFFTAG_XRESOLUTION, xres);
		float yres = float(sz.cy * 600.0 / dots.cy);
		TIFFSetField(tiff, TIFFTAG_YRESOLUTION, yres);
	}
	switch(bpp) {
		case 1: format.Set1mf(); break;
		case 2: format.Set2mf(); break;
		case 4: format.Set4mf(); break;
		case 8: format.Set8(); break;
		default: NEVER();
		case 24: format.Set24le(0xFF, 0xFF << 8, 0xFF << 16); break;
		case 32: format.Set32le(0xFF, 0xFF << 8, 0xFF << 16, 0xFF << 24); break;
	}
	if(bpp <= 8) {
		uint16 rpal[256], gpal[256], bpal[256];
		int c = 1 << bpp;
		memset(rpal, 0, sizeof(uint16) * c);
		memset(gpal, 0, sizeof(uint16) * c);
		memset(bpal, 0, sizeof(uint16) * c);
		for(int i = 0; i < c; i++) {
			rpal[i] = palette[i].r << 8;
			gpal[i] = palette[i].g << 8;
			bpal[i] = palette[i].b << 8;
		}
		TIFFSetField(tiff, TIFFTAG_COLORMAP, rpal, gpal, bpal);
	}
	int rowbytes = (bpp * size.cx + 31) >> 5 << 2;
	rowbuf.SetCount(rowbytes);
	linebytes = format.GetByteCount(size.cx);
}

void TIFEncoder::Data::WriteLineRaw(const byte *s)
{
	memcpy(rowbuf.Begin(), s, linebytes);
	TIFFWriteScanline(tiff, rowbuf.Begin(), line, 0);
	if(++line >= size.cy) {
		TIFFClose(tiff);
		tiff = NULL;
	}
}

TIFEncoder::TIFEncoder(int bpp)
: bpp(bpp)
{
}

TIFEncoder::~TIFEncoder()
{
}

int TIFEncoder::GetPaletteCount()
{
	return (bpp > 8 ? 0 : 1 << bpp);
}

void TIFEncoder::Start(Size sz)
{
	data = new Data(GetStream(), format);
	data->Start(sz, GetDots(), bpp, bpp <= 8 ? GetPalette() : NULL);
}

void TIFEncoder::WriteLineRaw(const byte *s)
{
	data->WriteLineRaw(s);
}


INITIALIZER(TIFRaster) {
	StreamRaster::Register<TIFRaster>();
}

}