#include "CppBase.h"

NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)
#define LLOG(x)     // DLOG(x)

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
	StringBuffer r;
	const char *l0 = s;
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
		if(iscib(*s)) {
			LTIMING("Expand ID");
			const char *bid = s;
			s++;
			while(iscid(*s))
				s++;
			String id(bid, s);
			LTIMING("Expand ID2");
			if(notmacro.Find(id) < 0) {
				const PPMacro *pp = macro.FindLastPtr(id);
				int segmenti = pp ? segment_id.Find(pp->segment_id) : -1;
				const CppMacro *m = FindMacro(id, segment_id, segmenti);
				if(!m && pp)
					m = &pp->macro;
				if(m && m->IsUndef())
					m = NULL;
				if(m) {
					LTIMING("Expand macro");
					Vector<String> param;
					bool function_like = false;
					const char *s0 = s;
					if(m->param.GetCount()) {
						while(*s && (byte)*s <= ' ')
							s++;
						if(*s == '(') {
							function_like = true;
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
								if(*s == '\0') { // macro use spread into more lines
									if(bid == l0) // begin of line
										prefix_macro = bid;
									else
										prefix_macro = String(' ', 1) + bid; // do not want to emit grounding in body
									return r;
								}
								else
								if(*s == '\"' || *s == '\'')
									s = SkipString(s);
								else
									s++;
						}
					}
					if(!!m->param.GetCount() == function_like) {
						int ti = notmacro.GetCount();
						Vector<String> eparam;
						eparam.SetCount(param.GetCount());
						for(int i = 0; i < param.GetCount(); i++)
							eparam[i] = Expand(param[i]);
						notmacro.Add(id);
						id = '\x1f' + Expand(m->Expand(param, eparam)); // \x1f is info for Pre that there was a macro expansion
						notmacro.Trim(ti);
					}
					else
						s = s0;
				}
				else
					notmacro.Add(id);
			}
			r.Cat(id);
		}
		else
		if(*s == '\"') {
			const char *e = SkipString(s);
			r.Cat(s, e);
			s = e;
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

void Cpp::DoFlatInclude(const String& header_path)
{
	LTIMING("DoFlatInclude");
	LLOG("Flat include " << header_path);
	if(header_path.GetCount()) {
		md5.Put(header_path);
		const FlatPP& pp = GetFlatPPFile(header_path);
		LLOG("DoFlatInclude " << header_path << ", " << pp.segment_id.GetCount() << " segments");
		for(int i = 0; i < pp.segment_id.GetCount(); i++)
			segment_id.FindAdd(pp.segment_id[i]);
		for(int i = 0; i < pp.usings.GetCount(); i++) {
			namespace_using.FindAdd(pp.usings[i]);
			LLOG(" Flat usings " << pp.usings[i]);
			md5.Put('$');
			md5.Put(pp.usings[i]);
		}
	}
}

#define IGNORE_ELSE

void Cpp::Do(const String& sourcefile, Stream& in, const String& currentfile, bool get_macros)
{
	LLOG("Cpp::Do " << sourcefile << ", current: " << currentfile);
	if(visited.Find(currentfile) >= 0 || visited.GetCount() > 20000)
		return;
	visited.Add(currentfile);
	String current_folder = GetFileFolder(currentfile);
	bool notthefile = sourcefile != currentfile;
	if(notthefile || get_macros) {
		LTIMING("DO2");
		const PPFile& pp = GetPPFile(currentfile);
		for(int i = 0; i < pp.item.GetCount() && !done; i++) {
			const PPItem& m = pp.item[i];
			if(m.type == PP_DEFINES) {
				LTIMING("PP_DEFINES");
				if(notthefile) // if getting macros, we are interested in included macros only
					segment_id.FindAdd(m.segment_id);
			}
			else
			if(m.type == PP_INCLUDE) {
				String s = GetIncludePath(m.text, current_folder);
				if(s.GetCount()) {
					if(notthefile && (s == sourcefile || IncludesFile(s, sourcefile))) {
						LLOG("Include IN " << s);
						md5.Put(s);
						Do(sourcefile, in, s, get_macros);
						RHITCOUNT("Include IN");
					}
					else {
						LLOG("Include FLAT " << s);
						RHITCOUNT("Include FLAT");
						DoFlatInclude(s);
					}
				}
			}
			else
			if(m.type == PP_NAMESPACE) {
				namespace_stack.Add(m.text);
				LLOG("pp namespace " << m.text << " " << namespace_stack);
			}
			else
			if(m.type == PP_NAMESPACE_END && namespace_stack.GetCount()) {
				namespace_stack.Drop();
				LLOG("pp end namespace " << namespace_stack);
			}
			else
			if(m.type == PP_USING) {
				namespace_using.FindAdd(m.text);
				md5.Put('$');
				md5.Put(m.text);
			}
		}
		if(sourcefile != currentfile)
			return;
	}

	md5.Put('@');
	md5.Put(Join(namespace_stack, ";"));
	
	done = true;
	
	if(get_macros)
		return;
	
	LTIMING("Expand");
	incomment = false;
	prefix_macro.Clear();
	StringBuffer result;
	result.Clear();
	result.Reserve(16384);
	int lineno = 0;
	bool incomment = false;
	int segment_serial = 0;
	segment_id.Add(--segment_serial);
#ifdef IGNORE_ELSE
	int ignore_else = 0;
#endif
	while(!in.IsEof()) {
		String l = prefix_macro + in.GetLine();
		if(l.StartsWith("//$")) { // Do not remove assist++ parser directives
			result.Cat(l + "\n");
			continue;
		}
		prefix_macro.Clear();
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
				CppMacro m;
				String id = m.Define(p.GetPtr());
				if(id.GetCount()) {
					PPMacro& pp = macro.Add(id);
					pp.macro = m;
					pp.segment_id = segment_serial;
					notmacro.Trim(kw.GetCount());
				}
			}
			else
			if(p.Id("undef")) {
				result.Cat(l + "\n");
				if(p.IsId()) {
					segment_id.Add(--segment_serial);
					PPMacro& m = macro.Add(p.ReadId());
					m.segment_id = segment_serial;
					m.macro.SetUndef();
					notmacro.Trim(kw.GetCount());
					segment_id.Add(--segment_serial);
				}
			}
			else {
				result.Cat('\n');
			#ifdef IGNORE_ELSE
				if(ignore_else) {
					if(p.Id("if") || p.Id("ifdef") || p.Id("ifndef"))
						ignore_else++;
					else
					if(p.Id("endif"))
						ignore_else--;
				}
				else {
					if(p.Id("else") || p.Id("elif"))
						ignore_else = 1;
				}
			#endif
				if(p.Id("include")) {
					LTIMING("Expand include");
					String s = GetIncludePath(p.GetPtr(), current_folder);
					DoFlatInclude(s);
					segment_id.Add(--segment_serial);
				}
			}
		}
		else {
			LTIMING("Expand expand");
		#ifdef IGNORE_ELSE
			if(ignore_else)
				result.Cat('\n');
			else
		#endif
				result.Cat(Expand(l) + "\n");
		}
		while(el--)
			result.Cat("\n");
	}
	output = result;
}

Index<String> Cpp::kw;

bool Cpp::Preprocess(const String& sourcefile, Stream& in, const String& currentfile,
                     bool get_macros)
{
	LLOG("===== Preprocess " << sourcefile << " <- " << currentfile);
	LTIMING("Cpp::Preprocess");
	macro.Clear();
	macro.Reserve(1000);
	segment_id.Clear();
	segment_id.Add(0);
	segment_id.Reserve(100);
	
	ONCELOCK {
		const char **h = CppKeyword();
		while(*h) {
			kw.Add(*h);
			h++;
		}
	}
	notmacro = clone(kw);

	done = false;
	incomment = false;
	output.Clear();
	visited.Clear();
	Do(NormalizePath(sourcefile), in, NormalizePath(currentfile), get_macros);
	if(!done)
		output.Clear();
	return done;
}

void GetAllMacros(Md5Stream& md5, const String& id, Index<int>& segment_id);

String Cpp::GetDependeciesMd5(const Vector<String>& m)
{
	String r;
	Md5Stream md5;
	md5.Put(Join(namespace_stack, ";"));
	md5.Put('@');
	md5.Put(Join(namespace_using.GetKeys(), ";"));
	md5.Put('@');
	for(int i = 0; i < m.GetCount(); i++)
		GetAllMacros(md5, m[i], segment_id);
	return md5.FinishString();
}

END_UPP_NAMESPACE
