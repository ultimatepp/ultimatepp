#include "RasterMultiPage.h"

RasterMultiPage::RasterMultiPage()
{
	CtrlLayout(*this, "Multi page raster demo");
	
	browse   <<= THISBACK(Browse);
	play 	 <<= THISBACK(Play);
	openNext <<= THISBACK(OpenNext);

	Sizeable().Zoomable();
}

void RasterMultiPage::Browse()
{
	FileSel fs;
	
	fs.PreSelect(fileName);
	fs.Type("Animation type", "*.gif, *.tif, *.tiff");
	
	if (fs.ExecuteOpen("Choose animation file"))
		fileName <<= ~fs;

	FileIn in(fileName.GetData().ToString());
	One<StreamRaster> raster = StreamRaster::OpenAny(in);
	if(!raster) {
		Exclamation("Invalid input");
		return;
	}
	images.Clear();
	delays.Clear();
	for (int i = 0; i < raster->GetPageCount(); ++i) {
		raster->SeekPage(i);
		images.Add(raster->GetImage());
		int delay = raster->GetPageDelay(i);
		delays.Add(delay <= 100 ? 100 : delay);
	}
	numberPages = FormatInt(raster->GetPageCount());
	openedPage = "0";
	image.SetImage(images[0]);
	ind = 0;
}

void RasterMultiPage::Play()
{
	if (images.GetCount() <= 1)
		return;
	TimeStop t;
	for (unsigned ms = 0; t.Elapsed() < 4000; ++ind) {
		if (ind >= images.GetCount())
			ind = 0;	
		image.SetImage(images[ind]);	
		openedPage = FormatInt(ind);
		Refresh();
		Ctrl::ProcessEvents();

		ms += delays[ind];
		while (t.Elapsed() < ms)
			Sleep(10);
	}
}

void RasterMultiPage::OpenNext()
{
	if (images.GetCount() <= 1)
		return;
	ind++;
	if (ind >= images.GetCount())
		ind = 0;
	image.SetImage(images[ind]);	
	openedPage = FormatInt(ind);
}
	
GUI_APP_MAIN
{
	RasterMultiPage().Run();
}

