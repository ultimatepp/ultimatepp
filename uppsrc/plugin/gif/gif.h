#ifndef _plugin_gif_gif_h_
#define _plugin_gif_gif_h_

#include <Draw/Draw.h>


namespace Upp {
	
INITIALIZE(GifRaster)

class GIFRaster : public StreamRaster {
public:
	class Data;
	One<Data> data;

public:
	GIFRaster();
	~GIFRaster();

	virtual bool               Create();
	virtual Size               GetSize();
	virtual Info               GetInfo();
	virtual Line               GetLine(int line);
	virtual const RasterFormat *GetFormat();
	virtual int                GetPaletteCount();
	virtual RGBA               *GetPalette();
	
	virtual int                GetPageCount();
	virtual int                GetActivePage() const;
	virtual void               SeekPage(int n);
	virtual int                GetPageAspect(int n);
	virtual int                GetPageDelay(int n);
	virtual Rect               GetPageRect(int n);
	virtual int				   GetPageDisposal(int n);

private: 
	bool                       Init();
};

class GIFEncoder : public StreamRasterEncoder {
public:
	class Data;
	One<Data> data;

public:
	GIFEncoder(bool ignore_alpha = false, String comment = Null);
	~GIFEncoder();

	GIFEncoder&  IgnoreAlpha(bool ia = true) { ignore_alpha = ia; return *this; }
	GIFEncoder&  Comment(String c)           { comment = c; return *this; }

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *s);

private:
	bool         ignore_alpha;
	String       comment;
};

}

#endif
