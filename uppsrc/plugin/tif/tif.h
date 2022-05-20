#ifndef _plugin_tif_tif_h_
#define _plugin_tif_tif_h_

#include <Draw/Draw.h>

struct tiff;

#include <plugin/tif/lib/tiff.h>
#include <plugin/tif/lib/tiffio.h>
#include <plugin/tif/lib/tiffiop.h>

#undef int8
#undef uint8
#undef int16
#undef uint16
#undef int32
#undef uint32
#undef int64
#undef uint64

#undef HAVE_INT8
#undef HAVE_INT16
#undef HAVE_INT32
#undef HAVE_UINT8
#undef HAVE_UINT16
#undef HAVE_UINT32

namespace Upp {

struct tiff *TIFFStreamOpen(const char *filename, const char *mode, Stream *stream, bool destruct = false);
struct tiff *TIFFFileStreamOpen(const char *filename, const char *mode);

//struct tiff* TIFFWrapOpen(const char*, const char*);
//int TIFFWrapGetField(::tiff* tif_data, uint32 tag, ...);

INITIALIZE(TIFRaster);

class TIFRaster : public StreamRaster {
public:
	struct Data;
	One<Data> data;

public:
	TIFRaster();
	~TIFRaster();

	virtual bool                Create();
	virtual Size                GetSize();
	virtual Info                GetInfo();
	virtual Line                GetLine(int line);
	virtual int                 GetPaletteCount();
	virtual const RGBA         *GetPalette();
	virtual const RasterFormat *GetFormat();

	virtual int                 GetPageCount();
	virtual int                 GetActivePage() const;
	virtual void                SeekPage(int n);

	virtual Value               GetMetaData(String id);
	virtual void                EnumMetaData(Vector<String>& id_list);

private:
	bool                        Init();
};

class TIFEncoder : public StreamRasterEncoder {
public:
	class Data;
	One<Data> data;

public:
	TIFEncoder(int bpp = 24);
	~TIFEncoder();

	TIFEncoder&  Bpp(int b)       { bpp = b; return *this; }

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *s);

private:
	int bpp;
};

}

#endif
