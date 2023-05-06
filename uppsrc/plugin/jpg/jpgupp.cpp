#include <Draw/Draw.h>
#include <setjmp.h>
#include "jpg.h"
#define HAVE_BOOLEAN
#define boolean int
#undef FAR
extern "C" {
#include "lib/jinclude.h"
#include "lib/jpeglib.h"
#include "lib/jerror.h"
}
#undef XINT32


namespace Upp {

#define LLOG(x)  // LOG(x)

static void NoOutput(j_common_ptr cinfo)
{
}

enum 
{
	STREAM_BUF_SIZE = 16384,
	ROW_BUF_SIZE    = 16384,
};

enum {
	EXIF_BYTE      =  1, // An 8-bit unsigned integer
	EXIF_ASCII     =  2, // An 8-bit byte containing one 7-bit ASCII code. The final byte is terminated with NULL
	EXIF_SHORT     =  3, // A 16-bit (2-byte) unsigned integer
	EXIF_LONG      =  4,  // A 32-bit (4-byte) unsigned integer
	EXIF_RATIONAL  =  5, // Two LONGs. The first LONG is the numerator and the second LONG expresses the denominator
	EXIF_UNDEFINED =  7, // An 8-bit byte that can take any value depending on the field definition
	EXIF_SLONG     =  9, // A 32-bit (4-byte) signed integer (2's complement notation)
	EXIF_SRATIONAL = 10, // Two SLONGs. The first SLONG is the numerator and the second SLONG is the denominator
};

struct jpg_stream_destination_mgr
{
	jpeg_destination_mgr  pub;
	Stream               *stream;
	JOCTET               *buffer;
};

typedef jpg_stream_destination_mgr *stream_dest_ptr;

static void init_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_IMAGE,
			STREAM_BUF_SIZE * sizeof(JOCTET));
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = STREAM_BUF_SIZE;
}

static boolean empty_output_buffer(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->stream->Put(dest->buffer, STREAM_BUF_SIZE * sizeof(JOCTET));
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = STREAM_BUF_SIZE;
	return true;
}

static void term_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	size_t done = STREAM_BUF_SIZE - dest->pub.free_in_buffer;
	if(done > 0)
		dest->stream->Put(dest->buffer, (dword)(done * sizeof(JOCTET)));
}

static void jpeg_stream_dest(j_compress_ptr cinfo, Stream& stream)
{
	if(cinfo->dest == NULL)
		cinfo->dest = (jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_destination_mgr));

	stream_dest_ptr dest = (stream_dest_ptr)cinfo->dest;
	dest->pub.init_destination    = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination    = term_destination;
	dest->pub.next_output_byte    = NULL;
	dest->pub.free_in_buffer      = 0;
	dest->stream                  = &stream;
	dest->buffer                  = NULL;
}

struct jpg_stream_source_mgr
{
	jpeg_source_mgr pub;
	Stream         *stream;
	JOCTET         *buffer;
	bool            start_of_file;
};

typedef jpg_stream_source_mgr *stream_src_ptr;

static void init_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	src->start_of_file = true;
}

static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	size_t nbytes = src->stream->Get(src->buffer, STREAM_BUF_SIZE * sizeof(JOCTET));
	if(nbytes == 0)
	{
		if(src->start_of_file)
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file       = false;

	return true;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	int above = (int)(num_bytes - src->pub.bytes_in_buffer);
	if(above < 0)
	{ // we're still within the input buffer
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer = -above;
	}
	else
	{
		src->stream->Seek(src->stream->GetPos() + above);
		src->start_of_file = false;
		src->pub.next_input_byte = NULL;
		src->pub.bytes_in_buffer = 0;
	}
}

static void term_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	src->pub.next_input_byte = NULL;
	src->pub.bytes_in_buffer = 0;
}

static void jpeg_stream_src(j_decompress_ptr cinfo, Stream& stream)
{
	stream_src_ptr src = (stream_src_ptr)cinfo->src;
	if(src == NULL)
	{
		cinfo->src = (jpeg_source_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_source_mgr));
		src = (stream_src_ptr)cinfo->src;
		src->buffer = (JOCTET *)(*cinfo->mem->alloc_small)((j_common_ptr)cinfo,
			JPOOL_PERMANENT, STREAM_BUF_SIZE * sizeof(JOCTET));
	}

	src->pub.init_source       = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data   = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source       = term_source;
	src->pub.bytes_in_buffer   = 0;
	src->pub.next_input_byte   = NULL;
	src->stream                = &stream;
}

struct jpeg_longjmp_error_mgr : public jpeg_error_mgr {
	jmp_buf jmpbuf;
};

static void error_exit(j_common_ptr cinfo)
{
	(*cinfo->err->output_message)(cinfo);
	jpeg_longjmp_error_mgr *jlem = (jpeg_longjmp_error_mgr *)cinfo->err;
	longjmp(jlem->jmpbuf, 1);
}

class JPGRaster::Data {
	friend class JPGRaster;

public:
	Data(JPGRaster& owner);
	~Data();

	bool Create(Stream& stream);
	void Construct();
	void Free();
	Raster::Info GetInfo();
	Raster::Line GetLine(int line);
	void ScanMetaData();
	void ScanExifData(const char *begin, const char *end);
	Value GetMetaData(String id);
	void EnumMetaData(Vector<String>& id_list);
	String GetThumbnail();

private:
	bool Init();

	int     Exif16(const char *s, const char *end) { return s + 2 < end ? exif_big_endian ? Peek16be(s) : Peek16le(s) : 0; }
	int     Exif32(const char *s, const char *end) { return s + 4 < end ? exif_big_endian ? Peek32be(s) : Peek32le(s) : 0; }
	double  ExifF5(const char *s, const char *end);

	enum IFD_TYPE { BASE_IFD, GPS_IFD };
	int  ExifDir(const char *begin, const char *end, int offset, IFD_TYPE type);

private:
	JPGRaster& owner;
	Stream *stream;
	int64 stream_fpos;

	RasterFormat format;
	RGBA palette[256];

	VectorMap<String, Value> metadata;

	jpeg_decompress_struct   cinfo;
	jpeg_longjmp_error_mgr  *jerr;

	Size size;
	Size dot_size;
	int row_bytes;
	int row_bytes_4;
	bool finish;
	bool exif_big_endian;

	int next_line;
};

void JPGRaster::Data::Construct()
{
	stream = NULL;
	size = dot_size = Null;
	next_line = 0;
	finish = false;

	jpeg_create_decompress(&cinfo);
}

JPGRaster::Data::Data(JPGRaster& owner_)
: owner(owner_)
{
	jerr = (jpeg_longjmp_error_mgr *)malloc(sizeof(jpeg_longjmp_error_mgr));
	Construct();
}

void JPGRaster::Data::Free()
{
	if(setjmp(jerr->jmpbuf))
		return;
	if(finish)
		jpeg_abort_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

JPGRaster::Data::~Data()
{
	Free();
	free(jerr);
}

bool JPGRaster::Data::Create(Stream& stream_)
{
	stream = &stream_;
	stream_fpos = stream->GetPos();
	cinfo.err = jpeg_std_error(jerr);
	cinfo.err->output_message = &NoOutput;
	cinfo.dct_method = JDCT_IFAST;
	jerr->error_exit = error_exit;

	return Init();
}

void JPGRaster::Data::ScanMetaData()
{
	if(!metadata.IsEmpty())
		return;
	for(jpeg_saved_marker_ptr p = cinfo.marker_list; p; p = p->next) {
		String data(p->data, p->data_length);
		String key;
		if(p->marker == JPEG_COM)
			key = "COM";
		else if(p->marker >= JPEG_APP0 && p->marker <= JPEG_APP0 + 15) {
			key = Format("APP%d", p->marker - JPEG_APP0);
			if(p->marker == JPEG_APP0 + 1 && !memcmp(data, "Exif\0\0", 6)) {
				ScanExifData(data.GetIter(6), data.end());
			}
		}
		if(metadata.Find(key) >= 0) {
			for(int i = 1;; i++) {
				String suffix;
				suffix << key << '$' << i;
				if(metadata.Find(suffix) < 0) {
					key = suffix;
					break;
				}
			}
		}
		metadata.Add(key, data);
	}
}

double JPGRaster::Data::ExifF5(const char *s, const char *end)
{
	unsigned num = Exif32(s + 0, end);
	unsigned den = Exif32(s + 4, end);
	return num / (double)(den ? den : 1);
}

int JPGRaster::Data::ExifDir(const char *begin, const char *end, int offset, IFD_TYPE type)
{
	if(offset < 0 || begin + offset > end)
		return 0;

	const char *e = begin + offset;
	int nitems = Exif16(e, end);
	e += 2;
	for(int i = 0; i < nitems; i++, e += 12) {
		int tag = Exif16(e, end);
		int fmt = Exif16(e + 2, end);
		int count = Exif32(e + 4, end);
		static const int fmtlen[] = {
			1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8
		};
		int len = 0;
		if(fmt > 0 && fmt <= __countof(fmtlen))
			len = fmtlen[fmt - 1] * count;
		const char *data = e + 8;
		if(len > 4)
			data = begin + Exif32(data, end);

		auto Text20 = [&](int atag, const char *id) {
			if(tag == atag) {
				String s;
				const char *e = min(end, data + 20);
				while(data < e && *data)
					s.Cat(*data++);
				metadata.Add(id, s);
			}
		};
			
		auto F5 = [&](int atag, const char *id) {
			if(tag == atag && fmt == EXIF_RATIONAL)
				metadata.Add(id, ExifF5(data, end));
		};
		
		auto I16 = [&](int atag, const char *id) {
			if(tag == atag && fmt == 3)
				metadata.Add(id, Exif16(data, end));
		};

		if(type == BASE_IFD) {
			I16(0x112, "orientation");
			I16(0x112, "Orientation");
			Text20(0x131, "Software");
			Text20(0x132, "DateTime");
			Text20(0x9003, "DateTimeOriginal");
			Text20(0x9004, "DateTimeDigitized");
			F5(0x011a, "XResolution");
			F5(0x011b, "YResolution");
			I16(0x0128, "ResolutionUnit");
			
			if(tag == 0x8825 && data + 20 < end) {
				int offset = Exif32(data, end);
				ExifDir(begin, end, offset, GPS_IFD);
			}
		}
		else if(type == GPS_IFD) {
			if((tag == 2 || tag == 4) && fmt == EXIF_RATIONAL && count == 3) {
				metadata.Add(tag == 2 ? "GPSLatitude" : "GPSLongitude",
					ExifF5(data + 0, end) + ExifF5(data + 8, end) / 60 + ExifF5(data + 16, end) / 3600);
			}
			else if(tag == 6 && fmt == EXIF_RATIONAL && count == 1)
				metadata.Add("GPSAltitude", ExifF5(data, end));
			else if(tag == 16 && fmt == EXIF_ASCII && count == 2 && *data)
				metadata.Add("GPSImgDirectionRef", String(*data, 1));
			else if(tag == 17 && fmt == EXIF_RATIONAL && count == 1)
				metadata.Add("GPSImgDirection", ExifF5(data + 0, end));
		}
	}
	return Exif32(e, end);
}

void JPGRaster::Data::ScanExifData(const char *begin, const char *end)
{
	const char *p = begin;
	if(p[0] == 'I' && p[1] == 'I')
		exif_big_endian = false;
	else if(p[0] == 'M' && p[1] == 'M')
		exif_big_endian = true;
	else
		return;
	for(int diroff = Exif32(p + 4, end); diroff && begin + diroff < end; diroff = ExifDir(begin, end, diroff, BASE_IFD))
		;
}

Value JPGRaster::Data::GetMetaData(String id)
{
	ScanMetaData();
	return metadata.Get(id, Value());
}

void JPGRaster::Data::EnumMetaData(Vector<String>& id_list)
{
	ScanMetaData();
	id_list <<= metadata.GetKeys();
}

String JPGRaster::Data::GetThumbnail()
{
	ScanMetaData();
	String app1 = GetMetaData("APP1");
	if(!app1.GetCount())
		return Null;
	const char *begin = ~app1 + 6;
	const char *end = app1.End();
	const char *p = begin + Exif32(begin + 4, end);
	if(p + 2 >= end) return Null;
	p += Exif16(p, end) * 12 + 2;
	if(p + 4 >= end) return Null;
	p = begin + Exif32(p, end);
	if(p <= begin || p + 2 >= end) return Null;
	word count = Exif16(p, end);
	p += 2;
	dword offset = 0;
	dword len = 0;
	for(int n = 0; n < count; n++) {
		if(p + 12 >= end) return Null;
		if(Exif32(p + 4, end) == 1) {
			dword val = 0;
			switch(Exif16(p + 2, end)) {
			case 4:
			case 9:
				val = Exif32(p + 8, end);
				break;
			case 3:
			case 8:
				val = Exif16(p + 8, end);
				break;
			}
			if(val)
				switch(Exif16(p, end)) {
				case 0x201:
					offset = val;
					break;
				case 0x202:
					len = val;
					break;
				}
		}
		p += 12;
	}
	return offset && len && begin + offset + len < end ? String(begin + offset, len) : String();
}

bool JPGRaster::Data::Init()
{
	if(setjmp(jerr->jmpbuf))
		return false;
	jpeg_stream_src(&cinfo, *stream);
	jpeg_save_markers(&cinfo, JPEG_COM, 0xFFFF);
	for(int i = 0; i <= 15; i++)
		jpeg_save_markers(&cinfo, JPEG_APP0 + i, 0xFFFF);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	switch(cinfo.output_components) {
		case 1: {
			format.Set8();
			for(int i = 0; i < 256; i++) {
				RGBA rgba;
				rgba.r = rgba.g = rgba.b = i;
				rgba.a = 255;
				palette[i] = rgba;
			}
			break;
		}
		case 3:
		case 4: {
			format.Set24le(0xFF, 0xFF00, 0xFF0000);
			break;
		}
		default: {
			LLOG("JPGRaster: invalid number of components: " << (int)cinfo.output_components);
			return false;
		}
	}

	size.cx = cinfo.output_width;
	size.cy = cinfo.output_height;

	row_bytes = cinfo.output_components * size.cx;
	row_bytes_4 = (row_bytes + 3) & -4;

	double dot_scaling = (cinfo.density_unit == 1 ? 600 : cinfo.density_unit == 2 ? 600.0 / 2.54 : 0);
	if(dot_scaling && cinfo.X_density > 0) {
		dot_size.cx = fround(cinfo.image_width  * dot_scaling / cinfo.X_density);
		dot_size.cy = fround(cinfo.image_height * dot_scaling / cinfo.Y_density);
	}

	finish = true;
	return true;
}

Raster::Info JPGRaster::Data::GetInfo()
{
	Raster::Info info;
	try {
		info.bpp = 24;
		info.colors = 0;
		info.dots = dot_size;
		info.hotspot = Point(0, 0);
		info.kind = IMAGE_OPAQUE;
	}
	catch(Exc e) {
		LLOG(e);
	}
	return info;
}

Raster::Line JPGRaster::Data::GetLine(int line)
{
	byte *rowbuf = new byte[row_bytes_4];
	if(setjmp(jerr->jmpbuf))
		return Raster::Line(rowbuf, &owner, true);

	ASSERT(line >= 0 && line < size.cy);
	if(line < next_line) {
		stream->Seek(stream_fpos);
		Stream *s = stream;
		Free();
		Construct();
		Create(*s);
		next_line = 0;
		LOG("Seek back");
	}
	JSAMPROW rowptr[] = { (JSAMPROW)rowbuf };
	while(next_line++ < line)
		jpeg_read_scanlines(&cinfo, rowptr, 1);
	jpeg_read_scanlines(&cinfo, rowptr, 1);

	if(cinfo.output_components == 4) { 
		/* Convert CMYK scanline to RGB */
		JSAMPLE k;
		for(int i = 0, j = 0; i < row_bytes_4; i++){			
			k = GETJSAMPLE(rowbuf[i+3]);
			rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		    rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		    rowbuf[j++] = GETJSAMPLE(rowbuf[i++]) * k / 255;
		}	
	}

	return Raster::Line(rowbuf, &owner, true);
}

JPGRaster::JPGRaster()
{
}

JPGRaster::~JPGRaster()
{
}

bool JPGRaster::Create()
{
	ASSERT(sizeof(JSAMPLE) == sizeof(byte));
	data = new Data(*this);
	return data->Create(GetStream());
}

Size JPGRaster::GetSize()
{
	return data->size;
}

Raster::Info JPGRaster::GetInfo()
{
	ASSERT(data);
	Raster::Info info;
	info.kind = IMAGE_OPAQUE;
	if(data->cinfo.output_components == 1) {
		info.bpp = 8;
		info.colors = 256;
	}
	else {
		info.bpp = 24;
		info.colors = 0;
	}
	info.dots = data->dot_size;
	info.hotspot = Null;
	Value v = GetMetaData("orientation");
	if(IsNumber(v))
		info.orientation = clamp((int)v - 1, 0, 7);
	return info;
}

Value JPGRaster::GetMetaData(String id)
{
	ASSERT(data);
	return data->GetMetaData(id);
}

void JPGRaster::EnumMetaData(Vector<String>& id_list)
{
	ASSERT(data);
	data->EnumMetaData(id_list);
}

Raster::Line JPGRaster::GetLine(int line)
{
	ASSERT(data);
	return data->GetLine(line);
}

const RGBA *JPGRaster::GetPalette()
{
	ASSERT(data);
	return data->palette;
}

const RasterFormat *JPGRaster::GetFormat()
{
	ASSERT(data);
	return &data->format;
}

class JPGEncoder::Data {
public:
	Data();
	~Data();

	void Start(Stream& stream, Size size, Size dots, int quality);
	void WriteLineRaw(const byte *rgba);

private:
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	Size size;
	int line;
};

JPGEncoder::Data::Data()
{
	cinfo.err = jpeg_std_error(&jerr);
	cinfo.err->output_message = &NoOutput;
	jpeg_create_compress(&cinfo);
	cinfo.dct_method = JDCT_IFAST;
}

JPGEncoder::Data::~Data()
{
	jpeg_destroy_compress(&cinfo);
}

void JPGEncoder::Data::Start(Stream& stream, Size size_, Size dots, int quality)
{
	size = size_;

	jpeg_stream_dest(&cinfo, stream);

	cinfo.image_width = size.cx;
	cinfo.image_height = size.cy;
	cinfo.input_components = 3; // # of color components per pixel
	cinfo.in_color_space = JCS_RGB; // colorspace of input image

	jpeg_set_defaults(&cinfo);

	if(dots.cx || dots.cy)
	{ // set up image density
		cinfo.density_unit = 1; // dots per inch
		cinfo.X_density = dots.cx ? fround(size.cx * 600.0 / dots.cx) : 0;
		cinfo.Y_density = dots.cy ? fround(size.cy * 600.0 / dots.cy) : 0;
	}

	jpeg_set_quality(&cinfo, quality, true); // limit to baseline-JPEG values
	jpeg_start_compress(&cinfo, true);

	line = 0;

	ASSERT(sizeof(JSAMPLE) == sizeof(byte));
}

void JPGEncoder::Data::WriteLineRaw(const byte *s)
{
	JSAMPROW rowptr[] = { (byte *)s };
	jpeg_write_scanlines(&cinfo, rowptr, 1);
	if(++line >= size.cy)
		jpeg_finish_compress(&cinfo);
}

JPGEncoder::JPGEncoder(int quality_)
: quality(quality_)
{
	format.Set24le(0xff, 0xff00, 0xff0000);
}

JPGEncoder::~JPGEncoder()
{
}

int JPGEncoder::GetPaletteCount()
{
	return 0;
}

void JPGEncoder::Start(Size sz)
{
	data = new Data;
	data->Start(GetStream(), sz, GetDots(), quality);
}

void JPGEncoder::WriteLineRaw(const byte *s)
{
	data->WriteLineRaw(s);
}

Image JPGRaster::GetExifThumbnail()
{
	ASSERT(data);
	StringStream ss(data->GetThumbnail());
	return StreamRaster::LoadStringAny(ss);
}

bool IsJPG(StreamRaster *s)
{
	return dynamic_cast<JPGRaster *>(s);
}

INITIALIZER(JPGRaster)
{
	StreamRaster::Register<JPGRaster>();
	SetIsJPGFn(IsJPG);
}

}
