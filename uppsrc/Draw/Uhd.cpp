#include "Draw.h"

namespace Upp {

// Upscale2x is based on xBR filter idea by Hyllian, google search on 'xBR filter'
// should eventually lead you to a forum topic where he made the algorithm public:
// http://board.byuu.org/viewtopic.php?f=10&t=2248
 
namespace Upscale2x_helper {
	int d(RGBA c1, RGBA c2)
	{
		int r = abs(c1.r - c2.r);
		int g = abs(c1.g - c2.g);
		int b = abs(c1.b - c2.b);
		return 48 * (r *  299 + g *  587 + b *  114)
		     +  7 * (r * -169 + g * -331 + b *  500)
		     +  6 * (r *  500 + g * -419 + b *  -81);
	}
};

Image Upscale2x_(const Image& src)
{
	using namespace Upscale2x_helper;
	Size isz = src.GetSize();
	ImageBuffer dst;
	dst.Create(2 * isz);
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
	        static const int8 cm[] = {
			//  -2  -1   0  +1  +2
				-1,  0,  1,  2, -1, // -2
				17, 19,  3,  4,  5, // -1
				16, 18, -1,  8,  6, //  0
				15, 14, 13,  9,  7, // +1
				-1, 12, 11, 10, -1, // +2
	        };
	
	        RGBA pp[40];
	        const int8 *q = cm;
			for(int dy = -2; dy <= 2; dy++)
				for(int dx = -2; dx <= 2; dx++) {
					if(*q >= 0) {
						int xx = x + dx;
						int yy = y + dy;
						pp[*q] = pp[*q + 20] = src[clamp(yy, 0, isz.cy - 1)][clamp(xx, 0, isz.cx - 1)];
					}
					q++;
				}
	
			RGBA c = src[y][x];
			RGBA *p = pp;
			
			for(int rot = 0; rot < 4; rot++) {
				RGBA t = c;
				if(d(c, p[14]) + d(c, p[4]) + d(p[19], p[16]) + d(p[19], p[1]) + 4 * d(p[18], p[3])
				   < d(p[18], p[13]) + d(p[18], p[17]) + d(p[3], p[8]) + d(p[3], p[0]) + 4 * d(c, p[19])) {
					RGBA nc = d(c, p[18]) <= d(c, p[3]) ? p[18] : p[3];
					t.r = (nc.r + c.r) >> 1;
					t.g = (nc.g + c.g) >> 1;
					t.b = (nc.b + c.b) >> 1;
				}
				// 0 1
				// 3 2
				dst[2 * y + (rot >= 2)][2 * x + (rot == 1 || rot == 2)] = t;
				p += 5;
			}
		}
	return Image(dst);
}

Image Upscale2x(const Image& src)
{
	if(IsNull(src))
		return src;
	Size s2 = src.Get2ndSpot();
	Image s;
	if(s2.cx > 0 || s2.cy > 0)  // When 2nd spot is defined, it is likely Chameleon rescaling item (e.g. button)
		s = Magnify(src, 2, 2); // in that case, filtering by smart rescale methods could lead to artifacts
	else {
		Size isz = src.GetSize();
		s = RecreateAlpha(Upscale2x_(GetOver(CreateImage(isz, White()), src)),
		                  Upscale2x_(GetOver(CreateImage(isz, Black()), src)));

		struct SFilter : ImageFilter9 { // Improve contours
			virtual RGBA operator()(const RGBA **mx) {
				RGBA s = mx[1][1];
				dword l = mx[0][1].a;
				dword r = mx[2][1].a;
				dword t = mx[1][0].a;
				dword b = mx[1][2].a;
				int l1 = 110;
				int l2 = 230;
				return l * r * t * b != 0 || s.a > l1 || mx[0][1].a > l2 || mx[2][1].a > l2 || mx[1][0].a > l2 || mx[1][2].a > l2 ? s
				       : RGBAZero();
			}
		} ef;
		s = Filter(s, ef);
	}
	ImageBuffer h(s);
	h.SetHotSpot(src.GetHotSpot() * 2);
	h.Set2ndSpot(s2 * 2);
	return Image(h);
}

Image Downscale2x(const Image& src)
{
	if(IsNull(src))
		return src;
	Size s2 = src.Get2ndSpot(); // see above...
	Image m = RescaleFilter(src, src.GetSize() / 2, s2.cx > 0 || s2.cy > 0 ? FILTER_BILINEAR : FILTER_LANCZOS3);
	ImageBuffer h(m);
	h.SetHotSpot(s2 / 2);
	h.Set2ndSpot(src.Get2ndSpot() / 2);
	return Image(h);
}

static bool sUHDMode;

void SetUHDMode(bool b)
{
	Iml::ResetAll();
	sUHDMode = b;
}

bool IsUHDMode()
{
	return sUHDMode;
}

void SyncUHDMode()
{
	bool uhd = GetStdFontCy() > 24;
	if(uhd != IsUHDMode())
		SetUHDMode(uhd);
}

Image DPI(const Image& img, int expected)
{
	if(img.GetSize().cy <= expected && IsUHDMode())
		return MakeImage(img, Upscale2x);
	return img;
}

};