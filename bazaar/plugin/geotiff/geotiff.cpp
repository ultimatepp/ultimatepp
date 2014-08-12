#include "geotiff.h"
#define uint8 tif_uint8
#define uint16 tif_uint16
#define int16 tif_int16
#define int32 tif_int32
#define uint32 tif_uint32
#include <plugin/tif/tif.h>
#include "libgeotiff/xtiffio.h"
#include "libgeotiff/gcore/gdal_priv.h"
#undef uint8
#undef uint16
#undef int16
#undef int32
#undef uint32

GDALDriver *CreateGTIFFDriver();

extern "C" TIFF* XTIFFOpen(const char* name, const char* mode)
{
    return TIFFFileStreamOpen(name, mode);
}

GLOBAL_VARP(One<GDALDriver>, GTIFFDriver, = CreateGTIFFDriver());

EXITBLOCK {
	VSICleanupFileManager();
}

GeoTIFF::Block::Block(Size size_, GDALDataType type_, byte pixel_bytes_)
: size(size_)
, type(type_)
, pixel_bytes(pixel_bytes_)
{
	bytes.Alloc(pixel_bytes * size.cx * size.cy);
}

#define CONVERT_LOOP(type, src_convert) \
	{ \
		const type *src = (const type *)raw_src; \
		const type *end = src + count; \
		for(; src < end; src++, out++) \
			*out = (src_convert); \
		break; \
	}
		

void GeoTIFF::Block::GetInt(int *out, int x, int y, int count) const
{
	ASSERT(x >= 0 && x + count <= size.cx);
	ASSERT(y >= 0 && y < size.cy);
	int offset = (x + y * size.cx) * pixel_bytes;
	const byte *raw_src = &bytes[offset];
	switch(type) {
		case GDT_Byte:     CONVERT_LOOP(byte, *src);
		case GDT_UInt16:   CONVERT_LOOP(uint16, *src);
		case GDT_Int16:    CONVERT_LOOP(int16, *src);
		case GDT_UInt32:   CONVERT_LOOP(uint32, (int)*src);
		case GDT_Int32:    CONVERT_LOOP(int32, (int)*src);
		case GDT_Float32:  CONVERT_LOOP(float, fround((double)*src));
		case GDT_Float64:  CONVERT_LOOP(double, fround(*src));
		case GDT_CInt16:   CONVERT_LOOP(Point16, src->x);
		case GDT_CInt32:   CONVERT_LOOP(Point, src->x);
		case GDT_CFloat32: CONVERT_LOOP(Point_<float>, fround((double)src->x));
		case GDT_CFloat64: CONVERT_LOOP(Pointf, fround(src->x));
		default: {
			Fill(out, out + count, (int)Null);
			RLOG("unsupported GDAL data type: " << (int)type);
			NEVER();
			break;
		}
	}
}

void GeoTIFF::Block::GetDouble(double *out, int x, int y, int count) const
{
	ASSERT(x >= 0 && x + count <= size.cx);
	ASSERT(y >= 0 && y < size.cy);
	int offset = (x + y * size.cx) * pixel_bytes;
	const byte *raw_src = &bytes[offset];
	switch(type) {
		case GDT_Byte:     CONVERT_LOOP(byte, *src);
		case GDT_UInt16:   CONVERT_LOOP(uint16, *src);
		case GDT_Int16:    CONVERT_LOOP(int16, *src);
		case GDT_UInt32:   CONVERT_LOOP(uint32, (int)*src);
		case GDT_Int32:    CONVERT_LOOP(int32, (int)*src);
		case GDT_Float32:  CONVERT_LOOP(float, (double)*src);
		case GDT_Float64:  CONVERT_LOOP(double, *src);
		case GDT_CInt16:   CONVERT_LOOP(Point16, src->x);
		case GDT_CInt32:   CONVERT_LOOP(Point, src->x);
		case GDT_CFloat32: CONVERT_LOOP(Point_<float>, (double)src->x);
		case GDT_CFloat64: CONVERT_LOOP(Pointf, src->x);
		default: {
			Fill(out, out + count, (double)Null);
			RLOG("unsupported GDAL data type: " << (int)type);
			NEVER();
			break;
		}
	}
}

GeoTIFF::Band::Band(GeoTIFF& owner_, GDALRasterBand *band_)
: owner(owner_), band(band_)
{
	size.cx = band->GetXSize();
	size.cy = band->GetYSize();
	band->GetBlockSize(&block_size.cx, &block_size.cy);
	block_count = idivceil(size, block_size);
	type = band->GetRasterDataType();
	pixel_bytes = (GDALGetDataTypeSize(type) + 7) >> 3;
	complex = GDALDataTypeIsComplex(type);
	int nodata_flag;
	nodata_value = band->GetNoDataValue(&nodata_flag);
	is_nodata = !!nodata_flag;
	is_stat = false;
	nodata_int = fround(nodata_value);
	blocks.Alloc(block_count.cx * block_count.cy);
}

double GeoTIFF::Band::GetMinimum() const
{
	int success;
	double min = band->GetMinimum(&success);
	if(success)
		return min;
	CalcStatistics();
	return stat_min;
}

double GeoTIFF::Band::GetMaximum() const
{
	int success;
	double max = band->GetMaximum(&success);
	if(success)
		return max;
	CalcStatistics();
	return stat_max;
}

void GeoTIFF::Band::GetInt(int *out, int x, int y, int count) const
{
	int y_in_block = y % block_size.cy;
	int block_y = y / block_size.cy;
	int block_offset = block_y * block_count.cx;
	int block_x_min = x / block_size.cx;
	int block_x_max = (x + count - 1) / block_size.cx;
	int x_in_block = x - block_size.cx * block_x_min;
	for(int block_x = block_x_min; block_x <= block_x_max; block_x++) {
		int fill_count = min(count, block_size.cx - x_in_block);
		int block_index = block_x + block_offset;
		if(!blocks[block_index])
			LoadBlock(x, block_y);
		if(blocks[block_index]) {
			blocks[block_index]->GetInt(out, x_in_block, y_in_block, fill_count);
			if(is_nodata) {
				for(int *p = out, *e = out + fill_count; p < e; p++)
					if(*p == nodata_int)
						*p = Null;
			}
		}
		else
			Fill(out, out + fill_count, (int)Null);
		x_in_block = 0;
		count -= fill_count;
		x += fill_count;
	}
}

void GeoTIFF::Band::GetDouble(double *out, int x, int y, int count) const
{
	int y_in_block = y % block_size.cy;
	int block_y = y / block_size.cy;
	int block_offset = block_y * block_count.cx;
	int block_x_min = x / block_size.cx;
	int block_x_max = (x + count - 1) / block_size.cx;
	int x_in_block = x - block_size.cx * block_x_min;
	for(int block_x = block_x_min; block_x <= block_x_max; block_x++) {
		int fill_count = min(count, block_size.cx - x_in_block);
		int block_index = block_x + block_offset;
		if(!blocks[block_index])
			LoadBlock(block_x, block_y);
		if(blocks[block_index]) {
			blocks[block_index]->GetDouble(out, x_in_block, y_in_block, fill_count);
			if(is_nodata) {
				for(double *p = out, *e = out + fill_count; p < e; p++)
					if(*p == nodata_value)
						*p = Null;
			}
		}
		else
			Fill(out, out + fill_count, (double)Null);
		x_in_block = 0;
		count -= fill_count;
		out += fill_count;
	}
}

int GeoTIFF::Band::GetInt(int x, int y) const
{
	int out;
	GetInt(&out, x, y, 1);
	return out;
}

double GeoTIFF::Band::GetDouble(int x, int y) const
{
	double out;
	GetDouble(&out, x, y, 1);
	return out;
}

double GeoTIFF::Band::GetIntAt(Pointf pt) const
{
	Pointf src = pt * owner.GetInverseTransform();
	int x = ffloor(src.x);
	int y = ffloor(src.y);
	if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
		return Null;
	return GetInt(x, y);
}

double GeoTIFF::Band::GetDoubleAt(Pointf pt) const
{
	Pointf src = pt * owner.GetInverseTransform();
	int x = ffloor(src.x);
	int y = ffloor(src.y);
	if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
		return Null;
	return GetDouble(x, y);
}

void GeoTIFF::Band::LoadBlock(int column, int row) const
{
	ASSERT(column >= 0 && column < block_count.cx);
	ASSERT(row >= 0 && row < block_count.cy);
	int block_index = column + row * block_count.cx;
	if(blocks[block_index])
		return;
	Point block_offset(block_size.cx * column, block_size.cy * row);
	One<Block> new_block = new Block(block_size, type, pixel_bytes);
	band->ReadBlock(column, row, new_block->bytes);
	blocks[block_index] = new_block;
}

void GeoTIFF::Band::CalcStatistics() const
{
	if(is_stat)
		return;
	is_stat = true;
	stat_min = stat_max = Null;
	bool done_stat = false;
	Buffer<double> row(block_size.cx);
	for(int by = 0; by < block_count.cy; by++)
		for(int bx = 0; bx < block_count.cx; bx++) {
			LoadBlock(bx, by);
			Size valid_size = min(block_size, size - Size(bx, by) * block_size);
			const One<Block>& block = blocks[bx + by * block_count.cx];
			if(block)
				for(int r = 0; r < valid_size.cy; r++) {
					block->GetDouble(row, 0, r, valid_size.cx);
					for(const double *p = row, *e = p + valid_size.cx; p < e; p++)
						if(!done_stat) {
							stat_min = stat_max = *p;
							done_stat = true;
						}
						else {
							if(*p < stat_min) stat_min = *p;
							if(*p > stat_max) stat_max = *p;
						}
				}
		}
}

GeoTIFF::GeoTIFF()
{
}

GeoTIFF::~GeoTIFF()
{
	Close();
}

bool GeoTIFF::Open(const char *fn)
{
	Close();
	
	GDALOpenInfo openinfo(filename = fn, GA_ReadOnly);
	dataset = GTIFFDriver()->pfnOpen(&openinfo);
	if(!dataset)
		return false;
	int nbands = dataset->GetRasterCount();
	for(int i = 1; i <= nbands; i++)
		bands.Add(new Band(*this, dataset->GetRasterBand(i)));
	double affine[6];
	dataset->GetGeoTransform(affine);
	transform.a.x = affine[0];
	transform.x.x = affine[1];
	transform.y.x = affine[2];
	transform.a.y = affine[3];
	transform.x.y = affine[4];
	transform.y.y = affine[5];
	if(fabs(Determinant(transform)) <= 1e-10)
		inverse_transform = Matrixf_1();
	else
		inverse_transform = MatrixfInverse(transform);
	return true;
}

void GeoTIFF::Close()
{
	bands.Clear();
	dataset.Clear();
}

String GeoTIFF::GetProjection() const
{
	return dataset->GetProjectionRef();
}

Size GeoTIFF::GetPixelSize() const
{
	return Size(dataset->GetRasterXSize(), dataset->GetRasterYSize());
}

Rectf GeoTIFF::GetExtent() const
{
	return Rectf(Sizef(GetPixelSize())) * GetTransform();
}
