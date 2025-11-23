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

	auto ReadInt = [&] {
		int n = 0, c;
		while((c = stream.Get()) >= '0' && c <= '9')
			n = n * 10 + (c - '0');
		return n;
	};

	auto SkipWhitespaces = [&] {
		while(IsSpace(stream.Peek()))
			(void) stream.Get();
		while(stream.Peek() == '#') // PPM can contain comment lines
			(void) stream.GetLine();
	};

	// PPM P6 format
	if(stream.Get() == 'P' && stream.Get() == '6') {
		SkipWhitespaces();
		if(size.cx = ReadInt(); size.cx <= 0 || size.cx > 99999) {
			return false;
		}
		SkipWhitespaces();
		if(size.cy = ReadInt(); size.cy <= 0 || size.cy > 99999) {
			return false;
		}
		SkipWhitespaces();
		if(int maxval = ReadInt(); maxval > 0 && maxval < 65536) {
			SkipWhitespaces();
			is16 = maxval > 255;
			pixel_pos = stream.GetPos();
			return !stream.IsEof();
		}
	}
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
