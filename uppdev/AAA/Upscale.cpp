#include <Draw/Draw.h>

using namespace Upp;

int sqr(int x)
{
	return x * x;
}

RGBA Blend(RGBA a, RGBA b)
{
	RGBA r;
	r.a = ((int)a.a + b.a) >> 1;
	r.r = ((int)a.r + b.r) >> 1;
	r.g = ((int)a.g + b.g) >> 1;
	r.b = ((int)a.b + b.b) >> 1;
	return r;
}

RGBA Get(const Image& img, int x, int y)
{
	return Rect(img.GetSize()).Contains(x, y) ? img[y][x] : RGBAZero();
}

int  SqrDistance(RGBA a, RGBA b)
{
	return sqr(77 * ((int)a.r - b.r)) + sqr(151 * ((int)a.g - b.g)) + sqr(28 * ((int)a.b - b.b));
}

bool IsSimilar(RGBA a, RGBA b, int max)
{
	return SqrDistance(a, b) <= sqr(max);
}

int GetColorIndex(Vector<RGBA>& colors, Color c, int max_distance)
{
	for(int i = 0; i < colors.GetCount(); i++)
		if(IsSimilar(colors[i], c, max_distance))
			return i;
	colors.Add(c);
	return colors.GetCount() - 1;
}

struct Colors : Vector<RGBA> {
	const Image& m;
	int    x, y;
	
	int  DX(int r) {
		r &= 7;
		return findarg(r, 0, 7, 6) >= 0 ? -1 : findarg(r, 2, 3, 4) >= 0 ? 1 : 0;
	}
	
	int DY(int r) {
		r &= 7;
		return findarg(r, 0, 1, 2) >= 0 ? -1 : findarg(r, 4, 5, 6) >= 0 ? 1 : 0;
	}
	
	RGBA Get(int dx, int dy) {
		return ::Get(m, x + dx, y + dy);
	}
	
	RGBA Get(int r) {
		return ::Get(m, x + DX(r), y + DY(r));
	}

	int GetCI(Color c)
	{
		return GetColorIndex(*this, c, 500);
	}

	int  GetCI(int r) {
		return GetCI(Get(r));
	}
	
	int  GetCI(int dx, int dy) {
		return GetColorIndex(*this, Get(dx, dy), 500);
	}
	
	Colors(const Image& m, int x, int y) : m(m), x(x), y(y) {}
};

Image Upscale(const Image& simg)
{
	Size isz = simg.GetSize();

	ImageBuffer ib(2 * isz);
	
	Fill(~ib, RGBAZero(), ib.GetLength());

	for(int x = 0; x < isz.cx; x++)
		for(int y = 0; y < isz.cy; y++) {
			DLOG("---------- " << x << ", " << y);
			RGBA c = simg[y][x];
			int p[16];
			// 0 1 2
			// 7 c 3
			// 6 5 4
			Colors colors(simg, x, y);
			for(int i = 0; i < 8; i++) {
				p[i] = p[i + 8] = colors.GetCI(i);
				DLOG(i << ": " << p[i]);
			}
			int ci = colors.GetCI(c);
			DDUMP(colors);
			// 0 1
			// 3 2
			int *pp = p;
			for(int r = 0; r < 4; r++) {
				RGBA& t = ib[2 * y + (r >= 2)][2 * x + (r == 1 || r == 2)];
				t = c;
				if(pp[1] == pp[7]
//				   && ci != pp[0]
//				   && (pp[0] != pp[1] || pp[0] != pp[6] || pp[0] != pp[2])
//				   && (pp[7] != pp[3] && pp[7] != pp[4])
//				   && (pp[1] != pp[5] && pp[1] != pp[4])
				) {
				    DLOG(r);
					DLOG("x: " << (r == 1 || r == 2));
					DLOG("y: " << (r >= 2));
					t = Blend(colors.Get(2 * r + 1), colors.Get(2 * r + 7));
				}
				pp += 2;
			}
		}
	
	return ib;
}

int AADetected(RGBA a, RGBA b, RGBA c)
{
	DLOG("AADetect " << a << ", " << b << ", " << c);

	if(IsSimilar(a, c, 500) || IsSimilar(b, c, 500) || IsSimilar(a, b, 500))
		return false;

	int d = c.g - a.g;
	int m = b.g - a.g;
	
	if(abs(c.r - a.r) > d) {
		d = c.r - a.r;
		m = b.r - a.r;
	}

	if(abs(c.b - a.b) > d) {
		d = c.b - a.b;
		m = b.b - a.b;
	}
	
	if(abs(m) > abs(d) || m * d <= 0)
		return false;
	
	DDUMP(d);
	DDUMP(m);

	DDUMP(abs(m * (c.r - a.r) / d + a.r - b.r));
	DDUMP(abs(m * (c.b - a.b) / d + a.b - b.b));
	
	DDUMP(m * (c.r - a.r) / d + a.r);
	DDUMP(m * (c.b - a.b) / d + a.b);
	
	return (abs(m * (c.r - a.r) / d + a.r - b.r) < 4 &&
	        abs(m * (c.g - a.g) / d + a.g - b.g) < 4 &&
	        abs(m * (c.b - a.b) / d + a.b - b.b) < 4) * abs(d);
}

Image UpscaleA(const Image& simg)
{
	Size isz = simg.GetSize();

	ImageBuffer ib(2 * isz);
	
	Fill(~ib, RGBAZero(), ib.GetLength());

	for(int x = 0; x < isz.cx; x++)
		for(int y = 0; y < isz.cy; y++) {
			DLOG("---------- " << x << ", " << y);
			RGBA c = simg[y][x];
			int p[16];
			// 0 1 2
			// 7 c 3
			// 6 5 4
			Colors m(simg, x, y);
			for(int i = 0; i < 8; i++) {
				p[i] = p[i + 8] = m.GetCI(i);
				DLOG(i << ": " << p[i]);
			}
			int ci = m.GetCI(c);

			RGBA t[4];
			t[0] = t[1] = t[2] = t[3] = c;
			bool hl = p[0] == p[1] && p[1] == p[2] ||
			          p[6] == p[5] && p[5] == p[4];
			bool vl = p[0] == p[7] && p[7] == p[6] ||
			          p[2] == p[3] && p[3] == p[4];
			bool dia = !hl && !vl;
			dia = true;
			int horz = AADetected(m.Get(7), c, m.Get(3)) * 1;
			int vert = AADetected(m.Get(1), c, m.Get(5)) * 1;
			int dia1 = AADetected(m.Get(0), c, m.Get(4)) * 1 * dia;
			int dia2 = AADetected(m.Get(2), c, m.Get(6)) * 1 * dia;
			const int *pp = p;
			if(horz || vert || dia1 || dia2) {
				if(horz && !vert)
					t[1] = t[3] = m.Get(3);
				else
				if(vert && !horz)
					t[2] = t[3] = m.Get(5);
				else
				const int *pp = p;
				for(int i = 0; i < 4; i++) {
					if(pp[0] == pp[1] && pp[1] == pp[7])
						t[decode(i, 2, 3, 3, 2, i)] = m.Get(i * 2);
					pp += 2;
				}
			}
			
			ib[2 * y + 0][2 * x + 0] = t[0];
			ib[2 * y + 0][2 * x + 1] = t[1];
			ib[2 * y + 1][2 * x + 0] = t[2];
			ib[2 * y + 1][2 * x + 1] = t[3];
		}
	
	return ib;
}

Image UpscaleA2(const Image& simg)
{
	Size isz = simg.GetSize();

	ImageBuffer ib(2 * isz);
	
	Fill(~ib, RGBAZero(), ib.GetLength());

	for(int x = 0; x < isz.cx; x++)
		for(int y = 0; y < isz.cy; y++) {
			DLOG("---------- " << x << ", " << y);
			RGBA c = simg[y][x];
			int p[16];
			// 0 1 2
			// 7 c 3
			// 6 5 4
			Colors m(simg, x, y);
			for(int i = 0; i < 8; i++) {
				p[i] = p[i + 8] = m.GetCI(i);
				DLOG(i << ": " << p[i]);
			}
			int ci = m.GetCI(c);

			RGBA t[4];
			t[0] = t[1] = t[2] = t[3] = c;
		//	if(AADetected(m.Get(0), c, m.Get(4)))
		//		t[3] = m.Get(4);
		//	if(AADetected(m.Get(6), c, m.Get(2)))
		//		t[1] = m.Get(2);
			if(AADetected(m.Get(7), c, m.Get(3)) &&
			   (pp[0] == pp[7] && pp[6] == pp[7] )
				t[1] = t[3] = m.Get(3);
			if(AADetected(m.Get(1), c, m.Get(5)))
				t[2] = t[3] = m.Get(5);
			
			ib[2 * y + 0][2 * x + 0] = t[0];
			ib[2 * y + 0][2 * x + 1] = t[1];
			ib[2 * y + 1][2 * x + 0] = t[2];
			ib[2 * y + 1][2 * x + 1] = t[3];
		}
	
	return ib;
}
/*				int best = max(horz, vert, dia1, dia2);
				if(best == horz) t[1] = t[3] = m.Get(3);
				else
				if(best == vert) t[2] = t[3] = m.Get(5);
				else
				if(best == dia1) t[3] = m.Get(4);
				else
				if(best == dia2) t[2] = m.Get(6);*/

static inline int GetResult(uint32 A, uint32 B, uint32 C, uint32 D)
{
	int x = 0;
	int y = 0;
	int r = 0;
	if (A == C) x++; else if (B == C) y++;
	if (A == D) x++; else if (B == D) y++;
	if (x <= 1) r++;
	if (y <= 1) r--;
	return r;
}

RGBA Blend(RGBA a, RGBA b, RGBA c, RGBA d)
{
	return Blend(Blend(a, b), Blend(c, d));
}

Image Upscale2(const Image& simg) // Super SAI
{
	Size isz = simg.GetSize();

	ImageBuffer ib(2 * isz);
	
	Fill(~ib, RGBAZero(), ib.GetLength());

	for(int x = 0; x < isz.cx; x++)
		for(int y = 0; y < isz.cy; y++) {
			DLOG("---------- " << x << ", " << y);

			Colors m(simg, x, y);

            int colorB0 = m.GetCI(-1, -1);
            int colorB1 = m.GetCI(0, -1);
            int colorB2 = m.GetCI(1, -1);
            int colorB3 = m.GetCI(2, -1);

            int color4 = m.GetCI(-1, 0);
            int color5 = m.GetCI(0, 0);
            int color6 = m.GetCI(1, 0);
            int colorS2 = m.GetCI(2, 0);

            int color1 = m.GetCI(-1, 1);
            int color2 = m.GetCI(0, 1);
            int color3 = m.GetCI(1, 1);
            int colorS1 = m.GetCI(2, 1);

            int colorA0 = m.GetCI(-1, 2);
            int colorA1 = m.GetCI(0, 2);
            int colorA2 = m.GetCI(1, 2);
            int colorA3 = m.GetCI(2, 2);

			RGBA product1a, product1b, product2a, product2b;
			
			RGBA c2 = m.Get(0, 1);
			RGBA c3 = m.Get(1, 1);
			RGBA c5 = m.Get(0, 0);
			RGBA c6 = m.Get(1, 0);

			if(color2 == color6 && color5 != color3)
				product2b = product1b = c2;
			else
			if(color5 == color3 && color2 != color6)
				product2b = product1b = c5;
			else
			if(color5 == color3 && color2 == color6) {
			   int r = 0;
			
			   r += GetResult (color6, color5, color1, colorA1);
			   r += GetResult (color6, color5, color4, colorB1);
			   r += GetResult (color6, color5, colorA2, colorS1);
			   r += GetResult (color6, color5, colorB2, colorS2);

			   if(r > 0)
			      product2b = product1b = c6;
			   else
			   if(r < 0)
			      product2b = product1b = c5;
			   else
			      product2b = product1b = Blend(c5, c6);
			}
			else {
				if(color6 == color3 && color3 == colorA1 && color2 != colorA2 && color3 != colorA0)
					product2b = Blend(c3, c3, c3, c2);
				else
				if(color5 == color2 && color2 == colorA2 && colorA1 != color3 && color2 != colorA3)
					product2b = Blend(c2, c2, c2, c3);
				else
					product2b = Blend(c2, c3);
				
				if(color6 == color3 && color6 == colorB1 && color5 != colorB2 && color6 != colorB0)
					product1b = Blend(c6, c6, c6, c5);
				else
				if(color5 == color2 && color5 == colorB2 && colorB1 != color6 && color5 != colorB3)
					product1b = Blend(c6, c5, c5, c5);
				else
					product1b = Blend(c5, c6);
			}
			
			if (color5 == color3 && color2 != color6 && color4 == color5 && color5 != colorA2)
			   product2a = Blend(c2, c5);
			else
			if (color5 == color1 && color6 == color5 && color4 != color2 && color5 != colorA0)
			   product2a = Blend(c2, c5);
			else
			   product2a = c2;
			
			if (color2 == color6 && color5 != color3 && color1 == color2 && color2 != colorB2)
			   product1a = Blend(c2, c5);
			else
			if (color4 == color2 && color3 == color2 && color1 != color5 && color2 != colorB0)
			   product1a = Blend(c2, c5);
			else
			   product1a = c5;
			
			ib[2 * y][2 * x] = product1a;
			ib[2 * y][2 * x + 1] = product1b;
			ib[2 * y + 1][2 * x] = product2a;
			ib[2 * y + 1][2 * x + 1] = product2b;
		}
	
	return ib;
}
