#include "TestLeptonica.h"

#include <plugin/tif/tif.h>

void TestLeptonica::onOpen()
{
	String fileName;
	FileSelector fs;
	PIX *pix;
	
/*
	FileIn s;
	if(!s.Open("/home/massimo/tmp/TestLept2.tif"))
	{
		PromptOK("Error creating first image file");
		return;
	}
	One<StreamRaster>streamRaster = StreamRaster::OpenAny(s);

	Raster::Info info = streamRaster->GetInfo();
	pixRaster.Load(*streamRaster);
*/
	int bpp = 32;
	int wh = 1024;
	pix = pixCreate(wh, wh, bpp);
	memset(pixGetData(pix), 0x0, pixGetHeight(pix)*pixGetWpl(pix)*4);

/*
	PIXCMAP *colorMap = pixcmapCreateLinear(bpp, 1 << bpp);
	pixSetColormap(pix, colorMap);
*/
/*
	PIXCMAP *colorMap = pixcmapCreate(bpp);
	pixcmapAddColor(colorMap, 0x00, 0x00, 0xff);
	pixcmapAddColor(colorMap, 0xff, 0x00, 0x00);
*/
	
	pixRaster.AddPIX(pix);

	if(bpp < 16)
	{
		int thick = wh / (1 << bpp);
		l_uint32 nColors = 1 << bpp;
		l_uint32 iColor = 0;
		int iThick = 0;
		for(int i = 0; i < wh; i++)
		{
			for(int j = 0; j < wh; j++)
			{
				pixSetPixel(pix, i, j, iColor);
				pixSetPixel(pix, j, i, iColor);
			}
			iThick++;
			if(iThick >= thick)
			{
				iThick = 0;
				iColor++;
			}
		}
	} // if bpp < 16
	else
	{
		l_uint32 r = 0, g = 0, b = 0;
		for(int y = 0; y < wh; y++)
		{
			for(int x = 0; x < wh; x++)
			{
				r = x * 256 / wh;
				g = y * 256 / wh;
				b = sqrt((x * 256 / wh) * (y * 256 / wh));
				pixSetPixel(pix, x, y, (r << 24) | (g << 16) | (b << 8));
			}
		}
		r = b;
	}
	pixDestroy(&pix);
	                                 
	int width = pixRaster.GetWidth();
	pix = pixRaster;
	pixWrite("/home/massimo/tmp/TestLeptOut.tif", pix, IFF_TIFF);
	FileOut of;
	if(!of.Open("/home/massimo/tmp/TestLeptOut2.tif"))
	{
		PromptOK("Error creating second image file");
		return;
	}
	TIFEncoder enc(pixRaster.GetInfo().bpp);
	enc.SetStream(of);
	enc.Create(pixRaster.GetSize(), pixRaster);
	enc.SetDots(Size(1024,1024));
	const RGBA *pal = enc.GetPalette();
	enc.Save(of, pixRaster);
	int pc = enc.GetPaletteCount();
	
	return;


/*
	fs.ReadOnlyOption();
	if(fs.ExecuteOpen("Please select a graphic file to view:"))
	{
		FileIn s;
		if(!s.Open(~fs))
		{
			PromptOK("Error opening image");
			return;
		}
		One<StreamRaster>streamRaster = StreamRaster::OpenAny(s);
		pixRaster.Load(*streamRaster);
		PIX *pix = pixRaster;
		pixWrite("/home/massimo/out.tif", pix, IFF_UNKNOWN);
	}
*/
}

void TestLeptonica::onQuit()
{
	Break();
}

TestLeptonica::TestLeptonica()
{
	CtrlLayout(*this, "Window title");
	
	// connects button handlers
	OpenButton <<= THISBACK(onOpen);
	QuitButton <<= THISBACK(onQuit);
}

GUI_APP_MAIN
{
	TestLeptonica testLept;

	testLept.Run();
}

