#include "Browser.h"

void PaintText(Draw& w, int& x, int y, const char *text, const Vector<ItemTextPart>& n,
           int starti, int count, bool focuscursor, Color _ink, bool italic)
{
	for(int i = starti; i < count; i++) {
		const ItemTextPart& p = n[i];
		Font f = BrowserFont();
		Color ink = SColorText;
		switch(p.type) {
		case ITEM_PNAME:
			f.Bold();
		case ITEM_NUMBER:
			ink = Red;
			break;
		case ITEM_TNAME:
			ink = Green;
		case ITEM_NAME:
			f.Bold();
			break;
		case ITEM_UPP:
			ink = Cyan;
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
		case ITEM_SIGN:
			ink = LtBlue;
			break;
		}
		if(italic)
			f.Italic();
		Size fsz = GetTextSize(text + p.pos, f, p.len);
		w.DrawText(x, y, text + p.pos, f, focuscursor ? _ink : ink, p.len);
		x += fsz.cx;
	}
}

void PaintText(Draw& w, int& x, int y, const CppItemInfo& m, const Vector<ItemTextPart>& n,
           int starti, int count, bool focuscursor, Color _ink)
{
	PaintText(w, x, y, ~m.natural, n, starti, count, focuscursor, _ink, m.overed);
}

void PaintCppItemImage(Draw& w, int& x, int ry, int access, int kind, bool focuscursor)
{
	Image img = decode(access, PROTECTED, BrowserImg::mprotected(),
	                           PRIVATE, BrowserImg::mprivate(),
	                           WITHBODY, BrowserImg::impl(),
	                           Image());
	if(!IsNull(img))
		w.DrawImage(x, ry - img.GetHeight() / 2, img);
	x += 3;
	img = BrowserImg::unknown();
	Image bk;
	switch(kind) {
	case FUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case FUNCTION:
		img = BrowserImg::function();
		break;
	case INSTANCEFUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case INSTANCEFUNCTION:
		img = BrowserImg::instance_function();
		break;
	case CLASSFUNCTIONTEMPLATE:
		bk = BrowserImg::template_function();
	case CLASSFUNCTION:
		img = BrowserImg::class_function();
		break;
	case STRUCTTEMPLATE:
		bk = BrowserImg::template_struct();
	case STRUCT:
		img = BrowserImg::type_struct();
		break;
	case INSTANCEVARIABLE:
		img = BrowserImg::instance_data();
		break;
	case CLASSVARIABLE:
		img = BrowserImg::class_data();
		break;
	case VARIABLE:
		img = BrowserImg::data();
		break;
	case ENUM:
		img = BrowserImg::type_enum();
		break;
	case INLINEFRIEND:
		img = BrowserImg::inline_friend();
		break;
	case TYPEDEF:
		img = BrowserImg::type_def();
		break;
	case CONSTRUCTOR:
		img = BrowserImg::constructor();
		break;
	case DESTRUCTOR:
		img = BrowserImg::destructor();
		break;
	case MACRO:
		img = BrowserImg::macro();
		break;
	case FRIENDCLASS:
		img = BrowserImg::friend_class();
		break;
	case KIND_INCLUDEFILE:
		img = IdeCommonImg::Header();
		break;
	case KIND_INCLUDEFILE_ANY:
		img = CtrlImg::File();
		break;
	case KIND_INCLUDEFOLDER:
		img = CtrlImg::Dir();
		break;
	}

	int by = ry - bk.GetSize().cy / 2;
	int iy = ry - img.GetSize().cy / 2;

	if(focuscursor) {
		DrawHighlightImage(w, x, by, bk);
		w.DrawImage(x, iy, img);
	}
	else {
		w.DrawImage(x, by, bk);
		w.DrawImage(x, iy, img);
	}
}

int CppItemInfoDisplay::DoPaint(Draw& w, const Rect& r, const Value& q,
	                            Color _ink, Color paper, dword style) const
{
	const CppItemInfo& m = ValueTo<CppItemInfo>(q);
	w.DrawRect(r, paper);
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(IsNull(q)) return 0;
	int x = r.left;
	int ry = r.top + r.GetHeight() / 2;
	
	PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);	

	if(m.inherited) {
		w.DrawImage(x + 10, r.top, BrowserImg::inherited());
		for(int i = 1; i < min(m.inherited, 5); i++)
			w.DrawRect(x + 10, r.top + 7 + 2 * i, 7, 1, SColorText);
	}
	x += 16;
	int y = ry - Draw::GetStdFontCy() / 2;
	int x0 = x;
	Vector<ItemTextPart> n = ParseItemNatural(m);
	int starti = 0;
	if(namestart)
		for(int i = 0; i < n.GetCount(); i++)
			if(n[i].type == ITEM_NAME) {
				starti = i;
				break;
			}
	PaintText(w, x, y, m, n, starti, n.GetCount(), focuscursor, _ink);
	if(starti) {
		const char *h = " : ";
		w.DrawText(x, y, h, BrowserFont(), SColorText);
		x += GetTextSize(h, BrowserFont()).cx;
	}
	PaintText(w, x, y, m, n, 0, starti, focuscursor, _ink);
	if(m.virt || m.over)
		w.DrawRect(x0, r.bottom - 2, x - x0, 1, m.over ? m.virt ? LtRed : LtBlue : SColorText);
	if(m.inherited && m.IsType())
		w.DrawRect(r.left, r.top, r.Width(), 1, SColorDisabled);

	if(showtopic) {
		String k = MakeCodeRef(m.scope, m.qitem);
		int cnt = GetRefLinks(k).GetCount();
		if(cnt) {
			Size sz = BrowserImg::Ref().GetSize();
			int xx = r.right - sz.cx - 1;
			int yy = r.top + (r.Height() - sz.cy) / 2;
			DrawHighlightImage(w, xx, yy, BrowserImg::Ref());
			if(cnt > 1) {
				String txt = AsString(cnt);
				Font fnt = Arial(Ctrl::VertLayoutZoom(10)).Bold();
				Size tsz = GetTextSize(txt, fnt);
				Point p(xx + (sz.cx - tsz.cx) / 2, yy + (sz.cy - tsz.cy) / 2);
				for(int ax = -1; ax <= 1; ax++)
					for(int ay = -1; ay <= 1; ay++)
						w.DrawText(p.x + ax, p.y + ay, txt, fnt, White);
				w.DrawText(p.x, p.y, txt, fnt, Blue);
			}
			x += sz.cx + 3;
		}
	}

	return x;
}

void CppItemInfoDisplay::Paint(Draw& w, const Rect& r, const Value& q,
                                  Color _ink, Color paper, dword style) const {
	DoPaint(w, r, q, _ink, paper, style);
}

Size CppItemInfoDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Rect(0, 0, INT_MAX, INT_MAX), q, Null, Null, 0),
	            max(16, BrowserFont().Info().GetHeight()));
}
