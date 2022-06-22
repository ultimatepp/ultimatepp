#include "ide.h"

bool IsCppKeyword(const String& id);
bool IsCppType(const String& id);

static bool sOperatorTab[256];

INITBLOCK {
	for(const char *s = "!+-*^/%~&|=[]:?."; *s; s++)
		sOperatorTab[(int)*s] = true;
}

Vector<ItemTextPart> ParseSignature(const String& name, const String& signature, int& paramcount)
{
	Vector<ItemTextPart> part;
	int len = name.GetLength();
	if(len == 0) {
		ItemTextPart& p = part.Add();
		p.pos = 0;
		p.len = signature.GetLength();
		p.type = ITEM_TEXT;
		p.pari = -1;
		return part;
	}
	bool param = false;
	int pari = -1;
	int par = 0;
	int lastidi = -1;
	const char *s = signature;
	while(*s) {
		ItemTextPart& p = part.Add();
		p.pos = (int)(s - ~signature);
		p.type = ITEM_TEXT;
		p.pari = pari;
		int n = 1;
		if(*s >= '0' && *s <= '9') {
			while(s[n] >= '0' && s[n] <= '9' || (s[n] == 'x' || s[n] == 'X'))
				n++;
			p.type = ITEM_NUMBER;
		}
		else
		if(iscid(*s)) {
			String id;
			n = 0;
			while(iscid(s[n]))
				id.Cat(s[n++]);
			if(id == name) {
				p.type = ITEM_NAME;
				param = true;
			}
			else
			if(IsCppType(id))
				p.type = ITEM_CPP_TYPE;
			else
			if(IsCppKeyword(id))
				p.type = ITEM_CPP;
			else
				lastidi = part.GetCount() - 1; // can be a parameter name
		}
		else
		if(sOperatorTab[*s]) {
			while(sOperatorTab[s[n]])
				n++;
			p.type = ITEM_CPP;
		}
		else {
			auto Pname = [&] {
				if(lastidi >= 0) {
					part[lastidi].type = ITEM_PNAME;
					lastidi = -1;
				}
			};
			p.type = ITEM_SIGN;
			if(pari >= 0) {
				if(*s == '(')
					par++;
				if(*s == ')') {
					par--;
					if(par < 0) {
						p.pari = -1;
						pari = -1;
						param = false;
						Pname();
					}
				}
				if(*s == ',' && par == 0) {
					Pname();
					p.pari = -1;
					pari++;
				}
			}
			else
			if(*s == '(' && param) {
				pari = 0;
				par = 0;
			}
		}
		p.len = n;
		s += n;
	}
	return part;
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
//		PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);
	
		x += Zx(16);
		int y = ry - Draw::GetStdFontCy() / 2;
		int x0 = x;
		int dummy;
		Vector<ItemTextPart> n = ParseSignature(m.name, m.signature, dummy);

#if 0
		DLOG("========================");
		LOG(m.signature);
		for(auto& h : n)
			LOG(m.signature.Mid(h.pos, h.len) << " " << h.pari);
#endif
		int starti = 0;
/*		if(namestart)
			for(int i = 0; i < n.GetCount(); i++)
				if(n[i].type == ITEM_NAME) {
					starti = i;
					break;
				}
*/		PaintText(w, x, y, m.signature, n, starti, n.GetCount(), focuscursor, ink, false);

//		StdDisplay().Paint(w, r, m.signature, ink, paper, style);
	}
}
