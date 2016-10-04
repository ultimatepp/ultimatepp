#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS ImageImg
#define IMAGEFILE <GenShard/image.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS ImageImg
#define IMAGEFILE <GenShard/image.iml>
#include <Draw/iml_source.h>

struct Shard : Moveable<Shard> {
	Pointf p[3];
	RGBA   c[2];
	
	void Paint(Painter& w, Size sz) const {
		w.Move(sz * p[0]).Line(sz * p[1]).Line(sz * p[2])
		 .Fill(sz * p[0], c[0], sz * (p[1] + p[2]) / 2, c[1]);
	}
};

Vector<Shard> Generate()
{
	Vector<Shard> si;
	for(int i = 0; i < 100; i++) {
		Shard& s = si.Add();
		for(int i = 0; i < 3; i++)
			s.p[i] = Pointf(Randomf(), Randomf());
		for(int i = 0; i < 2; i++)
			s.c[i] = (int)Random(256) * Color(Random(256), Random(256), Random(256));
	}
	return si;
}

Image MakeImage(const Vector<Shard>& si, Size sz)
{
	ImagePainter p(sz);
	p.Clear(White());
	for(int i = 0; i < si.GetCount(); i++)
		si[i].Paint(p, sz);
	return p;
}

double sq(int a, int b)
{
	double x = a - b;
	return x * x;
}

double Distance(const Image& a, const Image& b)
{
	int n = a.GetLength();
	if(n != b.GetLength())
		return 1e300;
	const RGBA *sa = a;
	const RGBA *ea = sa + n;
	const RGBA *sb = b;
	int d = 0;
	while(sa < ea) {
		d += sq(sa->r, sb->r) + sq(sa->g, sb->g) + sq(sa->g, sb->g);
		sa++;
		sb++;
	}
	return d;
}

struct MyApp : TopWindow {
	Image image;
	Vector< Vector<Shard> > gen;
	Vector<Image> img;
	
	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		Size isz = image.GetSize();
		w.DrawImage(0, 0, image);
		for(int j = 0; j < img.GetCount(); j++) 
			w.DrawImage(isz.cx * j, isz.cy, img[j]);
		w.DrawImage(0, 2 * isz.cy, MakeImage(gen[0], 8 * isz));
	}

	void Render()
	{
		Size isz = image.GetSize();
		img.SetCount(gen.GetCount());
		Vector<double> distance;
		{ RTIMING("MakeImg");
		distance.SetCount(gen.GetCount());
		for(int i = 0; i < gen.GetCount(); i++) {
			img[i] = MakeImage(gen[i], isz);
			distance[i] = Distance(image, img[i]);
		}
		}
		{ RTIMING("Sort");
			IndexSort2(distance, img, gen);
		}
	}
	
	void Mix()
	{
		Vector< Vector<Shard> > ngen;
		for(int i = 3 * gen.GetCount() / 4; i < gen.GetCount(); i++)
			gen[i] = Generate();
		int q = gen.GetCount();
		q *= q;
		for(int i = 0; i < gen.GetCount(); i++) {
			const Vector<Shard>& a = gen[Random(gen.GetCount())];
			const Vector<Shard>& b = gen[Random(20)];
			Vector<Shard>& c = ngen.Add();
			int ai = 0;
			int bi = 0;
			while((ai < a.GetCount() || bi < b.GetCount()) && c.GetCount() < 100) {
				switch(Random(4)) {
				case 0: if(ai < a.GetCount()) ai++; break;
				case 1: if(bi < b.GetCount()) bi++; break;
				case 2: if(ai < a.GetCount()) c.Add() = a[ai++]; break;
				case 3: if(bi < b.GetCount()) c.Add() = b[bi++]; break;
				}
			}
		}
		gen = ngen;
		Render();
		Refresh();
	}
	
	void Do()
	{
		Open();
		while(IsOpen()) {
			ProcessEvents();
			Mix();
			GuiSleep(0);
			Sync();
		}
	}

	MyApp() {
		Sizeable().Zoomable();
		image = ImageImg::test();
		for(int i = 0; i < 500; i++)
			gen.Add() = Generate();
		Render();
	}
};

GUI_APP_MAIN
{
	MyApp().Do();
}
