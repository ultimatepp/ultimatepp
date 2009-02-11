#include "Examples.h"

void ImageExact(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100)
	  .Stroke(2, Black());
}

void ImageExactFast(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_FAST)
	  .Stroke(2, Black());
/*
	int cx = 400;
	for(int l = 0; l < 5; l++) {
		sw.Translate(cx, 0);
		cx /= 2;
		sw.Rectangle(10, 10, 1000, 600)
		  .Fill(MakeMipMap(TestImg::test(), l), 100, 100, 100 + cx, 100, FILL_FAST);
	}*/
}

void ImageReflect(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
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

void ImagePadFast(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(TestImg::test(), 100, 100, 500, 100, FILL_PAD|FILL_FAST)
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
	RegisterExample("Image fill exact fast", ImageExactFast);
	RegisterExample("Image fill reflect", ImageReflect);
	RegisterExample("Image fill pad", ImagePad);
	RegisterExample("Image fill pad fast", ImagePadFast);
	RegisterExample("Image fill repeat", ImageRepeat);
	RegisterExample("Image vpad&hreflect", ImageVPadHReflect);
}
