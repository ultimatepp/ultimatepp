#include "ImageDisplayCtrl.h"

void ImageDisplayCtrl::Paint(Draw& w) {
	RLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": ImageDisplayCtrl::Paint");
	if (image.IsVoid() || image.IsEmpty()) {
		Size win = GetSize();
		w.DrawRect(win,SColorPaper());
	} else {
		Size win = GetSize();
		Size im  = image.GetSize();
		Size fsz = image.FitSize(win);
		Rect imagerect = Rect(win).CenterRect(fsz);
		Rect top(GetSize());
		Rect bottom(GetSize());
		Rect left(GetSize());
		Rect right(GetSize());
		top.bottom = imagerect.top;
		bottom.top = imagerect.bottom;
		left.right = imagerect.left;
		right.left = imagerect.right;
		w.DrawRect(top,SColorPaper());
		w.DrawRect(left,SColorPaper());
		w.DrawRect(right,SColorPaper());
		w.DrawRect(bottom,SColorPaper());
		RLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": DrawImage, " << image.GetSize() << " -> " << imagerect.Size());
		w.DrawImage(imagerect, image);
		RLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //DrawImage");
	}
	RLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //ImageDisplayCtrl::Paint");
}
