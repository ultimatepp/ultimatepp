#include "ide.h"

FileSelButton::FileSelButton(MODE mode, const char *title)
: title(title), mode(mode)
{
	button.NoWantFocus();
	button.SetImage(CtrlImg::right_arrow());
	button <<= THISBACK(OnAction);
}

void FileSelButton::OnAction()
{
	Ctrl *owner = button.GetParent();
	ASSERT(owner);
	String old = ~*owner;
	if(mode == MODE_DIR)
		ActiveDir(old);
	else
		Set(old);
	if(mode == MODE_OPEN ? ExecuteOpen(title) : mode == MODE_SAVE ? ExecuteSaveAs(title) : ExecuteSelectDir(title))
	{
		*owner <<= Get();
		owner->Action();
	}
}
