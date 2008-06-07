#include <CtrlLib/CtrlLib.h>
#include <plugin/jpg/jpg.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel fs;
	fs.Type("Image file", "*.bmp;*.png;*.tif;*.tiff;*.jpg;*.jpeg;*.gif");
	if(!fs.ExecuteOpen("Choose the image file to convert"))
		return;
	String fn = ~fs;
	JPGEncoder jpg(20);
	FileIn in(fn);
	One<StreamRaster> raster = StreamRaster::OpenAny(in);
	if(!raster) {
		Exclamation("Invalid input");
		return;
	}
	FileOut out(fn + ".out.png");
	jpg.SetStream(out);
	jpg.Create(raster->GetSize());
	for(int i = 0; i < raster->GetHeight(); i++) {
		RasterLine l = raster->GetLine(i);
		Buffer<RGBA> out(raster->GetWidth());
		for(int j = 0; j < raster->GetWidth(); j++) {
			out[j].g = out[j].b = out[j].r = l[j].g;
			out[j].a = 255;
		}
		jpg.WriteLine(out);
	}
}
