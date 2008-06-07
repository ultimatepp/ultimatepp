#ifndef _nImage__bmp_h_
#define _nImage__bmp_h_

#ifdef COMPILER_MSC
#pragma pack(push, 1)
#endif

struct BMP_FILEHEADER {
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;

	void    SwapEndian()
	{
#ifdef CPU_BIG_ENDIAN
		bfType = UPP::SwapEndian(bfType);
		bfSize = UPP::SwapEndian(bfSize);
		bfOffBits = UPP::SwapEndian(bfOffBits);
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
	int32      biWidth;
	int32      biHeight;
	word       biPlanes;
	word       biBitCount;
	dword      biCompression;
	dword      biSizeImage;
	int32      biXPelsPerMeter;
	int32      biYPelsPerMeter;
	dword      biClrUsed;
	dword      biClrImportant;

	void    SwapEndian()
	{
#ifdef CPU_BIG_ENDIAN
		biSize = UPP::SwapEndian(biSize);
		biWidth = UPP::SwapEndian(biWidth);
		biHeight = UPP::SwapEndian(biHeight);
		biPlanes = UPP::SwapEndian(biPlanes);
		biBitCount = UPP::SwapEndian(biBitCount);
		biCompression = UPP::SwapEndian(biCompression);
		biSizeImage = UPP::SwapEndian(biSizeImage);
		biXPelsPerMeter = UPP::SwapEndian(biXPelsPerMeter);
		biYPelsPerMeter = UPP::SwapEndian(biYPelsPerMeter);
		biClrUsed = UPP::SwapEndian(biClrUsed);
		biClrImportant = UPP::SwapEndian(biClrImportant);
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

struct BMP_ICONDIR {
	word           idReserved;   // Reserved (must be 0)
	word           idType;       // Resource Type (1 for icons)
	word           idCount;      // How many images?
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct BMP_ICONDIRENTRY {
	byte        bWidth;          // Width, in pixels, of the image
	byte        bHeight;         // Height, in pixels, of the image
	byte        bColorCount;
	byte        bReserved;
	short       wHotSpotX;
	short       wHotSpotY;
	dword       dwBytesInRes;    // How many bytes in this resource?
	dword       dwImageOffset;   // Where in the file is this image?
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
