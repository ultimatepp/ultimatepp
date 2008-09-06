#include "Browser.h"

void ReferenceDlg::EnterItem()
{
	String s = browser.GetItem();
	if(!IsNull(s))
		reference <<= s;
}

void ReferenceDlg::EnterItemOk()
{
	EnterItem();
	Break(IDOK);
}

void ReferenceDlg::Set(const String& s)
{
	StartBrowserBase();
	if(ExistsBrowserItem(s)) {
		browser.query.Clear();
		browser.Reload();
		browser.FindSet(s);
	}
	reference <<= s;
}

ReferenceDlg::ReferenceDlg()
{
	CtrlLayoutOKCancel(*this, "Reference");
	browser.WhenItem = THISBACK(EnterItem);
	browser.WhenItemDblClk = THISBACK(EnterItemOk);
	browser.show_inherited = false;
	browser.WithSearch(search_nest, search_item);
	query <<= callback(&browser, &Browser::DoDoQuery);
	Sizeable().Zoomable();
	Icon(TopicImg::Topic());
	Breaker(classlist, IDYES);
	StartBrowserBase();
	browser.Reload();
}
