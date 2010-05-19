#ifndef STATIC_IMAGE_HPP
#define STATIC_IMAGE_HPP

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class StaticImage : public Ctrl
{
public:
	StaticImage(){}

	virtual inline void Paint(Draw& w);
	virtual inline void SetImage(const Image& image);

private:
	Image _image;
};

inline void StaticImage::Paint(Draw& w)
{
	w.DrawImage(0, 0, _image);
}

inline void StaticImage::SetImage(const Image& image)
{
	_image = image;
	Refresh();
}

#endif // STATIC_IMAGE_HPP
