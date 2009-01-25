#include "Painter.h"

// int agg_line_counter; _DBG_

NAMESPACE_UPP

void BufferPainter::FillOp(const RGBA& c)
{
	PAINTER_TIMING("FillOp");
//	agg_line_counter = 0;
	if(inpath)
		path.close_polygon();
	pixf.noaa = pathattr.noaa;
	RGBA color = c;
	if(PathVisible(0) && color.a) {
		if(pathattr.opacity != 1.0) {
			color.a = int(color.a * pathattr.opacity);
			color.r = int(color.r * pathattr.opacity);
			color.g = int(color.g * pathattr.opacity);
			color.b = int(color.b * pathattr.opacity);
		}
		if(color.a) {
			rasterizer.reset();
			rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
			rasterizer.add_path(curved_trans);
			if(clip.GetCount()) {
				agg::rendering_buffer mask_rbuf;
				mask_rbuf.attach(~clip.Top(), size.cx, size.cy, size.cx);
				agg::alpha_mask_gray8 mask(mask_rbuf);
				agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
				renderer.color(color);
				agg::render_scanlines(rasterizer, sl, renderer);
			}
			else {
				renderer.color(*(color_type *)&color);
				agg::render_scanlines(rasterizer, scanline_p, renderer);
			}
			rasterizer.reset();
		}
	}
	inpath = false;
//	DDUMP(agg_line_counter);
}

struct UppImageAggSpan {
    struct RGBAV {
        dword r, g, b, a;
        
		void Set(dword v) { r = g = b = a = v; }
		void Put(dword weight, const RGBA& src) {
			r += weight * src.r;
			g += weight * src.g;
			b += weight * src.b;
			a += weight * src.a;
		}
    };

	agg::span_interpolator_linear<> interpolator;
	int         ax, ay, cx, cy, maxx, maxy;
	byte        style;
	byte        hstyle, vstyle;
	bool        fast;
	bool        fixed;
	const RGBA *image;
	int         alpha;
	
	void SetImage(const Image& img) {
		image = ~img;
		cx = img.GetWidth();
		cy = img.GetHeight();
		maxx = cx - 1;
		maxy = cy - 1;
		ax = 6000000 / cx * cx;
		ay = 6000000 / cy * cy;
	}
	
	RGBA Pixel(int x, int y) { return image[cx * y + x]; }

	RGBA GetPixel(int x, int y) {
		if(hstyle == FILL_HPAD)
			x = minmax(x, 0, maxx);
		else
		if(hstyle == FILL_HREFLECT)
			x = (x + ax) / cx & 1 ? (ax - x - 1) % cx : (x + ax) % cx;
		else
		if(hstyle == FILL_HREPEAT)
			x = (x + ax) % cx;
		if(vstyle == FILL_VPAD)
			y = minmax(y, 0, maxy);
		else
		if(vstyle == FILL_VREFLECT)
			y = (y + ay) / cy & 1 ? (ay - y - 1) % cy : (y + ay) % cy;
		else
		if(vstyle == FILL_VREPEAT)
			y = (y + ay) % cy;
		return fixed || (x >= 0 && x < cx && y >= 0 && y < cy) ? image[cx * y + x] : RGBAZero();
	}

	void prepare() {}
	
	void generate(RGBA *_span, int x, int y, unsigned len)
	{
		interpolator.begin(x + 0.5, y + 0.5, len);
		RGBA *span = (RGBA *)_span;
		fixed = hstyle && vstyle;
		while(len--) {
			int x_hr;
			int y_hr;
			interpolator.coordinates(&x_hr, &y_hr);
			x_hr -= 128;
			y_hr -= 128;
			int x_lr = x_hr >> 8;
			int y_lr = y_hr >> 8;
			if(hstyle == FILL_HREPEAT)
				x_lr = (x_lr + ax) % cx;
			if(vstyle == FILL_VREPEAT)
				y_lr = (y_lr + ay) % cy;
			if(fast) {
				RGBA v;
				if(x_lr > 0 && x_lr < maxx && y_lr > 0 && y_lr < maxy)
					v = Pixel(x_lr, y_lr);
				if(style == 0 && (x_lr < -1 || x_lr > cx || y_lr < -1 || y_lr > cy))
					v == RGBAZero();
				else
					v = GetPixel(x_lr, y_lr);
				if(alpha == 256)
					*span = v;
				else {
					span->r = byte((alpha * v.r) >> 8);
					span->g = byte((alpha * v.g) >> 8);
					span->b = byte((alpha * v.b) >> 8);
					span->a = byte((alpha * v.a) >> 8);
				}
			}
			else {
				RGBAV v;
				v.Set(256 * 256 / 2);
				x_hr &= 255;
				y_hr &= 255;
				if(x_lr > 0 && x_lr < maxx && y_lr > 0 && y_lr < maxy) {
					v.Put((256 - x_hr) * (256 - y_hr), Pixel(x_lr, y_lr));
					v.Put(x_hr * (256 - y_hr), Pixel(x_lr + 1, y_lr));
					v.Put((256 - x_hr) * y_hr, Pixel(x_lr, y_lr + 1));
					v.Put(x_hr * y_hr, Pixel(x_lr + 1, y_lr + 1));
				}
				else
				if(style == 0 && (x_lr < -1 || x_lr > cx || y_lr < -1 || y_lr > cy))
					v.Set(0);
				else {
					v.Put((256 - x_hr) * (256 - y_hr), GetPixel(x_lr, y_lr));
					v.Put(x_hr * (256 - y_hr), GetPixel(x_lr + 1, y_lr));
					v.Put((256 - x_hr) * y_hr, GetPixel(x_lr, y_lr + 1));
					v.Put(x_hr * y_hr, GetPixel(x_lr + 1, y_lr + 1));
				}
				v.r >>= 16;
				v.g >>= 16;
				v.b >>= 16;
				v.a >>= 16;
				if(alpha == 256) {
					span->r = (byte)v.r;
					span->g = (byte)v.g;
					span->b = (byte)v.b;
					span->a = (byte)v.a;
				}
				else {
					span->r = byte((alpha * v.r) >> 8);
					span->g = byte((alpha * v.g) >> 8);
					span->b = byte((alpha * v.b) >> 8);
					span->a = byte((alpha * v.a) >> 8);
				}
			}
			++span;
			++interpolator;
        }
    }
};

void BufferPainter::FillOp(const Image& image, const Matrix2D& transsrc, dword flags)
{
	if(image.GetWidth() == 0 || image.GetHeight() == 0)
		return;
	if(inpath)
		path.close_polygon();
	span_alloc sa;

	pixf.noaa = pathattr.noaa;
	Matrix2D m = transsrc * pathattr.mtx;
	m.invert();
	UppImageAggSpan sg;
	sg.interpolator.transformer(m);
	sg.alpha = int(pathattr.opacity * 256);
	sg.SetImage(image);
	sg.style = flags & 15;
	sg.hstyle = flags & 3;
	sg.vstyle = flags & 12;
	sg.fast = flags & FILL_FAST;

	rasterizer.reset();
	rasterizer.filling_rule(pathattr.evenodd ? agg::fill_even_odd : agg::fill_non_zero);
	rasterizer.add_path(curved_trans);

	if(clip.GetCount()) {
		agg::rendering_buffer mask_rbuf;
		mask_rbuf.attach(~clip.Top(), size.cx, size.cy, size.cx);
		agg::alpha_mask_gray8 mask(mask_rbuf);
		agg::scanline_u8_am<agg::alpha_mask_gray8> sl(mask);
		agg::render_scanlines_aa(rasterizer, sl, renb, sa, sg);
	}
	else
		agg::render_scanlines_aa(rasterizer, scanline_p, renb, sa, sg);
	rasterizer.reset();
	inpath = false;
}

END_UPP_NAMESPACE
