#include "cpp.h"

#include <string.h>

#define LLOG(x)

bool IsSpc(byte c)
{
	return c > 0 && c <= 32;
}

String CppMacro::ToString() const
{
	return String().Cat() << "(" << AsString(param) << ") " << body;
}

String CppMacro::Expand(const Vector<String>& p)
{
	String r;
	const char *s = body;
	while(*s) {
		if(IsAlpha(*s) || *s == '_') {
			const char *b = s;
			s++;
			while(IsAlNum(*s) || *s == '_')
				s++;
			String id(b, s);
			static String VA_ARGS("__VA_ARGS__"); // Speed optimization
			if(id == VA_ARGS) {
				bool next = false;
				for(int i = param.GetCount(); i < p.GetCount(); i++) {
					if(next)
						r.Cat(", ");
					r.Cat(p[i]);
					next = true;
				}
			}
			else {
				int q = param.Find(id);
				if(q >= 0) {
					if(q < p.GetCount())
						r.Cat(p[q]);
				}
				else
					r.Cat(id);
			}
			continue;
		}
		if(s[0] == '#' && s[1] == '#') {
			int q = r.GetLength();
			while(q > 0 && IsSpc(r[q - 1]))
				q--;
			r.Trim(q);
			s += 2;
			while((byte)*s <= ' ')
				s++;
			continue;
		}
		if(*s == '#') {
			const char *ss = s + 1;
			while(IsSpc(*ss))
				ss++;
			if(IsAlpha(*ss) || *ss == '_') {
				const char *b = ss;
				ss++;
				while(IsAlNum(*ss) || *ss == '_')
					ss++;
				String id(b, ss);
				int q = param.Find(id);
				if(q >= 0) {
					if(q <= p.GetCount()) {
						if(q < p.GetCount())
							r.Cat(AsCString(p[q]));
						s = ss;
						continue;
					}
				}
				r.Cat(String(s, ss));
				s = ss;
				continue;
			}
		}
		r.Cat(*s++);
	}
	LLOG("EXPAND: " << r);
	return r;
}

void Cpp::Define(const char *s)
{
	CParser p(s);
	try {
		String id;
		if(!p.IsId())
			return;
		CppMacro& m = macro.GetAdd(p.ReadId());
		m.param.Clear();
		if(p.Char('(')) {
			while(p.IsId()) {
				m.param.Add(p.ReadId());
				p.Char(',');
			}
			if(p.Char3('.', '.', '.'))
				m.variadic = true;
			p.Char(')');
		}
		m.body = p.GetPtr();
	}
	catch(CParser::Error) {}
}

const char *Cpp::SkipString(const char *s)
{
	CParser p(s);
	p.ReadOneString(*s);
	s = p.GetPtr();
	while((byte)*(s - 1) <= ' ')
		s--;
	return s;
}

void Cpp::ParamAdd(Vector<String>& param, const char *s, const char *e)
{
	while(s < e && (byte)*s <= ' ') s++;
	while(e > s && (byte)*(e - 1) <= ' ') e--;
	String h;
	while(s < e) {
		if((byte)*s <= ' ') {
			h.Cat(' ');
			s++;
			while(s < e && (byte)*s <= ' ')
				s++;
		}
		else
		if(*s == '\"' || *s == '\'') {
			const char *q = SkipString(s);
			h.Cat(String(s, q));
			s = q;
		}
		else
			h.Cat(*s++);
	}
	LLOG("param: " << h);
	param.Add(h);
}

String Cpp::Expand(const char *s)
{
	StringBuffer r;
	while(*s) {
		if(incomment) {
			if(s[0] == '*' && s[1] == '/') {
				incomment = false;
				s += 2;
				r.Cat("*/");
			}
			else
				r.Cat(*s++);
		}
		else
		if(IsAlpha(*s) || *s == '_') {
			const char *b = s;
			s++;
			while(IsAlNum(*s) || *s == '_')
				s++;
			String id(b, s);
			int q = macro.Find(id);
			if(q >= 0 && !macro[q].flag) {
				LLOG("Expanding " << id);
				Vector<String> param;
				const char *s0 = s;
				while(*s && (byte)*s <= ' ') s++;
				if(*s == '(') {
					s++;
					const char *b = s;
					int level = 0;
					for(;;)
						if(*s == ',' && level == 0) {
							ParamAdd(param, b, s);
							s++;
							b = s;
						}
						else
						if(*s == ')') {
							s++;
							if(level == 0) {
								ParamAdd(param, b, s - 1);
								break;
							}
							level--;
						}
						else
						if(*s == '(') {
							s++;
							level++;
						}
						else
						if(*s == '\0')
							break;
						else
						if(*s == '\"' || *s == '\'')
							s = SkipString(s);
						else
							s++;
				}
				else
					s = s0; // otherwise we eat spaces after parameterless macro
				macro[q].flag = true;
				r.Cat(Expand(macro[q].Expand(param)));
				macro[q].flag = false;
			}
			else
				r.Cat(id);
		}
		else
		if(s[0] == '/' && s[1] == '*') {
			incomment = true;
			s += 2;
			r.Cat("/*");
		}
		else
		if(s[0] == '/' && s[1] == '/') {
			r.Cat(s);
			break;
		}
		else
			r.Cat(*s++);
	}
	return r;
}

String Cpp::Preprocess(Stream& in, bool needresult)
{
	incomment = false;
	StringBuffer result;
	result.Clear();
	result.Reserve(16384);
	while(!in.IsEof()) {
		String l = in.GetLine();
		int el = 0;
		while(*l.Last() == '\\' && !in.IsEof()) {
			el++;
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		const char *s = l;
		while(*s == ' ')
			s++;
		if(*s == '#') {
			result.Cat("\n");
			if(strncmp(s + 1, "define", 6) == 0)
				Define(s + 7);
			else
			if(strncmp(s + 1, "undef", 5) == 0) {
				CParser p(s + 6);
				if(p.IsId())
					macro.UnlinkKey(p.ReadId());
			}
			else
			if(strncmp(s + 1, "include", 7) == 0 && level < 100) {
				String path = GetIncludePath(s + 8);
				DDUMP(path);
				if(path.GetCount()) {
					Cpp inc;
					inc.level = level + 1;
					inc.filedir = filedir;
					inc.include_path = include_path;
					FileIn in(path);
					inc.Preprocess(in, false);
					inc.macro.Sweep();
					for(int i = 0; i < inc.macro.GetCount(); i++)
						macro.GetAdd(inc.macro.GetKey(i)) = inc.macro[i];
				}
			}
		}
		else
			if(needresult)
				result.Cat(Expand(l) + "\n");
			else {
				const char *s = l;
				while(*s) {
					if(s[0] == '/' && s[1] == '*') {
						incomment = true;
						s += 2;
					}
					else
					if(s[0] == '*' && s[1] == '/') {
						incomment = false;
						s += 2;
					}					
					else
					if(s[0] == '/' && s[1] == '/' && !incomment)
						break;
					else
						s++;
				}
			}
		if(needresult)
			while(el--)
				result.Cat("\n");
	}
	return result;
}

String Cpp::GetIncludePath(const char *s)
{
	while(IsSpace(*s))
		s++;
	int type = *s;
	if(type == '<' || type == '\"' || type == '?') {
		s++;
		String name;
		if(type == '<') type = '>';
		while(*s != '\r' && *s != '\n') {
			if(*s == type) {
				if(type == '\"') {
					String fn = NormalizePath(name, filedir);
					if(FileExists(fn))
						return fn;
				}
				return GetFileOnPath(name, include_path, false);
			}
			name.Cat(*s++);
		}
	}
	return Null;
}
