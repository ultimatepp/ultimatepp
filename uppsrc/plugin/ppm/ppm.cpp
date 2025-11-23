#include "ppm.h"

namespace Upp {

PPMRaster::PPMRaster()
{
	size = Size(0, 0);
	is16 = false;
}

bool PPMRaster::Create()
{
	Stream& stream = GetStream();
	size = Size(0, 0);
	if(!stream.IsOpen()) {
		SetError();
		return false;
	}
	
	auto SkipWhitespaces = [&] {
		for(;;) {
			while(IsSpace(stream.Peek()))
				(void) stream.Get();
			if(stream.Peek() != '#')
				break;
			[[maybe_unused]] String s = stream.GetLine();
		}
	};
	
	auto ReadInt = [&] {
		int n = 0, c;
		while((c = stream.Get()) >= '0' && c <= '9') {
			// Overflow check: ensure n * 10 + digit doesn't overflow
			if(n > (INT_MAX - 9) / 10) {
				SetError();
				return -1;
			}
			n = n * 10 + (c - '0');
		}
		return n;
	};
	
	// PPM P6 format
	if(stream.Get() != 'P' || stream.Get() != '6') {
		SetError();
		return false;
	}
	
	SkipWhitespaces();
	if(size.cx = ReadInt(); size.cx <= 0 || size.cx >= std::numeric_limits<word>::max()) {
		SetError();
		return false;
	}
	
	SkipWhitespaces();
	if(size.cy = ReadInt(); size.cy <= 0 || size.cy >= std::numeric_limits<word>::max()) {
		SetError();
		return false;
	}
	
	SkipWhitespaces();
	if(int maxval = ReadInt(); maxval > 0 && maxval < std::numeric_limits<word>::max()) {
		SkipWhitespaces();
		is16 = maxval > 255;
		pixel_pos = stream.GetPos();
		return !stream.IsEof();
	}
	
	SetError();
	return false;
}

Raster::Info PPMRaster::GetInfo()
{
	return Raster::Info();
}

Size PPMRaster::GetSize()
{
	return size;
}

Raster::Line PPMRaster::GetLine(int ii)
{
	Stream& stream = GetStream();
	Line line;
	int row_bytes = size.cx * 3;
	byte *ptr = new byte[size.cx * 3];
	if(!IsError()) {
		Stream& stream = GetStream();
		stream.Seek(pixel_pos + ii * row_bytes * (1 + is16));
		if(is16) {
			byte *t = ptr;
			for(int n = size.cx; n-- >= 0;) {
				int q = stream.Get();
				if(q < 0) {
					SetError();
					break;
				}
				*t++ = q; stream.Get();
			}
			return Line(ptr, this, true);
		}
		else {
			if(!stream.GetAll(ptr, row_bytes))
				SetError();
		}
	}
	if(IsError())
		memset(ptr, 0, row_bytes);
	return Line(ptr, this, true);
}

int PPMRaster::GetPaletteCount()
{
	return 0;
}

RGBA * PPMRaster::GetPalette()
{
	return NULL;
}

const RasterFormat * PPMRaster::GetFormat()
{
	static RasterFormat fmt;
	ONCELOCK {
		fmt.Set24be(0xff0000, 0x00ff00, 0x0000ff);
	}
	return &fmt;
}

PPMRaster::~PPMRaster()
{
}

}
