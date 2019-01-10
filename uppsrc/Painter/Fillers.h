#ifndef S_PAINTER_FILLERS_H
#define S_PAINTER_FILLERS_H

namespace Upp {

struct SolidFiller : Rasterizer::Filler {
	RGBA *t;
	RGBA  c;
	bool  invert;
	
	void Start(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
};

struct SpanFiller : Rasterizer::Filler {
	RGBA       *t;
	const RGBA *s;
	int         y;
	RGBA       *buffer;
	SpanSource *ss;
	int         alpha;
	
	void Start(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
};

struct SubpixelFiller : Rasterizer::Filler {
	int16        *sbuffer;
	int16        *begin;
	RGBA         *t, *end;
	int16        *v;
	RGBA         *s;
	RGBA          color;
	SpanSource   *ss;
	int           alpha;
	RGBA         *buffer;
	int           y;
	bool          invert;

	void Write(int len);
	void RenderN(int val, int h, int n);

	void Start(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
	void End();
};

struct ClipFiller : Rasterizer::Filler {
	Buffer<byte> buffer;
	byte        *t;
	int          x;
	int          cx;
	int          last;
	byte        *lastn;
	bool         empty;
	bool         full;
	
	void Span(int c, int len);

	virtual void Render(int val);
	virtual void Render(int val, int len);
	virtual void Start(int x, int len);

	void   Clear();
	void   Finish(ClippingLine& cl);
	
	void   Init(int cx);
};

struct MaskFillerFilter : Rasterizer::Filler {
	Rasterizer::Filler *t;
	const byte         *mask;
	int                 empty;
	int                 full;

	void Start(int minx, int maxx);
	void Render(int val, int len);
	void Render(int val);
	void End() { t->End(); }
	
	void Set(Rasterizer::Filler *f, const byte *m) { t = f; mask = m; empty = full = 0; }
};

struct NoAAFillerFilter : Rasterizer::Filler {
	Rasterizer::Filler *t;

	void Start(int minx, int maxx);
	void Render(int val, int len);
	void Render(int val);
	void End() { t->End(); }
	
	void Set(Rasterizer::Filler *f)                 { t = f; }
};

}

#endif
