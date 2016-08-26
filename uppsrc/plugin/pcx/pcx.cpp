#include "pcx.h"

namespace Upp {

static Size GetDotSize(Size pixel_size, int xpm, int ypm)
{
	if(!xpm || !ypm)
		return Size(0, 0);
	static const double DOTS_PER_METER = 60000 / 2.54;
	return Size(fround(pixel_size.cx * DOTS_PER_METER / xpm), fround(pixel_size.cy * DOTS_PER_METER / ypm));
}

int sFd(int x)
{
	return x > 0 ? x : 120;
}

bool PCXRaster::Create()
{
	Stream& stream = GetStream();
	size = Size(0, 0);
	if(!stream.IsOpen()) {
		SetError();
		return false;
	}
	ASSERT(stream.IsLoading());

	if(!stream.GetAll(&header, sizeof(PCXHeader))) {
		SetError();
		return false;
	}
	header.SwapEndian();
	if(header.manufacturer != 10) {
		SetError();
		return false;
	}

	if(header.bitsPerPixel == 8 && header.planes == 3)
		fmt.Set24le(0x0000ff, 0x00ff00, 0xff0000);
	else
	if(header.bitsPerPixel == 1 || header.bitsPerPixel == 4 || header.bitsPerPixel == 8)
		fmt.Set8();
	else {
		SetError();
		return false;
	}
	size = Size(header.maxX - header.minX + 1, header.maxY - header.minY + 1);
	info.kind = IMAGE_OPAQUE;
	info.bpp = header.bitsPerPixel;
	info.dots = Size(size.cx * 600 / sFd(header.horzDpi), size.cy * 600 / sFd(header.vertDpi));
	info.hotspot = Point(0, 0);
	info.colors = fmt.GetColorCount();
	int colors = 1 << (header.planes * header.bitsPerPixel);
	palette.Alloc(colors);
	if(colors <= 16)
		for(int i = 0; i < colors; i++) {
			palette[i].r = header.palette[i].r;
			palette[i].g = header.palette[i].g;
			palette[i].b = header.palette[i].b;
			palette[i].a = 255;
		}
	else {
		if(stream.GetLeft() < 768) {
			SetError();
			return false;
		}
		stream.SeekEnd(-768);
		for(int i = 0; i < 256; i++) {
			palette[i].r = stream.Get();
			palette[i].g = stream.Get();
			palette[i].b = stream.Get();
			palette[i].a = 255;
		}
		stream.Seek(sizeof(PCXHeader));
	}

	row_bytes = fmt.GetByteCount(size.cx);
	scanline.Alloc(row_bytes);
	buffer.Alloc(header.bytesPerLine * header.planes);
	return true;
}

Size PCXRaster::GetSize()
{
	return size;
}

Raster::Info PCXRaster::GetInfo()
{
	return info;
}

Raster::Line PCXRaster::GetLine(int line)
{
	Stream& stream = GetStream();
	byte *ptr = new byte[row_bytes];
	if(!IsError()) {
		byte *t = buffer;
		byte *e = buffer + header.bytesPerLine * header.planes;
		while(t < e) {
			byte b = stream.Get();
			if((b & 0xc0) == 0xc0) {
				int n = b & 0x3f;
				if(t + n > e)
					break;
				memset(t, stream.Get(), n);
				t += n;
			}
			else
				*t++ = b;
		}
		byte *bs = buffer;
		t = ptr;
		e = ptr + row_bytes;
		if(header.bitsPerPixel == 1)
			for(int i = 0; i < header.bytesPerLine; i++) {
				byte h[8];
				memset(h, 0, 8);
				byte *s = bs;
				for(int plane = 0; plane < header.planes; plane++) {
					byte sb = *s;
					byte bit = 1 << plane;
					for(int j = 0; j < 8; j++)
						if(sb & (0x80 >> j))
							h[j] |= bit;
					s += header.bytesPerLine;
				}
				for(int q = 0; q < 8; q++) {
					if(t >= e)
						break;
					*t++ = h[q];
				}
				bs++;
			}
		else
		if(header.bitsPerPixel == 8 && header.planes == 3) {
			for(int i = 0; i < size.cx; i++) {
				*t++ = buffer[i];
				*t++ = buffer[i + header.bytesPerLine];
				*t++ = buffer[i + 2 * header.bytesPerLine];
			}
		}
		else
		if(header.bitsPerPixel == 4) {
			for(;;) {
				if(t >= e) break;
				byte b = *bs++;
				*t++ = (b & 0xf0) >> 4;
				if(t >= e) break;
				*t++ = b & 0x0f;
			}
		}
		else
		if(header.bitsPerPixel == 8)
			memcpy(t, bs, row_bytes);
		else
			goto error;
		return Line(ptr, this, true);
	}
error:
	SetError();
	memset(ptr, 0, row_bytes);
	return Line(ptr, this, true);
}

int PCXRaster::GetPaletteCount()
{
	return fmt.GetPaletteCount();
}

RGBA *PCXRaster::GetPalette()
{
	return fmt.GetPaletteCount() ? ~palette : NULL;
}

const RasterFormat *PCXRaster::GetFormat()
{
	return &fmt;
}

}
