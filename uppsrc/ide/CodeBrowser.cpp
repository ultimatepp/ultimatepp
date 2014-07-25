#include "ide.h"

// THIS IS SANDBOX FOR DEVELOPING NEW NAVIGATOR

struct Navigator : TopWindow {
	EditString search;
	ArrayCtrl  list;
	
	void Search();

	typedef Navigator CLASSNAME;

	Navigator();
};

void Navigator::Search()
{
	String s = ~search;
	list.Clear();
	const CppBase& b = CodeBase();
	for(int i = 0; i < b.GetCount(); i++) {
		String nest = b.GetKey(i);
		String unest = ToUpper(nest);
		const Array<CppItem>& item = b[i];
		for(int j = 0; j < item.GetCount(); j++) {
			const CppItem& m = item[j];
			String h = unest;
			h << "::" << m.uname;
			if(h.Find(s) >= 0)
				list.Add(nest, m.natural, h);
		}
	}
}

int CharFilterNavigator(int c)
{
	return IsAlNum(c) || c == '_' || c == ':' ? ToUpper(c) : 0;
}

Navigator::Navigator()
{
	int cy = search.GetMinSize().cy;
	Add(search.TopPos(0, cy).HSizePos());
	Add(list.VSizePos(cy, 0).HSizePos());
	list.NoHeader();
	list.AddColumn();
	list.AddColumn();
	list.AddColumn();
	list.SetLineCy(2 * GetStdFontCy());
	
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterNavigator);
}

void Ide::CodeBrowser()
{
	Navigator().Run();
}