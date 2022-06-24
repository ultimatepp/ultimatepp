#include "ide.h"

bool IsCppKeyword(const String& id);
bool IsCppType(const String& id);

static bool sOperatorTab[256];

INITBLOCK {
	for(const char *s = "!+-*^/%~&|=[]:?."; *s; s++)
		sOperatorTab[(int)*s] = true;
}

bool IsBasicType(const String& id)
{
	static Index<String> kt = { "__int16", "__int32", "__int64", "__int8", "auto", "char",
	                            "char8_t", "char16_t", "char32_t", "double", "float", "int",
	                            "long", "short", "unsigned", "void", "wchar_t" };
	return kt.Find(id) >= 0;
}

Vector<ItemTextPart> ParseSignature(const String& name, const String& signature, int *fn_info)
{
	Vector<ItemTextPart> part;
	int name_len = name.GetLength();
	if(name_len == 0) {
		ItemTextPart& p = part.Add();
		p.pos = 0;
		p.len = signature.GetLength();
		p.type = ITEM_TEXT;
		p.pari = -1;
		return part;
	}
	bool param = false;
	bool was_type = false;
	int pari = -1;
	int par = 0;
	int lastidi = -1;
	const char *s = signature;
	if(fn_info)
		*fn_info = -1;
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
			if(strncmp(s, name, name_len) == 0 && !iscid(s[name_len])) { // need strncmp because of e.g. operator++
				p.type = ITEM_NAME;
				n = name_len;
				const char *q = s + name_len;
				while(*q == ' ')
					q++;
				if(*q == '(') {
					param = true;
					if(fn_info)
						*fn_info = 0;
				}
			}
			else {
				String id;
				n = 0;
				while(iscid(s[n]))
					id.Cat(s[n++]);
				if(IsBasicType(id)) {
					if(param && par == 0)
						was_type = true;
					p.type = ITEM_CPP_TYPE;
				}
				else
				if(IsCppType(id))
					p.type = ITEM_CPP_TYPE;
				else
				if(IsCppKeyword(id))
					p.type = ITEM_CPP;
				else
				if(param) {
					if(lastidi >= 0 && par == 0)
						was_type = true;
					lastidi = part.GetCount() - 1; // can be a parameter name
				}
				if(param && fn_info)
					*fn_info = 1;
			}
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
					if(was_type)
						part[lastidi].type = ITEM_PNAME;
					lastidi = -1;
				}
				was_type = false;
			};
			p.type = ITEM_SIGN;
			if(pari >= 0) {
				if(*s == '(' || *s == '<')
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
				if(*s == '>')
					par--;
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
		Vector<ItemTextPart> n = ParseSignature(m.name, m.signature);

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

String SignatureQtf(const String& name, const String& signature, int pari)
{
	String qtf = "[%00-00K ";
	Vector<ItemTextPart> n = ParseSignature(name, signature);
	for(int i = 0; i < n.GetCount(); i++) {
		ItemTextPart& p = n[i];
		qtf << "[";
		if(p.pari == pari)
			qtf << "$y";
		switch(p.type) {
		case ITEM_PNAME:
			qtf << "*";
		case ITEM_NUMBER:
			qtf << "@r";
			break;
		case ITEM_TNAME:
			qtf << "*@g";
			break;
		case ITEM_NAME:
			qtf << "*";
			break;
		case ITEM_UPP:
			qtf << "@c";
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
			qtf << "@B";
			break;
		}
		qtf << ' ';
		qtf << '\1' << signature.Mid(p.pos, p.len) << '\1';
		qtf << ']';
	}
	return qtf + "]";
}
