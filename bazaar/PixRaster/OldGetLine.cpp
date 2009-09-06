Raster::Line PixRaster::GetLine(int line)
{
	RGBA *rgba, *rgbaPtr;
	PIX *pix;
	int width;
	PIXCMAP *colormap;
	l_uint32 *pixPtr;
	l_int32 mask;
	byte shift;
	int iPix;
	dword grayVal;
	
	// empty line on empty image
	if(!pixa || !pixaGetCount(pixa))
		return Raster::Line();
	
	// gets current PIX handle
	pix = pixaGetPix(pixa, curPage, L_CLONE);
	
	// if line out of bounds, return empty line
	if(line < 0 || line >= pixGetHeight(pix))
	{
		pixDestroy(&pix);
		return Raster::Line();
	}
	
	// gets image width
	width = pixGetWidth(pix);
	
	// if PIX is already in 32 bpp format, just return
	// a line with a pointer to data
	if(pixGetDepth(pix) >= 24)
	{
		rgba = (RGBA *)(pixGetData(pix) + width * line);
		pixDestroy(&pix);
		return Raster::Line(rgba, false);
	}

	// allocates an RGBA buffer for data
	rgba = new RGBA[width];
	rgbaPtr = rgba;
	
	// handle differently pixs with or without colormap
	colormap = pixGetColormap(pix);
	pixPtr = pixGetData(pix) + line * pixGetWpl(pix);
	if(colormap)
	{
		switch(pixGetDepth(pix))
		{
			case 1:
				for(iPix = 0, mask = 0x80000000; iPix < width; iPix++)
				{
					*rgbaPtr++ = ((RGBA *)colormap)[(*pixPtr & mask ? 1 : 0)];
					mask >>= 1;
					if(!mask)
					{
						mask = 0x80000000;
						pixPtr++;
					}
				}		
				break;

			case 2:
				for(iPix = 0, mask = 0xC0000000, shift = 30; iPix < width; iPix++)
				{
					*rgbaPtr++ = ((RGBA *)colormap)[(*pixPtr & mask) >> shift];
					mask >>= 2;
					shift -= 2;
					if(!mask)
					{
						mask = 0xC0000000;
						shift = 30;
						pixPtr++;
					}
				}					
				break;
				
			case 4:
				for(iPix = 0, mask = 0xF0000000, shift = 28; iPix < width; iPix++)
				{
					*rgbaPtr++ = ((RGBA *)colormap)[(*pixPtr & mask) >> shift];
					mask >>= 4;
					shift -= 4;
					if(!mask)
					{
						mask = 0xF0000000;
						shift = 28;
						pixPtr++;
					}
				}					
				break;
			
			case 8:
				for(iPix = 0, mask = 0xFF000000, shift = 24; iPix < width; iPix++)
				{
					*rgbaPtr++ = ((RGBA *)colormap)[(*pixPtr & mask) >> shift];
					mask >>= 8;
					shift -= 8;
					if(!mask)
					{
						mask = 0xFF000000;
						shift = 24;
						pixPtr++;
					}
				}					
				break;
				
			default:
				memset(rgba, 4*width, 0);
				break;
				
		} // switch
	}
	else
	{
		switch(pixGetDepth(pix))
		{
			case 1:
				for(iPix = 0, mask = 0x80000000; iPix < width; iPix++)
				{
					*(dword *)rgbaPtr++ = *pixPtr & mask ? 0x00ffffff : 0x00000000;
					mask >>= 1;
					if(!mask)
					{
						mask = 0x80000000;
						pixPtr++;
					}
				}		
				break;

			case 2:
				for(iPix = 0, mask = 0xC0000000, shift = 30; iPix < width; iPix++)
				{
					grayVal = ((*pixPtr & mask) >> shift) << 6;
					*(dword *)rgbaPtr++ = grayVal | (grayVal << 8) | (grayVal << 16);
					mask >>= 2;
					shift -= 2;
					if(!mask)
					{
						mask = 0xC0000000;
						shift = 30;
						pixPtr++;
					}
				}					
				break;
				
			case 4:
				for(iPix = 0, mask = 0xF0000000, shift = 28; iPix < width; iPix++)
				{
					grayVal = ((*pixPtr & mask) >> shift) << 4;
					*(dword *)rgbaPtr++ = grayVal | (grayVal << 8) | (grayVal << 16);
					mask >>= 4;
					shift -= 4;
					if(!mask)
					{
						mask = 0xF0000000;
						shift = 28;
						pixPtr++;
					}
				}					
				break;
			
			case 8:
				for(iPix = 0, mask = 0xFF000000, shift = 24; iPix < width; iPix++)
				{
					grayVal = (*pixPtr & mask) >> shift;
					*(dword *)rgbaPtr++ = grayVal | (grayVal << 8) | (grayVal << 16);
					mask >>= 8;
					shift -= 8;
					if(!mask)
					{
						mask = 0xFF000000;
						shift = 24;
						pixPtr++;
					}
				}					
				break;
				
			case 16:
				for(iPix = 0, mask = 0xFFFF0000, shift = 16; iPix < width; iPix++)
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
				break;
				
			default:
				memset(rgba, 4*width, 0);
				break;
				
		} // switch
	} // if colormap
	
	pixDestroy(&pix);
	return Raster::Line(rgba, true);
	
} // END PixRaster::GetLine()

