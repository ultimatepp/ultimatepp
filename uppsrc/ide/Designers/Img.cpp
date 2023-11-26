#include "Designers.h"

void IdeImgView::Paint(Draw& w)
{
	Size sz = GetSize();
	String t = (img_sz != img.GetSize() ? "Resized from: " : "Image size: ");
	t << Format("%d x %d", img_sz.cx, img_sz.cy);
	int tcy = Draw::GetStdFontCy();
	w.DrawRect(0, 0, sz.cx, tcy, SColorFace());
	w.DrawText(5, 0, t, StdFont(), SColorText());
	int ii = 0;
	for(int x = 0; x < sz.cx; x += 16) {
		int jj = ii;
		for(int y = tcy; y < sz.cy; y += 16)
			w.DrawRect(x, y, 16, 16, jj++ & 1 ? LtGray() : WhiteGray());
		ii++;
	}
	w.DrawImage(5, 5 + tcy, img);
}

void IdeImgView::EditMenu(Bar& menu)
{
	
}

bool IsImgFile(const String& path)
{
	String s = ToLower(GetFileExt(path));
	return s == ".png" || s == ".jpg" || s == ".gif" || s == ".bmp";
}

struct ImageViewModule : public IdeModule {
	virtual String       GetID() { return "ImageViewModule"; }
	virtual Image FileIcon(const char *path) {
		return IsImgFile(path) ? IdeCommonImg::image() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsImgFile(path)) {
			FileIn in(path);
			One<StreamRaster> o = StreamRaster::OpenAny(in);
			if(o) {
				Size sz = o->GetSize();
				if(ToLower(GetFileExt(path)) == ".png" && sz.cx <= 768 && sz.cy <= 512) {
					IdePngDes *d = new IdePngDes;
					d->Load(path);
					d->RestoreEditPos();
					return d;
				}
				else {
					IdeImgView *d = new IdeImgView;
					d->img_sz = sz;
					d->filename = path;
					if(sz.cx <= 1024 && sz.cy <= 768)
						d->img = o->GetImage();
					else {
						ImageEncoder m;
						Rescale(m, GetFitSize(sz, Size(1024, 768)), *o, sz);
						d->img = m;
					}
					return d;
				}
			}
			else
			if(ToLower(GetFileExt(path)) == ".png" && GetFileLength(path) <= 0) {
				IdePngDes *d = new IdePngDes;
				d->Create(path);
				d->RestoreEditPos();
				return d;
			}
		}
		return NULL;
	}
};

INITIALIZER(Img)
{
	RegisterIdeModule(Single<ImageViewModule>());
}
