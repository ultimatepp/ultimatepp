#include "Local.h"

#ifdef GUI_TELPP

NAMESPACE_UPP

#define LLOG(x)  LOG(x)
#define LDUMP(x) //DDUMP(x)

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	if(image.GetSerialId() != fbCursorImage.GetSerialId()) {
		fbCursorImage = image;
		fbCursorPos = Null;
		_TODO_
	}
}

END_UPP_NAMESPACE

#endif