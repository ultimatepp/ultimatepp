#ifndef _RasterMultiPage_RasterMultiPage_h
#define _RasterMultiPage_RasterMultiPage_h

#include <CtrlLib/CtrlLib.h>
#include <plugin/gif/gif.h>

using namespace Upp;

struct ImageControl : public Ctrl {
	Rect Fit(const Size &frame, const Size &object) {
		double objectAspect = object.cx/(double)object.cy;		
		if (frame.cx/(double)frame.cy > objectAspect) {
			double x = (frame.cx - objectAspect*frame.cy)/2.;
			return Rect(int(x), 0, int(x + objectAspect*frame.cy), frame.cy);
		} else {
			double y = (frame.cy - frame.cx/objectAspect)/2.;
			return Rect(0, int(y), frame.cx, int(y + frame.cx/objectAspect));
		}
	}
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		if (img) 
			w.DrawImage(Fit(sz, img.GetSize()), img);
	}
	ImageControl& SetImage(const Image& _img) {img = _img; Refresh(); return *this;}
	ImageControl() {Transparent(); NoWantFocus();}
	
	Image img; 
};

#define LAYOUTFILE <RasterMultiPage/RasterMultiPage.lay>
#include <CtrlCore/lay.h>

class RasterMultiPage : public WithRasterMultiPageLayout<TopWindow> {
public:
	typedef RasterMultiPage CLASSNAME;
	RasterMultiPage();
	
	void Browse();
	void Play();
	void OpenNext();

	Array<Image> images;
	Array<int> delays;
	int ind;
};

#endif

