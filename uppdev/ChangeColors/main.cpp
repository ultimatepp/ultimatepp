#include <CtrlLib/CtrlLib.h>

void Interpolate(int *x)
{
	for(int i = 0; i < 256; i++)
		DUMP(x[i]);
	LOG("----------");
	for(int i = 0; i < 256; i++)
		if(x[i] < 0) {
			int j = i;
			while(x[j] < 0)
				j++;
			int l = x[i - 1];
			int h = x[j];
			int n = j - i;
			for(int q = 0; q < n; q++)
				x[q + i] = l + (h - l) * q / n;
			i = j;
		}
	for(int i = 0; i < 256; i++)
		DUMP(x[i]);
	LOG("----------");
}

Image ChangeColors(const Image& img, const Vector<Color>& from, const Vector<Color>& to)
{
	int rc[256];
	int gc[256];
	int bc[256];
	for(int i = 1; i < 255; i++)
		rc[i] = gc[i] = bc[i] = -1;
	rc[0] = gc[0] = bc[0] = 0;
	rc[255] = gc[255] = bc[255] = 255;
	for(int i = 0; i < from.GetCount(); i++) {
		Color s = from[i];
		Color t = to[i];
		rc[s.GetR()] = t.GetR();
		gc[s.GetG()] = t.GetG();
		bc[s.GetB()] = t.GetB();
	}
	Interpolate(rc);
	Interpolate(gc);
	Interpolate(bc);

	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		t->r = rc[s->r];
		t->g = gc[s->g];
		t->b = bc[s->b];
		t->a = s->a;
		t++;
		s++;
	}
	return w;
}

struct MyApp : TopWindow {
	FrameRight<ArrayCtrl> list;
	Image picture;

	virtual void Paint(Draw& w) {
		w.DrawImage(GetSize(), picture);
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.picture = StreamRaster::LoadFileAny("d:/ImgTest/screenshot.bmp");
	Vector<Color> from, to;
//	from.Add(Green); to.Add(Black);
	from.Add(Red); to.Add(Yellow);
	from.Add(Blue); to.Add(Blue);

	app.picture = ChangeColors(app.picture, from, to);
	app.Run();
}
