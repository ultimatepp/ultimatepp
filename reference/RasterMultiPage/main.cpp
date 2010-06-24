#include "RasterMultiPage.h"

Image GetRect(const Image& orig, const Rect &r) 
{
	if(r.IsEmpty())
		return Image();
	ImageBuffer ib(r.GetSize());
	for(int y = r.top; y < r.bottom; y++) {
		const RGBA *s = orig[y] + r.left;
		const RGBA *e = orig[y] + r.right;
		RGBA *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			t++;
			s++;
		}
	}
	return ib;
}

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
	Image previous;
	Rect r;
	for (int i = 0; i < raster->GetPageCount(); ++i) {
		if (previous) {
			iw.DrawImage(r, previous);
			previous = Null;
		}
		raster->SeekPage(i);
		r = raster->GetPageRect(i);
		switch (raster->GetPageDisposal(i)) {
		case 1:	iw.DrawRect(r, White());
				break;
		case 2: iw.DrawRect(sz, White());
				break;
		case 4:	if (i > 0) 
					previous = ::GetRect(images[i-1], r);
				iw.DrawRect(sz, White());
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

