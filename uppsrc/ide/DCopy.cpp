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

void AssistEditor::DCopy()
{
	String r;
	int l, h;
	bool decla = false;
	if(!GetSelection(l, h)) {
		int i = GetLine(GetCursor());
		l = GetPos(i);
		h = l;
		while(h < GetLength() && h - l < 1000) {
			int c = GetChar(h);
			if(c == ';') {
				decla = true;
				break;
			}
			if(c == '{')
				break;
			h++;
			if(c == '\"') {
				while(h < GetLength()) {
					int c = GetChar(h);
					if(c == '\"' || c == '\n')
						break;
					h++;
					if(c == '\\' && h < GetLength())
						h++;
				}
			}
		}
	}
	else
		decla = true;

	Parser ctx;
	Context(ctx, l);
	String txt = Get(l, h - l);
	StringStream ss(txt);
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
	Parser parser; // we do not need/want preprocessing here
	parser.Do(ss, cpp, Null, Null, Null, CNULL, Split(cls, ':'),
	          Vector<String>(), Index<String>());

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
}
