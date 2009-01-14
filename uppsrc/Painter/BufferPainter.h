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

	typedef agg::path_base<vertex_upp_storage> path_storage;

	typedef agg::pixfmt_rgba32_pre pixfmt;
	typedef agg::rgba8 color_type;
	typedef agg::renderer_base<agg::pixfmt_rgba32_pre> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
	typedef agg::span_interpolator_linear<> interpolator_type;
	typedef agg::image_accessor_clip<pixfmt> img_source_type;
	typedef agg::span_image_filter_rgba_bilinear_clip<pixfmt, interpolator_type> span_gen_type;
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
	};

	Attr                          attr;
	Attr                          pathattr;
	Vector<Attr>                  attrstack;

	path_storage                  path;
	bool                          inpath;

	agg::rendering_buffer         rbuf;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_p8              scanline_p;
	renderer_base                 renb;
	renderer_solid                renderer;
	agg::pixfmt_rgba32_pre        pixf;

	typedef agg::conv_curve<path_storage>       Curved;
	typedef agg::conv_transform<Curved>         CurvedTrans;

	Curved                        curved;
	CurvedTrans                   curved_trans;
	
	Rectf                         pathrect;
	Pointf                        current, ccontrol, qcontrol;
	
	void   PathPoint0(double x, double y);
	void   PathPoint(double& x, double& y, bool rel);
	void   EndPoint(double& x, double& y, bool rel);
	void   MinMax(Pointf& min, Pointf& max, Pointf p) const;
	bool   PathVisible(double d) const;
	Pointf Current() const        { return current; }
	Rectf  PathRect() const       { return pathrect; }
	path_storage MakeStroke(double width);
	Pointf ReadPoint(CParser& p, bool rel);
	void   RenderClip(byte *t, int alpha);
	void   MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx);
	void   ColorStop0(Attr& a, double pos, const RGBA& color);
	void   FinishMask();
	void   SetRbuf();

public:
	BufferPainter(ImageBuffer& ib);
};
