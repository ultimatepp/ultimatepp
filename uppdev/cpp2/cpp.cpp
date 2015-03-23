#include "cpp.h"

#include <string.h>

inline bool IsSpc(byte c)
{
	return c > 0 && c <= 32;
}

String CppMacro::ToString() const
{
	return String().Cat() << "(" << AsString(param) << ") " << body;
}

String CppMacro::Expand(const Vector<String>& p) const
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
	return r;
}

void Cpp::Define(const char *s)
{
	CppMacro m;
	String id = m.Define(s);
	if(id.GetCount())
		macro.GetAdd(id) = pick(m);
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
	param.Add(h);
}

String Cpp::Expand(const char *s)
{
	Index<String> notmacro;
	return Expand(s, notmacro);
}

String Cpp::Expand(const char *s, Index<String>& notmacro)
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
		if(iscib2(*s)) {
			const char *b = s;
			s++;
			while(iscid2(*s))
				s++;
			String id(b, s);
			if(notmacro.Find(id) < 0) {
				const CppMacro *m = macro.FindPtr(id);
				if(m && !id.StartsWith("__$allowed_on_")) {
					Vector<String> param;
					const char *s0 = s;
					while(*s && (byte)*s <= ' ')
						s++;
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
					usedmacro.FindAdd(id);
					int ti = notmacro.GetCount();
					notmacro.Add(id);
					id = '\x1a' + Expand(m->Expand(param));
					notmacro.Trim(ti);
				}
			}
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

bool Cpp::Preprocess(const String& sourcefile, Stream& in, const String& currentfile)
{
	macro.Clear();
	Vector<String> ignorelist = Split("__declspec;__cdecl;"
                                      "__out;__in;__inout;__deref_in;__deref_inout;__deref_out;"
                                      "__AuToQuOtE;__xin;__xout;"
                                      "$drv_group;$allowed_on_parameter",
                                      ';');
	for(int i = 0; i < ignorelist.GetCount(); i++)
		macro.GetAdd(ignorelist[i]).variadic = true;
	done = false;
	incomment = false;
	Index<String> visited;
	Do(NormalizePath(sourcefile), in, NormalizePath(currentfile), visited);
	return done;
}

void Cpp::Do(const String& sourcefile, Stream& in, const String& currentfile, Index<String>& visited)
{
	DDUMP(currentfile);
	if(visited.Find(currentfile) >= 0 || visited.GetCount() > 20000)
		return;
	visited.Add(currentfile);
	String current_folder = GetFileFolder(currentfile);
	if(sourcefile != currentfile) {
		const PPFile& pp = GetPPFile(currentfile);
		pp.Dump();
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINE) {
				if(m.macro.body.GetCount())
					macro.GetAdd(m.id) = clone(m.macro);
			}
			else
			if(m.type == PP_INCLUDE) {
				String s = GetIncludePath(m.id, current_folder, include_path);
				if(s.GetCount())
					Do(sourcefile, in, s, visited);
			}
			else
			if(m.type == PP_NAMESPACE)
				namespace_stack.Add(m.id);
			else
			if(m.type == PP_NAMESPACE_END && namespace_stack.GetCount())
				namespace_stack.Drop();
			else
			if(m.type == PP_USING)
				namespace_using.FindAdd(m.id);
		}
		return;
	}
	
	incomment = false;
	StringBuffer result;
	result.Clear();
	result.Reserve(16384);
	int lineno = 0;
	bool incomment = false;
	while(!in.IsEof()) {
		String l = in.GetLine();
		lineno++;
		int el = 0;
		while(*l.Last() == '\\' && !in.IsEof()) {
			el++;
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		CParser p(l);
		if(p.Char('#')) {
			if(p.Id("define")) {
				result.Cat(l + "\n");
				Define(p.GetPtr());
			}
			else {
				result.Cat('\n');
				if(p.Id("include")) {
					String hdr = Expand(p.GetPtr());
					String header_path = GetIncludePath(hdr, current_folder, include_path);
					if(header_path.GetCount())
						Do(Null, NilStream(), header_path, visited);
				}
			}
		}
		else {
			result.Cat(Expand(l) + "\n");
		}
		while(el--)
			result.Cat("\n");
	}
	done = true;
	output = result;
}
