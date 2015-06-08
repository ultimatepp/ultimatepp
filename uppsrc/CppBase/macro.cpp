#include "CppBase.h"

NAMESPACE_UPP

inline bool IsSpc(byte c)
{
	return c > 0 && c <= 32;
}

String CppMacro::Define(const char *s)
{
	CParser p(s);
	String id;
	try {
		if(!p.IsId())
			return Null;
		p.NoSkipSpaces().NoSkipComments(); // '#define TEST(x)' is difference form '#define TEST (x)' - later is parameterless
		id = p.ReadId();
		param.Clear();
		if(p.Char('(')) {
			p.SkipSpaces();
			p.Spaces();
			while(p.IsId()) {
				if(param.GetCount())
					param << ",";
				param << p.ReadId();
				p.Char(',');
			}
			if(p.Char3('.', '.', '.'))
				param << '.';
			p.Char(')');
			if(param.GetCount() == 0) // #define foo() bar - need to 'eat' parenthesis, cheap way
				param = ".";
		}
		const char *b = p.GetPtr();
		while(!p.IsEof() && !p.IsChar2('/', '/'))
			p.SkipTerm();
		body = String(b, p.GetPtr());
		Md5Stream m;
		m.Put(param);
		m.Put(body);
		m.Finish(md5);
	}
	catch(CParser::Error) {
		return Null;
	}
	return id;
}

String CppMacro::ToString() const
{
	String r;
	if(param.GetCount()) {
		String h = param;
		h.Replace(".", "...");
		r << "(" << h << ")";
	}
	if(IsUndef())
		r << " #undef";
	else
		r << ' ' << body;
	return r;
}

void CppMacro::Serialize(Stream& s)
{
	s % param % body;
	s.SerializeRaw(md5, 16);
}

String CppMacro::Expand(const Vector<String>& p, const Vector<String>& ep) const
{
	String r;
	const char *s = body;
	String pp = param;
	bool variadic = false;
	if(*pp.Last() == '.') {
		variadic = true;
		pp.Trim(pp.GetCount() - 1);
	}
	Index<String> param(Split(pp, ','));
	static String VA_ARGS("__VA_ARGS__"); // static - Speed optimization
	while(*s) {
		if(IsAlpha(*s) || *s == '_') {
			const char *b = s;
			s++;
			while(IsAlNum(*s) || *s == '_')
				s++;
			String id(b, s);
			const char *ss = b;
			bool cat = false;
			while(ss > ~body && ss[-1] == ' ')
				ss--;
			if(ss >= ~body + 2 && ss[-1] == '#' && ss[-2] == '#')
				cat = true;
			ss = s;
			while(*ss && *ss == ' ')
				ss++;
			if(ss[0] == '#' && ss[1] == '#')
				cat = true;
			if(id == VA_ARGS) {
				bool next = false;
				for(int i = param.GetCount(); i < ep.GetCount(); i++) {
					if(next)
						r.Cat(", ");
					r.Cat((cat ? p : ep)[i]);
					next = true;
				}
			}
			else {
				int q = param.Find(id);
				if(q >= 0) {
					if(q < ep.GetCount())
						r.Cat((cat ? p : ep)[q]);
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
	return r;
}

END_UPP_NAMESPACE
