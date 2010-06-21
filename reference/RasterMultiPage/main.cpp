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
	Size sz = raster->GetSize();
	ImageDraw iw(sz); 
	for (int i = 0; i < raster->GetPageCount(); ++i) {
		raster->SeekPage(i);
		Rect r = raster->GetPageRect(i);
		switch (raster->GetPageDisposal(i)) {
		case 1:	iw.DrawRect(r, White());
				break;
		case 2: 
		case 4:	iw.DrawRect(sz, White());
				break;
		}
		iw.DrawImage(r.left, r.top, raster->GetImage(0, 0, r.right-r.left, r.bottom-r.top));
		images.Add(iw);
		int msdelay = 10*raster->GetPageDelay(i);
		delays.Add(msdelay <= 100 ? 100 : msdelay);
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
	for (unsigned ms = 0; t.Elapsed() < 5000; ++ind) {
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

