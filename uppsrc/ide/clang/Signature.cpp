#include "clang.h"

#define LTIMING(x)

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

bool IsIgnored(const String& id)
{
	static Index<String> kt = { "class", "struct", "union", "noexcept", "override", "template" };
	return kt.Find(id) >= 0;
}

String CleanupId(const char *s)
{ // removes unnecessary spaces, removes parameter names
	StringBuffer mm;
	bool was_param_type = false;
	bool was_id = false;
	bool was_space = false;
	bool was_name = false;
	bool function = false;
	bool destructor = false;
	int name_pos = 0; // to filter out eventual return value of function
	bool operator_def = false; // between operator and ( - suppress < > handling
	while(*s && *s != '{') {
		if(iscid(*s)) {
			const char *b = s;
			while(iscid(*s) || *s == ':')
				s++;
			String id(b, s);
			if((*s == ',' || *s == ')') && was_param_type) {
				was_param_type = false;
				continue;
			}
			if(IsIgnored(id))
				continue;
			auto IsOperator = [](const char *s) {
				return s[0] == 'o' && s[1] == 'p' && s[2] == 'e' && s[3] == 'r' &&
				       s[4] == 'a' && s[5] == 't' && s[6] == 'o' && s[7] == 'r';
			};
			if(id.GetCount() == 8 && IsOperator(id))
				operator_def = true;
			if(id.GetCount() > 8) {
				const char *s = ~id + id.GetCount() - 8;
				operator_def = IsOperator(s) && !iscid(s[-1]);
			}
			if(function && (IsBasicType(id) || !IsCppKeyword(id))) // TODO optimize this (IsCppKeywordNoType)
				was_param_type = true;
			if(was_id)
				mm.Cat(' ');
			name_pos = mm.GetCount();
			mm.Cat(id);
			was_id = true;
			was_name = true;
		}
		else
		if(*s == ' ') { // filter out spaces that are not necessary
			was_space = true;
			s++;
		}
		else
		if(*s == '<' && !operator_def) { // remove template stuff e.g. Buffer<T>(Vector<T>) -> Buffer(Vector);
			int lvl = 1;
			s++;
			while(*s && lvl) {
				if(*s == '>')
					lvl--;
				if(*s == '<')
					lvl++;
				s++;
			}
		}
		else {
			was_id = was_space = false;
			if(*s == '~' && !operator_def) // prevent culling of 'return value' in destructor
				destructor = true;
			if(*s == '(') {
				function = true;
				was_param_type = false;
				operator_def = false;
				if(name_pos && !destructor) {
					String h = String(mm).Mid(name_pos);
					mm = h;
				}
			}
			if(*s == ',')
				was_param_type = false;
			if((*s == '*' || *s == '&') && !was_name) // skip *& before parameter list
				s++;
			else
				mm.Cat(*s++);
		}
	}
	return mm;
}

String CleanupPretty(const String& signature)
{
	LTIMING("CleanupPretty");
	StringBuffer result;
	const char *s = signature;
	while(*s)
		if(iscib(*s)) {
			const char *b = s;
			while(iscid(*s))
				s++;
			int len = s - b;
			if(len == 5 && (memcmp(b, "class", 5) == 0 || memcmp(b, "union", 5) == 0) ||
			   len == 6 && (memcmp(b, "struct", 6) == 0 || memcmp(b, "extern", 6) == 0) ||
			   len == 8 && (memcmp(b, "override", 8) == 0 || memcmp(b, "noexcept", 8) == 0)) {
			   while(*s == ' ')
			       s++;
			   continue;
			}
			result.Cat(b, s);
		}
		else
		if(s[0] == ' ' && s[1] == '*' && s[2] == ' ') {
			result.Cat(' ');
			result.Cat('*');
			s += 3;
		}
		else
		if(s[0] == ' ' && s[1] == '&' && s[2] == ' ') {
			result.Cat('&');
			result.Cat(' ');
			s += 3;
		}
		else
		if(s[0] == ' ' && s[1] == '&' && s[2] == '&' && s[3] == ' ') {
			result.Cat('&');
			result.Cat('&');
			result.Cat(' ');
			s += 4;
		}
		else
		if(s[0] == ' ' && s[1] == '&' && s[2] == '&') {
			result.Cat('&');
			result.Cat('&');
			if(iscib(s[3]))
				result.Cat(' ');
			s += 3;
		}
		else
		if(s[0] == ' ' && s[1] == '&') {
			result.Cat('&');
			if(iscib(s[2]))
				result.Cat(' ');
			s += 2;
		}
		else
		if(*s == ' ') {
			while(*s == ' ')
				s++;
			result.Cat(' ');
		}
		else
			result.Cat(*s++);
	String r = result;
	while(*r.Last() == ' ')
		r.TrimLast();
	return r;
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
				if(IsCppType(id)) {
					p.type = ITEM_CPP_TYPE;
					if(lastidi >= 0 && par == 0)
						was_type = true;
				}
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

Image CxxIcon(int kind);

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
