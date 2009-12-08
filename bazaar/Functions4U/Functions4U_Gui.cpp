#ifdef flagGUI

#include <CtrlLib/CtrlLib.h>

#include "Functions4U.h"
#include "Functions4U/Functions4U_Gui.h"

inline const RGBA *GetPixel(Image &img, int x, int y) {
	return img + x + y*img.GetWidth();
}

inline RGBA *GetPixel(ImageBuffer &img, int x, int y) {
	return img + x + y*img.GetWidth();
}

Image Rotate180(const Image& orig) {
	Size sz = orig.GetSize();
	ImageBuffer dest(sz);
	for(int rw = 0; rw < sz.cy; rw++)
		for(int cl = 0; cl < sz.cx; cl++)
			dest[rw][cl] = orig[sz.cy - rw - 1][sz.cx - cl - 1];
	return dest;
}

#endif
