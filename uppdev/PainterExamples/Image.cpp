#include "Examples.h"

#if 0

void ImageExact(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
//	  .Fill(TestImg::test(), 200, 100, 300, 300) _DBG_
	  .Fill(TestImg::test(), 100, 100, 500, 100)
	  .Stroke(2, Black());
}

void ImageReflect(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
//	  .Fill(TestImg::test(), 200, 100, 300, 300, GRADIENT_REFLECT) _DBG_
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_REFLECT)
	  .Stroke(2, Black());
}

void ImageVPadHReflect(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_HREFLECT|FILL_VPAD)
	  .Stroke(2, Black());
}

void ImagePad(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_PAD)
	  .Stroke(2, Black());
}

void ImageRepeat(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_REPEAT)
	  .Stroke(2, Black());
}

INITBLOCK {
	RegisterExample("Image fill exact", ImageExact);
	RegisterExample("Image fill reflect", ImageReflect);
	RegisterExample("Image fill pad", ImagePad);
	RegisterExample("Image fill repeat", ImageRepeat);
	RegisterExample("Image vpad&hreflect", ImageVPadHReflect);
}

#endif
