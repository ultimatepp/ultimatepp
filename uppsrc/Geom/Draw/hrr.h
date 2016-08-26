#ifndef __Geom_Draw_hrr__
#define __Geom_Draw_hrr__

namespace Upp {

//#include "Stream.h"

class ImageWriter : public RasterEncoder {
public:
	ImageWriter() : output(NULL) {}
	ImageWriter(ImageBuffer& output, bool merge = true)                       { Open(output, merge); }
	ImageWriter(ImageBuffer& output, Point pos, bool merge = true)            { Open(output, pos, merge); }
	ImageWriter(ImageBuffer& output, Point pos, Rect clip, bool merge = true) { Open(output, pos, clip, merge); }

	void         Open(ImageBuffer& output, bool merge = true)                 { Open(output, Point(0, 0), merge); }
	void         Open(ImageBuffer& output, Point pos, bool merge = true)      { Open(output, pos, Rect(output.GetSize()), merge); }
	void         Open(ImageBuffer& output, Point pos, Rect clip, bool merge = true);

	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *data);

private:
	ImageBuffer  *output;
	Point        pos;
	int          left, width, offset;
	Rect         clip;
	int          line;
	Size         src_size;
	bool         merge;
};

class ImageBufferRaster : public Raster {
public:
	ImageBufferRaster(const ImageBuffer& buffer);
	ImageBufferRaster(const ImageBuffer& buffer, const Rect& crop);

	virtual Size       GetSize();
	virtual Info       GetInfo();
	virtual Line       GetLine(int line);

private:
	const ImageBuffer& buffer;
	Rect               crop;
};

class HRRInfo
{
	friend class HRR;

public:
	HRRInfo();
	HRRInfo(const Rectf& log_rect, const Rectf& map_rect = Null,
		int levels = 5, Color background = White,
		int method = METHOD_JPG, int quality = DFLT_JPG_QUALITY,
		bool mono = false, Color mono_black = Black, Color mono_white = White);

	void               Serialize(Stream& stream);

	bool               IsEmpty() const       { return levels == 0; }

	int                GetLevels() const     { return levels; }
	int                GetMaxSize() const    { return levels ? UNIT << (levels - 1) : 0; }
	Rectf              GetLogRect() const    { return log_rect; }
	Rectf              GetMapRect() const    { return map_rect; }
	Color              GetBackground() const { return background; }
	bool               IsMono() const        { return mono; }
	Color              GetMonoBlack() const  { return mono_black; }
	Color              GetMonoWhite() const  { return mono_white; }
	int                GetMethod() const     { return method; }
	int                GetQuality() const    { return quality; }

	One<StreamRaster>  GetDecoder() const;
	One<StreamRasterEncoder> GetEncoder() const;

	static double      GetEstimatedFileSize(int _levels, int method, int quality);

	static int         Pack(int method, int quality) { return (method << 16) | (quality & 0xFFFF); }
	static int         Method(int p)         { return p >> 16; }
	static int         Quality(int p)        { return p & 0xFFFF; }

	static Vector<int> EnumMethods();
	static Vector<int> EnumQualities(int method);
	static String      GetName(int method, int quality);
	static VectorMap<int, String> GetPackMap();

public:
	enum
	{
		HALF_BITS = 8,
		HALF      = 1 << HALF_BITS,
		UNIT      = HALF << 1,
		STDLEVEL  = 6,
		MAXLEVELS = 12,
		LCOUNT    = 2,
		UCOUNT    = 1 << LCOUNT,
	};

	enum
	{
		METHOD_JPG,
		METHOD_GIF,
		METHOD_RLE,
		METHOD_PNG,
		METHOD_ZIM,
		METHOD_BZM,

		DFLT_JPG_QUALITY = 50,
	};

protected:
	Color      background;
	Color      mono_black;
	Color      mono_white;
	Rectf      log_rect;
	Rectf      map_rect; // logical rectangle inflated to tightly bounding square
	int        levels;
	int        method;
	int        quality;
	bool       mono;
};

class HRR
{
public:
	struct Block
	{
		Block(const HRR& hrr) : hrr(hrr) {}

		void        Init(Size size, RGBA color);

		ImageBuffer block;
		Size        size;
		Rect        area;
		Rectf       log_area;
		int         level;
		const HRR&  hrr;
	};

	struct Cursor {
		Cursor(HRR& owner, const Rectf& extent, double measure, int alpha = 100,
			Color mono_black = Null, Color mono_white = Null, Color blend_bgnd = Null);

		bool Fetch(Rectf& part_rc);
		Image Get();

		HRR& owner;
		Rectf extent;
		double measure;
		Color mono_black;
		Color mono_white;
		int alpha;
		Color blend_bgnd;

		One<StreamRaster> raster;
		int level;
		int total;
		Rect rc;
		Point block;
		int cimg;
	};

	friend struct Cursor;

	enum
	{
		DEFAULT_CACHE_SIZEOF_LIMIT = 20000000,
	};

	typedef Gate1<Block&>      Writeback;

	HRR();
	HRR(const char *path, bool read_only = true);

	bool                       Open(const char *path, bool read_only = true);
	void                       Close();

	bool                       Create(const HRRInfo& _info, const char *path);
	void                       Write(Writeback drawback, bool downscale = true);
	Rectf                      GetLogBlockRect(int level, const Rect& rc) const;
	Matrixf                    GetPixMapMatrix(int level, int x, int y) const;
	int64                      GetFileWriteSize() const;

	void                       SetCacheSizeLimit(int cl)       { FlushCache(cache_sizeof_limit = cl); }
	int                        GetCacheSizeLimit() const       { return cache_sizeof_limit; }
	int                        GetCacheSize() const            { return cache_sizeof + directory_sizeof; }
	void                       ClearCache();

	String                     GetMap(String key) const        { return map.Get(key, Null); }
	void                       SetMap(String key, String value);
	void                       ClearMap(String key)            { SetMap(key, Null); }
	void                       ClearMap()                      { map.Clear(); }
	const VectorMap<String, String>& GetMap() const            { return map; }
	void                       FlushMap();

	bool                       IsOpen() const                  { return stream.IsOpen(); }
	bool                       IsError() const                 { return stream.IsError(); }
	int                        SizeOfInstance() const;

	void                       Paint(Draw& draw, Rect dest, Rectf src,
		int wash = 100, int max_pixel = 1, Color mono_black = Null, Color mono_white = Null, Color blend_bgnd = Null);

	void                       Paint(Draw& draw, const Matrixf& trg_pix, GisTransform transform,
		int wash = 100, int max_pixel = 1, Color mono_black = Null, Color mono_white = Null, Color blend_bgnd = Null);

	static int                 GetProgressCount(int levels, bool downscale = true);

	const HRRInfo&             GetInfo() const                 { return info; }

	static One<StreamRasterEncoder> (*CreateEncoder)(const HRRInfo& info);
	static One<StreamRaster>        (*CreateDecoder)(const HRRInfo& info);
	static One<StreamRasterEncoder> StdCreateEncoder(const HRRInfo& info);
	static One<StreamRaster>        StdCreateDecoder(const HRRInfo& info);

private:
	bool                       Write(Writeback drawback, bool downscale, int level, int px, int py,
		StreamRasterEncoder& format, Block *put);
	void                       Serialize();
	void                       FlushCache(int limit);

private:
	HRRInfo                    info;
	FileStream                 stream;
	int                        version;
	Vector<int64>              pixel_directory_offset;
	Vector<int64>              mask_directory_offset;
//	Vector< Vector<int> >      pixel_directory;
//	Vector< Vector<int> >      mask_directory;
	int                        directory_sizeof;
	ArrayMap<Point, Image>     image_cache;
	VectorMap<int, Size>       size_cache;
	int                        cache_sizeof;
	int                        cache_sizeof_limit;
	VectorMap<String, String>  map;
	int                        map_offset;
};

}

#endif//__Geom_Draw_hrr__
