#include "IconDes.h"

void IconDes::Interpolate()
{
	if(!IsCurrent())
		return;
	FinishPaste();
	SaveUndo();
	Slot& c = Current();
	c.base_image = c.image;
	::InterpolateImage(c.image, c.image.GetSize());
	MaskSelection();
}

bool IconDes::BeginTransform()
{
	SaveUndo();
	Refresh();
	SyncShow();
	if(!IsPasting()) {
		if(SelectionRect() == GetImageSize())
			return false;
		Move();
	}
	return true;
}

void IconDes::KeyMove(int dx, int dy)
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		c.pastepos.x += dx;
		c.pastepos.y += dy;
		MakePaste();
	}
	else {
		Image h = c.image;
		c.image = CreateImage(h.GetSize(), Null);
		UPP::Copy(c.image, Point(dx, dy), h, h.GetSize(), true);
	}
	Sync();
}

void IconDes::MirrorX()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		MirrorHorz(c.paste_image, c.paste_image.GetSize());
		MakePaste();
	}
	else
		MirrorHorz(c.image, c.image.GetSize());
	SyncShow();
}

void IconDes::SymmX()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		if(c.paste_image.GetLength() > 1024 * 1024)
			return;
		Image m = c.paste_image;
		Size sz = m.GetSize();
		MirrorHorz(m, m.GetSize());
		Image h = CreateImage(Size(2 * sz.cx, sz.cy), Null);
		UPP::Copy(h, Point(0, 0), c.paste_image, sz);
		UPP::Copy(h, Point(sz.cx, 0), m, sz);
		c.paste_image = h;
		MakePaste();
	}
	else {
		Size sz = c.image.GetSize();
		if(sz.cx < 2)
			return;
		UPP::Copy(c.image, Point(sz.cx - sz.cx / 2, 0), c.image, Size(sz.cx / 2, sz.cy));
		MirrorHorz(c.image, RectC(sz.cx - sz.cx / 2, 0, sz.cx / 2, sz.cy));
	}
	SyncShow();
}

void IconDes::MirrorY()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		MirrorVert(c.paste_image, c.paste_image.GetSize());
		MakePaste();
	}
	else
		MirrorVert(c.image, c.image.GetSize());
	SyncShow();
}

void IconDes::SymmY()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		if(c.paste_image.GetLength() > 1024 * 1024)
			return;
		Image m = c.paste_image;
		Size sz = m.GetSize();
		MirrorVert(m, m.GetSize());
		Image h = CreateImage(Size(sz.cx, 2 * sz.cy), Null);
		UPP::Copy(h, Point(0, 0), c.paste_image, sz);
		UPP::Copy(h, Point(0, sz.cy), m, sz);
		c.paste_image = h;
		MakePaste();
	}
	else {
		Size sz = c.image.GetSize();
		if(sz.cy < 2)
			return;
		UPP::Copy(c.image, Point(0, sz.cy - sz.cy / 2), c.image, Size(sz.cx, sz.cy / 2));
		MirrorVert(c.image, RectC(0, sz.cy - sz.cy / 2, sz.cx, sz.cy / 2));
	}
	SyncShow();
}

void IconDes::MirrorD(bool symm)
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		MirrorDiag(c.paste_image, c.paste_image.GetSize(), symm);
		MakePaste();
	}
	else
		MirrorDiag(c.image, c.image.GetSize(), symm);
	SyncShow();
}

void IconDes::Rotate()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(BeginTransform()) {
		c.paste_image = RotateClockwise(c.paste_image);
		MakePaste();
	}
	else {
		c.image = RotateClockwise(c.image);
		SetSelect(255);
	}
	SyncShow();
}

void IconDes::SmoothRescale()
{
	if(!IsCurrent())
		return;
	WithRescaleLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Rescale");
	dlg.cx <<= dlg.cy <<= dlg.method <<= dlg.Breaker();
	Slot& c = Current();
	BeginTransform();
	Image bk = IsPasting() ? c.paste_image : c.image;
	dlg.cx <<= bk.GetWidth();
	dlg.cy <<= bk.GetHeight();
	dlg.keep <<= true;
	dlg.method.Add(Null, "Rylek");
	dlg.method.Add(FILTER_NEAREST, "Nearest");
	dlg.method.Add(FILTER_BILINEAR, "Bilinear");
	dlg.method.Add(FILTER_BSPLINE, "Bspline");
	dlg.method.Add(FILTER_COSTELLO, "Costello");
	dlg.method.Add(FILTER_BICUBIC_MITCHELL, "Bicubic Mitchell");
	dlg.method.Add(FILTER_BICUBIC_CATMULLROM, "Bicubic Catmull Rom");
	dlg.method.Add(FILTER_LANCZOS2, "Lanczos 2");
	dlg.method.Add(FILTER_LANCZOS3, "Lanczos 3");
	dlg.method.Add(FILTER_LANCZOS4, "Lanczos 4");
//	dlg.method.Add(FILTER_LANCZOS5, "Lanczos 5");
	for(;;) {
		Size sz(minmax((int)~dlg.cx, 1, 9999), minmax((int)~dlg.cy, 1, 9999));
		Image m = RescaleFilter(bk, sz, ~dlg.method);
		if(IsPasting()) {
			c.paste_image = m;
			MakePaste();
		}
		else {
			c.image = m;
			Refresh();
		}
		SyncImage();
		SyncShow();
		switch(dlg.Run()) {
		case IDCANCEL:
			if(IsPasting()) {
				c.paste_image = bk;
				MakePaste();
			}
			else {
				c.image = bk;
				Refresh();
			}
			SyncImage();
			SyncShow();
			return;
		case IDOK:
			SyncList();
			return;
		}
		if(dlg.keep) {
			if(dlg.cx.HasFocus() && bk.GetWidth() > 0)
				dlg.cy <<= (int)~dlg.cx * bk.GetHeight() / bk.GetWidth();
			if(dlg.cy.HasFocus() && bk.GetHeight() > 0)
				dlg.cx <<= (int)~dlg.cy * bk.GetWidth() / bk.GetHeight();
		}
	}
}

Image IconDes::ImageStart()
{
	if(!IsCurrent())
		return CreateImage(Size(1, 1), Black);
	SaveUndo();
	Refresh();
	SyncShow();
	Slot& c = Current();
	if(!IsPasting())
		c.base_image = c.image;
	return IsPasting() ? c.paste_image : c.image;
}

void IconDes::ImageSet(const Image& m)
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	if(IsPasting()) {
		c.paste_image = m;
		MakePaste();
	}
	else {
		c.image = m;
		MaskSelection();
	}
	Refresh();
	SyncShow();
}

void IconDes::BlurSharpen()
{
	WithSharpenLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Blur/Sharpen");
	PlaceDlg(dlg);

	Couple(dlg, dlg.level, dlg.slider2, 1);
	Couple(dlg, dlg.radius, dlg.slider, 10, 1);
	
	Image bk = ImageStart();
	for(;;) {
		Image m = bk;
		for(int q = 0; q < (int)~dlg.radius; q++)
			m = Sharpen(m, -int(256 * (double)~dlg.level));
		ImageSet(m);
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Colorize()
{
	WithImageDblLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Chroma");
	PlaceDlg(dlg);
	Couple(dlg, dlg.level, dlg.slider, 1, 1);
	Image bk = ImageStart();
	for(;;) {
		RGBA c = rgbactrl.GetColor();
		double mg = 0;
		ForEachPixelStraight(bk, [&](RGBA& t) {
			mg = max(mg, (double)Grayscale(t));
		},
		false);
		if(mg)
			mg = 1 / mg;
		double a = Nvl(dlg.level, 1.0);
		double ca = 1 - a;
		ImageSet(ForEachPixelStraight(bk, [&](RGBA& t) {
			double x = Grayscale(t) * mg;
			t.r = Saturate255(int(a * (x * c.r + 0.5) + ca * t.r));
			t.g = Saturate255(int(a * (x * c.g + 0.5) + ca * t.g));
			t.b = Saturate255(int(a * (x * c.b + 0.5) + ca * t.b));
		}));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::FreeRotate()
{
	WithFreeRotateLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Rotate");
	PlaceDlg(dlg);
	Couple(dlg, dlg.angle, dlg.slider, 360);
	Image bk = ImageStart();
	Size tsz = bk.GetSize();
	Image src = Magnify(bk, 3, 3);
	for(;;) {
		Image h = DownSample3x(UPP::Rotate(src, (int)~dlg.angle * 10));
		Size sz = h.GetSize();
		ImageSet(Crop(h, (sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, tsz.cx, tsz.cy));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Chroma()
{
	WithImageDblLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Chroma");
	PlaceDlg(dlg);
	Couple(dlg, dlg.level, dlg.slider, 2, 0);
	Image bk = ImageStart();
	for(;;) {
		ImageSet(UPP::Grayscale(bk, 256 - (int)(minmax((double)~dlg.level, 0.0, 4.0) * 255)));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Couple(TopWindow& dlg, EditDouble& level, SliderCtrl& slider, double max, double init, double offset)
{
	level.Max(max);
	level <<= init;
	slider.MinMax(0, 1000);
	slider <<= init * 1000 / max;
	slider << [=, &dlg, &level, &slider] { level <<= (int)~slider / 1000.0 * max + offset; dlg.Break(); };
	level << [=, &dlg, &level, &slider] { slider <<= Nvl(int(((double)~level - offset) * 1000 / max), 500); dlg.Break(); };
}

void IconDes::Couple(TopWindow& dlg, EditInt& level, SliderCtrl& slider, int max, int init)
{
	level.Max(max);
	level <<= init;
	slider.MinMax(0, max);
	slider <<= init;
	slider << [=, &dlg, &level, &slider] { level <<= (int)~slider; dlg.Break(); };
	level << [=, &dlg, &level, &slider] { slider <<= Nvl(int((double)~level), init); dlg.Break(); };
}

void IconDes::Contrast()
{
	WithImageDblLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Contrast");
	PlaceDlg(dlg);
	Couple(dlg, dlg.level, dlg.slider, 2, 1);
	Image bk = ImageStart();
	for(;;) {
		ImageSet(UPP::Contrast(bk, (int)(minmax((double)~dlg.level, 0.0, 4.0) * 255)));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Alpha()
{
	WithImageDblLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Alpha");
	PlaceDlg(dlg);
	Couple(dlg, dlg.level, dlg.slider, 2, 1);
	Image bk = ImageStart();
	for(;;) {
		int a = (int)(minmax((double)~dlg.level, 0.0, 2.0) * 256);
		ImageSet(ForEachPixelStraight(bk, [&](RGBA& t) {
			t.a = Saturate255((t.a * a) >> 8);
		}));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::RemoveAlpha()
{
	WithRemoveAlphaLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Remove Alpha");
	PlaceDlg(dlg);
	Couple(dlg, dlg.thres, dlg.slider, 255, 128);
	Image bk = ImageStart();
	for(;;) {
		int thres = ~dlg.thres;
		ImageSet(ForEachPixel(bk, [&](RGBA& t) {
			if(t.a != 255) {
				if(t.a < thres)
					t = RGBAZero();
				else {
					t = Unmultiply(t);
					t.a = 255;
					t = Premultiply(t);
				}
			}
		}));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}

void IconDes::Colors()
{
	WithImgColorLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Alpha");
	PlaceDlg(dlg);
	dlg.r_mul <<= dlg.g_mul <<= dlg.b_mul <<= dlg.a_mul <<= 1;
	dlg.r_add <<= dlg.g_add <<= dlg.b_add <<= dlg.a_add <<= 0;
	dlg.all = true;
	dlg.r_mul <<= dlg.g_mul <<= dlg.b_mul <<= dlg.a_mul <<=
	dlg.r_add <<= dlg.g_add <<= dlg.b_add <<= dlg.a_add <<=
	dlg.all <<= dlg.Breaker();
	Image bk = ImageStart();
	for(;;) {
		bool all = dlg.all;
		dlg.g_mul.Enable(!all);
		dlg.g_add.Enable(!all);
		dlg.b_mul.Enable(!all);
		dlg.b_add.Enable(!all);
		if(all) {
			dlg.g_mul <<= dlg.b_mul <<= ~dlg.r_mul;
			dlg.g_add <<= dlg.b_add <<= ~dlg.r_add;
		}
		ImageBuffer ib(bk.GetSize());
		RGBA *t = ib;
		const RGBA *s = bk;
		const RGBA *e = bk + bk.GetLength();
		int r_mul = int(256 * (double)~dlg.r_mul);
		int r_add = int(256 * (double)~dlg.r_add);
		int g_mul = int(256 * (double)~dlg.g_mul);
		int g_add = int(256 * (double)~dlg.g_add);
		int b_mul = int(256 * (double)~dlg.b_mul);
		int b_add = int(256 * (double)~dlg.b_add);
		int a_mul = int(256 * (double)~dlg.a_mul);
		int a_add = int(256 * (double)~dlg.a_add);
		while(s < e) {
			Unmultiply(t, t, 1);
			t->r = Saturate255(((r_mul * s->r) >> 8) + r_add);
			t->g = Saturate255(((g_mul * s->g) >> 8) + g_add);
			t->b = Saturate255(((b_mul * s->b) >> 8) + b_add);
			t->a = Saturate255(((a_mul * s->a) >> 8) + a_add);
			Premultiply(t, t, 1);
			s++;
			t++;
		}
		ImageSet(ib);
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}
