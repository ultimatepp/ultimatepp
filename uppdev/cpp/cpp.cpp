#include "cpp.h"

#define LLOG(x)

bool IsSpc(byte c)
{
	return c > 0 && c <= 32;
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
			int q = param.Find(id);
			if(q >= 0) {
				if(q < p.GetCount())
					r.Cat(p[q]);
			}
			else
				r.Cat(id);
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
	String r;
	while(*s) {
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
				macro[q].flag = true;
				r.Cat(Expand(macro[q].Expand(param)));
				macro[q].flag = false;
			}
			else
				r.Cat(id);
		}
		else
			r.Cat(*s++);
	}
	return r;
}
