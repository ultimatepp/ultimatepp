#ifndef _Geom_Draw_imgtran_h_
#define _Geom_Draw_imgtran_h_

namespace Upp {

void LinearSet(ImageBuffer& dest, Point d0, Point d1, Point d2, RGBA color, const Rect *opt_clip = NULL);
void LinearCopy(ImageBuffer& dest, Point d0, Point d1, Point d2,
	const Image& src, Point s0, Point s1, Point s2, const Rect *opt_clip = NULL, bool interpolate = true);

void BilinearSet(ImageBuffer& dest, Point d1, Point d2, Point d3, Point d4, RGBA color, const Rect *clip = NULL);
void BilinearSet(ImageBuffer& dest, Point destpos[4], RGBA color, const Rect *clip = NULL);

void BilinearCopy(ImageBuffer& dest, Point d1, Point d2, Point d3, Point d4,
	const Image& src, Point s1, Point s2, Point s3, Point s4, const Rect *clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Point destpos[4],
	const Image& src, Point srcpos[4], const Rect *opt_clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Point d1, Point d2, Point d3, Point d4,
	const Image& src, Rect sr, const Rect *opt_clip = NULL, bool interpolate = true);
void BilinearCopy(ImageBuffer& dest, Point destpos[4],
	const Image& src, Rect sr, const Rect *opt_clip = NULL, bool interpolate = true);

Image BilinearRotate(const Image& m, int angle);

}

#endif
