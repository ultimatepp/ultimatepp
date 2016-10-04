#ifndef _ImageDisplayCtrl_ImageDisplayCtrl_h
#define _ImageDisplayCtrl_ImageDisplayCtrl_h

#include <CtrlLib/Ctrllib.h>

class ImageDisplayCtrl : public Ctrl {
public:
	virtual void   Paint(Draw& w);
public:
	ImageDisplayCtrl& SetImage(const Image& img) { image=img; Refresh(); return *this; }
	const Image& GetImage() const { return image; }
	Image& GetImage() { return image; }
protected:
	Image image;
};

#endif
