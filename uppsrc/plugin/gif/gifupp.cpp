#include "gif.h"

namespace Upp {

struct GifGlobalInfo // information about gif file
{
	void     Serialize(Stream& stream);

	byte     tag[6];
	word     width;
	word     height;
	byte     flags;

	static const int GCT_PRESENT   = 0x80;
	static const int GCT_SORTED    = 0x08;
	static const int GCT_BITS_MASK = 0x07;
	static const int GCT_BPP_MASK  = 0x07;
	static const int GCT_BPP_SHIFT = 4;

	byte     bgnd;     // background color index
	byte     raw_aspect;

	bool     gct_flag; // true = GCT is present
	bool     gct_sort; // true = GCT is sorted
	int      gct_bits; // log2(records in GCT)
	int      gct_recs; // records in GCT = 1 << gct_bits
	int      gct_size; // bytes in GCT = 3 * gct_rect
	int      bpp;      // bits per pixel
	int      pixels;   // pixel types = 1 << bpp
	int      aspect;   // aspect ratio * 1000, 0 = undefined

	int64    glo_fpos; // file position of this global info
	int64    gct_fpos; // file position of gct
	int64    loc_fpos; // address of 1st local subimage descriptor
};

void GifGlobalInfo::Serialize(Stream& stream)
{
	glo_fpos = stream.GetPos();
	stream.SerializeRaw(tag, 6);
	if(stream.IsError() || memcmp(tag, "GIF8", 4)) {
		stream.SetError();
		return;
	}
	stream % width % height % flags % bgnd % raw_aspect;
	if(stream.IsLoading()) {
		gct_flag = flags & GCT_PRESENT;
		gct_sort = flags & GCT_SORTED;
		gct_bits = (flags & GCT_BITS_MASK) + 1;
		gct_recs = 1 << gct_bits;
		gct_size = 3 * gct_recs;
		bpp      = ((flags >> GCT_BPP_SHIFT) & GCT_BPP_MASK) + 1;
		pixels   = 1 << bpp;
		if(raw_aspect == 0)
			aspect = 0;
		else
			aspect = ((raw_aspect + 15) * 125u) >> 3;
	}

	loc_fpos = gct_fpos = stream.GetPos();
	if(gct_flag)
		loc_fpos += gct_size;
}

struct GifLocalInfo // subimage info
{
	bool    Load(Stream& stream);

	word    x;
	word    y;
	word    width;
	word    height;
	byte    lct_flag;    // 1 = LCT is present
	byte    lct_sort;    // 1 = LCT is sorted
	word    lct_bits;    // log2(records in LCT)
	word    lct_recs;    // records in LCT = 1 << lct_bits
	word    lct_size;    // bytes in LCT = 3 * lct_recs
	byte    interlace;   // 1 = interlaced

	int     transparent; // transparent color index, -1 = unused
	String  comment;     // comment string

	int64   loc_fpos;    // file address of this structure
	int64   lct_fpos;    // file address of LCT
	int64   dat_fpos;    // data position (1st byte after LCT)
	
	int 	delay;		 // delay time
	int 	disposal;	 // Image processing
};

bool GifLocalInfo::Load(Stream& stream)
{
	byte flags;
	int  c1;

	transparent = -1;
	delay = 0;

	while((c1 = stream.Get()) != 0x2C)
		if(c1 == 0x00)
			;
		else if(c1 == 0x21) {
			int code = stream.Get();
			int len = stream.Get();
			int64 next = stream.GetPos() + len;
			if(code == 0xF9) { // read transparent color index
				byte flags = stream.Get(); // packed fields
				delay = stream.Get16le();  // delay time in 1/100 sec
				transparent = stream.Get();
				if(!(flags & 1))
					transparent = -1;
				disposal = (flags >> 2) & 7;		// 16  32
			}
			else if(code == 0xFE) { // comment
				stream.Seek(next);
				while((len = stream.Get()) != 0) {
					int old = comment.GetLength();
					StringBuffer b(old + len);
					memcpy(b, comment, old);
					bool res = stream.GetAll(~b + old, len);
					comment = b;
					if(!res)
						return false;
				}
				next = 0;
			}
			if(next) {
				stream.Seek(next);
				while((len = stream.Get()) > 0)
					stream.SeekCur(len);
			}
		}
		else
			return false;

	loc_fpos  = stream.GetPos() - 1;
	x         = stream.Get16le();
	y         = stream.Get16le();
	width     = stream.Get16le();
	height    = stream.Get16le();
	flags     = stream.Get();
	lct_flag  = (flags & 0x80) != 0;
	lct_sort  = (flags & 0x20) != 0;
	lct_bits  = (flags & 0x07) + 1;
	lct_recs  = 1 << lct_bits;
	lct_size  = 3 * lct_recs;
	interlace = (flags & 0x40) != 0;
	lct_fpos  = stream.GetPos();
	dat_fpos  = lct_fpos;
	if(lct_flag)
		dat_fpos += lct_size;
	return true;
}

class GIFRaster::Data {
public:
	Data(GIFRaster& owner, Stream& stream);
	~Data();

	bool               Create();
	Raster::Line       GetLine(int line);
	bool               SeekPage(int page);
	void               GetPagesData();
	const RasterFormat *GetFormat() const  { return &format; }
	int                GetPaletteCount()   { return 256; }
	RGBA               *GetPalette()       { return palette; }

public:
	Size size;
	Raster::Info info;
	struct Page {
		int64 beginPos; 
		int delay;
		int x;
		int y;
		int width;
		int height;
		int disposal;
	};
	Array<Page>     pages;
	int             page_index;
	int 			aspect;   	// aspect ratio * 1000, 0 = undefined
	
private:
	void ClearTable();
	bool LoadSubimage(int64 &beginPos, int &delay, int &x, int &y, int &width, int &height, int &disposal);
	bool GotoSubimage(int& transparent_index);
	bool FetchDataBlock();
	bool Fetch(byte *&cptr, const byte *cend);
	
private:
	GIFRaster& owner;
	Stream& stream;
	RasterFormat format;
	RGBA gpalette[256];
	RGBA palette[256];
	GifGlobalInfo ggi;
	Vector<byte> temp;
	bool first_row;
	
	static const int MAX_BITS = 12;
	static const int MAX_COUNT = 1 << MAX_BITS;
	static const int HASH_COUNT = 5331;

	struct Item {
		union {
			const Item *prefix;
			Item       *child;
		};
		Item       *left, *right;
		short       length;
		byte        character;
	};

	Item              items[MAX_COUNT];
	int               item_count;
	byte              old_first;
	int               old_code;
	int               old_bits;
	int               max_bit_count;
	int               start_bpp;
	byte             *data_ptr;
	byte             *data_end;
	byte              data_block[256];
	dword             old_byte;
	int               old_shift;
};

GIFRaster::Data::Data(GIFRaster& owner, Stream& stream)
: owner(owner), stream(stream)
{
	page_index = 0;
}

GIFRaster::Data::~Data()
{
}

void GIFRaster::Data::ClearTable()
{
	for(int i = 0, n = 1 << start_bpp; i < n; i++) {
		Item& item = items[i];
		item.character = i;
		item.length = 1;
		item.prefix = 0;
	}
	item_count = (1 << start_bpp);
	items[item_count++].length = -1;
	items[item_count++].length = -2;
	old_code = -1;
	old_bits = start_bpp;
	max_bit_count = 1 << old_bits;
	while(item_count > max_bit_count) {
		old_bits++;
		max_bit_count <<= 1;
	}
}

bool GIFRaster::Data::FetchDataBlock()
{
//	TIMING("GifEncoder::FetchDataBlock");

	int count = stream.Term();
	if(count <= 0)
		return false;
	stream.Get();
	if(!stream.GetAll(data_block, count))
		return false;
	data_ptr = data_block;
	data_end = data_ptr + count;
	return true;
}

bool GIFRaster::Data::Fetch(byte *&cptr, const byte *cend)
{
//	TIMING("GifEncoder::Fetch");
	byte *cp = cptr;
	byte _old_shift = old_shift;
	byte _old_bits = old_bits;
	byte *_data_ptr = data_ptr;
	byte *_data_end = data_end;
	dword _old_byte = old_byte;
	while(cp < cend) {
//		static TimingInspector ti("Fetch-Get");
//		ti.Start();
		int code;
		if(_old_shift + _old_bits <= 8) {
			code = (_old_byte >> _old_shift) & ((1 << _old_bits) - 1);
			_old_shift += _old_bits;
		}
		else {
			if(_data_ptr >= _data_end) {
				if(!FetchDataBlock()) {
//					ti.End();
					cptr = cp;
					return false;
				}
				_data_ptr = data_ptr;
				_data_end = data_end;
			}
			_old_byte |= *_data_ptr++ << 8;
			if(_old_shift + _old_bits <= 16) {
				code = (_old_byte >> _old_shift) & ((1 << _old_bits) - 1);
				_old_shift += _old_bits - 8;
				_old_byte >>= 8;
			}
			else {
				if(_data_ptr >= _data_end) {
					if(!FetchDataBlock()) {
//						ti.End();
						cptr = cp;
						return false;
					}
					_data_ptr = data_ptr;
					_data_end = data_end;
				}
				_old_byte |= *_data_ptr++ << 16;
				ASSERT(_old_shift + _old_bits <= 24);
				code = (_old_byte >> _old_shift) & ((1 << _old_bits) - 1);
				_old_shift += _old_bits - 16;
				_old_byte >>= 16;
			}
		}
//		ti.End();

		if(code < item_count) {
//			TIMING("Fetch-existing code");
			const Item& item = items[code];
			if(item.length < 0)
				if(item.length == -1) {
					ClearTable();
					_old_bits = old_bits;
				}
				else {
					cptr = cp;
					return false; // end of stream
				}
			else { // copy new pixels to output
				byte *p = (cp += item.length);
				byte first;
				const Item *i = &item;
				*--p = i->character;
				int left = item.length;
				if(--left > 0) {
					for(; left >= 8; p -= 8, left -= 8) {
						p[-1] = (i = i->prefix)->character;
						p[-2] = (i = i->prefix)->character;
						p[-3] = (i = i->prefix)->character;
						p[-4] = (i = i->prefix)->character;
						p[-5] = (i = i->prefix)->character;
						p[-6] = (i = i->prefix)->character;
						p[-7] = (i = i->prefix)->character;
						p[-8] = (i = i->prefix)->character;
					}
					if(left & 4) {
						p[-1] = (i = i->prefix)->character;
						p[-2] = (i = i->prefix)->character;
						p[-3] = (i = i->prefix)->character;
						p[-4] = (i = i->prefix)->character;
						p -= 4;
					}
					if(left & 2) {
						p[-1] = (i = i->prefix)->character;
						p[-2] = (i = i->prefix)->character;
						p -= 2;
					}
					if(left & 1)
						p[-1] = (i = i->prefix)->character;
				}
				first = i->character;
//*/
//				while(i->prefix)
//					*--p = (i = i->prefix)->character;
				if(old_code >= 0 && item_count < MAX_COUNT) { // add to string table
					Item& item = items[item_count++];
					item.character = first;
					item.prefix = &items[old_code];
					item.length = item.prefix->length + 1;
				}
				if(item_count >= max_bit_count && item_count < MAX_COUNT) {
					max_bit_count <<= 1;
					++_old_bits;
				}
				old_code = code;
				old_first = first;
			}
		}
		else { // special case: duplicate first character of old_code
//			TIMING("Fetch-new code");
			if(old_code < 0 || code > item_count) { // decoder error
				stream.SetError();
				cptr = cp;
				return false;
			}
			ASSERT(item_count < MAX_COUNT);
			Item& item = items[item_count++];
			item.prefix = &items[old_code];
			item.length = item.prefix->length + 1;
			item.character = old_first;
			const Item *i = &item;
			byte *p = (cp += item.length);
			*--p = i->character;
			while(i->prefix)
				*--p = (i = i->prefix)->character;
			if(item_count >= max_bit_count && item_count < MAX_COUNT) {
				max_bit_count <<= 1;
				++_old_bits;
			}
			old_code = code;
		}
	}
	cptr = cp;
	old_shift = _old_shift;
	old_bits = _old_bits;
	data_ptr = _data_ptr;
	data_end = _data_end;
	old_byte = _old_byte;
	return true;
}

bool GIFRaster::Data::GotoSubimage(int& transparent_index)
{
	stream.Seek(pages[page_index].beginPos);
	
	GifLocalInfo l;
	if(!l.Load(stream))
		return false;
	
	if(l.transparent >= 0)
		transparent_index = l.transparent;

	if(l.x + l.width > size.cx || l.y + l.height > size.cy)
		return false; // out of bitmap rectangle

	// ignore LCT
	stream.Seek(l.dat_fpos);

	if((start_bpp = stream.Get()) <= 0 || start_bpp > 12)
		return false;

	data_ptr = data_end = data_block; // empty data block
	old_shift = 8;
	old_byte = 0;
	ClearTable();

	int cache = max(int(l.width), 1000);
	Buffer<byte> data(cache + MAX_COUNT);
	bool more = true;
	byte *data_ptr = data, *data_end = data;
	int pass = 0;
	for(int y = 0; y < l.height;) {
		if(more && data_ptr + l.width > data_end) { // fetch more data
			if(data_end > data_ptr)
				Copy((byte *)data, data_ptr, data_end);
			data_end -= (data_ptr - data);
			data_ptr = data;
			more = Fetch(data_end, data + cache);
		}
		int avail = min(int(l.width), int(intptr_t(data_end - data_ptr)));
		if(avail == 0)
			break;
		byte *wrt = &temp[y * l.width];
		memcpy(wrt, data_ptr, avail);
		data_ptr += avail;
		if(l.interlace)
			switch(pass) {
			case 0: if((y += 8) < l.height) break; y = 4 - 8; pass++;
			case 1: if((y += 8) < l.height) break; y = 2 - 4; pass++;
			case 2: if((y += 4) < l.height) break; y = 1 - 2; pass++;
			case 3: y += 2; break;
			}
		else
			y++;
	}
	return true;
}

bool GIFRaster::Data::LoadSubimage(int64 &beginPos, int &delay, int &x, int &y, int &width, int &height, int &disposal)
{ 
	beginPos = stream.GetPos();
	delay = 0;
	GifLocalInfo l;
	if(!l.Load(stream))
		return false;
	
	delay  	 = l.delay;
	x 	   	 = l.x;
	y 	   	 = l.y;
	width  	 = l.width;
	height 	 = l.height;
	disposal = l.disposal;
	
	if(l.x + l.width > size.cx || l.y + l.height > size.cy)
		return false; // out of bitmap rectangle

	memcpy(palette, gpalette, 256 * sizeof(RGBA));

	if(l.lct_flag) {
		stream.Seek(l.lct_fpos);
		for(int i = 0; i < l.lct_recs; i++) {
			palette[i].r = stream.Get();
			palette[i].g = stream.Get();
			palette[i].b = stream.Get();
			palette[i].a = 255;
		}
	}

	stream.Seek(l.dat_fpos);

	if((start_bpp = stream.Get()) <= 0 || start_bpp > 12)
		return false;

	while(FetchDataBlock()) // flush remaining data blocks
		;

	stream.Get(); // fetch terminating empty subblock
	return true;
}

bool GIFRaster::Data::Create()
{
	stream % ggi;
	if(stream.IsError() || ggi.width <= 0 || ggi.height <= 0)
		return false;
	size.cx = ggi.width;
	size.cy = ggi.height;
	info.bpp = 8;
	info.colors = ggi.gct_recs;
	info.dots = size;
	info.hotspot = Null;
	info.kind = IMAGE_UNKNOWN;
	temp.Clear();
	first_row = true;

	stream.Seek(ggi.gct_fpos);
	for(int i = 0; i < ggi.gct_recs; i++) {
		gpalette[i].r = stream.Get();
		gpalette[i].g = stream.Get();
		gpalette[i].b = stream.Get();
		gpalette[i].a = 255;
	}
	format.Set8();
	GetPagesData();
	
	SeekPage(0);
	return true;
}

Raster::Line GIFRaster::Data::GetLine(int line)
{
	return Raster::Line(&temp[pages[page_index].width * line], &owner, false);
}

bool GIFRaster::Data::SeekPage(int npage) {
	page_index = npage;

	stream.Seek(ggi.loc_fpos);
	int transparent_index = -1;
	if(!GotoSubimage(transparent_index))
		return false;

	if(transparent_index >= 0 && transparent_index < 256)
		palette[transparent_index] = RGBAZero();	
	
	return true;
}

void GIFRaster::Data::GetPagesData() {
	temp.SetCount(size.cx * size.cy, 0);

	stream.Seek(ggi.loc_fpos);
	while(true) {
		int64 beginPos;
		int delay, x, y, width, height, disposal;
		if(!LoadSubimage(beginPos, delay, x, y, width, height, disposal)) {
			return;
		}
		Page &page = pages.Add();
		page.beginPos = beginPos;
		page.delay = delay;
		page.x = x;
		page.y = y;
		page.width = width;
		page.height = height;
		page.disposal = disposal;
	}
}

GIFRaster::GIFRaster()
{
}

GIFRaster::~GIFRaster()
{
}

bool GIFRaster::Create()
{
	data = new Data(*this, GetStream());
	return data->Create();
}

Size GIFRaster::GetSize()
{
	return data->size;
}

Raster::Info GIFRaster::GetInfo()
{
	return data->info;
}

Raster::Line GIFRaster::GetLine(int line)
{
	return data->GetLine(line);
}

const RasterFormat *GIFRaster::GetFormat()
{
	return data->GetFormat();
}

int GIFRaster::GetPaletteCount()
{
	return data->GetPaletteCount();
}

RGBA *GIFRaster::GetPalette()
{
	return data->GetPalette();
}

int GIFRaster::GetPageCount()
{
	return data->pages.GetCount();
}

int GIFRaster::GetActivePage() const
{
	return data->page_index;
}

void GIFRaster::SeekPage(int n)
{
	data->SeekPage(n);
}

int GIFRaster::GetPageAspect(int n) 
{
	return data->aspect;
}

int GIFRaster::GetPageDelay(int n) 
{
	return data->pages[n].delay;	
}

Rect GIFRaster::GetPageRect(int n) 
{
	return Rect(data->pages[n].x, data->pages[n].y, 
			 data->pages[n].x + data->pages[n].width, data->pages[n].y + data->pages[n].height);
}

int GIFRaster::GetPageDisposal(int n)
{
	return data->pages[n].disposal;	
}
	
class GIFEncoder::Data {
public:
	Data(Stream& stream, RasterFormat& format);
	~Data();

	void Start(Size sz, bool ignore_alpha, String comment, const RGBA *palette);
	void WriteLineRaw(const byte *s);

private:
	void FlushDataBlock();
	void ClearTable();

private:
	static const int MAX_BITS = 12;
	static const int MAX_COUNT = 1 << MAX_BITS;
	static const int HASH_COUNT = 5331;

	RasterFormat&    format;
	Stream& stream;

	struct Item {
		union {
			const Item *prefix;
			Item       *child;
		};
		Item       *left, *right;
		short       length;
		byte        character;
	};

	Item              items[MAX_COUNT];
	int               item_count;
	int               start_bpp;
	byte              data_block[256];
	byte             *data_ptr;
	byte             *data_end;
	int               old_code;
	int               old_bits;
	int               max_bit_count;

	int               _old_shift;
	int               _old_byte;
	int               _old_bits;
	int               restart_code;

	Size              size;
	int               ypos;

	Item              *i;
	Item              *p;
	Item              *new_item;
};

GIFEncoder::Data::Data(Stream& stream, RasterFormat& format)
: format(format), stream(stream)
{
}

GIFEncoder::Data::~Data()
{
}

void GIFEncoder::Data::FlushDataBlock()
{
//	TIMING("GifEncoder::FlushDataBlock");

	int count = (int)(uintptr_t)(data_ptr - data_block);
	if(count) {
		ASSERT(count <= 255);
		stream.Put(count);
		stream.Put(data_block, count);
		data_ptr = data_block;
	}
}

void GIFEncoder::Data::ClearTable()
{
	for(int i = 0, n = 1 << start_bpp; i < n; i++) {
		Item& item = items[i];
		item.character = i;
		item.length = 1;
		item.left = item.right = item.child = 0;
	}
	item_count = (1 << start_bpp);
	items[item_count++].length = -1;
	items[item_count++].length = -2;
	old_code = -1;
	old_bits = start_bpp;
	max_bit_count = 1 << old_bits;
	while(item_count > max_bit_count) {
		old_bits++;
		max_bit_count <<= 1;
	}
}

void GIFEncoder::Data::Start(Size sz, bool ignore_alpha, String comment, const RGBA *palette)
{
	size = sz;
	format.Set8();

	int transparent_index = -1;
	if(!ignore_alpha)
		transparent_index = 255;
	start_bpp = 8;
//	for(start_bpp = 1; start_bpp < 8 && raw_image->pixel.palette.GetCount() > (1 << start_bpp); start_bpp++)
//		;

	GifGlobalInfo g;
	memcpy(g.tag, !IsNull(comment) ? "GIF89a" : "GIF87a", 6);
	g.width = sz.cx;
	g.height = sz.cy;
	g.flags = g.GCT_PRESENT | ((start_bpp - 1) << g.GCT_BPP_SHIFT) | (start_bpp - 1);
//	if(optimize_palette)
//		g.flags |= g.GCT_SORTED;
	g.bgnd = 0;
	g.raw_aspect = 0;

	stream % g;
 { // write palette
		for(int i = 0, n = 1 << start_bpp; i < n; i++) {
			RGBA rgba = (i == transparent_index ? RGBAZero() : palette[i]);
			stream.Put(rgba.r);
			stream.Put(rgba.g);
			stream.Put(rgba.b);
		}
	}

	// write comment block
	if(!IsNull(comment)) {
		stream.Put(0x21);
		stream.Put(0xFE);
		for(int t = 0, n; (n = min(comment.GetLength() - t, 255)) > 0; t += n) {
			stream.Put(n);
			stream.Put(comment.Begin() + t, n);
		}
		stream.Put(0x00);
	}

	// store transparent color index
	if(transparent_index >= 0) {
		stream.Put(0x21);
		stream.Put(0xF9);
		stream.Put(4);
		stream.Put(1); // transparent index present
		stream.Put16le(0); // delay time = 0
		stream.Put(transparent_index);
		stream.Put(0);
	}

	if(start_bpp == 1)
		start_bpp++;

	stream.Put(0x2C);
	stream.Put16le(0);
	stream.Put16le(0);
	stream.Put16le(sz.cx);
	stream.Put16le(sz.cy);
	stream.Put(0x00); // no LCT, non-interlaced, no sort, LCT size-1 = 0

	data_ptr = data_block;
	data_end = data_ptr + 255;
	ClearTable();

	stream.Put(start_bpp); // start bits

	ypos = 0;
	_old_shift = 0;
	_old_byte = 0;
	_old_bits = old_bits;

//	int _lsh = start_bpp, _rsh = 32 - start_bpp;

//	static TimingInspector ti_put("PUT_CODE");

#define PUT_CODE(code) \
	do { \
		/*TimingInspector::Routine r(ti_put);*/ \
		_old_byte |= (code) << _old_shift; \
		_old_shift += _old_bits; \
		if(_old_shift >= 16) { \
			if(data_ptr >= data_end) \
				FlushDataBlock(); \
			*data_ptr++ = _old_byte; \
			if(data_ptr >= data_end) \
				FlushDataBlock(); \
			*data_ptr++ = _old_byte >>= 8; \
			_old_byte >>= 8; \
			_old_shift -= 16; \
		} \
		else if(_old_shift >= 8) { \
			if(data_ptr >= data_end) \
				FlushDataBlock(); \
			*data_ptr++ = _old_byte; \
			_old_byte >>= 8; \
			_old_shift -= 8; \
		} \
	} \
	while(false)

	restart_code = 1 << start_bpp;
	PUT_CODE(restart_code);

	i = new_item = NULL;
}

void GIFEncoder::Data::WriteLineRaw(const byte *sp)
{
	const byte *se = sp + size.cx;

	if(!i) {
		i = &items[*sp++];
		new_item = &items[item_count];
		ASSERT(i >= items && i <= &items[MAX_COUNT]);
	}

	for(;;) {
	SEEK:
		if(sp >= se)
			break;
		byte c = *sp++;
		if((i = (p = i)->child) == 0)
			p->child = new_item;
		else
			for(;;) {
				if(i->character == c)
					goto SEEK;
				if(c < i->character) {
					if(i->left)
						i = i->left;
					else {
						i->left = new_item;
						break;
					}
				}
				else {
					if(i->right)
						i = i->right;
					else {
						i = i->right = new_item;
						break;
					}
				}
			}

		// code not found - rewind last char & output current prefix, add new code to table
		sp--;
		PUT_CODE(p - items);
		if(item_count < MAX_COUNT) { // add new entry to item table
			if(item_count >= max_bit_count) {
				max_bit_count <<= 1;
				_old_bits++;
			}
			new_item->character = c;
			new_item->left = new_item->right = new_item->child = 0;
			item_count++;
		}
		else { // restart table
			PUT_CODE(restart_code);
			ClearTable();
			_old_bits = old_bits;
		}

		i = &items[*sp++];
		new_item = &items[item_count];
		ASSERT(i >= items && i <= &items[MAX_COUNT]);
	}

	if(++ypos >= size.cy) {
		PUT_CODE(i - items);
		// put terminator code & close output block
		PUT_CODE(restart_code + 1);
		if(_old_shift > 0) {
			if(data_ptr >= data_end)
				FlushDataBlock();
			*data_ptr++ = _old_byte;
		}
		FlushDataBlock();

		stream.Put(0x00);
		stream.Put(0x3B);
	}
}

GIFEncoder::GIFEncoder(bool ignore_alpha_, String comment_)
: ignore_alpha(ignore_alpha_), comment(comment_)
{
}

GIFEncoder::~GIFEncoder()
{
}

int GIFEncoder::GetPaletteCount()
{
	return 255;
}

void GIFEncoder::Start(Size sz)
{
	data = new Data(GetStream(), format);
	data->Start(sz, ignore_alpha, comment, GetPalette());
}

void GIFEncoder::WriteLineRaw(const byte *s)
{
	data->WriteLineRaw(s);
}

INITIALIZER(GifRaster)
{
	StreamRaster::Register<GIFRaster>();
}

}
