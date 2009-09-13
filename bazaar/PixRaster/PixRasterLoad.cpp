#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS USED TO CORRECTLY HANDLE ENDIANNESS OF THE MACHINE
#ifdef L_BIG_ENDIAN
	#define SHIFT_START 0
	#define SHIFT_CHECK(shift) (shift > 24)
	#define SHIFT_STEP(shift) shift += 8
#else 
	#define SHIFT_START 24
	#define SHIFT_CHECK(shift) (shift < 0)
	#define SHIFT_STEP(shift) shift -= 8
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TRANSFER A BYTE
static inline byte ReverseByte(byte sb)
{
	byte db = 0;
	if(sb & 0x01) db |= 0x80;
	if(sb & 0x02) db |= 0x40;
	if(sb & 0x04) db |= 0x20;
	if(sb & 0x08) db |= 0x10;
	if(sb & 0x10) db |= 0x08;
	if(sb & 0x20) db |= 0x04;
	if(sb & 0x40) db |= 0x02;
	if(sb & 0x80) db |= 0x01;
	
	return db;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// transfers a byte line -- straight bit order
static void TransferLine_STRAIGHT(const byte *rasPnt, l_uint32 *pixPnt, int nBytes)
{
	l_uint32 w;
	short shift;
	for(int iByte = 0, shift = SHIFT_START, w = 0; iByte < nBytes; iByte++)
	{
		l_uint32 u = *rasPnt++/* ^ 0xff*/;
		w |= u << shift;
//		w |= ((l_uint32)*rasPnt++) << shift;
		SHIFT_STEP(shift);
		if(SHIFT_CHECK(shift))
		{
			*pixPnt++ = w;
			w = 0;
			shift = SHIFT_START;
		}
	}
	if(shift != SHIFT_START)
		*pixPnt = w;
		
} // END TransferLine_STRAIGHT()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// transfers a byte line -- reverse bit order
static void TransferLine_REVERSE(const byte *rasPnt, l_uint32 *pixPnt, int nBytes)
{
		l_uint32 w;
		short shift;
		for(int iByte = 0, shift = SHIFT_START, w = 0; iByte < nBytes; iByte++)
		{
			byte sb = *rasPnt++;
			byte db = 0;
			if(sb & 0x01) db |= 0x80;
			if(sb & 0x02) db |= 0x40;
			if(sb & 0x04) db |= 0x20;
			if(sb & 0x08) db |= 0x10;
			if(sb & 0x10) db |= 0x08;
			if(sb & 0x20) db |= 0x04;
			if(sb & 0x40) db |= 0x02;
			if(sb & 0x80) db |= 0x01;
			w |= ((l_uint32)*rasPnt++) << shift;
			SHIFT_STEP(shift);
			if(SHIFT_CHECK(shift))
			{
				*pixPnt++ = w;
				w = 0;
				shift = SHIFT_START;
			}
		}
		if(shift != SHIFT_START)
			*pixPnt = w;
		
} // END TransferLine_REVERSE()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// raster loading/conversion routines
bool Pix::LoadRASTER_RGBA(Raster &raster)
{
	Destroy();
	
} // END Pix::LoadRASTER_RGBA()

bool Pix::LoadRASTER_1(Raster &raster)
{
	Destroy();

	// get source Raster dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 1);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 7) / 8;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		TransferLine_REVERSE(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);

		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_1()

bool Pix::LoadRASTER_1_MSBFIRST(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 1);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 7) / 8;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		TransferLine_STRAIGHT(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_1_MSBFIRST()

bool Pix::LoadRASTER_2(Raster &raster)
{
	Destroy();

	// get source Raster dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height,2);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 3) / 4;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		TransferLine_REVERSE(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);

		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_2()

bool Pix::LoadRASTER_2_MSBFIRST(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 2);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 3) / 4;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		TransferLine_STRAIGHT(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_2()

bool Pix::LoadRASTER_4(Raster &raster)
{
	Destroy();
	
	// get source Raster dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 4);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 1) / 2;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		TransferLine_REVERSE(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);

		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_4()

bool Pix::LoadRASTER_4_MSBFIRST(Raster &raster)
{
	Destroy();
	
	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 4);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = (width + 1) / 2;
	
	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		Raster::Line line = raster.GetLine(iLine);
		const byte *data = line.GetRawData();
		TransferLine_STRAIGHT(line.GetRawData(), pixLine, nBytes);
//		TransferLine_STRAIGHT(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;

} // END Pix::LoadRASTER_4()

bool Pix::LoadRASTER_8(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 8);
	if(!pix)
		return false;

	// get PIX width (in words) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// number of bytes on each source line (full or partial)
	// as PIX is padded to 32 bit words, we don't bother to
	// get the exact end of source raster -- we transfer full bytes
	// BUT, we must operate on full words on dest data, because of
	// byte swapping done by Leptonica on le machines.....
	int nBytes = width;

	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		Raster::Line line = raster.GetLine(iLine);
		const byte *b = line.GetRawData();
		TransferLine_STRAIGHT(line.GetRawData(), pixLine, nBytes);
		
//		TransferLine_STRAIGHT(raster.GetLine(iLine).GetRawData(), pixLine, nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;
	
} // END Pix::LoadRASTER_8()

bool Pix::LoadRASTER_8ALPHA(Raster &raster)
{
	Destroy();

	Cerr() << "Upp Leptonica library : 8 bpp with alpha channel still not supported";
	NEVER();
	
} // END Pix::LoadRASTER_8ALPHA()

bool Pix::LoadRASTER_16(Raster &raster)
{
	Destroy();

	Cerr() << "Upp Leptonica library : 16 bpp still not supported";
	NEVER();
	
} // END Pix::LoadRASTER_16()

bool Pix::LoadRASTER_16_MSBFIRST(Raster &raster)
{
	Destroy();

	Cerr() << "Upp Leptonica library : 16 bpp still not supported";
	NEVER();
	
} // END Pix::LoadRASTER_16()

bool Pix::LoadRASTER_24(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 32);
	if(!pix)
		return false;

	// get PIX width (in bytes) and pix data pointer
	int wpl = pixGetWpl(pix);
	l_uint32 *pixLine = pixGetData(pix);
	
	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		Raster::Line line = raster.GetLine(iLine);
		const RGBA *rasPtr = line.GetRGBA();
//		const RGBA *rasPtr = raster.GetLine(iLine).GetRGBA();
		l_uint32 *pixPtr = pixLine;
		
		for(int iDWord = 0; iDWord < wpl; iDWord++)
		{
			l_uint32 w = 0;
			w |= rasPtr->r << 24; 
			w |= rasPtr->g << 16; 
			w |= rasPtr->b << 8; 
			w |= rasPtr->a << 0; 
			
			*pixPtr++ = w;
			rasPtr++;
		}
		
		// next dest line
		pixLine += wpl;
	}
	return true;
	
} // END Pix::LoadRASTER_24()

bool Pix::LoadRASTER_24_MSBFIRST(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 24);
	if(!pix)
		return false;

	// get PIX width (in bytes) and pix data pointer
	int wpl = pixGetWpl(pix);
	int nBytes = wpl * 4;
	l_uint32 *pixLine = pixGetData(pix);
	
	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		memmove(pixLine, raster.GetLine(iLine).GetRGBA(), nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;
	
} // END Pix::LoadRASTER_24()

bool Pix::LoadRASTER_32(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 32);
	if(!pix)
		return false;

	// get PIX width (in bytes) and pix data pointer
	int wpl = pixGetWpl(pix);
	int nBytes = wpl * 4;
	l_uint32 *pixLine = pixGetData(pix);
	
	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		byte *rasPtr = (byte *)raster.GetLine(iLine).GetRGBA();
		l_uint32 *pixPtr = pixLine;
		
		for(int iWord = 0; iWord < wpl; iWord++)
		{
			l_uint32 w = 0;
			w |= ((dword)(*(rasPtr + 0))) << 24; 
			w |= ((dword)(*(rasPtr + 1))) << 16; 
			w |= ((dword)(*(rasPtr + 2))) << 8; 
			w |= ((dword)(*(rasPtr + 3))) << 0;
			
			*pixLine++ = w;
			rasPtr += 4;
		}
		
		// next dest line
		pixLine += wpl;
	}
	return true;
	
} // END Pix::LoadRASTER_32()

bool Pix::LoadRASTER_32_MSBFIRST(Raster &raster)
{
	Destroy();

	// get source Rastet dimensions
	int width = raster.GetWidth();
	int height = raster.GetHeight();

	// create the PIX
	pix = pixCreateNoInit(width, height, 32);
	if(!pix)
		return false;

	// get PIX width (in bytes) and pix data pointer
	int wpl = pixGetWpl(pix);
	int nBytes = wpl * 4;
	l_uint32 *pixLine = pixGetData(pix);
	
	// transfer all scanlines
	for(int iLine = 0; iLine < height; iLine++)
	{
		// transfers the line
		memmove(pixLine, raster.GetLine(iLine).GetRGBA(), nBytes);
		
		// next dest line
		pixLine += wpl;
	}
	return true;
	
} // END Pix::LoadRASTER_32()

END_UPP_NAMESPACE
