#include "ide.h"

#if 0
// THIS IS SANDBOX FOR DEVELOPING NEW NAVIGATOR

struct Navigator : TopWindow {
	EditString search;
	ArrayCtrl  list;
	
	struct NavItem {
		String         nest;
		String         name;
		String         uname;
		String         natural;
		String         type;
		String         pname;
		String         ptype;
		String         tname;
		String         ctname;
		byte           access;
		byte           kind;
		int16          at;
		int            line;
		int            file;
		bool           impl;
		
		void Set(const CppItem& m);
	};
	
	struct NavigatorDisplay : Display {
		const Array<NavItem>& item;
	
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	
		NavigatorDisplay(const Array<NavItem>& item) : item(item) {}
	};

	Array<NavItem>   item;
	NavigatorDisplay navidisplay;
	
	void Search();

	typedef Navigator CLASSNAME;

	Navigator();
};

void Navigator::NavItem::Set(const CppItem& m)
{
	name = m.name;
	uname = m.uname;
	natural = m.natural;
	type = m.type;
	pname = m.pname;
	ptype = m.ptype;
	tname = m.tname;
	ctname = m.ctname;
	access = m.access;
	kind = m.kind;
	at = m.at;
	line = m.line;
	file = m.file;
	impl = m.impl;
}

void Navigator::NavigatorDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return;
	const NavItem& m = item[(int)q];
	w.DrawRect(r, paper);
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(IsNull(q)) return;
	int x = r.left;
	int fcy = Draw::GetStdFontCy();
	int y = r.top + r.GetHeight() / 2 - fcy;
	int x0 = x;
	Vector<ItemTextPart> n = ParseItemNatural(m.name, m.natural, m.ptype, m.pname, m.type,
	                                          m.tname, m.ctname, ~m.natural + m.at);
	int starti = 0;
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].type == ITEM_NAME) {
			starti = i;
			break;
		}
	PaintText(w, x, y, ~m.natural, n, 0, starti, focuscursor, ink, false);
	if(m.nest.GetCount()) {
		String h = ' ' + m.nest;
		h.Cat("::");
		w.DrawText(x, y, h, StdFont().Bold(), Magenta());
		x += GetTextSize(h, StdFont()).cx;
	}
	int x1 = r.left;
	PaintText(w, x1, y + fcy, ~m.natural, n, starti, n.GetCount(), focuscursor, ink, false);
}

void Navigator::Search()
{
	String s = ~search;
	list.Clear();
	item.Clear();
	const CppBase& b = CodeBase();
	ArrayMap<String, NavItem> imap;
	for(int i = 0; i < b.GetCount(); i++) {
		String nest = b.GetKey(i);
		String unest = ToUpper(nest);
		const Array<CppItem>& ci = b[i];
		for(int j = 0; j < ci.GetCount(); j++) {
			const CppItem& m = ci[j];
			String h = unest;
			h << "::" << m.uname;
			if(h.Find(s) >= 0) {
				String key = nest + '\1' + m.qitem;
				int q = imap.Find(key);
				if(q < 0) {
					NavItem& ni = imap.Add(key);
					ni.Set(m);
					ni.nest = nest;
				}
				else {
					NavItem& mm = imap[q];
					String n = mm.natural;
					if(m.natural.GetCount() > mm.natural.GetCount())
						mm.natural = m.natural;
					if(CombineCompare(mm.impl, m.impl)(m.file, mm.file)(m.line, mm.line) < 0)
						mm.Set(m);
				}
			}
		}
	}
	item = imap.PickValues();
	list.SetVirtualCount(item.GetCount());
}

int CharFilterNavigator(int c)
{
	return IsAlNum(c) || c == '_' || c == ':' ? ToUpper(c) : 0;
}

Navigator::Navigator()
:	navidisplay(item)
{
	int cy = search.GetMinSize().cy;
	Add(search.TopPos(0, cy).HSizePos());
	Add(list.VSizePos(cy, 0).HSizePos());
	list.NoHeader();
	list.AddRowNumColumn().SetDisplay(navidisplay);
	list.SetLineCy(2 * GetStdFontCy());
	
	search <<= THISBACK(Search);
	search.SetFilter(CharFilterNavigator);
}

void Ide::CodeBrowser()
{
	Navigator().Run();
}
#endif