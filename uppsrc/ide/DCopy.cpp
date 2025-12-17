#include "ide.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

bool AssistEditor::WaitCurrentFile()
{
	if(!HasLibClang())
		return false;
	if(annotating) {
		if(!IsCurrentFileParsing())
			SyncCurrentFile();
		Progress pi("Parsing");
		while(annotating)
			if(pi.StepCanceled())
				return false;
	}
	return true;
}

void AssistEditor::DCopy()
{ // changes declaration <-> definition
	String r;
	int l, h;
	if(!GetSelection32(l, h))
		l = h = GetCursor();

	int first_line = GetLine(l);
	int last_line = GetLine(h);
	
	if(last_line - first_line > 10000) {
		Exclamation("Too many lines.");
		return;
	}

	if(!WaitCurrentFile())
		return;

	String result;

	for(const AnnotationItem& m : annotations) {
		if(first_line <= m.pos.y && m.pos.y <= last_line) {
			String cls = GetClass(m);
			if(IsFunction(m.kind)) {
				String text;
				for(int i = m.pos.y; i < GetLineCount(); i++) {
					String l = GetUtf8Line(i);
					int q = max(l.ReverseFind(';'), l.ReverseFind('{'));
					if(q >= 0)
						l.Trim(q);
					text << l << ' ';
					if(q >= 0)
						break;
				}
				
				const char *fn_name_pos = nullptr;
				const char *fn_params_pos = nullptr;
				const char *id_pos = nullptr;
				
				int lvl = 0;
				CParser p(text);
				bool is_operator = false;
				try {
					while(!p.IsEof()) {
						const char *ptr = p.GetPtr();
						if(p.Char('(')) {
							if(lvl == 0) {
								fn_params_pos = ptr;
								if(!is_operator)
									fn_name_pos = id_pos;
							}
							lvl++;
						}
						else
						if(p.Char(')'))
							lvl--;
						else
						if(p.Id("operator")) {
							fn_name_pos = ptr;
							is_operator = true;
						}
						else
						if(p.IsId()) {
							id_pos = p.GetPtr();
							do
								p.ReadId();
							while(p.Char2(':', ':') && p.IsId());
						}
						else {
							id_pos = nullptr;
							p.Skip();
						}
					}
				}
				catch(CParser::Error) {}

				String ret, name, params;
				if(fn_params_pos) {
					params = fn_params_pos;
					if(fn_name_pos) {
						name = String(fn_name_pos, fn_params_pos);
						ret = String(text, fn_name_pos);
					}
				}
				
				auto Clean = [](String& s) {
					s = NormalizeSpaces((TrimBoth(Filter(s, [](int c) { return c < 32 ? 32 : c; }))));
				};
				
				Clean(ret);
				Clean(name);
				Clean(params);
				
				if(ret.Find('~') >= 0)
					ret.Clear();
				ret.TrimStart("~");
				ret.TrimStart("virtual ");
				ret.TrimStart("static ");
				ret.TrimStart("friend ");
				params.TrimEnd("override");
				params = TrimBoth(params);
				
				if(!m.definition) {
					String params2;
					const char *s = params;
					while(*s) {
						if(*s == '=' || s[0] == ' ' && s[1] == '=') { // skip default parameters
							while(*s) {
								if(*s == ',' || *s == ')') {
									params2.Cat(*s++);
									break;
								}
								s++;
							}
						}
						else
							params2.Cat(*s++);
					}
					params = params2;
				}

				if(ret.GetCount() && name.GetCount() && params.GetCount()) { // prefer original text
					if(m.definition) {
						if(IsMethod(m.kind))
							result << '\t';
						if(ret.GetCount())
							result << ret << ' ';
						result << m.name << params << ";\n";
					}
					else {
						String cret;
						if(IsMethod(m.kind)) { // attempt to qualify local classes in return value type
							bool qualified = false;
							const char *begin = ret;
							String st = m.nest + "::";
							VectorMap<String, String> qname;
							for(const AnnotationItem& am : annotations) {
								if(IsStruct(am.kind) && am.nest.StartsWith(st))
									qname.Add(am.name, am.nest);
								if(IsTypedef(am.kind) && am.nest == m.nest)
									qname.Add(am.name, am.nest + "::" + am.name);
							}
							try {
								CParser p(ret);
								while(!p.IsEof()) {
									if(p.Char2(':', ':'))
										qualified = true;
									else
									if(p.IsId() && !qualified) {
										const char *q = p.GetPtr();
										String id = p.ReadId();
										if(p.Char2(':', ':'))
											qualified = true;
										else
										if(!p.IsId() && qname.Find(id) >= 0) {
											cret.Cat(begin, q);
											begin = p.GetPtr();
											cret << qname.Get(id, id);
										}
									}
									else {
										qualified = false;
										p.Skip();
									}
								}
							}
							catch(CParser::Error) {}
							cret.Cat(begin, ret.End());
						}
						else
							cret = ret;
						result << cret << ' ' << cls << m.name << params << "\n{\n}\n\n";
					}
				}
				else { // just in case our heuristics split of original text failed
					if(m.definition) {
						if(cls.GetCount())
							result << '\t';
						result << m.pretty << ";\n";
					}
					else
						result << MakeDefinition(m);
				}
			}
			if(m.kind == CXCursor_VarDecl) {
				if(cls.GetCount()) { // class variable
					if(m.definition) {
						int q = FindId(m.pretty, m.name);
						if(q >= 0) {
							int w = m.pretty.ReverseFind(' ', q);
							if(w >= 0)
								result << "\tstatic " << m.pretty.Mid(0, w + 1) << m.pretty.Mid(q) << ";\n";
						}
					}
					else {
						String h = m.pretty;
						h.TrimStart("static ");
						h = TrimLeft(h);
						int q = FindId(h, m.name);
						if(q >= 0)
							result << h.Mid(0, q) << cls << h.Mid(q) << ";\n";
					}
				}
				else { // just toggle extern
					String h = m.pretty;
					if(FindId(GetUtf8Line(m.pos.y), "extern") < 0)
						h = "extern " + h;
					result << h << ";\n";
				}
			}
		}
	}

	if(result.GetCount() == 0) {
		PromptOK("No relevant declarations found.");
		return;
	}
	
	WriteClipboardText(result);
}
