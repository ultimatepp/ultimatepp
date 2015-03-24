#include "cpp.h"

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
		id = p.ReadId();
		param.Clear();
		if(p.Char('(')) {
			while(p.IsId()) {
				if(param.GetCount())
					param << ",";
				param << p.ReadId();
				p.Char(',');
			}
			if(p.Char3('.', '.', '.'))
				param << '.';
			p.Char(')');
		}
		body = p.GetPtr();
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
	r << ' ' << body;
	return r;
}

String CppMacro::Expand(const Vector<String>& p) const
{
	String r;
	const char *s = body;
	String pp = param;
	bool variadic = false;
	if(*pp.Last() == '.') {
		variadic = true;
		pp.Trim(pp.GetCount() - 1);
	}
	Index<String> param(pick(Split(pp, ',')));
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
	return r;
}
