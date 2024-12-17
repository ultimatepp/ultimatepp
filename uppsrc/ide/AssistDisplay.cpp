#include "ide.h"

// TODO: Move to (renamed) ide/Browser

Image CxxIcon(int kind)
{
	switch(kind) {
	case CXCursor_CXXMethod: return BrowserImg::instance_function();
	case CXCursor_Destructor: return BrowserImg::destructor();
	case CXCursor_EnumConstantDecl: return BrowserImg::type_enum();
	case CXCursor_ClassDecl: return BrowserImg::type_struct();
	case CXCursor_StructDecl: return BrowserImg::type_struct();
	case CXCursor_ClassTemplate: return BrowserImg::template_struct();
	case CXCursor_FunctionTemplate: return BrowserImg::template_function();
	case CXCursor_ConversionFunction: return BrowserImg::function();
	case CXCursor_FieldDecl: return BrowserImg::instance_data();
	case CXCursor_VarDecl: return BrowserImg::data();
	case CXCursor_TypedefDecl: return BrowserImg::type_def();
	case CXCursor_FunctionDecl: return BrowserImg::function();
	case CXCursor_Constructor: return BrowserImg::constructor();
	case CXCursor_EnumDecl: return BrowserImg::type_enum();
	case CXCursor_UnionDecl: return BrowserImg::type_struct();
	case CXCursor_TypeAliasDecl: return BrowserImg::type_def();
	case CXCursor_TypeAliasTemplateDecl: return BrowserImg::type_def();
	case CXCursor_MacroDefinition: return BrowserImg::macro();
	case KIND_INCLUDEFILE: return IdeCommonImg::Header();
	case KIND_INCLUDEFILE_ANY: return CtrlImg::File();
	case KIND_INCLUDEFOLDER: return CtrlImg::Dir();
	case KIND_ERROR: return IdeImg::errors();
	}
	return BrowserImg::unknown();
}

void PaintText(Draw& w, int& x, int y, const char *text, const Vector<ItemTextPart>& n,
               int starti, int count, bool focuscursor, Color _ink, bool italic)
{
	static int maxascent = MaxAscent(BrowserFont());
	for(int i = 0; i < count; i++) {
		const ItemTextPart& p = n[starti + i];
		Font f = BrowserFont();
		Color ink = SColorText;
		switch(p.type) {
		case ITEM_PNAME:
			f.Bold();
		case ITEM_NUMBER:
			ink = SRed();
			break;
		case ITEM_TNAME:
			ink = SGreen();
		case ITEM_NAME:
			f.Bold();
			break;
		case ITEM_UPP:
			ink = SCyan();
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
		case ITEM_OPERATOR:
		case ITEM_SIGN:
			ink = SLtBlue();
			break;
		}
		if(italic)
			f.Italic();
		Size fsz = GetTextSize(text + p.pos, f, p.len);
		w.DrawText(x, y + maxascent - f.GetAscent(), text + p.pos,
		           f, focuscursor ? _ink : ink, p.len);
		x += fsz.cx;
	}
}

int PaintCpp(Draw& w, const Rect& r, int kind, const String& name, const String& pretty, Color ink, bool focuscursor, bool retval_last)
{
	int x = r.left;
	
	Image img = CxxIcon(kind);
	Size isz = img.GetSize();
	w.DrawImage(x + (DPI(16) - isz.cx) / 2, r.top + (r.GetHeight() - isz.cy) / 2, img);

	x += DPI(16);
	int y = r.top + (r.GetHeight() - Draw::GetStdFontCy()) / 2;

#ifdef _DEBUG
	String ks = " [" + AsString(kind) + "] ";
	w.DrawText(x, y, ks);
	x += GetTextSize(ks, StdFont()).cx;
#endif

	Vector<ItemTextPart> n = ParsePretty(name, pretty);
	int count = n.GetCount();
	if(retval_last) {
		for(int i = 0; i < n.GetCount(); i++)
			if(findarg(n[i].type, ITEM_NAME, ITEM_OPERATOR) >= 0 || pretty[n[i].pos] == '(') {
				PaintText(w, x, y, pretty, n, i, count - i, focuscursor, ink, false);
				count = i;
				while(count) { // remove trailing spaces
					const ItemTextPart& p = n[count - 1];
					if(p.len == 1 && pretty[p.pos] == ' ')
						count--;
					else
						break;
				}
				if(count) {
					w.DrawText(x, y, " → ", StdFont(), SGray());
					x += GetTextSize(" → ", StdFont()).cx;
				}
				break;
			}
		if(count > 2) { // autocomplete sometimes fully qualifies the the name, looks ugly, remove XXX:: from the end
			ItemTextPart& p = n[count - 1];
			if(p.type == ITEM_CPP && p.len == 2 && pretty[p.pos] == ':' && pretty[p.pos + 1] == ':' &&
			   n[count - 2].type == ITEM_TEXT)
				count -= 2;
		}
	}
	PaintText(w, x, y, pretty, n, 0, count, focuscursor, ink, false);
	return x;
}

String CppText(const String& name, const String& pretty)
{ // converts pretty to text that is unique wrt PaintCpp - to avoid duplicities in autocomplete
	String g;
	Vector<ItemTextPart> n = ParsePretty(name, pretty);
	for(int i = 0; i < n.GetCount(); i++)
		if(findarg(n[i].type, ITEM_NAME, ITEM_OPERATOR) >= 0 || pretty[n[i].pos] == '(') {
			for(int j = i; j < n.GetCount(); j++)
				g << pretty.Mid(n[j].pos, n[j].len);
			while(i) {
				const ItemTextPart& p = n[i - 1];
				if(p.len == 1 && pretty[p.pos] == ' ')
					i--;
				else
					break;
			}
			g << '@';
			if(i > 2) { // autocomplete sometimes fully qualifies the the name, looks ugly, remove XXX:: from the end
				ItemTextPart& p = n[i - 1];
				if(p.type == ITEM_CPP && p.len == 2 && pretty[p.pos] == ':' && pretty[p.pos + 1] == ':' &&
				   n[i - 2].type == ITEM_TEXT)
					i -= 2;
			}
			for(int j = 0; j < i; j++)
				g << pretty.Mid(n[j].pos, n[j].len);
			break;
		}
	return TrimBoth(g);
}

void AssistEditor::AssistDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii >= 0 && ii < editor->assist_item_ndx.GetCount() && editor->assist_item_ndx[ii] < editor->assist_item.GetCount()) {
		AutoCompleteItem& m = editor->assist_item[editor->assist_item_ndx[ii]];

		w.DrawRect(r, paper);
		
		PaintCpp(w, r, m.kind, m.name, m.pretty, ink, (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT), true);
	}
}
