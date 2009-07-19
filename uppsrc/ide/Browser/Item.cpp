#include "Browser.h"

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_source.h>

#define LLOG(x) // DLOG(x)

bool IsCppKeyword(const String& id)
{
	static Index<String> kw;
	ONCELOCK {
		const char **cppk = CppKeyword();
		for(int i = 0; cppk[i]; i++)
			kw.Add(cppk[i]);
	}
	return kw.Find(id) >= 0;
}

bool IsCppType(const String& id)
{
	static const char *t[] = {
		"int", "long", "short", "void", "float", "double", "char", "signed", "unsigned", "bool",
	    "const", "mutable", "struct", "class", "union"
	};
	static Index<String> kt;
	ONCELOCK {
		for(int i = 0; i < __countof(t); i++)
			kt.Add(t[i]);
	}
	return kt.Find(id) >= 0;
}

int InScListIndext(const char *s, const char *list)
{
	int ii = 0;
	for(;;) {
		const char *q = s;
		while(*list == ' ') list++;
		for(;;) {
			if(*q == '\0' && *list == '\0') return ii;
			if(*q != *list) {
				if(*q == '\0' && (*list == '<' || *list == ';' || *list == ',' || *list == '>'))
					return ii;
				if(*list == '\0') return -1;
				break;
			}
			q++;
			list++;
		}
		while(*list && *list != ';' && *list != '<' && *list != '>' && *list != ',') list++;
		if(*list == '\0') return -1;
		list++;
		ii++;
	}
}

static String s_pick_("pick_");

static bool sOperatorTab[256];

INITBLOCK {
	for(const char *s = "!+-*^/%~&|=[]:?."; *s; s++)
		sOperatorTab[(int)*s] = true;
}

inline bool sOperator(byte c)
{
	return sOperatorTab[c];
}

Vector<ItemTextPart> ParseItemNatural(const String& name, const CppItem& m, const char *s)
{
	LLOG("ParseItemNatural " << m.natural << ", pname: " << m.pname
	                         << ", tname: " << m.tname << ", m.ctname: " << m.ctname);
	Vector<ItemTextPart> part;
	bool param = false;
	int pari = -1;
	int par = 0;
	while(*s) {
		ItemTextPart& p = part.Add();
		p.pos = (int)(s - ~m.natural);
		p.type = ITEM_TEXT;
		p.pari = pari;
		int n = 1;
		if(*s >= '0' && *s <= '9') {
			while(s[n] >= '0' && s[n] <= '9')
				n++;
			p.type = ITEM_NUMBER;
		}
		else
		if(iscid(*s) || *s == ':') {
			if(strncmp(s, name, name.GetLength()) == 0 && !iscid(s[name.GetLength()])) {
				p.type = ITEM_NAME;
				n = name.GetLength();
				param = true;
			}
			else {
				String id;
				n = 0;
				while(IsAlNum(s[n]) || s[n] == '_' || s[n] == ':')
					id.Cat(s[n++]);
				if(IsCppType(id))
					p.type = ITEM_CPP_TYPE;
				else
				if(IsCppKeyword(id))
					p.type = ITEM_CPP;
				else
				if(InScList(id, m.pname))
					p.type = ITEM_PNAME;
				else
				if(id == s_pick_) {
					p.type = ITEM_UPP;
				}
				else
				if(InScList(id, m.tname) || InScList(id, m.ctname))
					p.type = ITEM_TNAME;
				else
				if(param) {
					int ii = InScListIndext(id, m.ptype);
					if(ii >= 0)
						p.type = ITEM_PTYPE + ii;
				}
				else {
					int ii = InScListIndext(id, m.type);
					if(ii >= 0)
						p.type = ITEM_TYPE + ii;
				}
				LLOG("id: " << id << ", type: " << p.type);
			}
		}
		else
		if(sOperator(*s)) {
			while(sOperator(s[n]))
				n++;
			p.type = ITEM_CPP;
		}
		else {
			p.type = ITEM_SIGN;
			if(pari >= 0) {
				if(*s == '(' || *s == '<')
					par++;
				if(*s == ')' || *s == '>') {
					par--;
					if(par < 0) {
						p.pari = -1;
						pari = -1;
						param = false;
					}
				}
				if(*s == ',' && par == 0) {
					p.pari = -1;
					pari++;
				}
			}
			else
			if(*s == '(' && param) {
				pari = 0;
				par = 0;
			}
			while(s[n] && !iscid(s[n]))
				n++;
		}
		p.len = n;
		s += n;
	}
	return part;
}

Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m, const char *s)
{
	return ParseItemNatural(m.name, m, s);
}

Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m)
{
	return ParseItemNatural(m, ~m.natural + m.at);
}
