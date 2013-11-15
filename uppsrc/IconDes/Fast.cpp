#include "IconDes.h"

NAMESPACE_UPP

void IconDes::MaskSelection()
{
	ImageBuffer ib(Current().image);
	RGBA *t = ~ib;
	const RGBA *e = t + ib.GetLength();
	const RGBA *s = Current().base_image;
	const RGBA *k = ~Current().selection;
	while(t < e) {
		if(!k->r)
			*t = *s;
		t++;
		s++;
		k++;
	}
	Current().image = ib;
	Refresh();
	SyncShow();
}

void IconDes::ApplyDraw(IconDraw& iw, dword flags)
{
	ApplyImage(iw, flags);
}

void IconDes::ApplyImage(Image m, dword flags, bool alpha)
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	Size isz = GetImageSize();
	RGBA cc = CurrentColor();
	int empty = doselection ? cc.r : 0;
	ImageBuffer ib(CurrentImage());
	for(int y = 0; y < isz.cy; y++) {
		const RGBA *s = c.base_image[y];
		RGBA *t = ib[y];
		const RGBA *d = m[y];
		const RGBA *k = doselection ? d : c.selection[y];
		for(int x = 0; x < isz.cx; x++) {
			RGBA c = *s;
			if(alpha) {
				cc.a = d->r;
				AlphaBlendStraight(&c, &cc, 1);
			}
			else {
				if(d->r == 255) {
					if(flags & K_ALT)
						c.a = cc.a;
					else
					if(flags & K_CTRL) {
						RGBA h = cc;
						h.a = c.a;
						c = h;
					}
					else
						c = cc;
				}
				if(d->r == 128)
					c.a = c.r = c.g = c.b = empty;
			}
			if(c != *t && (doselection || k->r)) {
				*t = c;
				RefreshPixel(x, y);
			}
			t++;
			s++;
			d++;
			k++;
		}
	}
	CurrentImage() = ib;
}

void IconDes::Set(Point p, RGBA rgba, dword flags)
{
	if(!IsCurrent())
		return;
	if(InImage(p)) {
		ImageBuffer ib(CurrentImage());
		RGBA& t = ib[p.y][p.x];
		if(t != rgba && (doselection || Current().selection[p.y][p.x].r)) {
			if(flags & K_ALT)
				t.a = rgba.a;
			else
			if(flags & K_CTRL) {
				rgba.a = t.a;
				t = rgba;
			}
			else
				t = rgba;
			RefreshPixel(p);
		}
		CurrentImage() = ib;
	}
}

END_UPP_NAMESPACE
