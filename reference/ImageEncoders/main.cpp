#include <CtrlLib/CtrlLib.h>

#include <plugin/png/png.h>
#include <plugin/bmp/bmp.h>
#include <plugin/gif/gif.h>
#include <plugin/jpg/jpg.h>
#include <plugin/tif/tif.h>

using namespace Upp;

GUI_APP_MAIN
{
	ImageDraw iw(200, 50);
	iw.DrawRect(0, 0, 200, 50, White);
	iw.DrawText(10, 10, "ImageEncoder TEST", Arial(18), Red);
	DrawFatFrame(iw, 0, 0, 200, 50, Blue, 2);
	Image m = iw;

	PNGEncoder png;
	png.SaveFile(GetExeDirFile("default-32.png"), m);
	png.Bpp(1);
	png.SaveFile(GetExeDirFile("1.png"), m);
	png.Bpp(2);
	png.SaveFile(GetExeDirFile("2.png"), m);
	png.Bpp(4);
	png.SaveFile(GetExeDirFile("4.png"), m);
	png.Bpp(8);
	png.SaveFile(GetExeDirFile("8.png"), m);
	png.Interlace();
	png.SaveFile(GetExeDirFile("8i.png"), m);
	png.Bpp(24);
	png.SaveFile(GetExeDirFile("24.png"), m);
	png.Bpp(32);
	png.SaveFile(GetExeDirFile("32.png"), m);

	BMPEncoder bmp;
	bmp.SaveFile(GetExeDirFile("default-24.bmp"), m);
	bmp.Bpp(1);
	bmp.SaveFile(GetExeDirFile("1.bmp"), m);
	bmp.Bpp(4);
	bmp.SaveFile(GetExeDirFile("4.bmp"), m);
	bmp.Bpp(8);
	bmp.SaveFile(GetExeDirFile("8.bmp"), m);
	bmp.Bpp(4).Mono();
	bmp.SaveFile(GetExeDirFile("4m.bmp"), m);
	bmp.Bpp(8).Mono();
	bmp.SaveFile(GetExeDirFile("8m.bmp"), m);
	bmp.Bpp(24);
	bmp.SaveFile(GetExeDirFile("24.bmp"), m);
	bmp.Bpp(32);
	bmp.SaveFile(GetExeDirFile("32.bmp"), m);

	GIFEncoder gif;
	gif.SaveFile(GetExeDirFile("gif.gif"), m);

	JPGEncoder jpg;
	jpg.SaveFile(GetExeDirFile("default(50).jpg"), m);
	jpg.Quality(20);
	jpg.SaveFile(GetExeDirFile("(20).jpg"), m);
	jpg.Quality(100);
	jpg.SaveFile(GetExeDirFile("(100).jpg"), m);


	TIFEncoder tif;
	tif.SaveFile(GetExeDirFile("default-24.tif"), m);
	tif.Bpp(1);
	tif.SaveFile(GetExeDirFile("1.tif"), m);
	tif.Bpp(2);
	tif.SaveFile(GetExeDirFile("2.tif"), m);
	tif.Bpp(4);
	tif.SaveFile(GetExeDirFile("4.tif"), m);
	tif.Bpp(8);
	tif.SaveFile(GetExeDirFile("8.tif"), m);
	tif.Bpp(24);
	tif.SaveFile(GetExeDirFile("24.tif"), m);
	tif.Bpp(32);
	tif.SaveFile(GetExeDirFile("32.tif"), m);
}
