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

void ImageFillFlags(Painter& sw)
{
	sw.Begin();
	for(int i = 0; i < 5; i++) {
		sw.DrawText(10, 20, get_i(i, "0", "FILL_PAD", "FILL_REPEAT", "FILL_REFLECT", "FILL_HPAD|FILL_VREFLECT"), Arial(28));
		sw.Rectangle(10, 60, 290, 200)
		  .Fill(TestImg::test(), 90, 90, 200, 90, get_i(i, 0, FILL_PAD, FILL_REPEAT, FILL_REFLECT, FILL_HPAD|FILL_VREFLECT))
		  .Stroke(1, Black());
		sw.Translate(300, 0);
	}
	sw.End();
	sw.Translate(0, 400);
	for(int i = 0; i < 2; i++) {
		sw.DrawText(10, 20, i ? "FILL_FAST" : "0"), Arial(28);
		sw.Rectangle(10, 60, 500, 500)
		  .Fill(TestImg::test(), 10, 60, 500, 60, i ? FILL_FAST : 0);
		sw.Translate(600, 0);
	}
}

INITBLOCK {
	/*
	RegisterExample("Image fill exact", ImageExact);
	RegisterExample("Image fill exact fast", ImageExactFast);
	RegisterExample("Image fill reflect", ImageReflect);
	RegisterExample("Image fill pad", ImagePad);
	RegisterExample("Image fill pad fast", ImagePadFast);
	RegisterExample("Image fill repeat", ImageRepeat);
	RegisterExample("Image fill", ImageRepeat);
	RegisterExample("Image vpad&hreflect", ImageVPadHReflect);
	*/
	RegisterExample("Image fill flags", ImageFillFlags);
}
