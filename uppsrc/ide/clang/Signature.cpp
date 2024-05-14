#include "clang.h"

#define LTIMING(x)

static bool sOperatorTab[256];

INITBLOCK {
	for(const char *s = "!+-*^/%~&|=[]:?."; *s; s++)
		sOperatorTab[(int)*s] = true;
}

bool IsCppKeyword(const String& id)
{
	static Index<String> kw = {
        "__asm", "__cdecl", "__declspec", "__except", "__fastcall",
        "__finally", "__inline", "__int16", "__int32", "__int64",
        "__int8", "__leave", "__stdcall", "__try", "__uuidof",
        "alignas", "alignof", "and", "and_eq", "asm", "auto",
        "bitand", "bitor", "bool", "break", "case", "catch",
        "char", "char8_t", "char16_t", "char32_t", "class",
        "co_await", "co_return", "co_yield", "compl", "concept",
        "const", "const_cast", "consteval", "constexpr", "constinit",
        "continue", "decltype", "default", "delete", "dllexport",
        "dllimport", "do", "double", "dynamic_cast", "else", "enum",
        "explicit", "export", "extern", "false", "final", "float",
        "for", "force_inline", "friend", "goto", "if", "import",
        "inline", "int", "long", "module", "mutable", "namespace",
        "never_inline", "new", "noexcept", "not", "not_eq", "nullptr",
        "operator", "or", "or_eq", "override", "private", "protected",
        "public", "register", "reinterpret_cast", "requires", "return",
        "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "thread",
        "thread_local", "throw", "true", "try", "typedef", "typeid",
        "typename", "union", "unsigned", "using", "virtual",
        "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
	};
	return kw.Find(id) >= 0;
}

bool IsBasicType(const String& id)
{
	static Index<String> kt = { "__int16", "__int32", "__int64", "__int8", "auto", "char",
	                            "char8_t", "char16_t", "char32_t", "double", "float", "int",
	                            "long", "short", "unsigned", "void", "wchar_t", "bool" };
	return kt.Find(id) >= 0;
}

bool IsIgnored(const String& id)
{
	static Index<String> kt = { "class", "struct", "union", "noexcept", "override", "final", "template", "enum" };
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
	bool inparams = false;
	int name_pos = 0; // to filter out eventual return value of function
	bool operator_def = false; // between operator and ( - suppress < > handling
	static String s_attribute = "__attribute__";
	auto SkipT = [&] {
		int lvl = 1;
		s++;
		while(*s && lvl) {
			if(*s == '>')
				lvl--;
			if(*s == '<')
				lvl++;
			s++;
		}
	};
	while(*s && *s != '{') {
		if(iscid(*s)) {
			auto IsOperator = [](const char *s) {
				return memcmp(s, "operator", 8) == 0;
			};

			String id;
			while(iscid(*s) || *s == ':') {
				id.Cat(*s++);
				if(*s == '<') {
					if(id.GetCount() == 8 && IsOperator(id))
						break;
					if(id.GetCount() > 8) {
						const char *s = ~id + id.GetCount() - 8;
						if(IsOperator(s) && !iscid(s[-1]))
							break;
					}
					SkipT(); // Skip template arguments like in Foo<Bar>::Method() -> Foo::Method
				}
			}
			if(id == s_attribute) {
				while(mm.GetCount() && mm[mm.GetCount() - 1] == ' ')
					mm.SetLength(mm.GetCount() - 1);
				break;
			}
			if((*s == ',' || *s == ')' || *s == '[') && was_param_type) {
				was_param_type = false;
				continue;
			}
			if(IsIgnored(id))
				continue;
			if(was_id)
				mm.Cat(' ');
			if(!operator_def) // because of conversion operators e.g. Foo::operator bool()
				name_pos = mm.GetCount();
			if(id.GetCount() == 8 && IsOperator(id))
				operator_def = true;
			if(id.GetCount() > 8) {
				const char *s = ~id + id.GetCount() - 8;
				operator_def = IsOperator(s) && !iscid(s[-1]);
			}
			if(function && (IsBasicType(id) || !IsCppKeyword(id)))
				was_param_type = true;
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
			SkipT();
		}
		else
		if(*s == '-' && s[1] == '>') // ignore return value in -> notation (auto Foo() -> double)
			return TrimRight(mm);
		else {
			was_id = was_space = false;
			if(*s == '~' && !operator_def) // prevent culling of 'return value' in destructor
				destructor = true;
			if(*s == '(') {
				if(!inparams) {
					if(name_pos && !destructor) {
						String h = String(mm).Mid(name_pos);
						mm = h;
					}
					function = true;
					was_param_type = false;
					operator_def = false;
				}
				inparams = true;
			}
			if(*s == ',')
				was_param_type = false;
			if((*s == '*' || *s == '&') && !was_name) // skip *& before parameter list
				s++;
			else
				mm.Cat(*s++);
		}
	}
	String m = mm;
	m.TrimEnd("=0");
	return m;
}

String CleanupPretty(const String& signature)
{
	LTIMING("CleanupPretty");
	StringBuffer result;
	const char *s = signature;
	int plvl = 0;
	int tlvl = 0;
	while(*s && *s != '{')
		if(s[0] == ' ' && s[1] == '=' && s[2] == ' ' && s[3] == '{')
			break;
		else
		if(s[0] == '(' && s[1] == 'u' && s[2] == 'n' && s[3] == 'n' && s[4] == 'a' &&
		   s[5] == 'm' && s[6] == 'e' && s[7] == 'd' && s[8] == ')') { // remove (unnamed)
			s += 9;
			while(*s == ' ')
				s++;
		}
		else
		if(iscib(*s)) {
			const char *b = s;
			while(iscid(*s))
				s++;
			int len = int(s - b);
			if(len == 5 && (memcmp(b, "class", 5) == 0 && tlvl == 0 || memcmp(b, "union", 5) == 0) ||
			   len == 6 && (memcmp(b, "struct", 6) == 0 && tlvl == 0 || memcmp(b, "extern", 6) == 0 || memcmp(b, "inline", 6) == 0) ||
			   len == 7 && (memcmp(b, "virtual", 7) == 0) ||
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
		if(s[0] == '-' && s[1] == '>')
			break;
		else
		if(*s == '=' && plvl == 0)
			break;
		else {
			if(*s == '(') plvl++;
			if(*s == ')') plvl--;
			if(*s == '<') tlvl++;
			if(*s == '>') tlvl--;
			result.Cat(*s++);
		}
	String r = result;
	while(*r.Last() == ' ')
		r.TrimLast();
	return r;
}

Vector<ItemTextPart> ParsePretty(const String& name, const String& signature, int *fn_info)
{
	bool op = memcmp(name, "operator", 8) == 0 && !iscid(name[8]);
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
		if(iscid(*s) || (*s == '~' && *name == '~')) {
			if(memcmp(s, name, name_len) == 0 && !iscid(s[name_len]) && !op) { // need strncmp because of e.g. operator++
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
			else
			if(*s != '~') {
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
				if(id == "operator")
					p.type = ITEM_OPERATOR;
				else
				if(IsCppKeyword(id)) {
					if(id == "virtual")
						part.Drop();
					else
						p.type = ITEM_CPP;
				}
				else
				if(param) {
					if(lastidi >= 0 && par == 0)
						was_type = true;
					lastidi = part.GetCount() - 1; // can be a parameter name
				}
				if(param && fn_info)
					*fn_info = 1;
			}
			else // should not happen, but be safe
				s++;
		}
		else
		if(sOperatorTab[(byte)*s]) {
			while(sOperatorTab[(byte)s[n]])
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

String SignatureQtf(const String& name, const String& pretty, int pari)
{
	String qtf = "[%00-00K ";
	Vector<ItemTextPart> n = ParsePretty(name, pretty);
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
		qtf << '\1' << pretty.Mid(p.pos, p.len) << '\1';
		qtf << ']';
	}
	return qtf + "]";
}

String GetClass(const AnnotationItem& m)
{
	String cls = m.id;
	int q;
	if(m.kind == CXCursor_Constructor) {
		q = cls.Find("::" + m.name + "(");
		if(q >= 0)
			q += 2;
	}
	else
	if(m.kind == CXCursor_Destructor) {
		q = cls.Find('~');
	}
	else
		q = FindId(cls, m.name);

	if(q >= 0) {
		cls.Trim(q);
		if(m.nspace.GetCount())
			cls.TrimStart(m.nspace + "::");
		return cls;
	}
	
	return Null;
}

String GetNameFromId(const String& id)
{
	String name;
	try {
		CParser p(id);
		while(!p.IsEof())
			if(p.IsId())
				name = p.ReadId();
			else
			if(!p.Char(':'))
				break;
	}
	catch(CParser::Error) {}
	return name;
}

String MakeDefinition(const AnnotationItem& m, const String& klass)
{
	String result;
	String pretty = m.pretty;
	pretty.TrimStart("static ");
	int q = FindId(pretty, m.name);
	if(q < 0)
		result << pretty;
	else
		result << pretty.Mid(0, q) << klass << pretty.Mid(q);

	const char *s = result;
	int lvl = 0;
	String r;
	while(*s) {
		if(*s == '(')
			lvl++;
		if(*s == ')')
			lvl--;
		if(lvl == 1 && *s == '=') { // skip default parameter
			while(r.GetCount() && s[-1] == ' ') {
				s--;
				r.TrimLast();
			}
			while(*s) {
				if((*s == ',' || *s == ')') && lvl == 1) {
					r.Cat(*s++);
					break;
				}
				if(*s == '(')
					lvl++;
				if(*s == ')')
					lvl--;
				s++;
			}
		}
		else
			r.Cat(*s++);
	}
	r << "\n{\n}\n\n";
	return r;
}

String MakeDefinition(const AnnotationItem& m)
{
	return MakeDefinition(m, GetClass(m));
}