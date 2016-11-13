#ifndef __nDraw_plugin_png__
#define __nDraw_plugin_png__

#include <Draw/Draw.h>

namespace Upp {
	
INITIALIZE(PNGRaster);

class PNGRaster : public StreamRaster {
	class Data;
	One<Data> data;

public:
	PNGRaster();
	~PNGRaster();

	virtual bool    Create();
	virtual Size    GetSize();
	virtual Info    GetInfo();
	virtual Line    GetLine(int line);
	virtual const RGBA *GetPalette();
	virtual const RasterFormat *GetFormat();

private:
	bool            Init();
};

class PNGEncoder : public StreamRasterEncoder {
	class Data;
	One<Data> data;

public:
	PNGEncoder(int bpp = 32, ImageKind kind = IMAGE_UNKNOWN, bool interlace = false);
	~PNGEncoder();

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *data);

	PNGEncoder&  Bpp(int b)                      { bpp = b; return *this; }
	PNGEncoder&  Interlace(int b = true)         { interlace = b; return *this; }

private:
	int          bpp;
	ImageKind    kind;
	bool         interlace;
};

}

#endif
