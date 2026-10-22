#ifndef _plugin_tif_tif_h_
#define _plugin_tif_tif_h_

#include <Draw/Draw.h>

struct tiff;

namespace Upp {

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
