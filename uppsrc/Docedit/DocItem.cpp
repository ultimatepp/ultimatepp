#include "Docedit.h"

#define IMAGECLASS DppImg
#define IMAGEFILE  <Docedit/Docedit.iml>
#include <Draw/iml_source.h>

bool IsCppKeyword(const String& id)
{
	static Index<String> kw;
	if(kw.GetCount() == 0)
		for(int i = 0; CppKeyword[i]; i++)
			kw.Add(CppKeyword[i]);
	return kw.Find(id) >= 0;
}

void DocItemDisplay::Paint(Draw& w, const Rect& r, const Value& q,
	                       Color _ink, Color paper, dword style) const
{
	ValueArray va = q;
	String txt = va.Get(0);
	String name = va.Get(1);
	int    kind = va.Get(2);
	int    access = va.Get(3);
	int    status = va.Get(4);
	String pname = va.Get(5);
	String tname = va.Get(6);
	Color c = Color(230, 255, 230);
	switch(status) {
	case DocDir::IGNORED: c = SGray; break;
	case DocDir::OBSOLETELINK:
	case DocDir::OBSOLETE: c = SLtRed; break;
	case DocDir::UNDOCUMENTED: c = paper; break;
	}
	w.DrawRect(r, style & (FOCUS|CURSOR) ? paper : c);
	if(IsNull(q)) return;
	int x0 = r.left;
	if(access == PROTECTED)
		w.DrawImage(x0, r.top + 1, DppImg::Protected());
	x0 += 6;
	Image img = DppImg::Other();
	switch(kind) {
	case FUNCTION:
		img = DppImg::Function();
		break;
	case INSTANCEFUNCTION:
		img = DppImg::InstanceFunction();
		break;
	case CLASSFUNCTION:
		img = DppImg::ClassFunction();
		break;
	case FUNCTIONTEMPLATE:
		img = DppImg::FunctionTemplate();
		break;
	case INSTANCEFUNCTIONTEMPLATE:
		img = DppImg::InstanceFunctionTemplate();
		break;
	case CLASSFUNCTIONTEMPLATE:
		img = DppImg::ClassFunctionTemplate();
		break;
	case STRUCT:
		img = DppImg::Struct();
		break;
	case STRUCTTEMPLATE:
		img = DppImg::StructTemplate();
		break;
	case INSTANCEVARIABLE:
		img = DppImg::InstanceVariable();
		break;
	case CLASSVARIABLE:
		img = DppImg::ClassVariable();
		break;
	case VARIABLE:
		img = DppImg::Variable();
		break;
	case ENUM:
		img = DppImg::Enum();
		break;
	case INLINEFRIEND:
		img = DppImg::InlineFriend();
		break;
	case TYPEDEF:
		img = DppImg::Typedef();
		break;
	case CONSTRUCTOR:
		img = DppImg::Constructor();
		break;
	case DESTRUCTOR:
		img = DppImg::Destructor();
		break;
	case MACRO:
		img = DppImg::Macro();
		break;
	}
	if(style & (CURSOR|FOCUS))
		DrawHighlightImage(w, x0, r.top + 1, img);
	else
		w.DrawImage(x0, r.top + 1, img);
	x0 += 20;
	int x = x0;
	int y = r.top + 2;
	const char *s = txt;
	while(*s && y < r.bottom) {
		Font f = Arial(11);
		Color ink = SBlack;
		int n = 1;
		if(*s >= '0' && *s <= '9') {
			while(s[n] >= '0' && s[n] <= '9')
				n++;
			ink = SRed;
		}
		else
		if(iscid(*s)) {
			if(strncmp(s, name, name.GetLength()) == 0 && !iscid(s[name.GetLength()])) {
				f = Arial(11).Bold();
				n = name.GetLength();
				name.Clear();
			}
			else {
				String id;
				n = 0;
				while(IsAlNum(s[n]) || s[n] == '_') {
					id.Cat(s[n]);
					n++;
				}
				if(IsCppKeyword(id))
					ink = SLtBlue;
				else
				if(InScList(id, pname)) {
					ink = SRed;
					f = Arial(11).Bold();
				}
				else
				if(InScList(id, tname)) {
					ink = SGreen;
					f = Arial(11).Bold();
				}
			}
		}
		else
			ink = SMagenta;
		if(style & (CURSOR|FOCUS)) ink = _ink;
		Size fsz = w.GetTextSize(s, f, n);
		if(x + fsz.cx >= r.right && fsz.cx < r.Width()) {
			if(x0 + 70 < r.right)
				x = x0 + 24;
			else
				x = x0;
			y += fsz.cy;
		}
		w.DrawText(x, y, s, f, ink, n);
		x += fsz.cx;
		s += n;
	}
}

void DocNestDisplay::Paint(Draw& w, const Rect& r, const Value& q,
	                       Color _ink, Color paper, dword style) const
{
	String text = q;
	w.DrawRect(r, paper);
	if(IsNull(text)) {
		w.DrawText(r.left, r.top, "Globals", Arial(11).Bold().Italic(),
		           style & (CURSOR|FOCUS) ? _ink : SBlue);
		return;
	}
	int x = r.left;
	int y = r.top;
	const char *s = text;
	Font  idf = Arial(11).Bold();
	Color idink = SBlack;
	while(*s && y < r.bottom) {
		Font f = Arial(11);
		Color ink = SBlack;
		int n = 1;
		if(*s >= '0' && *s <= '9') {
			while(s[n] >= '0' && s[n] <= '9')
				n++;
			ink = SRed;
		}
		else
		if(iscid(*s)) {
			String id;
			n = 0;
			while(IsAlNum(s[n]) || s[n] == '_') {
				id.Cat(s[n]);
				n++;
			}
			if(IsCppKeyword(id)) {
				ink = SLtBlue;
				if(id == "class" || id == "typename")
					idink = SGreen;
			}
			else {
				ink = idink;
				f = idf;
				idink = SBlack;
				idf = Arial(11);
			}
		}
		else {
			ink = SMagenta;
			if(*s == ':') {
				idf = Arial(11).Bold();
				idink = SBlack;
			}
		}
		if(style & (CURSOR|FOCUS)) ink = _ink;
		Size fsz = w.GetTextSize(s, f, n);
		w.DrawText(x, y, s, f, ink, n);
		x += fsz.cx;
		s += n;
	}
}

int GetItemHeight(const String& txt, const String& nm, const String& pname,
                  const String& tname, int cx)
{
	String name = nm;
	int x = 26;
	int y = ScreenInfo().GetFontInfo(Arial(11)).GetHeight() + 3;
	const char *s = txt;
	while(*s) {
		Font f = Arial(11);
		int n = 1;
		if(*s >= '0' && *s <= '9')
			while(s[n] >= '0' && s[n] <= '9')
				n++;
		else
		if(iscid(*s)) {
			if(strncmp(s, name, name.GetLength()) == 0 && !iscid(s[name.GetLength()])) {
				f = Arial(11).Bold();
				n = name.GetLength();
				name.Clear();
			}
			else {
				String id;
				n = 0;
				while(IsAlNum(s[n]) || s[n] == '_') {
					id.Cat(s[n]);
					n++;
				}
				if(!IsCppKeyword(id) && InScList(id, pname) || InScList(id, tname))
					f = Arial(11).Bold();
			}
		}
		Size fsz = ScreenInfo().GetTextSize(s, f, n);
		if(x + fsz.cx >= cx && fsz.cx < cx) {
			if(26 + 70 < cx)
				x = 26 + 24;
			else
				x = 26;
			y += fsz.cy;
		}
		x += fsz.cx;
		s += n;
	}
	return y;
}

void InitItemArray(ArrayCtrl& item)
{
	item.NoHeader();
	item.AddIndex();
	item.AddIndex().Accel();
	item.AddIndex();
	item.AddIndex();
	item.AddIndex();
	item.AddIndex();
	item.AddIndex();
	item.AddIndex();
	item.AddIndex();
	item.AddColumnAt(0, "").Add(1).Add(2).Add(3).Add(4).Add(5).Add(6).Add(7).Add(8)
	    .SetDisplay(Single<DocItemDisplay>()).Accel().HeaderTab().SetMargin(0);
	item.HeaderObject().Tab(0).SetMargin(0);
}

void LoadItems(const DocSet& set, const String& nameing, const String& nesting, ArrayCtrl& item)
{
	const ArrayMap<String, DocItem>& nt = set.Get(~nameing).Get(nesting);
	item.Clear();
	int cx = item.HeaderObject().GetTabWidth(0) - 2 * item.HeaderObject().Tab(0).GetMargin();
	for(int i = 0; i < nt.GetCount(); i++) {
		const DocItem& im = nt[i];
		if(im.cppitem) {
			item.Add(nt.GetKey(i), im.cppitem->name, im.cppitem->kind, im.cppitem->access,
			         im.status, im.cppitem->pname, im.cppitem->tname,
			         im.package, im.cppitem->file, im.cppitem->line);
			item.SetLineCy(item.GetCount() - 1,
			               GetItemHeight(nt.GetKey(i), im.cppitem->name,
			                             im.cppitem->pname, im.cppitem->tname, cx));
		}
		else {
			item.Add(nt.GetKey(i), Null, Null, Null, im.status, Null, Null, im.package,
			         Null, Null);
			item.SetLineCy(item.GetCount() - 1,
			               GetItemHeight(nt.GetKey(i), Null, Null, Null, cx));
		}
	}
}

int ByOrder(int a1, int a2, int *order)
{
	if(a1 == a2) return 0;
	while(*order != -1) {
		if(a1 == *order) return -1;
		if(a2 == *order) return  1;
		order++;
	}
	return 0;
}

int CompareItems(const Vector<Value>& row1, const Vector<Value>& row2)
{
	int q;
	int status_order[] = {
		STATUS_UNDOCUMENTED, STATUS_OBSOLETE, STATUS_IGNORED, STATUS_EXTERNAL, STATUS_NORMAL,
		-1
	};
	int kind1 = row1[2];
	int kind2 = row2[2];
	if(kind1 != STRUCT && kind2 != STRUCT && kind1 != STRUCTTEMPLATE && kind2 != STRUCTTEMPLATE) {
		q = (int)row2[3] - (int)row1[3];
		if(q) return q;
		q = ByOrder(row1[4], row2[4], status_order);
		if(q) return q;
	}
	int kind_order[] = {
		STRUCT, STRUCTTEMPLATE,
		MACRO,
		CONSTRUCTOR, DESTRUCTOR,
		ENUM, TYPEDEF,
		INSTANCEFUNCTION, CLASSFUNCTION, INLINEFRIEND,
		INSTANCEVARIABLE, CLASSVARIABLE,
		INSTANCEFUNCTION, INSTANCEVARIABLE,
		VARIABLE, FUNCTION, FUNCTIONTEMPLATE,
		-1
	};
	q = ByOrder(kind1, kind2, kind_order);
	if(q) return q;
	return strcmp((String)row1[1], (String)row2[1]);
}

int CompareNests(const Value& v1, const Value& v2)
{
	return strcmp(String(v1), String(v2));
}
