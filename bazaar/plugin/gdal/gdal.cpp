#include "gdal.h"

#define uint8 tif_uint8
#define uint16 tif_uint16
#define int16 tif_int16
#define int32 tif_int32
#define uint32 tif_uint32

#include <gdal/gcore/gdal_priv.h>
#include <gdal/port/cpl_conv.h>

#undef uint8
#undef uint16
#undef int16
#undef int32
#undef uint32

/*
GDALDriver *CreateGTIFFDriver();

extern "C" TIFF* XTIFFOpen(const char* name, const char* mode)
{
    return TIFFFileStreamOpen(name, mode);
}

GLOBAL_VARP(One<GDALDriver>, GTIFFDriver, = CreateGTIFFDriver());

EXITBLOCK {
	VSICleanupFileManager();
}
*/
Gdal::Block::Block(Size size_, GDALDataType type_, byte pixel_bytes_)
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
		

void Gdal::Block::GetInt(int *out, int x, int y, int count) const
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

void Gdal::Block::GetDouble(double *out, int x, int y, int count) const
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

Gdal::Band::Band(Gdal& owner_, GDALRasterBand *band_)
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

double Gdal::Band::GetMinimum() const
{
	int success;
	double min = band->GetMinimum(&success);
	if(success)
		return min;
	CalcStatistics();
	return stat_min;
}

double Gdal::Band::GetMaximum() const
{
	int success;
	double max = band->GetMaximum(&success);
	if(success)
		return max;
	CalcStatistics();
	return stat_max;
}

void Gdal::Band::GetInt(int *out, int x, int y, int count) const
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

void Gdal::Band::GetDouble(double *out, int x, int y, int count) const
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

int Gdal::Band::GetInt(int x, int y) const
{
	int out;
	GetInt(&out, x, y, 1);
	return out;
}

double Gdal::Band::GetDouble(int x, int y) const
{
	double out;
	GetDouble(&out, x, y, 1);
	return out;
}

double Gdal::Band::GetIntAt(Pointf pt) const
{
	Pointf src = pt * owner.GetInverseTransform();
	int x = ffloor(src.x);
	int y = ffloor(src.y);
	if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
		return Null;
	return GetInt(x, y);
}

double Gdal::Band::GetDoubleAt(Pointf pt) const
{
	Pointf src = pt * owner.GetInverseTransform();
	int x = ffloor(src.x);
	int y = ffloor(src.y);
	if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
		return Null;
	return GetDouble(x, y);
}

void Gdal::Band::LoadBlock(int column, int row) const
{
	ASSERT(column >= 0 && column < block_count.cx);
	ASSERT(row >= 0 && row < block_count.cy);
	int block_index = column + row * block_count.cx;
	if(blocks[block_index])
		return;
	Point block_offset(block_size.cx * column, block_size.cy * row);
	One<Block> new_block = new Block(block_size, type, pixel_bytes);
	band->ReadBlock(column, row, new_block->bytes);
	blocks[block_index] = pick(new_block);
}

void Gdal::Band::CalcStatistics() const
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

Gdal::Gdal()
{
	dataset = NULL;
}

Gdal::~Gdal()
{
	Close();
}

bool Gdal::Open(const char *fn)
{
    ONCELOCK {
        MemoryIgnoreLeaksBegin();

//		GDALRegister_GDB();
		GDALRegister_GTiff();
		GDALRegister_GXF();
//		GDALRegister_OGDI();
		GDALRegister_HFA();
		GDALRegister_AAIGrid();
		GDALRegister_AIGrid();
//		GDALRegister_AIGrid2();
		GDALRegister_CEOS();
		GDALRegister_SAR_CEOS();
//		GDALRegister_SDTS();
		GDALRegister_ELAS();
		GDALRegister_EHdr();
		GDALRegister_GenBin();
		GDALRegister_PAux();
		GDALRegister_ENVI();
		GDALRegister_DOQ1();
		GDALRegister_DOQ2();
		GDALRegister_DTED();
		GDALRegister_MFF();
		GDALRegister_HKV();
//		GDALRegister_PNG();
		GDALRegister_JPEG();
//		GDALRegister_JPEG2000();
//		GDALRegister_JP2KAK();
		GDALRegister_MEM();
		GDALRegister_JDEM();
//		GDALRegister_GRASS();
		GDALRegister_PNM();
		GDALRegister_GIF();
		GDALRegister_Envisat();
//		GDALRegister_FITS();
//		GDALRegister_ECW();
//		GDALRegister_JP2ECW();
//		GDALRegister_ECW_JP2ECW();
		GDALRegister_FujiBAS();
		GDALRegister_FIT();
		GDALRegister_VRT();
		GDALRegister_USGSDEM();
		GDALRegister_FAST();
//		GDALRegister_HDF4();
//		GDALRegister_HDF4Image();
		GDALRegister_L1B();
//		GDALRegister_LDF();
		GDALRegister_BSB();
		GDALRegister_XPM();
		GDALRegister_BMP();
		GDALRegister_GSC();
		GDALRegister_NITF();
		GDALRegister_RPFTOC();
//		GDALRegister_MrSID();
//		GDALRegister_PCIDSK();
		GDALRegister_BT();
//		GDALRegister_DODS();
//		GDALRegister_GMT();
//		GDALRegister_netCDF();
		GDALRegister_LAN();
		GDALRegister_CPG();
		GDALRegister_AirSAR();
		GDALRegister_RS2();
		GDALRegister_ILWIS();
		GDALRegister_PCRaster();
		GDALRegister_IDA();
		GDALRegister_NDF();
		GDALRegister_RMF();
//		GDALRegister_HDF5();
//		GDALRegister_HDF5Image();
//		GDALRegister_MSGN();
//		GDALRegister_MSG();
		GDALRegister_RIK();
		GDALRegister_Leveller();
		GDALRegister_SGI();
		GDALRegister_SRTMHGT();
		GDALRegister_DIPEx();
		GDALRegister_ISIS3();
		GDALRegister_ISIS2();
		GDALRegister_PDS();
//		GDALRegister_IDRISI();
		GDALRegister_Terragen();
		GDALRegister_WCS();
//		GDALRegister_WMS();
		GDALRegister_HTTP();
//		GDALRegister_SDE();
		GDALRegister_GSAG();
		GDALRegister_GSBG();
		GDALRegister_GS7BG();
		GDALRegister_GRIB();
		GDALRegister_INGR();
		GDALRegister_ERS();
		GDALRegister_PALSARJaxa();
		GDALRegister_DIMAP();
		GDALRegister_GFF();
		GDALRegister_COSAR();
		GDALRegister_TSX();
//		GDALRegister_ADRG();
		GDALRegister_COASP();
		GDALRegister_BLX();
		GDALRegister_LCP();
//		GDALRegister_PGCHIP();
//		GDALRegister_TMS();
		GDALRegister_EIR();
//		GDALRegister_GEOR();

        MemoryIgnoreLeaksEnd();
    }
 
	Close();
	
	MemoryIgnoreLeaksBegin();
	dataset = (GDALDataset *) GDALOpen(fn, GA_ReadOnly);
	MemoryIgnoreLeaksEnd();
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

void Gdal::Close()
{
	bands.Clear();
	if(dataset) {
		GDALClose((GDALDatasetH) dataset);
		dataset = NULL;
	}
}

String Gdal::GetProjection() const
{
	return dataset->GetProjectionRef();
}

Size Gdal::GetPixelSize() const
{
	return Size(dataset->GetRasterXSize(), dataset->GetRasterYSize());
}

Rectf Gdal::GetExtent() const
{
	return Rectf(Sizef(GetPixelSize())) * GetTransform();
}
