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
				if(m.definition) {
					if(cls.GetCount())
						result << '\t';
					result << m.pretty << ";\n";
				}
				else
					result << MakeDefinition(m);
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

#if 0
	String r;
	int l, h;
	bool decla = false;
	if(!GetSelection32(l, h)) {
		int i = GetLine(GetCursor32());
		l = GetPos32(i);
		h = l;
		while(h < GetLength32() && h - l < 1000) {
			int c = GetChar(h);
			if(c == ';') {
				decla = true;
				break;
			}
			if(c == '{')
				break;
			h++;
			if(c == '\"') {
				while(h < GetLength32()) {
					int c = GetChar(h);
					if(c == '\"' || c == '\n')
						break;
					h++;
					if(c == '\\' && h < GetLength32())
						h++;
				}
			}
		}
	}
	else
		decla = true;
	
	ParserContext ctx;
	Context(ctx, l);
	String txt = Get(l, h - l);
	String cls = ctx.current_scope;
	int best = 0;
	const Index<String>& ns = CodeBase().namespaces;
	for(int i = 0; i < ns.GetCount(); i++) {
		String h = ns[i];
		for(int pass = 0; pass < 2; pass++) {
			if(h.GetCount() > best && cls.StartsWith(h))
				best = h.GetCount();
			h << "::";
		}
	}
	cls.Remove(0, best);

	CppBase cpp;
	SimpleParse(cpp, txt, cls);

	for(int i = 0; i < cpp.GetCount(); i++) {
		const Array<CppItem>& n = cpp[i];
		bool decl = decla;
		for(int j = 0; j < n.GetCount(); j++)
			if(n[j].impl)
				decl = false;
		for(int j = 0; j < n.GetCount(); j++) {
			const CppItem& m = n[j];
			if(m.IsCode()) {
				if(decl)
					r << MakeDefinition(cls, m.natural) << "\n{\n}\n\n";
				else {
					if(cpp.IsType(i))
					   r << String('\t', Split(cpp.GetKey(i), ':').GetCount());
					r << m.natural << ";\n";
				}
			}
			if(m.IsData()) {
				String nat = m.natural;
				if(cls.GetCount()) {
					nat.Replace("static", "");
					nat = TrimLeft(nat);
					const char *s = nat;
					while(*s) {
						if(iscib(*s)) {
							const char *b = s;
							while(iscid(*s)) s++;
							String id(b, s);
							if(m.name == id) {
								if(cls.GetCount())
									r << cls << "::" << m.name << s;
								else
									r << m.name << s;
								break;
							}
							r << id;
						}
						else
							r << *s++;
					}
				}
				else {
					int q = nat.ReverseFind("::");
					if(q >= 0) { // Foo Class2 :: Class::variable; -> static Foo variable;
						int e = q + 2;
						for(;;) {
							while(q >= 0 && nat[q - 1] == ' ')
								q--;
							if(q == 0 || !iscid(nat[q - 1]))
								break;
							while(q >= 0 && iscid(nat[q - 1]))
								q--;
							int w = nat.ReverseFind("::", q);
							if(w < 0)
								break;
							q = w;
						}
						nat.Remove(q, e - q);
						r << "static " << nat;
					}
					else
						r << "extern " << nat;
				}
				r << ";\n";
			}
		}
	}

	WriteClipboardText(r);
#endif
}
