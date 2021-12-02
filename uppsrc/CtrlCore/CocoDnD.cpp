#include <CtrlCore/CtrlCore.h>

#ifdef PLATFORM_COCOA

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

// --------------------------------------------------------------------------------------------

Image MakeDragImage(const Image& arrow, Image sample);

Image MakeDragImage(const Image& arrow, const Image& arrow98, Image sample)
{
	return MakeDragImage(arrow, sample);
}

END_UPP_NAMESPACE

#endif
	