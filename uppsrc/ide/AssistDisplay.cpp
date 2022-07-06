#include "ide.h"

Image CxxIcon(int kind)
{
	switch(kind) {
	case CXCursor_CXXMethod: return BrowserImg::instance_function();
	case CXCursor_Destructor: return BrowserImg::destructor();
	case CXCursor_EnumConstantDecl: return BrowserImg::type_enum();
	case CXCursor_ClassDecl: return BrowserImg::type_struct();
	case CXCursor_StructDecl: return BrowserImg::type_struct();
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
	}
	return BrowserImg::unknown();
}

void AssistEditor::AssistDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii >= 0 && ii < editor->assist_item_ndx.GetCount()) {
		AutoCompleteItem& m = editor->assist_item[editor->assist_item_ndx[ii]];

		w.DrawRect(r, paper);
		bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);

		int x = r.left;
		int ry = r.top + r.GetHeight() / 2;
		
		Image img = CxxIcon(m.kind);
		Size isz = img.GetSize();
		w.DrawImage(x + (DPI(16) - isz.cx) / 2, r.top + (r.GetHeight() - isz.cy) / 2, img);
	
		x += DPI(16);
		int y = ry - Draw::GetStdFontCy() / 2;
	#ifdef _DEBUG
		String ks = " [" + AsString(m.kind) + "] ";
		w.DrawText(x, y, ks);
		x += GetTextSize(ks, StdFont()).cx;
	#endif
		int x0 = x;
		Vector<ItemTextPart> n = ParseSignature(m.name, m.signature);

		int starti = 0;
		PaintText(w, x, y, m.signature, n, starti, n.GetCount(), focuscursor, ink, false);
	}
}
