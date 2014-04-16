#ifndef _plugin_bmp_bmp_h_
#define _plugin_bmp_bmp_h_

#include <Draw/Draw.h>

NAMESPACE_UPP

class PPMRaster : public StreamRaster {
	Size         size;
	bool         is16;
	RasterFormat fmt;
	int64        pixel_pos;

public:
	virtual bool    Create();
	virtual Info    GetInfo();
	virtual Size    GetSize();
	virtual Line    GetLine(int line);
	virtual int     GetPaletteCount();
	virtual RGBA   *GetPalette();
	virtual const RasterFormat *GetFormat();

	PPMRaster();
	~PPMRaster();
};

class PPMEncoder : public StreamRasterEncoder {
	Size         size;

public:
	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *data);
};

END_UPP_NAMESPACE

#endif