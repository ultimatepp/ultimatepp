struct SpanSource {
	virtual void Get(RGBA *span, int x, int y, unsigned len) = 0;
	virtual ~SpanSource() {}
};

class ClippingLine : NoCopy {
	byte *data;
	
public:
	void Clear()                     { if(!IsFull()) delete[] data; data = NULL; }
	void Set(const byte *s, int len) { data = new byte[len]; memcpy(data, s, len); }
	void SetFull()                   { ASSERT(!data); data = (byte *)1; }

	bool IsEmpty() const             { return !data; }
	bool IsFull() const              { return data == (byte *)1; }
	operator const byte*() const     { return data; }
	
	ClippingLine()                       { data = NULL; }
	~ClippingLine()                      { Clear(); }
};

struct PainterTarget : LinearPathConsumer {
	virtual void Fill(double width, SpanSource *ss, const RGBA& color);
};

inline RGBA Mul8(const RGBA& s, int mul)
{
	RGBA t;
	t.r = (mul * s.r) >> 8;
	t.g = (mul * s.g) >> 8;
	t.b = (mul * s.b) >> 8;
	t.a = (mul * s.a) >> 8;
	return t;
}

inline void AlphaBlend(RGBA& t, const RGBA& c)
{
	int alpha = 256 - (c.a + (c.a >> 7));
	t.r = c.r + (alpha * t.r >> 8);
	t.g = c.g + (alpha * t.g >> 8);
	t.b = c.b + (alpha * t.b >> 8);
	t.a = c.a + ((256 - c.a) * t.a >> 8);
}

inline void AlphaBlendCover8(RGBA& t, const RGBA& c, int cover)
{
	int a = c.a * cover >> 8;
	int alpha = 256 - a - (a >> 7);
	t.r = (c.r * cover >> 8) + (alpha * t.r >> 8);
	t.g = (c.g * cover >> 8) + (alpha * t.g >> 8);
	t.b = (c.b * cover >> 8) + (alpha * t.b >> 8);
	t.a = a + (alpha * t.a >> 8);
}

class BufferPainter : public Painter {
protected:
	virtual void   ClearOp(const RGBA& color);

	virtual void   MoveOp(const Pointf& p, bool rel);
	virtual void   LineOp(const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p1, const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p2, const Pointf& p, bool rel);
	virtual void   ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel);
	virtual void   SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep,
	                        const Pointf& p, bool rel);
	virtual void   CloseOp();
	virtual void   DivOp();

	virtual void   CharacterOp(const Pointf& p, int ch, Font fnt);

	virtual void   FillOp(const RGBA& color);
	virtual void   FillOp(const Image& image, const Xform2D& transsrc, dword flags);
	virtual void   FillOp(const Pointf& p1, const RGBA& color1,
	                      const Pointf& p2, const RGBA& color2,
	                      int style);
	virtual void   FillOp(const RGBA& color1, const RGBA& color2, const Xform2D& transsrc,
	                      int style);
	virtual void   FillOp(const Pointf& f, const RGBA& color1,
	                      const Pointf& c, double r, const RGBA& color2,
	                      int style);
	virtual void   FillOp(const Pointf& f, const RGBA& color1, const RGBA& color2,
	                      const Xform2D& transsrc, int style);

	virtual void   StrokeOp(double width, const RGBA& rgba);
	virtual void   StrokeOp(double width, const Image& image, const Xform2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, const Pointf& p1, const RGBA& color1,
	                        const Pointf& p2, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, const RGBA& color1, const RGBA& color2,
	                        const Xform2D& transsrc,
	                        int style);
	virtual void   StrokeOp(double width, const Pointf& f, const RGBA& color1,
	                        const Pointf& c, double r, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, const Pointf& f,
	                        const RGBA& color1, const RGBA& color2,
	                        const Xform2D& transsrc, int style);

	virtual void   ClipOp();

	virtual void   ColorStopOp(double pos, const RGBA& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const String& dash, double start);
	virtual void   DashOp(const Vector<double>& dash, double start);
	virtual void   InvertOp(bool invert);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

	virtual void   BeginMaskOp();
	virtual void   BeginOnPathOp(double q, bool abs);

private:
	enum {
		MOVE, LINE, QUADRATIC, CUBIC, CHAR
	};
	struct LinearData {
		int    type;
		Pointf p;
	};
	struct QuadraticData : LinearData {
		Pointf p1;
	};
	struct CubicData : QuadraticData {
		Pointf p2;
	};
	struct CharData : LinearData {
		int  ch;
		int  _filler;
		Font fnt;
	};
	struct PathLine : Moveable<PathLine> {
		Pointf p;
		double len;
	};
	struct DashInfo {
		WithDeepCopy<Vector<double>>    dash;
		double                          start;
	};
	struct ColorStop : Moveable<ColorStop> {
		double stop;
		RGBA   color;
		
		bool operator<(const ColorStop& b) const { return stop < b.stop; }
	};
	struct SimpleAttr {
		Xform2D                         mtx;
		double                          miter_limit;
		double                          opacity;
		const DashInfo                 *dash;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		bool                            invert;
	};
	struct Attr : Moveable<Attr, SimpleAttr> {
		int                             mtx_serial; // used to detect changes to speedup preclip
		WithDeepCopy<Vector<ColorStop>> color_stop;

		int                             cliplevel;
		bool                            hasclip;
		bool                            mask;
		bool                            onpath;
	};
	
	PainterTarget             *alt = NULL;
	double                     alt_tolerance = Null;
	ImageBuffer                dummy;
	ImageBuffer&               ib;
	int                        mode;
	Buffer<int16>              subpixel;
	int                        render_cx;
	int                        dopreclip;

	Attr                       attr;
	Array<Attr>                attrstack;
	Vector<Buffer<ClippingLine>> clip;
	Array< ImageBuffer >       mask;
	Vector<Vector<PathLine>>   onpathstack;
	Vector<double>             pathlenstack;
	int                        mtx_serial = 0;
	ArrayMap<String, DashInfo> dashes;
	
	Image                      gradient;
	RGBA                       gradient1, gradient2;
	int                        gradientn;

	Sizef                      size; // = ib.GetSize()
	Rectf                      preclip;
	int                        preclip_mtx_serial = -1;
	bool                       regular;

	struct PathInfo {
		Vector<Vector<byte>>               path;
		bool                               ischar;
		Pointf                             path_min, path_max;
	};
	
	enum { BATCH_SIZE = 128 }; // must be 2^n
	
	Buffer<PathInfo> paths;
	int              path_index = 0;
	PathInfo        *path_info;
	Attr             pathattr;

	Pointf           current, ccontrol, qcontrol, move;
	
	Rasterizer       rasterizer;
	Buffer<RGBA>     span;

	Vector<PathLine> onpath;
	double           pathlen;
	
	struct OnPathTarget;
	friend struct OnPathTarget;
	
	bool co = false;
	bool imagecache = true;

	struct OnPathTarget : LinearPathConsumer {
		Vector<BufferPainter::PathLine> path;
		Pointf pos;
		double len;
		
		virtual void Move(const Pointf& p) {
			BufferPainter::PathLine& t = path.Add();
			t.len = 0;
			pos = t.p = p;
		}
		virtual void Line(const Pointf& p) {
			BufferPainter::PathLine& t = path.Add();
			len += (t.len = Distance(pos, p));
			pos = t.p = p;
		}
		
		OnPathTarget() { len = 0; pos = Pointf(0, 0); }
	};
	
	struct PathJob {
		Transformer         trans;
		Stroker             stroker;
		Dasher              dasher;
		OnPathTarget        onpathtarget;
		LinearPathConsumer *g;
		double              tolerance;
		bool                evenodd;
		bool                regular;
		bool                preclipped;


		PathJob(Rasterizer& rasterizer, double width, const PathInfo *path_info,
		        const SimpleAttr& attr, const Rectf& preclip, bool regular);
	};
	
	struct CoJob {
		SimpleAttr        attr;
		PathInfo         *path_info;
		Rectf             preclip;
		double            width;
		Rasterizer        rasterizer;
		RGBA              color;
		RGBA              c;
		int               subpath;
		bool              evenodd;
		bool              regular;
	};
	
	friend struct CoJob;
	
	Array<CoJob>     cojob, cofill;
	int              jobcount, fillcount;

	CoWorkNX         fill_job;
	
	template <class T> T& PathAdd(int type);
	
	Pointf           PathPoint(const Pointf& p, bool rel);
	Pointf           EndPoint(const Pointf& p, bool rel);
	void             DoMove0();
	void             DoPath0();
	void             DoPath()         { if(IsNull(current)) DoPath0(); }
	void             ClearPath();
	Buffer<ClippingLine> RenderPath(double width, Event<One<SpanSource>&> ss, const RGBA& color);
	void             RenderImage(double width, const Image& image, const Xform2D& transsrc,
	                             dword flags);
	void             RenderRadial(double width, const Pointf& f, const RGBA& color1,
	                              const Pointf& c, double r, const RGBA& color2,
	                              const Xform2D& m, int style);
	void             RenderRadial(double width, const Pointf& f, const RGBA& color1, const RGBA& color2,
	                              const Xform2D& transsrc, int style);
	void             MakeGradient(RGBA color1, RGBA color2, int cx);
	void             Gradient(const RGBA& color1, const RGBA& color2, const Pointf& p1, const Pointf& p2);
	void             ColorStop0(Attr& a, double pos, const RGBA& color);
	void             FinishMask();

	static void RenderPathSegments(LinearPathConsumer *g, const Vector<byte>& path, const SimpleAttr *attr, double tolerance);

	void CoRasterize(int from, int to);
	void FinishPathJob();
	void FinishFillJob()                                       { fill_job.Finish(); }
	                               
	enum { FILL = -1, CLIP = -2, ONPATH = -3 };

public:
	ImageBuffer&       GetBuffer()                             { return ib; }
	const ImageBuffer& GetBuffer() const                       { return ib; }

	void               Finish();
	
	BufferPainter&     Co(bool b = true)                       { Finish(); co = b; return *this; }
	BufferPainter&     PreClip(bool b = true)                  { dopreclip = b; preclip_mtx_serial = -1; return *this; }
	BufferPainter&     PreClipDashed()                         { dopreclip = 2; preclip_mtx_serial = -1; return *this; }
	BufferPainter&     ImageCache(bool b = true)               { imagecache = b; return *this; }
	BufferPainter&     NoImageCache(bool b = true)             { return ImageCache(false); }

	BufferPainter(ImageBuffer& ib, int mode = MODE_ANTIALIASED);
	BufferPainter(PainterTarget& t, double tolerance = Null);
	
	~BufferPainter()                                           { Finish(); }
};

#include "Interpolator.hpp"
