#include "bmp.h"

namespace Upp {

#include "bmphdr.h"

bool LoadBMPHeader(Stream& stream, BMPHeader& header, bool file)
{
	if(!stream.IsOpen())
		return false;
	ASSERT(stream.IsLoading());
	if(file)
	{
		BMP_FILEHEADER bmfh;
		if(!stream.GetAll(&bmfh, sizeof(bmfh)))
			return false;
		bmfh.SwapEndian();
		if(bmfh.bfType != 'B' + 256 * 'M')
			return false;
	}
	Zero(header);
	if(!stream.GetAll(&header, sizeof(BMP_INFOHEADER)) || header.biSize < sizeof(BMP_INFOHEADER))
		return false;
	header.SwapEndian();
	if(header.biBitCount != 1 && header.biBitCount != 4 && header.biBitCount != 8
	&& header.biBitCount != 16 && header.biBitCount != 24 && header.biBitCount != 32)
		return false;
	if(header.biSizeImage == 0) {
		if(header.biCompression != 0 /* BI_RGB */)
			return false;
		header.biSizeImage = tabs(header.biHeight) * (((header.biWidth * header.biBitCount + 31) >> 3) & -4);
	}
	stream.SeekCur(header.biSize - sizeof(BMP_INFOHEADER));
	if(header.biBitCount <= 8) {
		if(!stream.GetAll(header.palette, (header.biClrUsed ? header.biClrUsed : 1 << header.biBitCount) * sizeof(BMP_RGB)))
			return false;
	}
	else if(header.biBitCount == 16 || header.biBitCount == 32)
	{ // prepare 16-bit rgb masks & shifts
		if(header.biCompression == 0 /* BI_RGB */)
			;
		else if(header.biCompression == 3 /* BI_BITFIELDS */)
		{ // read bitfield masks
			if(!stream.GetAll(header.palette, 12))
				return false;
		}
		else
			return false;
	}
	if(header.biBitCount >= 16 && header.biClrUsed != 0)
		stream.SeekCur(header.biClrUsed * sizeof(BMP_RGB));
	return true;
}

static Size GetDotSize(Size pixel_size, int xpm, int ypm)
{
	if(!xpm || !ypm)
		return Size(0, 0);
	static const double DOTS_PER_METER = 60000 / 2.54;
	return Size(fround(pixel_size.cx * DOTS_PER_METER / xpm), fround(pixel_size.cy * DOTS_PER_METER / ypm));
}

bool BMPRaster::Create()
{
	Stream& stream = GetStream();
	size = Size(0, 0);
	if(!stream.IsOpen()) {
		SetError();
		return false;
	}
	ASSERT(stream.IsLoading());
	BMPHeader header;
	if(!LoadBMPHeader(stream, header, file)) {
		SetError();
		return false;
	}
	switch(header.biBitCount) {
	case 1: fmt.Set1mf(); break;
	case 4: fmt.Set4mf(); break;
	case 8: fmt.Set8(); break;
	case 16:
		if(header.biCompression == 3 /* BI_BITFIELD */)
			fmt.Set16le(*(dword *)(header.palette + 0), *(dword *)(header.palette + 1), *(dword *)(header.palette + 2));
		else
			fmt.Set16le(31 << 10, 31 << 5, 31);
		break;
	case 24:
		fmt.Set24le(0xff0000, 0x00ff00, 0x0000ff);
		break;
	case 32:
		if(header.biCompression == 3 /* BI_BITFIELD */)
			fmt.Set32le(*(dword *)(header.palette + 0), *(dword *)(header.palette + 1), *(dword *)(header.palette + 2));
		else
			fmt.Set32le(0xff0000, 0x00ff00, 0x0000ff);
		break;
	}
	if(header.biBitCount <= 8) {
		int ncolors = 1 << header.biBitCount;
		palette.Alloc(ncolors);
		const BMP_RGB *q = header.palette;
		for(int i = 0; i < ncolors; i++) {
			palette[i].r = q->rgbRed;
			palette[i].g = q->rgbGreen;
			palette[i].b = q->rgbBlue;
			palette[i].a = 255;
			q++;
		}
	}

	size = Size(header.biWidth, tabs(header.biHeight));
	info.kind = IMAGE_OPAQUE;
	info.bpp = header.biBitCount;
	info.dots = GetDotSize(size, header.biXPelsPerMeter, header.biYPelsPerMeter);
	info.hotspot = Point(0, 0);
	info.colors = fmt.GetColorCount();
	topdown = header.biHeight < 0;
	row_bytes = (fmt.GetByteCount(size.cx) + 3) & ~3;
	scanline.Alloc(row_bytes);
	soff = stream.GetPos();

	if(header.biBitCount == 8 && header.biCompression == 1 /* BI_RLE8 */) {
		ImageBuffer ib(size);
		int x = 0;
		int y = 0;
		RGBA *t = ib[header.biHeight < 0 ? 0 : size.cy - 1];
		while(!stream.IsEof()) {
			int c = stream.Get();
			if(stream.IsEof())
				return false;
			if(c) {
				c = min(c, size.cx - c);
				int q = stream.Get();
				if(q < 0)
					return false;
				Fill(t, palette[q], c);
				t += c;
				x += c;
			}
			else {
				c = stream.Get();
				if(c < 0 || c == 1)
					break;
				if(c == 0) {
					x = 0;
					++y;
					if(y == size.cy)
						break;
					t = ib[header.biHeight < 0 ? y : size.cy - 1 - y];
				}
				else
				if(c == 2) {
					x += stream.Get();
					y += stream.Get();
					if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
						return false;
					t = ib[header.biHeight < 0 ? y : size.cy - 1 - y];
				}
				else {
					int al = c & 1;
					while(c--) {
						int q = stream.Get();
						if(q < 0 || x >= size.cx)
							return false;
						*t++ = palette[q];
						x++;
					}
					if(al)
						stream.Get();
				}
			}
		}
		rle = new ImageRaster(ib);
		return true;
	}
	if(header.biBitCount == 4 && header.biCompression == 2 /* BI_RLE4 */) {
		ImageBuffer ib(size);
		int x = 0;
		int y = 0;
		RGBA *t = ib[header.biHeight < 0 ? 0 : size.cy - 1];
		while(!stream.IsEof()) {
			int c = stream.Get();
			if(stream.IsEof())
				return false;
			if(c) {
				c = min(c, size.cx - x);
				int q = stream.Get();
				if(q < 0)
					return false;
				int q1 = q & 15;
				q = (q >> 4) & 15;
				x += c;
				while(c--) {
					*t++ = palette[q];
					Swap(q, q1);
				}
			}
			else {
				c = stream.Get();
				if(c < 0 || c == 1)
					break;
				if(c == 0) {
					x = 0;
					++y;
					if(y == size.cy)
						break;
					t = ib[header.biHeight < 0 ? y : size.cy - 1 - y];
				}
				else
				if(c == 2) {
					x += stream.Get();
					y += stream.Get();
					if(x < 0 || x >= size.cx || y < 0 || y >= size.cy)
						return false;
					t = ib[header.biHeight < 0 ? y : size.cy - 1 - y];
				}
				else {
					if(x + c >= size.cx)
						return false;
					x += c;
					int i = 0;
					while(i < c) {
						int q = stream.Get();
						if(q < 0)
							return false;
						*t++ = palette[(q >> 4) & 15];
						if(++i >= c) break;
						*t++ = palette[q & 15];
						++i;
					}
					c = 3 - (c & 3);
					while(c--)
						stream.Get();
				}
			}
		}
		rle = new ImageRaster(ib);
	}
	return true;
}

Size BMPRaster::GetSize()
{
	return size;
}

Raster::Info BMPRaster::GetInfo()
{
	return info;
}

Raster::Line BMPRaster::GetLine(int line)
{
	if(rle)
		return rle->GetLine(line);
	byte *ptr = new byte[row_bytes];
	if(!IsError()) {
		Stream& stream = GetStream();
		topdown = false;
		stream.Seek(soff + (topdown ? line * row_bytes : (size.cy - line - 1) * row_bytes));
		if(stream.GetAll(ptr, row_bytes))
			return Line(ptr, this, true);
		SetError();
	}
	memset(ptr, 0, row_bytes);
	return Line(ptr, this, true);
}

int BMPRaster::GetPaletteCount()
{
	return fmt.GetPaletteCount();
}

RGBA *BMPRaster::GetPalette()
{
	return fmt.GetPaletteCount() ? ~palette : NULL;
}

const RasterFormat *BMPRaster::GetFormat()
{
	return &fmt;
}

BMPRaster::~BMPRaster()
{
}

INITIALIZER(BMPRaster)
{
	StreamRaster::Register<BMPRaster>();
}

}
