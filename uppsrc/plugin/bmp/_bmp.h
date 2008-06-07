#ifndef _nImage__bmp_h_
#define _nImage__bmp_h_

#ifdef CPU_BIG_ENDIAD
#error "Fix big endian issues!"
#endif

#ifdef COMPILER_MSC
#pragma pack(push, 1)
#endif

struct BMP_FILEHEADER {
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;

	void    EndianSwap()
	{
#ifdef CPU_BIG_ENDIAN
		EndianSwap(bfType);
		EndianSwap(bfSize);
		EndianSwap(bfOffBits);
#endif
	}
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct BMP_INFOHEADER
{
	dword      biSize;
	long       biWidth;
	long       biHeight;
	word       biPlanes;
	word       biBitCount;
	dword      biCompression;
	dword      biSizeImage;
	long       biXPelsPerMeter;
	long       biYPelsPerMeter;
	dword      biClrUsed;
	dword      biClrImportant;

	void    EndianSwap()
	{
#ifdef CPU_BIG_ENDIAN
		EndianSwap(biSize);
		EndianSwap(biWidth);
		EndianSwap(biHeight);
		EndianSwap(biPlanes);
		EndianSwap(biBitCount);
		EndianSwap(biCompression);
		EndianSwap(biSizeImage);
		EndianSwap(biXPelsPerMeter);
		EndianSwap(biYPelsPerMeter);
		EndianSwap(biClrUsed);
		EndianSwap(biClrImportant);

#endif
	}
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct BMP_RGB
{
    byte    rgbBlue;
    byte    rgbGreen;
    byte    rgbRed;
    byte    rgbReserved;
};

struct ICONDIR
{
	word           idReserved;   // Reserved (must be 0)
	word           idType;       // Resource Type (1 for icons)
	word           idCount;      // How many images?

	void    EndianSwap()
	{
#ifdef CPU_BIG_ENDIAN
		EndianSwap(idReserved);
		EndianSwap(idType);
		EndianSwap(idCount);
#endif
	}
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct ICONDIRENTRY
{
	byte        bWidth;          // Width, in pixels, of the image
	byte        bHeight;         // Height, in pixels, of the image
	byte        bColorCount;
	byte        bReserved;
	short       wHotSpotX;
	short       wHotSpotY;
	dword       dwBytesInRes;    // How many bytes in this resource?
	dword       dwImageOffset;   // Where in the file is this image?

	void    EndianSwap()
	{
#ifdef CPU_BIG_ENDIAN
		EndianSwap(wHotSpotX);
		EndianSwap(wHotSpotY);
		EndianSwap(dwBytesInRes);
		EndianSwap(dwImageOffset);

#endif
	}
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

#ifdef COMPILER_MSC
#pragma pack(pop)
#endif

struct BMPHeader : public BMP_INFOHEADER
{
	BMP_RGB palette[256];
};

#endif
