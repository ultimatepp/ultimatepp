#include "PixRaster.h"

NAMESPACE_UPP

// sets raster format depending on current page's pix format
void PixRaster::SetRasterFormat(int page)
{
	PIX *pix;
	
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	// removes old format for page
	if(formats[page])
	{
		delete formats[page];
		formats[page] = NULL;
	}
	RasterFormat *format = new RasterFormat;
	
	pix = pixaGetPix(pixa, page, L_CLONE);
	switch(pixGetDepth(pix))
	{
		case 1:
			format->Set1mf();
			break;
	
		case 2:
			format->Set2mf();
			break;
	
		case 4:
			format->Set4mf();
			break;
	
		case 8:
			format->Set8();
			break;
	
		case 16:
			// Upp does 16bpp color, Leptonica does 16bpp grayscale
			// MakeRGBA() will fail converting from Leptonica format anyways
			// we should add a 16bpp grayscale to Upp
			// in the meanwhile 16 bit Leptonica are converted to RGBA
			format->Set32be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;
	
		case 24:
			format->Set24be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;
	
		case 32:
			format->Set32be(
				0x000000FF << L_RED_SHIFT,
				0x000000FF << L_GREEN_SHIFT,
				0x000000FF << L_BLUE_SHIFT
			);
			break;
	} // switch
	
	formats[page] = format;
	pixDestroy(&pix);
}

// checks wether page format is valid
bool PixRaster::IsRasterFormatValid(int page)
{
	if(IsEmpty())
		return false;

	// gets true page number
	page = getTruePage(page);
	
	return (formats[page] != NULL);
}
		
// destroys page format
void PixRaster::DestroyRasterFormat(int page)
{
	if(IsEmpty())
		return;

	// gets true page number
	page = getTruePage(page);
	
	if(formats[page])
	{
		delete formats[page];
		formats[page] = NULL;
	}
}
		
// synthesyze grayscale palettes
void PixRaster::CreateGrayPalette2(int page)
{
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}
	RGBA *pal = new RGBA[2];
	
	*(dword *)pal       = 0xFFFFFFFF;
	*((dword *)pal + 1) = 0xFF000000;
	
	localPalettes[page] = pal;
	
} // END PixRaster::CreateGrayPalette2()

void PixRaster::CreateGrayPalette4(int page)
{
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}
	RGBA *pal = new RGBA[4];

	*(dword *)pal       = 0xFF0000;
	*((dword *)pal + 1) = 0xFF555555;
	*((dword *)pal + 2) = 0xFFAAAAAA;
	*((dword *)pal + 3) = 0xFFFFFFFF;
	
	localPalettes[page] = pal;
	
} // END PixRaster::CreateGrayPalette4()

void PixRaster::CreateGrayPalette16(int page)
{
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}
	RGBA *pal = new RGBA[16];

	dword *palPnt = (dword *)pal;
	for(dword i = 0; i < 16; i++)
		*palPnt++ = i << 4 | i << 12 | i << 20 | 0xff000000;
	
	localPalettes[page] = pal;
	
} // END PixRaster::CreateGrayPalette16()

void PixRaster::CreateGrayPalette256(int page)
{
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}
	RGBA *pal = new RGBA[256];

	dword *palPnt = (dword *)pal;
	dword val;
	for(dword i = 0; i < 256; i++)
	{
        val = (255 * i) / 255;
		*palPnt++ = val | val << 8 | val << 16 | 0xff000000;
	}
	
	localPalettes[page] = pal;
	
} // END PixRaster::CreateGrayPalette256()
		
// convert PIX palette
void PixRaster::ConvertPIXPalette(PIXCMAP *colormap, int page)
{
	int iColor, numColors;
	l_int32 rval, gval, bval;
	RGBA *palPnt;
	
	if(IsEmpty())
		return;
	page = getTruePage(page);
	
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}

	// get number of entries in colormap
	numColors = pixcmapGetCount(colormap);
	if(numColors > 256)
		numColors = 256;
	RGBA *pal = new RGBA[numColors];
	
	// convert and copy it to local palette
	palPnt = pal;
	for (iColor = 0; iColor < numColors; iColor++)
	{
		pixcmapGetColor(colormap, iColor, &rval, &gval, &bval);
		palPnt->a = 255;
		palPnt->r = (byte)rval;
		palPnt->g = (byte)gval;
		palPnt->b = (byte)bval;
		palPnt++;
	}
	
	localPalettes[page] = pal;
	
} // END PixRaster::ConvertPIXPalette()

// destroys local palette
void PixRaster::DestroyLocalPalette(int page)
{
	if(IsEmpty())
		return;

	// gets true page number
	page = getTruePage(page);
	if(localPalettes[page])
	{
		delete[] localPalettes[page];
		localPalettes[page] = NULL;
	}
}

// checks wether local palette is valid
bool PixRaster::IsLocalPaletteValid(int page)
{
	if(IsEmpty())
		return false;

	// gets true page number
	page = getTruePage(page);
	
	return (localPalettes[page] != NULL);
}

// check for empty pixraster
bool PixRaster::IsEmpty()
{
	return (!pixa || !pixaGetCount(pixa));
}
		
// gets real page
int PixRaster::getTruePage(int page)
{
	// if error page, throw exception
	// shouldn't happen, but....
	if(page == PIXRASTER_INVALID_PAGE)
		NEVER();
	// if empty raster, returns INVALID_PAGE
	else if(IsEmpty() )
		return 0;
	// special value for current page
	else if(page == PIXRASTER_CURPAGE)
		return curPage;
	// special value for last page
	else if(page == PIXRASTER_LASTPAGE)
		return pixaGetCount(pixa) - 1;
	// negative values mean back offset from last page
	else if(page < 0)
	{
		page = pixaGetCount(pixa) - 1 + page;
		if(page < 0)
			page = 0;
		return page;
	}
	else if(page >= pixaGetCount(pixa))
		return pixaGetCount(pixa) - 1;
	else
		return page;
}

		
void PixRaster::SeekPage(int page)
{
	if(IsEmpty())
		return;

	// gets true page number
	page = getTruePage(page);

	if(page == curPage)
		return;

	curPage = page; 
}

int PixRaster::GetPageCount()
{
	if(IsEmpty())
		return 0;
	return pixaGetCount(pixa);
}

int PixRaster::GetActivePage()
{
	return curPage;
}

Size PixRaster::GetSize(int page)
{
	Size sz(0, 0);

	// if empty raster, returns a zero size
	if(IsEmpty())
		return sz;
	
	// gets true page number
	page = getTruePage(page);
	
	// return size of current page
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);
	pixGetDimensions(pix, &sz.cx, &sz.cy, NULL);
	pixDestroy(&pix);
	return sz;
	
} // END PixRaster::GetSize()

Raster::Info PixRaster::GetInfo(int page)
{
	Raster::Info info;
	memset(&info, sizeof(info), 0);

	if(IsEmpty())
		return info;

	// gets true page number
	page = getTruePage(page);
	
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);

	// image dimensions from pixel sizes and resolution
	info.bpp = pixGetDepth(pix);
	int width = pixGetWidth(pix);
	int height = pixGetHeight(pix);
	int xRes = pixGetXRes(pix);
	int yRes = pixGetYRes(pix);
	if(xRes)
		info.dots.cx = iscale(width, 600, xRes);
	else
		info.dots.cx = 0;
	if(yRes)
		info.dots.cy = iscale(height, 600, yRes);
	else
		info.dots.cy = 0;
	
	// image kind -- Leptonica just handles opaque images
	info.kind = IMAGE_OPAQUE;
	
	// number of colors
	if(pixGetColormap(pix))
		info.colors = pixcmapGetCount(pixGetColormap(pix));
	else
		info.colors = 1 << pixGetDepth(pix);
	
	pixDestroy(&pix);
	return info;

} // END PixRaster::GetInfo()

Raster::Line PixRaster::GetLine(int line, int page)
{
	// empty line on empty image
	if(IsEmpty())
		return Raster::Line();

	// gets true page number
	page = getTruePage(page);
	
	// gets current PIX handle
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);
	
	// if line out of bounds, return empty line
	if(line < 0 || line >= pixGetHeight(pix))
	{
		pixDestroy(&pix);
		return Raster::Line();
	}
	
	// gets PIX bpp -- we must handle special case for 16bpp grayscale
	// which don't have an Upp counterpart
	// as a side problem, for 16 bit grayscaled BPP we must precalculate
	// both raw and RGBA data
	if(pixGetDepth(pix) == 16)
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
		for(int iPix = 0, mask = 0xFFFF0000, shift = 16; iPix < width; iPix++)
		{
			grayVal = ((*pixPtr & mask) >> shift) >> 4;
			*(dword *)rgbaPtr++ = grayVal | (grayVal << 8) | (grayVal << 16);
			mask >>= 16;
			shift -= 16;
			if(!mask)
			{
				mask = 0xFFFF0000;
				shift = 16;
				pixPtr++;
			}
		}					
	
		pixDestroy(&pix);
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
	for(int iDWord = 0; iDWord < Wpl; iDWord++)
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
	pixDestroy(&pix);
	return l;
	
} // END PixRaster::GetLine()

int PixRaster::GetPaletteCount(int page)
{
	PIX *pix;
	PIXCMAP *colormap;
	int palCount;
	int bpp;
	
	// no palette on empty image
	if(IsEmpty())
		return 0;

	// gets true page number
	page = getTruePage(page);
	
	// gets current PIX handle
	pix = pixaGetPix(pixa, page, L_CLONE);
	
	// get current pix's depth
	bpp = pixGetDepth(pix);
	
	// we must handle un-paletted PIX grayscale images
	// so, in that case, we use one of 4 predefined grayscale palettes
	colormap = pixGetColormap(pix);
	if(!colormap)
		if(bpp <= 8)
			palCount = 1 << bpp;
		else
			palCount = 0;
	else
		palCount = pixcmapGetCount(colormap);
	
	pixDestroy(&pix);
	return palCount;
	
} // END PixRaster::GetPaletteCount()

const RGBA *PixRaster::GetPalette(int page)
{
	PIX *pix;
	PIXCMAP *colormap;
	int bpp;
	RGBA *palette;
	
	// no palette on empty image
	if(IsEmpty())
		return 0;
	
	// gets true page number
	page = getTruePage(page);
	
	// if palette is up to date, just return it
	if(IsLocalPaletteValid(page))
		return localPalettes[page];
	
	// gets current PIX handle
	pix = pixaGetPix(pixa, page, L_CLONE);
	
	// get current pix's depth
	bpp = pixGetDepth(pix);
	
	// we must handle un-paletted PIX grayscale images
	// so, in that case, we use one of 4 predefined grayscale palettes
	colormap = pixGetColormap(pix);
	if(!colormap || bpp == 1 /* 1bpp PIXs seems not care of colormap.... */)
	{
		switch(bpp)
		{
			case 1:
				CreateGrayPalette2(page);
				palette = localPalettes[page];
				break;

			case 2:
				CreateGrayPalette4(page);
				palette = localPalettes[page];
				break;

			case 4:
				CreateGrayPalette16(page);
				palette = localPalettes[page];
				break;

			case 8:
				CreateGrayPalette256(page);
				palette = localPalettes[page];
				break;
				
			default:
				palette = 0;
				break;
		}
	}
	else
	{
		// convert PIX palette
		ConvertPIXPalette(colormap, page);
		palette = localPalettes[page];
	}

	pixDestroy(&pix);
	return palette;
	
} // END PixRaster::GetPalette()

const RasterFormat *PixRaster::GetFormat(int page)
{
	if(IsEmpty())
		return NULL;

	// gets true page number
	page = getTruePage(page);
	
	if(!IsRasterFormatValid(page))
		SetRasterFormat(page);
	return formats[page];
		
} // END PixRaster::GetFormat()

// constructor -- empty raster
PixRaster::PixRaster()
{
	pixa = NULL;
	curPage = 0;

} // END Constructor class PixRaster

// constructor -- takes a Leptonica Pix
// PIX get cloned and its reference count increased
PixRaster::PixRaster(PIX *pix, CopyModes copyMode)
{
	pixa = pixaCreate(0);
	pixaAddPix(pixa, pix, copyMode);
	
	// sets the current page
	curPage = 0;
	
	// allocates empty format and local palette
	formats.Add((RasterFormat *)NULL);
	localPalettes.Add((RGBA *)NULL);
	
} // END Constructor class PixRaster

// constructor -- takes an array of Leptonica Pix
PixRaster::PixRaster(PIXA *_pixa, CopyModes copyMode)
{
	pixa = pixaCopy(_pixa, copyMode);
	
	// sets the current page
	curPage = 0;
	
	// allocates empty format and local palette
	for(int i = 0; i < pixaGetCount(pixa); i++)
	{
		formats.Add((RasterFormat *)NULL);
		localPalettes.Add((RGBA *)NULL);
	}
	
} // END Constructor class PixRaster

// destructor -- must free PIXA array
void PixRaster::Destroy()
{
	// frees formats and local palettes
	for(int i = 0; i < pixaGetCount(pixa); i++)
	{
		DestroyRasterFormat(i);
		DestroyLocalPalette(i);
	}
	
	// frees pix array
	if(pixa)
		pixaDestroy(&pixa);
	
} // END Destructor class PixRaster

// loads / appends Pix from another raster object
void PixRaster::Load(Raster& raster, bool Append, CopyModes copyMode)
{
	PixRaster *pixRaster;
	
	// if we don't want the array to be appended,
	// we destroy current array, if any
	if(!Append && pixa)
		Destroy();
	
	// if empty raster, we create it
	if(IsEmpty())
	{
		pixa=pixaCreate(0);
		curPage = 0;
	}
	
	// shortcut if raster is a pixRaster
	Raster::Info info = raster.GetInfo();
	int width = raster.GetWidth();
	pixRaster = dynamic_cast<PixRaster *>(&raster);
	if(pixRaster)
	{
		ASSERT(copyMode == PIXRASTER_COPY || copyMode == PIXRASTER_CLONE);
		AddPIXA(pixRaster->GetPIXA(copyMode));
		return;
	}
	
	// not a PixRaster, we should go the slow way
	
	// loop for all pages on source raster
	int oldPage = raster.GetActivePage();
	for(int iPage = 0; iPage < raster.GetPageCount(); iPage++)
	{
		PIX *pix;
		
		// gets current page on source raster
		raster.SeekPage(iPage);
		
		// gets information from source object
		const RasterFormat *format = raster.GetFormat();

		// convert source image to PIX
		if(!format)
		{
			// null RasterFormat -- standard Upp RGBA
			pix = LoadRASTER_RGBA(raster);
		}
		else
		{
			switch(format->GetType())
			{
				case RASTER_1:
					pix = LoadRASTER_1(raster);
					break;
					
				case RASTER_1 | RASTER_MSBFIRST:
					pix = LoadRASTER_1_MSBFIRST(raster);
					break;
					
				case RASTER_2:
					pix = LoadRASTER_2(raster);
					break;
					
				case RASTER_2 | RASTER_MSBFIRST:
					pix = LoadRASTER_2_MSBFIRST(raster);
					break;
					
				case RASTER_4:
					pix = LoadRASTER_4(raster);
					break;
					
				case RASTER_4 | RASTER_MSBFIRST:
					pix = LoadRASTER_4_MSBFIRST(raster);
					break;
					
				case RASTER_8:
				case RASTER_8 | RASTER_MSBFIRST:
					pix = LoadRASTER_8(raster);
					break;
					
				case RASTER_8ALPHA:
				case RASTER_8ALPHA | RASTER_MSBFIRST:
					pix = LoadRASTER_8ALPHA(raster);
					break;
					
				case RASTER_16:
					pix = LoadRASTER_16(raster);
					break;
					
				case RASTER_16 | RASTER_MSBFIRST:
					pix = LoadRASTER_16_MSBFIRST(raster);
					break;
					
				case RASTER_24:
					pix = LoadRASTER_24(raster);
					break;
					
				case RASTER_24 | RASTER_MSBFIRST:
					pix = LoadRASTER_24_MSBFIRST(raster);
					break;
					
				case RASTER_32:
				case RASTER_32ALPHA:
				case RASTER_32PREMULTIPLIED:
					pix = LoadRASTER_32(raster);
					break;
					
				case RASTER_32 | RASTER_MSBFIRST:
				case RASTER_32ALPHA | RASTER_MSBFIRST:
				case RASTER_32PREMULTIPLIED | RASTER_MSBFIRST:
					pix = LoadRASTER_32_MSBFIRST(raster);
					break;
					
			} // switch
		}
		
		// if source raster has a palette, create a corresponding
		// PIXCMAP and fill it
		const RGBA *palette = raster.GetPalette();
		int palCount = raster.GetPaletteCount(); // needed because of bug of png plugin	
		if(palette && palCount)
		{
			int bpp = raster.GetInfo().bpp;
			PIXCMAP *colorTable = pixcmapCreate(bpp);
			palCount = min(1 << bpp, palCount);
			for(int iPal = 0; iPal < palCount; iPal++)
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
		if(dotX)
			resX = iscale(600, raster.GetWidth(), dotX);
		else
			resX = 0;
		if(dotY)
			resY = iscale(600, raster.GetHeight(), dotY);
		else
			resY = 0;
		pixSetXRes(pix, resX);
		pixSetYRes(pix, resY);
		
		// adds the newly loaded PIX to array
		AddPIX(pix, copyMode);
		pixDestroy(&pix);

	} // for iPage
	raster.SeekPage(oldPage);
	
} // END PixRaster::Load()

// gets PIX and PIX array from raster
// warning -- object references are NOT changed
// so objects must NOT be destroyed
PIX *PixRaster::GetPIX(int page, CopyModes copyMode)
{
	PIX *pix;
	
	if(IsEmpty())
		return NULL;
	
	page = getTruePage(page);
	
	if(copyMode == PIXRASTER_COPY)
		pix = pixaGetPix(pixa, page, L_COPY);
	else
		pix = pixaGetPix(pixa, page, L_CLONE);
	if(pix && copyMode == PIXRASTER_REF)
		pix->refcount--;
	return pix;
	
} // END PixRaster::GetPix()

PIXA *PixRaster::GetPIXA(CopyModes copyMode)
{
	PIXA *_pixa;
	
	switch(copyMode)
	{
		case PIXRASTER_REF:
			return pixa;

		case PIXRASTER_CLONE:
		case PIXRASTER_COPY_CLONE:
		case PIXRASTER_COPY:
			return pixaCopy(pixa, copyMode);
		
		default:
			NEVER();
	}
	return NULL;
	
} // END PixRaster::GetPIXA()

// sets PIX and PIX array
void PixRaster::SetPIX(PIX *pix, int page, CopyModes copyMode)
{
	if(IsEmpty())
		return;
	
	page = getTruePage(page);
	
	if(copyMode == PIXRASTER_COPY)
		pix = pixCopy(NULL, pix);
	else if(copyMode == PIXRASTER_CLONE)
		pix = pixClone(pix);
	pixaReplacePix(pixa, page, pix, NULL);

	DestroyLocalPalette(page);
	DestroyRasterFormat(page);
	
} // END PixRaster::SetPIX()

void PixRaster::SetPIXA(PIXA *_pixa, CopyModes copyMode)
{
	Destroy();
	curPage = 0;
	AddPIXA(_pixa, copyMode);
	
} // END PixRaster::SetPIXA()

// adds a PIX or a PIX array
void PixRaster::AddPIX(PIX *pix, CopyModes copyMode)
{
	if(!pixa)
		pixa = pixaCreate(0);
	pixaAddPix(pixa, pix, copyMode);
	
	// allocates empty format and local palette
	formats.Add((RasterFormat *)NULL);
	localPalettes.Add((RGBA *)NULL);
	
} // END PixRaster::AddPIX()

void PixRaster::AddPIXA(PIXA *_pixa, CopyModes copyMode)
{
	if(!pixa)
		pixa = pixaCreate(0);
	pixaJoin(pixa, _pixa, 0, 0);
	
	// allocates empty format and local palette
	for(int i = 0; i < pixaGetCount(_pixa); i++)
	{
		formats.Add((RasterFormat *)NULL);
		localPalettes.Add((RGBA *)NULL);
	}

} // END PixRaster::AddPIXA()

// inserts a PIX or a PIX array
void PixRaster::InsertPIX(PIX *pix, int where, CopyModes copyMode)
{
	// if adding at end, just append
	if(where == PIXRASTER_END || !pixa)
	{
		AddPIX(pix, copyMode);
		return;
	}
	
	// gets true page
	where = getTruePage(where);
	
	// insert pix into pixa array
	pixaInsertPix(pixa, where, pix, NULL);

	// inserts empty format and palette
	formats.Insert(where, (RasterFormat *)NULL);
	localPalettes.Insert(where, (RGBA *)NULL);	
	
}

void PixRaster::InsertPIXA(PIXA *_pixa, int where, CopyModes copyMode)
{
	// if adding at end, just append
	if(where == PIXRASTER_END || !pixa)
	{
		AddPIXA(_pixa, copyMode);
		return;
	}
	
	// gets true page
	where = getTruePage(where);
	
	if(copyMode == PIXRASTER_COPY_CLONE)
		copyMode = PIXRASTER_CLONE;
	
	for(int i = 0; i < pixaGetCount(_pixa); i++)
	{
		// insert pix into pixa array
		PIX *pix = pixaGetPix(_pixa, i, copyMode);
		pixaInsertPix(pixa, where, pix, NULL);
	
		// inserts empty format and palette
		formats.Insert(where, (RasterFormat *)NULL);
		localPalettes.Insert(where, (RGBA *)NULL);
		where++;
	}
	
}

// removes a PIX or a series of PIX
void PixRaster::RemovePIX(int where, int count)
{
	if(IsEmpty())
		return;
	if(where == PIXRASTER_END)
		where = pixaGetCount(pixa) - 1;
	where = getTruePage(where);

	if(pixaGetCount(pixa) - where < count)
		count = pixaGetCount(pixa) - where;	
	for(int i = 0; i < count; i++)
	{
		pixaRemovePix(pixa, where);
		delete formats[where];
		formats.Remove(where);
		delete[] localPalettes[where];
		localPalettes.Remove(where);
	}
	
}

// duplicates a page at the end of array (useful for stack-like operations)
void PixRaster::Dup(int page)
{
	PIX *pix = GetPIX(page, PIXRASTER_CLONE);
	AddPIX(pix, PIXRASTER_COPY);
	pixDestroy(&pix);
	SeekPage(PIXRASTER_LASTPAGE);
	
}

// Drops last page(s) of array (ditto)
void PixRaster::Drop(int count)
{
	if(IsEmpty())
		return;
	int start = pixaGetCount(pixa) - count;
	if(start < 0)
	{
		count += start;
		start = 0;
	}
	RemovePIX(start, count);
	SeekPage(PIXRASTER_LASTPAGE);
	
}

END_UPP_NAMESPACE
