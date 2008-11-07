#include "Browser.h"

void ReferenceDlg::EnterItem()
{
	reference <<= GetCodeRef();
}

void ReferenceDlg::EnterItemOk()
{
	if(item.IsCursor()) {
		EnterItem();
		Break(IDOK);
	}
}

void ReferenceDlg::Set(const String& s)
{
	Goto(s, Null);
	reference <<= s;
}

ReferenceDlg::ReferenceDlg()
{
	CtrlLayoutOKCancel(*this, "Reference");
	Breaker(classlist, IDYES);
	item.WhenEnterRow = THISBACK(EnterItem);
	item.WhenLeftDouble = THISBACK(EnterItemOk);
	Sizeable().Zoomable();
	Icon(TopicImg::Topic());
	display.showtopic = true;
	Load();
}
