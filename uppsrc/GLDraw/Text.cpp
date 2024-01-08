#include "GLDraw.h"

namespace Upp {
	
Image RenderGlyphByPainter2(Point at, double angle, int chr, Font font, Color color, Size sz)
{
	ImageBuffer ib(sz);
	BufferPainter sw(ib);
	sw.Clear(RGBAZero());
	sw.EvenOdd(true);
	sw.Translate(at.x, at.y);
	if(angle)
		sw.Rotate(angle);
	wchar text = chr;
	sw.Text(0, 0, &text, font, 1);
	sw.Fill(color);
	Image h = ib;
	return Premultiply(h);
}

int texture_glyph_cache_max = 64*1024*1024;

struct sGlyphTextureMaker : LRUCache<GLTexture>::Maker {
	double angle;
	int    chr;
	Font   font;
	Color  color;

	virtual String Key() const {
		String h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		return h;
	}
	virtual int Make(GLTexture& object) const {
		GL_TIMING("Do glyph");
		Point at(font[chr], font.GetLineHeight());
		int n = 2 * (at.x + at.y);
		at.x = max(at.x, at.y);
		at.y = max(at.x, at.y);
		Image img = AutoCrop(WithHotSpot(RenderGlyphByPainter2(at, angle, chr, font, color, Size(n, n)), at.x, at.y), RGBAZero());
		object = GLTexture(img, 0);
		return 4 * img.GetLength();
	}
};

GLTexture GetGlyphGLTextureCached(double angle, int chr, Font font, Color color)
{
	GL_TIMING("GetGlyphGLTextureCached");
	static LRUCache<GLTexture> cache;
	sGlyphTextureMaker cm;
	cm.angle = angle;
	cm.chr = chr;
	cm.font = font;
	cm.color = color;
	cache.Shrink(texture_glyph_cache_max, 20000);
	return cache.Get(cm);
}


void GLDrawText(const GLContext2D& dd, Pointf pos, double angle, const wchar *text, Font font,
                Color ink, int n, const int *dx)
{
	GL_TIMING("GLDrawText");
	int x = 0;
	Pointf u;
	if(angle)
		u = Polar(-angle);
	if(n < 0)
		n = strlen__(text);
	for(int i = 0; i < n; i++) {
		GLTexture m = GetGlyphGLTextureCached(-angle, text[i], font, ink);
		Point h = m.GetHotSpot();
		Pointf p = (angle ? pos + x * u : Pointf(x + pos.x, pos.y)) - (Pointf)m.GetHotSpot();
		GLDrawTexture(dd, Rectf(p, m.GetSize()), m);
		x += dx ? *dx++ : font[text[i]];
	}
}

};
