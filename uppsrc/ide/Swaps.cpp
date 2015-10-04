#include "ide.h"

#define LLOG(x)  // DLOG(x)

void AssistEditor::SwapSContext(Parser& p)
{
	int i = GetCursor();
	if(Ch(i - 1) == ';')
		i--;
	else
		for(;;) {
			int c = Ch(i);
			if(c == '{') {
				i++;
				break;
			}
			if(c == 0 || c == ';' || c == '}')
				break;
			i++;
		}
	Context(p, i);
}

bool Ide::SwapSIf(const char *cref)
{
	if(designer || !editor.assist_active)
		return false;
	Parser p;
	editor.SwapSContext(p);
	int q = CodeBase().Find(p.current_scope);
	LLOG("SwapS scope: " << p.current_scope << ", name " << p.current_name << ", key " << p.current_key);
	if(q < 0)
		return false;
	const Array<CppItem>& nn = CodeBase()[q];
	if(cref && MakeCodeRef(p.current_scope, p.current_key) != cref || IsNull(p.current_name))
		return false;
	Vector<const CppItem *> n;
	bool destructor = p.current_key.Find('~') >= 0;
	for(int i = 0; i < nn.GetCount(); i++) {
		const CppItem& m = nn[i];
		if(m.name == p.current_name && destructor == (m.kind == DESTRUCTOR) && !m.IsType())
			n.Add(&m);
	}
	if(!cref && n.GetCount() < 2)
		for(int i = 0; i < nn.GetCount(); i++)
			if(nn[i].IsType()) {
				GotoCpp(nn[i]);
				return false;
			}
	if(n.GetCount() == 0 || IsNull(editfile))
		return false;
	int file = GetSourceFileIndex(editfile);
	int line = p.current.line;
	LLOG("SwapS line: " << line);
	int i;
	for(i = 0; i < n.GetCount(); i++) {
		LLOG("file: " << GetSourceFilePath(n[i]->file) << ", line: " << n[i]->line);
		if(n[i]->file == file && n[i]->line == line) {
			i++;
			break;
		}
	}
	GotoCpp(*n[i % n.GetCount()]);
	return true;
}

void Ide::SwapS()
{
	SwapSIf(NULL);
}
