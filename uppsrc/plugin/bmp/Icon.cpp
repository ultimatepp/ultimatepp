#include "bmp.h"

namespace Upp {

#include "bmphdr.h"

Vector<Image> ReadIcon(String data, bool just_best)
{
	VectorMap<Size, Tuple2<int, Image> > best;
	Vector<Image> out;
	const byte *in = data;
	int count = Peek16le(in + OFFSETOF(BMP_ICONDIR, idCount));
	if(count < 0 || count > 100)
		return out;
	const byte *end = (const byte *)data.end();
	for(int i = 0; i < count; i++) {
		const byte *hdr = in + sizeof(BMP_ICONDIR) + i * sizeof(BMP_ICONDIRENTRY);
		if(hdr > end)
			return out;
		dword offset = Peek32le(hdr + OFFSETOF(BMP_ICONDIRENTRY, dwImageOffset));
		hdr = in + offset;
		int hdrsize = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biSize));
		if(hdr + hdrsize > end)
			return out;
		Size bmpsz(
			Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biWidth)),
			Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biHeight)));
		Size size(bmpsz.cx, bmpsz.cy >> 1);
		if(size.cx < 0 || size.cx > 10000 || size.cy < 0 || size.cy > 10000)
			return out;
		int compression = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biCompression));
		int bitcount = Peek16le(hdr + OFFSETOF(BMP_INFOHEADER, biBitCount));
		int clrused = Peek32le(hdr + OFFSETOF(BMP_INFOHEADER, biClrUsed));
		int rowbytes = ((bmpsz.cx * bitcount + 31) >> 3) & -4;
		int maskbytes = ((size.cx + 31) >> 3) & -4;

		hdr += hdrsize;
		RasterFormat fmt;
		switch(bitcount) {
		case 1: fmt.Set1mf(); break;
		case 4: fmt.Set4mf(); break;
		case 8: fmt.Set8(); break;
		case 16:
			if(compression == 3 /* BI_BITFIELD */) {
				fmt.Set16le(Peek32le(hdr), Peek32le(hdr + 4), Peek32le(hdr + 8));
				hdr += 12;
			}
			else
				fmt.Set16le(31 << 10, 31 << 5, 31);
			break;
		case 24:
			fmt.Set24le(0xff0000, 0x00ff00, 0x0000ff);
			break;
		case 32:
			if(compression == 3 /* BI_BITFIELD */) {
				fmt.Set32le(Peek32le(hdr), Peek32le(hdr + 4), Peek32le(hdr + 8));
				hdr += 12;
			}
			else
				fmt.Set32le(0xff0000, 0x00ff00, 0x0000ff, 0xff000000);
			break;
		}

		if(clrused == 0 && bitcount <= 8)
			clrused = 1 << bitcount;
		Vector<RGBA> palette;
		if(bitcount <= 8) {
			palette.SetCount(clrused);
			for(int i = 0; i < clrused; i++, hdr += 4) {
				RGBA rgba;
				rgba.r = hdr[OFFSETOF(BMP_RGB, rgbRed)];
				rgba.g = hdr[OFFSETOF(BMP_RGB, rgbGreen)];
				rgba.b = hdr[OFFSETOF(BMP_RGB, rgbBlue)];
				rgba.a = 255;
				palette[i] = rgba;
			}
		}

		ImageBuffer buffer(size);
		for(int y = 0; y < size.cy; y++, hdr += rowbytes) {
			if(hdr + rowbytes > end)
				return out;
			fmt.Read(buffer[size.cy - y - 1], hdr, size.cx, palette.Begin());
		}
		for(int y = 0; y < size.cy; y++, hdr += maskbytes) {
			if(hdr + maskbytes > end)
				return out;
			const byte *in = hdr;
			RGBA *out = buffer[size.cy - y - 1];
			int cx = size.cx;
			while(cx >= 8) {
				byte b = *in++;
				if(b & 0x80) out[0] = RGBAZero();
				if(b & 0x40) out[1] = RGBAZero();
				if(b & 0x20) out[2] = RGBAZero();
				if(b & 0x10) out[3] = RGBAZero();
				if(b & 0x08) out[4] = RGBAZero();
				if(b & 0x04) out[5] = RGBAZero();
				if(b & 0x02) out[6] = RGBAZero();
				if(b & 0x01) out[7] = RGBAZero();
				out += 8;
				cx -= 8;
			}
			if(cx) {
				byte b = *in++;
				do {
					if(b & 0x80) *out = RGBAZero();
					out++;
					b <<= 1;
				}
				while(--cx);
			}
		}
		Image m = buffer;
		if(just_best) {
			int q = best.Find(m.GetSize());
			auto x = MakeTuple(bitcount, m);
			if(q < 0)
				best.Add(m.GetSize(), x);
			else
			if(bitcount > best[q].a)
				best[q] = x;
		}
		else
			out.Add(m);
	}
	if(just_best)
		for(auto x : best)
			out.Add(x.b);
	return out;
}

String WriteIcon(const Vector<Image>& icons, int flags)
{
	Vector<byte> bpp;
	if(flags & WI_32BIT) bpp.Add(32);
	if(flags & WI_8BIT) bpp.Add(8);
	if(flags & WI_4BIT) bpp.Add(4);
	if(flags & WI_MONO) bpp.Add(1);
	ASSERT(!bpp.IsEmpty());
	int hdrsize =  sizeof(BMP_ICONDIR) + icons.GetCount() * bpp.GetCount() * sizeof(BMP_ICONDIRENTRY);
	StringBuffer out;
	out.SetCount(hdrsize);
	memset(~out, 0, hdrsize);
	Poke16le(~out + OFFSETOF(BMP_ICONDIR, idType), flags & WI_CURSOR ? 2 : 1);
	Poke16le(~out + OFFSETOF(BMP_ICONDIR, idCount), icons.GetCount() * bpp.GetCount());
	int icx = 0;
	for(int b = 0; b < bpp.GetCount(); b++) {
		int bits = bpp[b];
		RasterFormat format;
		switch(bits) {
			case 1: format.Set1mf(); break;
			case 4: format.Set4mf(); break;
			case 8: format.Set8(); break;
			default: format.Set32le(0xff0000, 0x00ff00, 0x0000ff, 0xff000000); break;
		}
		for(int i = 0; i < icons.GetCount(); i++) {
			int out_offset = out.GetLength();
			Image img = icons[i];
			Size sz = img.GetSize();
			int rowbytes = ((sz.cx * bits + 31) >> 3) & -4;
			int maskbytes = ((sz.cx + 31) >> 3) & -4;
			BMP_INFOHEADER bmih;
			Zero(bmih);
			bmih.biSize = sizeof(bmih);
			bmih.biWidth = sz.cx;
			bmih.biHeight = 2 * sz.cy;
			bmih.biBitCount = bpp[b];
			bmih.biPlanes = 1;
			bmih.biCompression = 0; //BI_RGB
			bmih.biSizeImage = sz.cy * (rowbytes + maskbytes);
			One<PaletteCv> cv;
			out.Cat((const char *)&bmih, sizeof(bmih));
			if(bits <= 8) {
				int ncolors = 1 << bits;
				Buffer<RGBA> palette(ncolors, RGBAZero());
				ImageRaster ir(img);
				CreatePalette(ir, ~palette, ncolors);
				cv = new PaletteCv();
				CreatePaletteCv(palette, ncolors, *cv);
				for(int c = 0; c < ncolors; c++) {
					BMP_RGB quad;
					quad.rgbRed = palette[c].r;
					quad.rgbGreen = palette[c].g;
					quad.rgbBlue = palette[c].b;
					quad.rgbReserved = 0;
					out.Cat((const char *)&quad, sizeof(quad));
				}
			}
			int bmpoff = out.GetLength();
			out.Cat(0, bmih.biSizeImage);
			byte *data = (byte *)~out + bmpoff;
			for(int y = sz.cy; --y >= 0; data += rowbytes)
				format.Write(data, img[y], sz.cx, ~cv);
			for(int y = sz.cy; --y >= 0; data += maskbytes) {
				int cx = sz.cx;
				byte *out = data;
				const RGBA *in = img[y];
				while(cx >= 8) {
					byte b = 0;
					if(!in[0].a) b |= 0x80;
					if(!in[1].a) b |= 0x40;
					if(!in[2].a) b |= 0x20;
					if(!in[3].a) b |= 0x10;
					if(!in[4].a) b |= 0x08;
					if(!in[5].a) b |= 0x04;
					if(!in[6].a) b |= 0x02;
					if(!in[7].a) b |= 0x01;
					in += 8;
					cx -= 8;
					*out++ = b;
				}
				if(cx) {
					byte b = 0, mask = 0x80;
					do {
						if(in++->a) b |= mask;
						mask >>= 1;
					}
					while(--cx);
					*out = b;
				}
			}
			char *entry = ~out + sizeof(BMP_ICONDIR) + sizeof(BMP_ICONDIRENTRY) * icx++;
			entry[OFFSETOF(BMP_ICONDIRENTRY, bWidth)] = sz.cx;
			entry[OFFSETOF(BMP_ICONDIRENTRY, bHeight)] = sz.cy;
			//entry[OFFSETOF(BMP_ICONDIRENTRY, bColorCount)];
			//Poke16le(entry + OFFSETOF(BMP_ICONDIRENTRY, wPlanes)
			//Poke16le(entry + OFFSETOF(BMP_ICONDIRENTRY, wBitCount)
			Poke32le(entry + OFFSETOF(BMP_ICONDIRENTRY, dwBytesInRes), out.GetLength() - out_offset);
			Poke32le(entry + OFFSETOF(BMP_ICONDIRENTRY, dwImageOffset), out_offset);
		}
	}
	return String(out);
}

}
