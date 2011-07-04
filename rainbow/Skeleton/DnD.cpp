#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

// --------------------------------------------------------------------------------------------

Ptr<Ctrl> sDnDSource;

Ctrl * Ctrl::GetDragAndDropSource()
{
	return sDnDSource;
}

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

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
                        const VectorMap<String, ClipData>& data)
{
	return DND_NONE;
}

void Ctrl::SetSelectionSource(const char *fmts) {}

END_UPP_NAMESPACE

#endif
