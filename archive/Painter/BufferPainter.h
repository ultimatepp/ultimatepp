// #define USE_MMX

inline RGBA Mul8(const RGBA& s, int mul)
{
	RGBA t;
	t.r = (mul * s.r) >> 8;
	t.g = (mul * s.g) >> 8;
	t.b = (mul * s.b) >> 8;
	t.a = (mul * s.a) >> 8;
	return t;
}

inline RGBA MulA(const RGBA& s, int alpha)
{
	return Mul8(s, alpha + (alpha >> 7));
}

#ifndef USE_MMX

inline void AlphaBlend(RGBA& t, const RGBA& c)
{
	int alpha = 256 - (c.a + (c.a >> 7));
	t.r = c.r + (alpha * t.r >> 8);
	t.g = c.g + (alpha * t.g >> 8);
	t.b = c.b + (alpha * t.b >> 8);
	t.a = c.a + (alpha * t.a >> 8);
}

inline void AlphaBlendCover(RGBA& t, const RGBA& c, byte cover)
{
	AlphaBlend(t, MulA(c, cover));
}

inline void FinishBlend() {}

#else

#include <xmmintrin.h>

inline __m64 RGBAtoMMX(const RGBA& rgba)
{
	return _mm_unpacklo_pi8(_mm_cvtsi32_si64(*(dword *)&rgba), _mm_setzero_si64());
}

inline RGBA MMXtoRGBA(__m64 c)
{
	int x = _mm_cvtsi64_si32(_mm_packs_pu16(c, _mm_setzero_si64()));
	return *(RGBA *)&x;
}

inline void AlphaBlend(RGBA& t, const RGBA& c)
{
	__m64 alpha = _mm_set1_pi16(256 - (c.a + (c.a >> 7)));
	__m64 tx = RGBAtoMMX(t);
	tx = _mm_mullo_pi16(alpha, tx);
	tx = _mm_srli_pi16(tx, 8);
	tx = _mm_add_pi16(tx, RGBAtoMMX(c));
	t = MMXtoRGBA(tx);
}

inline void AlphaBlendCover(RGBA& t, const RGBA& c, byte cover)
{
	__m64 cx = _mm_srli_pi16(_mm_mullo_pi16(RGBAtoMMX(c), _mm_set1_pi16(cover + (cover >> 7))), 8);
	__m64 tx = RGBAtoMMX(t);
	byte a = MMXtoRGBA(cx).a;
	__m64 alpha = _mm_set1_pi16(256 - (a + (a >> 7)));
	tx = _mm_mullo_pi16(alpha, tx);
	tx = _mm_srli_pi16(tx, 8);
	tx = _mm_add_pi16(tx, cx);
	t = MMXtoRGBA(tx);
}

inline void AlphaBlendCover(dword *t, dword s, const byte *c, int len)
{
	dword *e = t + len;
	__m64 zero = _mm_setzero_si64();
	__m64 src = _mm_unpacklo_pi8(_mm_cvtsi32_si64(s), zero);
	while(t < e) {
		byte cover = *c;
		__m64 cx = _mm_srli_pi16(_mm_mullo_pi16(src, _mm_set1_pi16(cover + (cover >> 7))), 8);
		__m64 tx = _mm_unpacklo_pi8(_mm_cvtsi32_si64(*t), zero);
		byte a = (dword)_mm_cvtsi64_si32(_mm_packs_pu16(cx, zero)) >> 24;
		__m64 alpha = _mm_set1_pi16(256 - (a + (a >> 7)));
		tx = _mm_mullo_pi16(alpha, tx);
		tx = _mm_srli_pi16(tx, 8);
		tx = _mm_add_pi16(tx, cx);
		*t = _mm_cvtsi64_si32(_mm_packs_pu16(tx, zero));
		t++;
		c++;
	}
	_mm_empty();
}

inline void FinishBlend()
{
	_mm_empty();
}

#endif

class BufferPainter : public Painter {
protected:
	virtual void   ClearOp(const RGBA& color);

	virtual void   MoveOp(double x, double y, bool rel);
	virtual void   LineOp(double x, double y, bool rel);
	virtual void   QuadraticOp(double x1, double y1, double x, double y, bool rel);
	virtual void   QuadraticOp(double x, double y, bool rel);
	virtual void   CubicOp(double x1, double y1, double x2, double y2, double x, double y, bool rel);
	virtual void   CubicOp(double x2, double y2, double x, double y, bool rel);
	virtual void   CloseOp();

	virtual void   FillOp(const RGBA& color);
	virtual void   FillOp(const Image& image, const Matrix2D& transsrc,
	                      dword flags);
	virtual void   FillOp(double x1, double y1, const RGBA& color1,
	                      double x2, double y2, const RGBA& color2,
	                      int style);
	virtual void   FillOp(double fx, double fy, const RGBA& color1, 
	                      double x1, double y1, double r, const RGBA& color2,
	                      int style);
	
	virtual void   StrokeOp(double width, const RGBA& rgba);
	virtual void   StrokeOp(double width, const Image& image, const Matrix2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, double x1, double y1, const RGBA& color1,
	                        double x2, double y2, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, double fx, double fy, const RGBA& color1, 
	                        double x, double y, double r, const RGBA& color2,
	                        int style);

	virtual void   ClipOp();

	virtual void   ColorStopOp(double pos, const RGBA& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Vector<double>& dash, double start);
	virtual void NoAAOp(bool noaa);


	virtual void   TransformOp(const Matrix2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();
	
	virtual void   BeginMaskOp();

private:
	class vertex_upp_storage {
		Vector<Pointf> v;
		Vector<int>    c;
    public:
		typedef double value_type;
		void remove_all() { v.Clear(); c.Clear(); }
		void free_all()   { remove_all(); }
		void add_vertex(double x, double y, unsigned cmd) { v.Add(Pointf(x, y)); c.Add(cmd); }
		void modify_vertex(int i, double x, double y) { v[i].x = x; v[i].y = y; }
		void modify_vertex(int i, double x, double y, unsigned cmd) { modify_vertex(i, x, y); c[i] = cmd; }
		void modify_command(int i, unsigned cmd) { c[i] = cmd; }
		void swap_vertices(unsigned v1, unsigned v2) { v.Swap(v1, v2); c.Swap(v1, v2); }
		unsigned last_command() const { return c.GetCount() ? c.Top() : agg::path_cmd_stop; }
		unsigned last_vertex(double* x, double* y) const {
			if(c.GetCount() == 0) {
				*x = *y = 0.0;
				return agg::path_cmd_stop;
			}
			return vertex(v.GetCount() - 1, x, y);
		}
		unsigned prev_vertex(double* x, double* y) const {
			if(c.GetCount() < 2) {
				*x = *y = 0.0;
				return agg::path_cmd_stop;
			}
			return vertex(v.GetCount() - 2, x, y);
		}
		double last_x() const { return v.GetCount() ? v.Top().x : 0.0; }
		double last_y() const { return v.GetCount() ? v.Top().y : 0.0; }
		unsigned total_vertices() const { return v.GetCount(); }
		unsigned vertex(int i, double* x, double* y) const {
			*x = v[i].x;
			*y = v[i].y;
			return c[i];
		}
		unsigned command(int i) const { return (int)i < c.GetCount() ? c[i] : agg::path_cmd_stop; }
    };

	class upp_pixfmt {
		Size  sz;
		RGBA *buffer;

    public:
		typedef RGBA pixel_type;
		typedef RGBA color_type;
		typedef byte value_type;
		typedef int  calc_type;
		typedef agg::const_row_info<RGBA> row_data;
		
		bool noaa;
		
		void attach(ImageBuffer& ib) { buffer = ib; sz = ib.GetSize(); }

		int width()  const { return sz.cx;  }
		int height() const { return sz.cy; }
		int stride() const { return sz.cx; }
		
		RGBA* row_ptr(int y)              { return buffer + sz.cx * y; }
		RGBA *ptr(int x, int y)           { return buffer + sz.cx * y + x; }
		const RGBA* row_ptr(int y) const  { return buffer + sz.cx * y; }
		
		void blend_hline(int x, int y, int len, RGBA c, byte cover);
		void blend_solid_hspan(int x, int y, int len, const RGBA& c, const byte* covers);
		void blend_color_hspan(int x, int y, int len, const RGBA* colors,
		                       const byte* covers, byte cover);
    };

	struct upp_pixfmt_clip {
		Size  sz;
		byte *buffer;

		typedef byte pixel_type;
		typedef byte color_type;
		typedef byte value_type;
		typedef int  calc_type;
		typedef agg::const_row_info<RGBA> row_data;

		int width()  const { return sz.cx;  }
		int height() const { return sz.cy; }
		int stride() const { return sz.cx; }
		
		byte* row_ptr(int y)              { return buffer + sz.cx * y; }
		byte *ptr(int x, int y)           { return buffer + sz.cx * y + x; }
		const byte* row_ptr(int y) const  { return buffer + sz.cx * y; }
		
		void blend_hline(int x, int y, int len, byte, byte cover);
		void blend_solid_hspan(int x, int y, int len, byte, const byte* covers);
		void blend_color_hspan(int x, int y, int len, byte *,
		                       const byte* covers, byte cover);
    };

	typedef agg::path_base<vertex_upp_storage> path_storage;

	typedef upp_pixfmt pixfmt;
	typedef RGBA color_type;
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
	typedef agg::span_interpolator_linear<> interpolator_type;
	typedef agg::span_allocator<color_type> span_alloc;
	typedef renderer_base::color_type x;
	
	Size                          size;
	Sizef                         sizef;
	ImageBuffer&                  buffer;
	Array< Buffer<byte> >         clip;
	Array< ImageBuffer >          mask;
	
	struct Attr : Moveable<Attr> {
		Matrix2D                        mtx;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		double                          miter_limit;
		WithDeepCopy< Vector<double> >  dash;
		WithDeepCopy< Vector<double> >  stop;
		WithDeepCopy< Vector<RGBA> >    stop_color;
		double                          dash_start;
		double                          opacity;
		int                             cliplevel;
		bool                            hasclip;
		bool                            mask;
		bool                            noaa;
	};

	Attr                          attr;
	Attr                          pathattr;
	Vector<Attr>                  attrstack;

	path_storage                  path;
	bool                          inpath;

	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	pixfmt                        pixf;

	typedef agg::conv_curve<path_storage>       Curved;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedTrans                   curved_trans;
	
	Rectf                         pathrect;
	Pointf                        current, ccontrol, qcontrol;
	
	struct StrokeInfo {
		bool         evenodd;
		path_storage path;
	};
	
	void   PathPoint0(double x, double y);
	void   PathPoint(double& x, double& y, bool rel);
	void   EndPoint(double& x, double& y, bool rel);
	void   MinMax(Pointf& min, Pointf& max, Pointf p) const;
	bool   PathVisible(double d) const;
	Pointf Current() const        { return current; }
	Rectf  PathRect() const       { return pathrect; }
	StrokeInfo BeginStroke(double width);
	void   EndStroke(StrokeInfo& f);
	Pointf ReadPoint(CParser& p, bool rel);
	void   RenderClip(byte *t);
	void   MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx);
	void   ColorStop0(Attr& a, double pos, const RGBA& color);
	void   FinishMask();
	void   SetRbuf();

	friend class Painter;

public:
	BufferPainter(ImageBuffer& ib);
};
