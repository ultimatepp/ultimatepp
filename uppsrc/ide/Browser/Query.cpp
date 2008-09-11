#include "Browser.h"

void BrowserQuery::Clear()
{
	name.Clear();
	package.Clear();
	file.Clear();
	a_private = a_protected = a_public = true;
	code = data = type = macro = true;
	documented = notdocumented = true;
}

void BrowserQuery::Serialize(Stream& s)
{
	s % name % package % file;
	s % a_private % a_protected % a_public;
	s % code % data % type % macro;
	s % documented % notdocumented;
}

void QueryDlg::Clear()
{
	name <<= Null;
	package.SetCursor(0);
}

void QueryDlg::EnterPackage()
{
	file.Clear();
	int q = package.GetCursor();
	file.Enable(q);
	if(q) {
		file.Add("<all>", BrowserImg::All(), StdFont().Bold(), SColorDisabled);
		const Package& p = GetIdeWorkspace().GetPackage(q - 1);
		for(int i = 0; i < p.GetCount(); i++)
			file.Add(p[i], IdeFileImage(p[i]));
		file.SetCursor(0);
	}
}

QueryDlg::QueryDlg()
{
	CtrlLayoutOKCancel(*this, "Browser++ query");
	ActiveFocus(name);
	package.WhenEnterItem = THISBACK(EnterPackage);
	clear <<= THISBACK(Clear);
	Sizeable().Zoomable();
}

int QueryDlg::Perform(BrowserQuery& q)
{
	const Workspace& wspc = GetIdeWorkspace();
	package.Clear();
	package.Add("<all>", BrowserImg::All(), StdFont().Bold(), SColorDisabled);
	for(int i = 0; i < wspc.GetCount(); i++)
		package.Add(wspc[i], IdeCommonImg::Package());
	name <<= q.name;
	if(package.FindSetCursor(q.package)) {
		if(!file.FindSetCursor(q.file))
			file.SetCursor(0);
	}
	else
		package.SetCursor(0);
	code = q.code;
	data = q.data;
	type = q.type;
	macro = q.macro;
	a_protected = q.a_protected;
	a_private = q.a_private;
	a_public = q.a_public;
	documented = q.documented;
	notdocumented = q.notdocumented;
	if(Execute() != IDOK)
		return false;
	q.name = ~name;
	q.code = code;
	q.data = data;
	q.type = type;
	q.macro = macro;
	q.a_protected = a_protected;
	q.a_private = a_private;
	q.a_public = a_public;
	q.documented = documented;
	q.notdocumented = notdocumented;
	name.AddHistory();
	q.file = file.GetCursor() > 0 ? file.GetCurrentName() : String();
	q.package = package.GetCursor() > 0 ? package.GetCurrentName() : String();
	return true;
}

void QueryDlg::Serialize(Stream& s)
{
	name.SerializeList(s);
	s % name;
	s % package % file;
	SerializePlacement(s);
}

bool Browser::DoQuery()
{
	if(querydlg.Perform(query) != IDOK)
		return false;
	Reload();
	nesting.GoBegin();
	return true;
}
