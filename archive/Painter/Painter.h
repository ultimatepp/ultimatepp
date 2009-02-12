#ifndef _SDraw_SDraw_h
#define _SDraw_SDraw_h

#include <Draw/Draw.h>

#define  PAINTER_TIMING(x)  // RTIMING(x)

#define  AGGUPP 1

#include "agg_basics.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
#include "agg_conv_dash.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_alpha_mask_u8.h"

NAMESPACE_UPP

struct Matrix2D : agg::trans_affine {
	Matrix2D operator*(const Matrix2D& s) const  { Matrix2D x = *this; x *= s; return x; }
	
	void   Transform(double& x, double& y) const { transform(&x, &y); }
	Pointf Transformed(double x, double y) const { Transform(x, y); return Pointf(x, y); }
	Pointf Transformed(Pointf p) const           { Transform(p.x, p.y); return p; }
	Matrix2D Inverted() const                    { Matrix2D h = *this; h.invert(); return h; }
};

Matrix2D Translate2D(double x, double y);
Matrix2D Rotate2D(double angle);
Matrix2D Scale2D(double scalex, double scaley);
Matrix2D Scale2D(double scale);
Matrix2D Sheer2D();

enum {
	LINECAP_BUTT = agg::butt_cap,
	LINECAP_SQUARE = agg::square_cap,
	LINECAP_ROUND = agg::round_cap,

	LINEJOIN_MITER = agg::miter_join_revert,
	LINEJOIN_ROUND = agg::round_join,
	LINEJOIN_BEVEL = agg::bevel_join,
	
	FILL_EXACT      = 0,

	FILL_HPAD       = 1,
	FILL_HREPEAT    = 2,
	FILL_HREFLECT   = 3,

	FILL_VPAD       = 4,
	FILL_VREPEAT    = 8,
	FILL_VREFLECT   = 12,

	FILL_PAD        = FILL_HPAD|FILL_VPAD,
	FILL_REPEAT     = FILL_HREPEAT|FILL_VREPEAT,
	FILL_REFLECT    = FILL_HREFLECT|FILL_VREFLECT,
	
	FILL_FAST       = 128,
	
	GRADIENT_PAD     = 0,
	GRADIENT_REPEAT  = 1,
	GRADIENT_REFLECT = 2,
};

class Painting;

class Painter : public Draw {
protected:
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual Rect GetClipOp() const;
	virtual bool IsPaintingOp(const Rect& r) const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	virtual void DrawPaintingOp(const Rect& target, const Painting& w);

protected:
	virtual void   ClearOp(const RGBA& color) = 0;

	virtual void   MoveOp(double x, double y, bool rel) = 0;
	virtual void   LineOp(double x, double y, bool rel) = 0;
	virtual void   QuadraticOp(double x1, double y1, double x, double y, bool rel) = 0;
	virtual void   QuadraticOp(double x, double y, bool rel) = 0;
	virtual void   CubicOp(double x1, double y1, double x2, double y2, double x, double y, bool rel) = 0;
	virtual void   CubicOp(double x2, double y2, double x, double y, bool rel) = 0;
	virtual void   CloseOp() = 0;

	virtual void   FillOp(const RGBA& color) = 0;
	virtual void   FillOp(const Image& image, const Matrix2D& transsrc, dword flags) = 0;
	virtual void   FillOp(double x1, double y1, const RGBA& color1,
	                      double x2, double y2, const RGBA& color2,
	                      int style) = 0;
	virtual void   FillOp(double fx, double fy, const RGBA& color1, 
	                      double x1, double y1, double r, const RGBA& color2,
	                      int style) = 0;
	
	virtual void   StrokeOp(double width, const RGBA& rgba) = 0;
	virtual void   StrokeOp(double width, const Image& image, const Matrix2D& transsrc,
	                        dword flags) = 0;
	virtual void   StrokeOp(double width, double x1, double y1, const RGBA& color1,
	                        double x2, double y2, const RGBA& color2,
	                        int style) = 0;
	virtual void   StrokeOp(double width, double fx, double fy, const RGBA& color1, 
	                        double x, double y, double r, const RGBA& color2,
	                        int style) = 0;

	virtual void   ClipOp() = 0;

	virtual void   CharacterOp(double x, double y, int ch, Font fnt);
	virtual void   TextOp(double x, double y, const wchar *text, Font fnt, int n = -1, double *dx = NULL);
	
	virtual void   ColorStopOp(double pos, const RGBA& color) = 0;
	virtual void   ClearStopsOp() = 0;
	
	virtual void   OpacityOp(double o) = 0;
	virtual void   LineCapOp(int linecap) = 0;
	virtual void   LineJoinOp(int linejoin) = 0;
	virtual void   MiterLimitOp(double l) = 0;
	virtual void   EvenOddOp(bool evenodd) = 0;
	virtual void   DashOp(const Vector<double>& dash, double start = 0) = 0;
	virtual void   NoAAOp(bool noaa) = 0;

	virtual void   TransformOp(const Matrix2D& m) = 0;

	virtual void   BeginOp() = 0;
	virtual void   EndOp() = 0;
	
	virtual void   BeginMaskOp() = 0;

private:
	Pointf ReadPoint(CParser& p);
	void   RectPath(int x, int y, int cx, int cy);
	void   RectPath(const Rect& r);

	static RGBA GetRGBA(StringStream& ss);

public:
	void     Clear(const RGBA& color);

	Painter& Move(double x, double y, bool rel);
	Painter& Line(double x, double y, bool rel);
	Painter& Quadratic(double x1, double y1, double x, double y, bool rel);
	Painter& Quadratic(double x, double y, bool rel);
	Painter& Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel);
	Painter& Cubic(double x2, double y2, double x, double y, bool rel);

	Painter& Move(double x, double y);
	Painter& Line(double x, double y);
	Painter& Quadratic(double x1, double y1, double x, double y);
	Painter& Quadratic(double x, double y);
	Painter& Cubic(double x1, double y1, double x2, double y2, double x, double y);
	Painter& Cubic(double x2, double y2, double x, double y);

	Painter& RelMove(double x, double y);
	Painter& RelLine(double x, double y);
	Painter& RelQuadratic(double x1, double y1, double x, double y);
	Painter& RelQuadratic(double x, double y);
	Painter& RelCubic(double x1, double y1, double x2, double y2, double x, double y);
	Painter& RelCubic(double x2, double y2, double x, double y);

	Painter& Close();

	Painter& Fill(const RGBA& color);
	Painter& Fill(const Image& image, const Matrix2D& transsrc = Matrix2D(), dword flags = 0);
	Painter& Fill(const Image& image, double x1, double y1, double x2, double y2,
	              dword flags = 0);
	Painter& Fill(double x1, double y1, const RGBA& color1,
	              double x2, double y2, const RGBA& color2, int style = GRADIENT_PAD);
	Painter& Fill(double fx, double fy, const RGBA& color1,
	              double x, double y, double r, const RGBA& color2, int style = GRADIENT_PAD);
	Painter& Fill(double x, double y, const RGBA& color1,
	              double r, const RGBA& color2, int style = GRADIENT_PAD);
	
	Painter& Stroke(double width, const RGBA& color);
	Painter& Stroke(double width, const Image& image, const Matrix2D& transsrc, dword flags = 0);
	Painter& Stroke(double width, const Image& image, double x1, double y1, double x2, double y2,
	                dword flags = 0);
	Painter& Stroke(double width, double x1, double y1, const RGBA& color1,
	                double x2, double y2, const RGBA& color2,
	                int style = GRADIENT_PAD);
	Painter& Stroke(double width, double fx, double fy, const RGBA& color1,
	                double x1, double y1, double r, const RGBA& color2, int style = GRADIENT_PAD);
	Painter& Stroke(double width, double x1, double y1, const RGBA& color1,
	                double r, const RGBA& color2, int style = GRADIENT_PAD);

	Painter& Clip();

	Painter& Character(double x, double y, int ch, Font fnt);
	Painter& Text(double x, double y, const wchar *text, Font fnt, int n = -1, double *dx = NULL);
	Painter& Text(double x, double y, const WString& s, Font fnt, double *dx = NULL);
	Painter& Text(double x, double y, const String& s, Font fnt, double *dx = NULL);
	Painter& Text(double x, double y, const char *text, Font fnt, int n = -1, double *dx = NULL);
	
	Painter& Path(CParser& p);
	Painter& Path(const char *path);
	
	Painter& ColorStop(double pos, const RGBA& color);
	Painter& ClearStops();
	
	Painter& Opacity(double o);
	Painter& LineCap(int linecap);
	Painter& LineJoin(int linejoin);
	Painter& MiterLimit(double l);
	Painter& EvenOdd(bool evenodd);
	Painter& Dash(const Vector<double>& dash, double start = 0);
	Painter& Dash(const char *dash, double start = 0);
	Painter& NoAA(bool noaa = true);

	void   Transform(const Matrix2D& m);

	void   Begin();
	void   End();
	
	void   BeginMask();
	
	void   Translate(double x, double y);
	void   Rotate(double a);
	void   Scale(double scalex, double scaley);
	void   Scale(double scale);
	
	void   Paint(const Painting& p);
	
	Painter& Arc(double x, double y, double rx, double ry,
	             double start_angle, double sweep_angle, bool startline = false);
	Painter& Ellipse(double x, double y, double rx, double ry);
	Painter& Circle(double x, double y, double r);
	Painter& Rectangle(double x, double y, double cx, double cy);
};

#include "BufferPainter.h"
#include "Painting.h"

END_UPP_NAMESPACE

#include "Painter.hpp"

#endif
