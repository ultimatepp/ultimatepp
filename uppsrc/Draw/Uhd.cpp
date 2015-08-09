#include "Draw.h"

namespace Upp {

static bool sUHDMode;

void SetUHDMode(bool b)
{
	for(int i = 0; i < GetImlCount(); i++)
		GetIml(i).Reset();
	sUHDMode = b;
}

bool IsUHDMode()
{
	return sUHDMode;
}

Image DPI(const Image& img)
{
	if(IsUHDMode()) {
		if(img.GetResolution() == IMAGE_RESOLUTION_STANDARD) {
			Image m = CachedRescale(img, 2 * img.GetSize(), FILTER_LANCZOS3);
			ImageBuffer h(m);
			h.SetResolution(IMAGE_RESOLUTION_UHD);
			return h;
		}
	}
	else {
		if(img.GetResolution() == IMAGE_RESOLUTION_UHD) {
			Image m = CachedRescale(img, img.GetSize() - 2, FILTER_LANCZOS3);
			ImageBuffer h(m);
			h.SetResolution(IMAGE_RESOLUTION_UHD);
			return h;
		}
	}
	return img;
}

Image DPI(const Image& img, int expected)
{
	if(img.GetSize().cy <= expected && IsUHDMode()) {
		Image m = CachedRescale(img, 2 * img.GetSize(), FILTER_LANCZOS3);
		ImageBuffer h(m);
		h.SetResolution(IMAGE_RESOLUTION_UHD);
		return h;
	}
	return img;
}

};