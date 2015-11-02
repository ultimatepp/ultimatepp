#include "PixRaster.h"

NAMESPACE_UPP

// used in PIX file I/O
enum {
    READ_24_BIT_COLOR = 0,     /* read in as 24 (really 32) bit pix */
    CONVERT_TO_PALETTE = 1,    /* convert to 8 bit colormapped pix */
    READ_GRAY = 2              /* read gray only */
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pix class wrapping Leptonica PIX image
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// internal destructor
void Pix::Destroy()
{
	// destroys underlying PIX
	if(pix)
		pixDestroy(&pix);
	
	// frees format and palette
	if(rasterFormat)
	{
		delete rasterFormat;
		rasterFormat = NULL;
	}
	if(localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}
	markers.Clear();

} // END Pix::Destroy()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sets raster format depending on current page's pix format
void Pix::SetRasterFormat()
{
	// removes old format for page
	if (rasterFormat)
	{
		delete rasterFormat;
		rasterFormat = NULL;
	}
	
	if(IsEmpty())
		return;

	rasterFormat = new RasterFormat;

	switch (pixGetDepth(pix))
	{

		case 1:
			rasterFormat->Set1mf();
			break;

		case 2:
			rasterFormat->Set2mf();
			break;

		case 4:
			rasterFormat->Set4mf();
			break;

		case 8:
			rasterFormat->Set8();
			break;

		case 16:
			// Upp does 16bpp color, Leptonica does 16bpp grayscale
			// MakeRGBA() will fail converting from Leptonica format anyways
			// we should add a 16bpp grayscale to Upp
			// in the meanwhile 16 bit Leptonica are converted to RGBA
			rasterFormat->Set32be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;

		case 24:
			rasterFormat->Set24be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;

		case 32:
			rasterFormat->Set32be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;
			
		default:
			NEVER();
			
	} // switch

} // END Pix::SetRasterFormat()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// synthesyze grayscale palettes
void Pix::CreateGrayPalette2()
{
	if (localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}

	if (IsEmpty())
		return;

	localPalette = new RGBA[2];

	*(dword *)localPalette       = 0xFFFFFFFF;
	*((dword *)localPalette + 1) = 0xFF000000;

} // END Pix::CreateGrayPalette2()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Pix::CreateGrayPalette4()
{
	if (localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}

	if (IsEmpty())
		return;

	localPalette = new RGBA[4];

	*(dword *)localPalette       = 0xFF0000;
	*((dword *)localPalette + 1) = 0xFF555555;
	*((dword *)localPalette + 2) = 0xFFAAAAAA;
	*((dword *)localPalette + 3) = 0xFFFFFFFF;

} // END Pix::CreateGrayPalette4()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Pix::CreateGrayPalette16()
{
	if (localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}

	if (IsEmpty())
		return;

	localPalette = new RGBA[16];

	dword *palPnt = (dword *)localPalette;

	for (dword i = 0; i < 16; i++)
		*palPnt++ = i << 4 | i << 12 | i << 20 | 0xff000000;

} // END Pix::CreateGrayPalette16()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Pix::CreateGrayPalette256()
{
	if (localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}

	if (IsEmpty())
		return;

	localPalette = new RGBA[256];

	dword *palPnt = (dword *)localPalette;
	dword val;

	for (dword i = 0; i < 256; i++)
	{
		val = (255 * i) / 255;
		*palPnt++ = val | val << 8 | val << 16 | 0xff000000;
	}

} // END Pix::CreateGrayPalette256()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// convert PIX palette
void Pix::ConvertPIXPalette(PIXCMAP *colormap)
{
	int iColor, numColors;
	l_int32 rval, gval, bval;
	RGBA *palPnt;

	if (localPalette)
	{
		delete[] localPalette;
		localPalette = NULL;
	}

	if (IsEmpty())
		return;

	// get number of entries in colormap
	numColors = pixcmapGetCount(colormap);

	if (numColors > 256)
		numColors = 256;

	localPalette = new RGBA[numColors];

	// convert and copy it to local palette
	palPnt = localPalette;

	for (iColor = 0; iColor < numColors; iColor++)
	{
		pixcmapGetColor(colormap, iColor, &rval, &gval, &bval);
		palPnt->a = 255;
		palPnt->r = (byte)rval;
		palPnt->g = (byte)gval;
		palPnt->b = (byte)bval;
		palPnt++;
	}

} // END Pix::ConvertPIXPalette()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// loads Pix from another raster object
void Pix::Load(Raster& raster, bool deepCopy, int page)
{
	PixBase *pixBase;

	Destroy();
	
	// shortcut if raster is a pixRaster
	Raster::Info info = raster.GetInfo();

	int width = raster.GetWidth();

	// check whether we're loading from a PixBase derived raster
	// the process don't require conversion then
	pixBase = dynamic_cast<PixBase *>(&raster);
	if (pixBase)
	{
		pix = pixBase->GetPIX(page);
		if(!pix)
			return;
		if(deepCopy)
			pix = pixCopy(NULL, pix);
		else
			pix = pixClone(pix);
		
		return;
	}

	// not a PixRaster, we should go the slow way

	// stores old active page, we need to change it
	int oldPage = raster.GetActivePage();

	// gets current page on source raster
	raster.SeekPage(page);

	// gets information from source object
	const RasterFormat *format = raster.GetFormat();

	// convert source image to PIX
	if (!format)
	{
		// null RasterFormat -- standard Upp RGBA
		LoadRASTER_RGBA(raster);
	}

	else
	{
		switch (format->GetType())
		{

			case RASTER_1:
				LoadRASTER_1(raster);
				break;

			case RASTER_1 | RASTER_MSBFIRST:
				LoadRASTER_1_MSBFIRST(raster);
				break;

			case RASTER_2:
				LoadRASTER_2(raster);
				break;

			case RASTER_2 | RASTER_MSBFIRST:
				LoadRASTER_2_MSBFIRST(raster);
				break;

			case RASTER_4:
				LoadRASTER_4(raster);
				break;

			case RASTER_4 | RASTER_MSBFIRST:
				LoadRASTER_4_MSBFIRST(raster);
				break;

			case RASTER_8:

			case RASTER_8 | RASTER_MSBFIRST:
				LoadRASTER_8(raster);
				break;

			case RASTER_8ALPHA:

			case RASTER_8ALPHA | RASTER_MSBFIRST:
				LoadRASTER_8ALPHA(raster);
				break;

			case RASTER_16:
				LoadRASTER_16(raster);
				break;

			case RASTER_16 | RASTER_MSBFIRST:
				LoadRASTER_16_MSBFIRST(raster);
				break;

			case RASTER_24:
				LoadRASTER_24(raster);
				break;

			case RASTER_24 | RASTER_MSBFIRST:
				LoadRASTER_24_MSBFIRST(raster);
				break;

			case RASTER_32:
			case RASTER_32ALPHA:
			case RASTER_32PREMULTIPLIED:
				LoadRASTER_32(raster);
				break;

			case RASTER_32 | RASTER_MSBFIRST:
			case RASTER_32ALPHA | RASTER_MSBFIRST:
			case RASTER_32PREMULTIPLIED | RASTER_MSBFIRST:
				LoadRASTER_32_MSBFIRST(raster);
				break;
				
			default:
				NEVER();

		} // switch
	}

	// if source raster has a palette, create a corresponding
	// PIXCMAP and fill it
	const RGBA *palette = raster.GetPalette();
	int palCount = raster.GetPaletteCount(); // needed because of bug of png plugin
	if (palette && palCount)
	{
		int bpp = raster.GetInfo().bpp;
		PIXCMAP *colorTable = pixcmapCreate(bpp);
		palCount = min(1 << bpp, palCount);

		for (int iPal = 0; iPal < palCount; iPal++)
		{
			pixcmapAddColor(colorTable, palette->r, palette->g, palette->b);
			palette++;
		}

		pixSetColormap(pix, colorTable);
	}

	// gets physical resolution from source raster pys dimensions, if any
	int dotX = raster.GetInfo().dots.cx;

	int dotY = raster.GetInfo().dots.cy;

	int resX, resY;

	if (dotX)
		resX = iscale(600, raster.GetWidth(), dotX);
	else
		resX = 0;

	if (dotY)
		resY = iscale(600, raster.GetHeight(), dotY);
	else
		resY = 0;
	pixSetXRes(pix, resX);
	pixSetYRes(pix, resY);

	// restores old page on source raster
	raster.SeekPage(oldPage);
	
} // END Pix::Load()
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// empty constructor -- sets NULL PIX
Pix::Pix()
{
	pix = NULL;
	rasterFormat = NULL;
	localPalette = NULL;
	markers.Clear();

} // END Pix::Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// creates a PIX in given format and size
Pix::Pix(int width, int height, int depth, RGBA *colorTable)
{
	pix = pixCreate(width, height, depth);
	if(colorTable)
	{
		NEVER(); // FIXME-- Convert color table to PIX format
	}
	rasterFormat = NULL;
	localPalette = NULL;
	markers.Clear();

} // END Pix::Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor -- takes a Leptonica PIX
// TAKES OWNERSHIP of underlying PIX object which is NULLed
Pix::Pix(PIX **_pix)
{
	pix = *_pix;
	*_pix = NULL;
	rasterFormat = NULL;
	localPalette = NULL;
	markers.Clear();

} // END Pix::Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// copy constructor -- reference increments underlying PIX
Pix::Pix(Pix const &_pix)
{
	pix = pixClone((Pix &)_pix);
	rasterFormat = NULL;
	localPalette = NULL;
	markers <<= _pix.markers;
	
} // END Pix::Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deep copy constructor -- deep copy of underlying PIX
Pix::Pix(Pix const &_pix, int i)
{
	pix = pixCopy(NULL, (Pix &)_pix);
	rasterFormat = NULL;
	localPalette = NULL;
	markers <<= _pix.markers;

} // END Pix::Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// creation of Pix from a raster object
Pix::Pix(Raster &raster, bool deepCopy, int page)
{
	pix = NULL;
	Load(raster, deepCopy, page);	
}
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// creation of Pix from file/stream
Pix::Pix(FileIn &fs, int page)
{
	pix = NULL;
	Load(fs, page);	

}

Pix::Pix(String const &fileName, int page)
{
	pix = NULL;
	Load(fileName, page);	

}
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
Pix::~Pix()
{
	Destroy();

} // END Pix::~Pix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// assignment -- increment reference of underlying PIX
Pix &Pix::operator=(Pix &_pix)
{
	Destroy();
	if(!_pix)
		return _pix;
	
	pix = pixClone(_pix);
	rasterFormat = NULL;
	localPalette = NULL;
	markers <<= _pix.markers;
	return *this;

} // END Pix::operator=()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deep copy -- deep copies underlying PIX
Pix &Pix::operator <<=(Pix &_pix)
{
	Destroy();
	if(!_pix)
		return *this;
	
	pix = pixCopy(NULL, _pix);
	rasterFormat = NULL;
	localPalette = NULL;
	markers <<= _pix.markers;
	return *this;

} // END Pix::operator <<=()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix &Pix::DeepCopy(Pix &_pix)
{
	Destroy();
	if(!_pix)
		return *this;
	
	pix = pixCopy(NULL, _pix);
	rasterFormat = NULL;
	localPalette = NULL;
	markers <<= _pix.markers;
	return *this;

} // END Pix::DeepCopy()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// standard Raster functions -- they operate on active page
Size Pix::GetSizeEx(int)
{
	Size sz;
	
	pixGetDimensions(pix, &sz.cx, &sz.cy, NULL);
	return sz;

} // END Pix::GetSizeEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Raster::Info Pix::GetInfoEx(int)
{
	Raster::Info info;
	memset(&info, sizeof(info), 0);

	if (IsEmpty())
		return Raster::Info();

	// image dimensions from pixel sizes and resolution
	info.bpp = pixGetDepth(pix);

	int width = pixGetWidth(pix);

	int height = pixGetHeight(pix);

	int xRes = pixGetXRes(pix);

	int yRes = pixGetYRes(pix);

	if (xRes)
		info.dots.cx = iscale(width, 600, xRes);
	else
		info.dots.cx = 0;

	if (yRes)
		info.dots.cy = iscale(height, 600, yRes);
	else
		info.dots.cy = 0;

	// image kind -- Leptonica just handles opaque images
	info.kind = IMAGE_OPAQUE;

	// number of colors
	if (pixGetColormap(pix))
		info.colors = pixcmapGetCount(pixGetColormap(pix));
	else
		info.colors = 1 << pixGetDepth(pix);

	return info;

} // END Pix::GetInfoEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Raster::Line Pix::GetLineEx(int line, int)
{
	// empty line on empty image
	if (IsEmpty())
		return Raster::Line();

	// if line out of bounds, return empty line
	if (line < 0 || line >= pixGetHeight(pix))
		return Raster::Line();

	// gets PIX bpp -- we must handle special case for 16bpp grayscale
	// which don't have an Upp counterpart
	// as a side problem, for 16 bit grayscaled BPP we must precalculate
	// both raw and RGBA data

	if (pixGetDepth(pix) == 16)
	{
		// we return here an RGBA buffer filled with
		// MSBs from PIX
		dword mask;
		byte shift;
		dword grayVal;

		// gets image width
		int width = pixGetWidth(pix);

		// allocates an RGBA buffer for data
		RGBA *rgba = new RGBA[width];
		RGBA *rgbaPtr = rgba;

		l_uint32 *pixPtr = pixGetData(pix) + line * pixGetWpl(pix);

		for (int iPix = 0, mask = 0xFFFF0000, shift = 16; iPix < width; iPix++)
		{
			grayVal = ((*pixPtr & mask) >> shift) >> 4;
			*(dword *)rgbaPtr++ = grayVal | (grayVal << 8) | (grayVal << 16);
			mask >>= 16;
			shift -= 16;

			if (!mask)
			{
				mask = 0xFFFF0000;
				shift = 16;
				pixPtr++;
			}
		}

		return Raster::Line((byte *)rgba, this, true);
	}

	// normal case, we use RasterFormat's converting support
	// BUT, for LE machines we must re-swap bytes inside words
	// to reverse the weird Leptonica mechanics
	int Wpl = pixGetWpl(pix);

	byte *pixData = (byte *)(pixGetData(pix) + line * Wpl);

#ifdef L_LITTLE_ENDIAN
	byte *pixPtr = pixData;

	byte *fmtData = new byte[Wpl*4];

	byte *fmtPtr = fmtData;

	for (int iDWord = 0; iDWord < Wpl; iDWord++)
	{
		*fmtPtr++ = *(pixPtr + 3);
		*fmtPtr++ = *(pixPtr + 2);
		*fmtPtr++ = *(pixPtr + 1);
		*fmtPtr++ = *(pixPtr + 0);
		pixPtr += 4;
	}

	Raster::Line l(fmtData, this, true);

#else
	Raster::Line l(pixData, this, false);
#endif
	return l;

} // END Pix::GetLineEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Pix::GetPaletteCountEx(int)
{
	PIXCMAP *colormap;
	int palCount;
	int bpp;

	if (IsEmpty())
		return 0;

	// get current pix's depth
	bpp = pixGetDepth(pix);

	// we must handle un-paletted PIX grayscale images
	// so, in that case, we use one of 4 predefined grayscale palettes
	colormap = pixGetColormap(pix);

	if (!colormap)
		if (bpp <= 8)
			palCount = 1 << bpp;
		else
			palCount = 0;
	else
		palCount = pixcmapGetCount(colormap);

	return palCount;

} // END Pix::GetPaletteCountEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RGBA *Pix::GetPaletteEx(int)
{
	PIXCMAP *colormap;
	int bpp;

	if (IsEmpty())
		return NULL;

	// if palette is up to date, just return it
	if(localPalette)
		return localPalette;

	// get current pix's depth
	bpp = pixGetDepth(pix);

	// we must handle un-paletted PIX grayscale images
	// so, in that case, we use one of 4 predefined grayscale palettes
	colormap = pixGetColormap(pix);

	if (!colormap || bpp == 1 /* 1bpp PIXs seems not care of colormap.... */)
	{
		switch (bpp)
		{

			case 1:
				CreateGrayPalette2();
				break;

			case 2:
				CreateGrayPalette4();
				break;

			case 4:
				CreateGrayPalette16();
				break;

			case 8:
				CreateGrayPalette256();
				break;

			default:
				break;
		}
	}

	else
	{
		// convert PIX palette
		ConvertPIXPalette(colormap);
	}

	return localPalette;

} // END Pix::GetPaletteEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RasterFormat *Pix::GetFormatEx(int)
{
	if(!rasterFormat)
		SetRasterFormat();
	
	return rasterFormat;

} // END Pix::GetFormatEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// file I/O
bool Pix::Load(FileIn &fs, int page)
{
	int format;
#ifdef WIN32
	HANDLE fh;
#else
	int fh;
#endif

	Destroy();

	fh = fs.GetHandle();
	if (!fh)
		return false;
	FILE *fp = fdopen(fh, "r");
	if(!fp)
		return false;

	if(findFileFormat(fp, &format))
	{
		fclose(fp);
		return false;
	}
	switch (format)
	{
		case IFF_BMP:
			if (page || (pix = pixReadStreamBmp(fp)) == NULL )
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_JFIF_JPEG:
			if (page || (pix = pixReadStreamJpeg(fp, READ_24_BIT_COLOR, 1, NULL, 0)) == NULL)
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_PNG:
			if (page || (pix = pixReadStreamPng(fp)) == NULL)
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_TIFF:
		case IFF_TIFF_PACKBITS:
		case IFF_TIFF_RLE:
		case IFF_TIFF_G3:
		case IFF_TIFF_G4:
		case IFF_TIFF_LZW:
		case IFF_TIFF_ZIP:
			// read requested page
			if ((pix = pixReadStreamTiff(fp, page)) == NULL)
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_PNM:
			if (page || (pix = pixReadStreamPnm(fp)) == NULL)
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_GIF:
			if (page || (pix = pixReadStreamGif(fp)) == NULL)
			{
				fclose(fp);
				return false;
			}
			pixSetInputFormat(pix, format);
			break;

		case IFF_UNKNOWN:
			fclose(fp);
			return false;
			break;
	}
	fclose(fp);
	return true;

} // END Pix::Load()

bool Pix::Load(String fileName, int page)
{
	FileIn f(fileName);
	return Load(f, page);
	
} // END Pix::Load()

bool Pix::Save(String fileName, int quality, bool progressive) // @@ to do - add compression and fine control
{
	// gets file extension
	String fileExt = ToUpper(fileName.Right(4));
	int format;
	if(fileExt == ".JPG")
		format = IFF_JFIF_JPEG;
	else if(fileExt == ".PNG")
		format = IFF_PNG;
	else if(fileExt == ".TIF")
		format = IFF_TIFF;
	else if(fileExt == ".BMP")
		format = IFF_BMP;
	else if(fileExt == ".PNM")
		format = IFF_PNM;
	else if(fileExt == ".PS")
		format = IFF_PS;
	else if(fileExt == ".GIF")
		format = IFF_GIF;
	else // still unsupported formats
	{
		format = IFF_UNKNOWN;
		return false;
	}
	if(format == IFF_JFIF_JPEG)
		return pixWriteJpeg(fileName, pix, quality, progressive ? 1 : 0) == 0;
	else
		return pixWrite(fileName, pix, format) == 0;

} // END Pix::Save()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PixRaster PRIVATE/PROTECTED /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gets real page
int PixRaster::getTruePage(int page)
{
	// if error page, throw exception
	// shouldn't happen, but....
	if (page == PIXRASTER_INVALID_PAGE)
		NEVER();

	// if empty raster, returns INVALID_PAGE
	else {
		if (IsEmpty() )
			return 0;

	// special value for current page
		else {
			if (page == PIXRASTER_CURPAGE)
				return curPage;

	// special value for last page
			else {
				if (page == PIXRASTER_LASTPAGE)
					return pages.GetCount() - 1;
	// negative values mean back offset from last page
				else {
					if (page < 0) {
						page = pages.GetCount() - 1 + page;

						if (page < 0)
							page = 0;

						return page;
					}
					else {
						if (page >= pages.GetCount())
							return pages.GetCount() - 1;
						else
							return page;
					}
				}
			}
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gets PIX from raster
// warning -- object references are NOT changed
// so objects must NOT be destroyed
PIX *PixRaster::GetPIX(int page)
{
	if (IsEmpty())
		return NULL;

	page = getTruePage(page);

	return At(page).GetPIX();

} // END PixRaster::GetPix()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PixRaster PUBLIC ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// loads / appends Pix from another raster object
void PixRaster::Load(Raster& raster, bool Append, bool deepCopy)
{
	if(!Append)
		pages.Clear();
	
	for(int page = 0; page < raster.GetPageCount(); page++)
		pages.Add(new Pix(raster, deepCopy, page));

} // END PixRaster::Load()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor -- empty raster
PixRaster::PixRaster()
{
	curPage = 0;

} // END Constructor class PixRaster

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor -- takes a Leptonica Pix
// PixRaster TAKES OWNERSHIP of underlying PIX object which is NULLed
PixRaster::PixRaster(PIX **pix)
{
	pages.Add(new Pix(pix));

	// sets the current page
	curPage = 0;

} // END Constructor class PixRaster

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor -- takes an array of Leptonica Pix
// PIXA gets destroyed and NULLed
PixRaster::PixRaster(PIXA **pixa)
{
	PIX *pix;
	
	curPage = 0;
	if(!pixa || !*pixa)
		return;
	
	for(int i = 0; i < pixaGetCount(*pixa); i++)
	{
		pix = pixaGetPix(*pixa, i, L_CLONE);
		pages.Add(new Pix(&pix));
	}
	pixaDestroy(pixa);

} // END Constructor class PixRaster

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// destructor
PixRaster::~PixRaster()
{
	
} // END Destructor class PixRaster
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PixRaster::SeekPage(int page)
{
	if (IsEmpty())
		return;

	// gets true page number
	page = getTruePage(page);
	curPage = page;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PixRaster::GetPageCount()
{
	return pages.GetCount();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PixRaster::GetActivePage()
{
	return curPage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Size PixRaster::GetSizeEx(int page)
{
	// if empty raster, returns a zero size
	if (IsEmpty())
		return Size(0,0);

	// gets true page number
	page = getTruePage(page);
	
	return At(page).GetSize();

} // END PixRaster::GetSizeEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Raster::Info PixRaster::GetInfoEx(int page)
{
	if (IsEmpty())
		return Raster::Info();

	// gets true page number
	page = getTruePage(page);
	
	return At(page).GetInfo();

} // END PixRaster::GetInfoEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Raster::Line PixRaster::GetLineEx(int line, int page)
{
	// empty line on empty image
	if (IsEmpty())
		return Raster::Line();

	// gets true page number
	page = getTruePage(page);
	
	return At(page).GetLine(line);

} // END PixRaster::GetLineEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PixRaster::GetPaletteCountEx(int page)
{
	// no palette on empty image
	if (IsEmpty())
		return 0;

	// gets true page number
	page = getTruePage(page);
	
	return At(page).GetPaletteCount();

} // END PixRaster::GetPaletteCountEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RGBA *PixRaster::GetPaletteEx(int page)
{
	// no palette on empty image
	if (IsEmpty())
		return 0;

	// gets true page number
	page = getTruePage(page);

	return At(page).GetPalette();

} // END PixRaster::GetPaletteEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RasterFormat *PixRaster::GetFormatEx(int page)
{
	if (IsEmpty())
		return NULL;

	// gets true page number
	page = getTruePage(page);

	return At(page).GetFormat();
	
} // END PixRaster::GetFormatEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// read page polymarkers
Markers *PixRaster::GetMarkersEx(int page)
{
	if (IsEmpty())
		return NULL;

	// gets true page number
	page = getTruePage(page);
	
	return At(page).GetMarkers();

} // END PixRaster::GetPolyMarkersEx()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sets PIX and PIX array
void PixRaster::Set(Pix &pix, int page, bool deepCopy)
{
	if (IsEmpty())
		return;

	page = getTruePage(page);
	
	if(deepCopy)
		pages.At(page) <<= pix;
	else
		pages.At(page) = pix;

} // END PixRaster::Set()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// adds a PIX or a PIX array
void PixRaster::Add(Pix &pix, bool deepCopy)
{
	if(deepCopy)
		pages.Add(new Pix(pix, 1));
	else
		pages.Add(new Pix(pix));

} // END PixRaster::Add()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PixRaster::Add(PixRaster &pixr, bool deepCopy)
{
	for(int i = 0; i < pixr.GetPageCount(); i++)
	{
		if(deepCopy)
			pages.Add(new Pix(pixr[i], 1));
		else
			pages.Add(new Pix(pixr[i]));
	}

} // END PixRaster::Add()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inserts a PIX or a PIX array
void PixRaster::Insert(Pix &pix, int where, bool deepCopy)
{
	// if adding at end, just append
	if (where == PIXRASTER_END || !GetPageCount())
	{
		Add(pix, deepCopy);
		return;
	}

	// gets true page
	where = getTruePage(where);

	// insert pix into pixa array
	if(deepCopy)
		pages.Insert(where, new Pix(pix, 1));
	else
		pages.Insert(where, new Pix(pix));

} // END PixRaster::Insert()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PixRaster::Insert(PixRaster &pixr, int where, bool deepCopy)
{
	// if adding at end, just append
	if (where == PIXRASTER_END || !GetPageCount())
	{
		Add(pixr, deepCopy);
		return;
	}

	// gets true page
	where = getTruePage(where);

	for (int i = 0; i < pixr.GetPageCount(); i++)
	{
		if(deepCopy)
			pages.Insert(where, new Pix(pixr[i], 1));
		else
			pages.Insert(where, new Pix(pixr[i]));
		where++;
	}

} // END PixRaster::Insert()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// removes a Pix or a series of Pix
void PixRaster::Remove(int where, int count)
{
	if (IsEmpty())
		return;

	if (where == PIXRASTER_END)
		where = GetPageCount() - 1;
	where = getTruePage(where);

	if (GetPageCount() - where < count)
		count = GetPageCount() - where;

	pages.Remove(where, count);

} // END PixRaster::Remove()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// duplicates a page at the end of array (useful for stack-like operations)
Pix &PixRaster::Dup(int page)
{
	// if raster empty, creates an empty Pix
	if (IsEmpty())
	{
		pages.Add(new Pix);
		return pages[0];
	}

	if (page == PIXRASTER_END)
		page = GetPageCount() - 1;
	page = getTruePage(page);

	Pix *pix = new Pix(pages[page], 1);
	pages.Add(pix);
	SeekPage(PIXRASTER_LASTPAGE);

	return *this;

} // END PixRaster::Dup()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Drops last page(s) of array (ditto)
void PixRaster::Drop(int count)
{
	if (IsEmpty())
		return;

	int start = GetPageCount() - count;
	if (start < 0)
	{
		count += start;
		start = 0;
	}
	Remove(start, count);
	SeekPage(PIXRASTER_LASTPAGE);

} // END PixRaster::Drop()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// array access
Pix &PixRaster::At(int page)
{
	ASSERT(!IsEmpty());
	
	page = getTruePage(page);
	return pages[page];
	
} // END PixRaster::At()
		

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File I/O operations
bool PixRaster::Load(FileIn &fs, bool Append)
{
	// if not appending, destroy actual content
	if(!Append)
		pages.Clear();

	int page = 0;
	Pix *pix = new Pix(fs, page);
	if(!pix)
		return false;
	while(pix)
	{
		pages.Add(pix);
		page++;
		pix = new Pix(fs, page);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PixRaster::Load(String fileName, bool Append)
{
	FileIn f(fileName);
	return Load(f, Append);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PixRaster::Save(String fileName, int page) // @@ to do - add compression and type handling
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PixRaster::SaveAll(String fileName)
{
	return false;
}

END_UPP_NAMESPACE
