#include "ButtonItem.h"
#include "IMapRender.h"

void ButtonItem::StatePerformed(dword state, const String& param)
{
}

bool ButtonItem::ProcessLeftDown(Point p, dword keyflags)
{
	if (Contains(p))
	{
		if (GetParent() && GetParent()->IsState(STATE_SELECTED))
			PromptOK("Getted!");
		return true;
	}

	return false;
}
