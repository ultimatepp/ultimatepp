#include "ide.h"

Image CxxIcon(int kind);

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
