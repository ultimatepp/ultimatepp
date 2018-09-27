#include <CtrlCore/CtrlCore.h>

#ifdef PLATFORM_COCOA

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

// --------------------------------------------------------------------------------------------

Image MakeDragImage(const Image& arrow, Image sample);

Image MakeDragImage(const Image& arrow, const Image& arrow98, Image sample)
{
#ifdef PLATFORM_WIN32
	if(IsWin2K())
		return MakeDragImage(arrow, sample);
	else
#endif
		return arrow98;
}

END_UPP_NAMESPACE

#endif
	