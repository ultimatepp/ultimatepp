#ifndef _plugin_pcx_pcx_h_
#define _plugin_pcx_pcx_h_

#include <Draw/Draw.h>

namespace Upp {

#include "pcxhdr.h"

class PCXRaster : public StreamRaster {
	PCXHeader        header;
	Size             size;
	int              row_bytes;
	Buffer<RGBA>     palette;
	Buffer<byte>     scanline;
	Buffer<byte>     buffer;
	RasterFormat     fmt;
	Raster::Info     info;
	One<ImageRaster> rle;

public:
	virtual bool    Create();
	virtual Info    GetInfo();
	virtual Size    GetSize();
	virtual Line    GetLine(int line);
	virtual int     GetPaletteCount();
	virtual RGBA   *GetPalette();
	virtual const RasterFormat *GetFormat();
};

}

#endif
