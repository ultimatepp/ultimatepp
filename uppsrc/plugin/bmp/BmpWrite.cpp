#include "bmp.h"

namespace Upp {

#include "bmphdr.h"

int BMPEncoder::GetPaletteCount()
{
	if(bpp > 8 || grayscale) return 0;
	return bpp == 8 ? 256 : bpp == 4 ? 16 : 2;
}

void BMPEncoder::Start(Size size)
{
	BMPHeader header;
	Zero(header);
	header.biSize = sizeof(BMP_INFOHEADER);
	header.biWidth = size.cx;
	header.biHeight = size.cy;
	header.biBitCount = bpp;
	header.biPlanes = 1;
	header.biCompression = 0;
	int ncolors = 0;
	switch(bpp) {
	case 1:  format.Set1mf(); ncolors = 2; break;
	case 4:  format.Set4mf(); ncolors = 16; break;
	case 8:  format.Set8(); ncolors = 256; break;
	case 16: format.Set16le(0xf800, 0x07E0, 0x001f); break;
	case 32: format.Set32le(0xff0000, 0x00ff00, 0x0000ff); break;
	default: format.Set24le(0xff0000, 0x00ff00, 0x0000ff); break;
	}
	if(ncolors) {
		if(grayscale)
			for(int i = 0; i < ncolors; i++) {
				BMP_RGB& p = header.palette[i];
				p.rgbRed = p.rgbGreen = p.rgbBlue = 255 * i / (ncolors - 1);
			}
		else {
			const RGBA *palette = GetPalette();
			for(int i = 0; i < ncolors; i++) {
				BMP_RGB& p = header.palette[i];
				p.rgbRed = palette[i].r;
				p.rgbGreen = palette[i].g;
				p.rgbBlue = palette[i].b;
			}
		}
	}
	if(bpp == 16) {
		dword *bitfields = reinterpret_cast<dword *>(header.palette);
		bitfields[2] = 0x001f;
		bitfields[1] = 0x07E0;
		bitfields[0] = 0xf800;
		header.biCompression = 3/* BI_BITFIELDS */;
		ncolors = 3;
	}
	row_bytes = (format.GetByteCount(size.cx) + 3) & ~3;
	scanline.Alloc(row_bytes);
	header.biSizeImage = size.cy * row_bytes;
	Size dots = GetDots();
	if(dots.cx && dots.cy) {
		header.biXPelsPerMeter = fround(header.biWidth  * (1000.0 / 25.4 * 600.0) / dots.cx);
		header.biYPelsPerMeter = fround(header.biHeight * (1000.0 / 25.4 * 600.0) / dots.cy);
	}
	BMP_FILEHEADER bmfh;
	Zero(bmfh);
	bmfh.bfType = 'B' + 256 * 'M';
	bmfh.bfOffBits = sizeof(bmfh) + sizeof(BMP_INFOHEADER) + sizeof(BMP_RGB) * ncolors;
	bmfh.bfSize = sizeof(bmfh) + sizeof(BMP_INFOHEADER) + ncolors + header.biSizeImage;
	bmfh.SwapEndian();
	GetStream().Put(&bmfh, sizeof(bmfh));
	header.SwapEndian();
	int h = sizeof(BMP_INFOHEADER) + sizeof(BMP_RGB) * ncolors;
	GetStream().Put(&header, h);
	soff = GetStream().GetPos();
	GetStream().SetSize(sizeof(bmfh) + h + size.cy * row_bytes);
	linei = size.cy;
	linebytes = format.GetByteCount(size.cx);
}

void BMPEncoder::WriteLineRaw(const byte *s)
{
	GetStream().Seek(soff + row_bytes * --linei);
	memcpy(scanline, s, linebytes);
	GetStream().Put(scanline, row_bytes);
}

}
