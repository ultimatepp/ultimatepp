#ifndef _plugin_geotiff_geotiff_h_
#define _plugin_geotiff_geotiff_h_

#include <Draw/Draw.h>
#include <Geom/Geom.h>
#include "libgeotiff/gcore/gdal.h"

class GDALDriver;
class GDALDataset;
class GDALRasterBand;

using namespace Upp;

class GeoTIFF {
public:
	class Block {
	public:
		Block(Size size, GDALDataType type, byte pixel_bytes);
		
		void         GetInt(int *out, int x, int y, int count) const;
		void         GetDouble(double *out, int x, int y, int count) const;
		
	public:
		Size         size;
		GDALDataType type;
		byte         pixel_bytes;
		Buffer<byte> bytes;
	};

	class Band {
	public:
		Band(GeoTIFF& owner, GDALRasterBand *band);
	
		Size           GetSize() const       { return size; }
		Size           GetBlockSize() const  { return block_size; }
		
		GDALDataType   GetType() const       { return type; }
		int            GetBytes() const      { return pixel_bytes; }
		bool           IsComplex() const     { return complex; }
		
		double         GetMinimum() const;
		double         GetMaximum() const;
		
		void           GetInt(int *out, int x, int y, int count) const;
		void           GetDouble(double *out, int x, int y, int count) const;
		int            GetInt(int x, int y) const;
		double         GetDouble(int x, int y) const;
		
		double         GetIntAt(Pointf pt) const;
		double         GetDoubleAt(Pointf pt) const;
	
	protected:
		void           LoadBlock(int column, int row) const;
		void           CalcStatistics() const;
	
	private:
		GeoTIFF&       owner;
		GDALRasterBand *band;
		Size           size;
		Size           block_size;
		GDALDataType   type;
		bool           complex;
		bool           is_nodata;
		mutable bool   is_stat;
		mutable double stat_min;
		mutable double stat_max;
		byte           pixel_bytes;
		Size           block_count;
		int            nodata_int;
		double         nodata_value;
		mutable Buffer< One<Block> > blocks;
	};

public:
	GeoTIFF();
	~GeoTIFF();
	
	bool             Open(const char *filename);
	bool             IsOpen() const               { return dataset; }
	String           GetFileName() const          { return filename; }
	void             Close();

	String           GetProjection() const;

	Size             GetPixelSize() const;
	Rectf            GetExtent() const;
	const Matrixf&   GetTransform() const         { return transform; }
	const Matrixf&   GetInverseTransform() const  { return inverse_transform; }
	
	int              GetBandCount() const         { return bands.GetCount(); }
	const Band&      GetBand(int band) const      { return bands[band]; }
	const Band&      operator [] (int band) const { return bands[band]; }
	
private:
	String           filename;
	mutable One<GDALDataset> dataset;
	Array<Band>      bands;
	Matrixf          transform;
	Matrixf          inverse_transform;
};

#endif
