#include "ide.h"

// THIS IS SANDBOX FOR DEVELOPING NEW NAVIGATOR

struct Navigator : TopWindow {
	EditString search;
	ArrayCtrl  list;
	
	void Search();

	typedef Navigator CLASSNAME;

	Navigator();
};

struct NavigatorDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		CppItem m;
		LoadFromString(m, q);
		w.DrawRect(r, paper);
		bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
		if(IsNull(q)) return;
		int x = r.left;
		int fcy = Draw::GetStdFontCy();
		int y = r.top + r.GetHeight() / 2 - fcy;
		int x0 = x;
		Vector<ItemTextPart> n = ParseItemNatural(m.name, m, ~m.natural + m.at);
		int starti = 0;
		for(int i = 0; i < n.GetCount(); i++)
			if(n[i].type == ITEM_NAME) {
				starti = i;
				break;
			}
		PaintText(w, x, y, ~m.natural, n, 0, starti, focuscursor, ink, false);
		if(m.item.GetCount()) {
			String h = ' ' + m.item;
			h.Cat("::");
			w.DrawText(x, y, h, StdFont().Bold(), Magenta());
			x += GetTextSize(h, StdFont()).cx;
		}
		int x1 = r.left;
		PaintText(w, x1, y + fcy, ~m.natural, n, starti, n.GetCount(), focuscursor, ink, false);
	}
};

void Navigator::Search()
{
	String s = ~search;
	list.Clear();
	// TODO: Be smarter with '::'
	CppBase& b = CodeBase();
	for(int i = 0; i < b.GetCount(); i++) {
		String nest = b.GetKey(i);
		String unest = ToUpper(nest);
		Array<CppItem>& item = b[i];
		for(int j = 0; j < item.GetCount(); j++) {
			CppItem m;
			
			{ DTIMING("Copy");
			m = item[j]; }
			m.item = nest; // Ugly trick...
			String h = unest;
			{ DTIMING("CAT");
			h << "::" << m.uname; }
			int q;
			{ DTIMING("FIND");
			q = h.Find(s);
			}
			if(h.Find(s) >= 0) {
				String sh;
				{ DTIMING("StoreAsString");
					sh = StoreAsString(m);
				}
				DTIMING("Add");
				list.Add(nest, sh, h);
			}
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
	list.AddColumn().SetDisplay(Single<NavigatorDisplay>());
	list.AddColumn();
	list.SetLineCy(2 * GetStdFontCy());
	
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterNavigator);
}

void Ide::CodeBrowser()
{
	Navigator().Run();
}