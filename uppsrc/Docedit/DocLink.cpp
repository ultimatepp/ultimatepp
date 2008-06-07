#include "Docedit.h"

void DocLinkDlg::Nameing()
{
	nesting.Clear();
	if(IsNull(nameing)) {
		Nesting();
		return;
	}
	const VectorMap<String, ArrayMap<String, DocItem> >& ns = set->Get(~nameing);
	nesting.Clear();
	for(int i = 0; i < ns.GetCount(); i++)
		nesting.Add(ns.GetKey(i));
	nesting.Sort(0, CompareNests);
	nesting.GoBegin();
	Nesting();
}

void DocLinkDlg::Nesting()
{
	item.Clear();
	if(!nesting.IsCursor()) {
		Item();
		return;
	}
	LoadItems(*set, ~nameing, nesting.GetKey(), item);
	if(sort)
		item.Sort(CompareItems);
	item.GoBegin();
}

String DocLinkDlg::Get()
{
	return "dpp://" + String(~nameing) + "/" + String(nesting.GetKey()) + "/"
	       + String(item.GetKey());
}

void DocLinkDlg::Item()
{
	link <<= Get();
	String s = doc_dir.GetText(DocKey(~nameing, nesting.GetKey(), item.GetKey(), lang));
	label.KillCursor();
	label.Clear();
	label.Disable();
	if(IsNull(s)) return;
	Array<RichPosInfo> pi = ParseQTF(s).GetPosInfo(Rect(0, 0, 5000, 5000), PageY());
	for(int i = 0; i < pi.GetCount(); i++)
		if(pi[i].type == RichPosInfo::LABEL)
			label.Add(FromUnicode(pi[i].data, CHARSET_WIN1252));
	if(label.GetCount())
		label.Enable();
}

void DocLinkDlg::Label()
{
	if(label.IsCursor())
		link <<= Get() + "#" + String(label.GetKey());
}

bool DocLinkDlg::Perform(const DocSet& _set, bool _sort, String& _link, dword _lang,
                         const String& _nameing, const String& _nesting)
{
	lang = _lang;
	set = &_set;
	sort = _sort;
	nameing.Clear();
	for(int i = 0; i < _set.GetCount(); i++)
		nameing.Add(_set.GetKey(i));
	if(nameing.GetCount())
		nameing.SetIndex(0);
	if(!IsNull(_nameing) && nameing.HasKey(_nameing))
		nameing <<= _nameing;
	Nameing();
	if(!IsNull(_nesting))
		nesting.FindSetCursor(_nesting);
	link <<= _link;
	link.SetFocus();
	if(Execute() == IDOK) {
		link.AddHistory();
		_link = link;
		return true;
	}
	return false;
}

DocLinkDlg::DocLinkDlg()
{
	CtrlLayoutOKCancel(*this, "Link");
	nesting.AddColumn().SetDisplay(Single<DocNestDisplay>());
	nesting.NoHeader().NoGrid();
	nameing <<= THISBACK(Nameing);
	nesting.WhenEnterRow = THISBACK(Nesting);
	InitItemArray(item);
	item.WhenLeftClick = THISBACK(Item);
	label.NoHeader().NoGrid();
	label.AddColumn();
	label.WhenLeftClick = THISBACK(Label);
}

bool ParseLink(const String& link, DocKey& key, String& label)
{
	if(memcmp(link, "dpp://", 6)) return false;
	Vector<String> p = Split(~link + 6, '/', false);
	if(p.GetCount() != 3) return false;
	key.nameing = p[0];
	key.nesting = p[1];
	key.item = p[2];
	int q = key.item.Find('#');
	label = Null;
	if(q < 0) return true;
	label = key.item.Mid(q + 1);
	key.item = key.item.Mid(0, q);
	return true;
}
