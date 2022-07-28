#include "ide.h"

#define LLOG(x)  // DLOG(x)

void Ide::SwapS()
{
	if(designer)
		return;
	if(!editor.WaitCurrentFile())
		return;
	AnnotationItem cm = editor.FindCurrentAnnotation();
	struct Sf : Moveable<Sf> {
		String path;
		int    line;
		bool   definition;
	};
	Vector<Sf> set, list;
	for(int pass = 0; pass < 4 && set.GetCount() < 2; pass++) {
		set.Clear();
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& m : f.value.items) {
				if(findarg(pass, 0, 3) >= 0 ? m.id == cm.id : // 3 in case nothing was found in pass 2
				   pass == 1 ? m.nest == cm.nest && m.name == cm.name :
				               m.nest == cm.nest && IsStruct(m.kind) && cm.nest.GetCount()) {
					Sf& sf = set.Add();
					sf.path = f.key;
					sf.line = m.line;
					sf.definition = m.definition;
				}
				if(set.GetCount() > 20) // sanity
					break;
			}
		if(set.GetCount() > 1)
			break;
	}
	
	if(set.GetCount() == 0)
		return;

	Sort(set, [](const Sf& a, const Sf& b) {
		return CombineCompare(a.path, b.path)(a.line, b.line) < 0;
	});
	
	bool deff = false;
	
	for(;;) { // create a list of candidates alternating definition and declaration
		int q = FindMatch(set, [&](const Sf& a) { return a.definition == deff; });
		if(q >= 0) {
			list.Add(set[q]);
			set.Remove(q);
		}
		else
			break;
		deff = !deff;
	}
	list.Append(set); // add remaining items
	int q = max(FindMatch(list, [&](const Sf& a) { return a.path == editfile && a.line == editor.GetCurrentLine(); }), 0);
	q = (q + 1) % list.GetCount();
	GotoPos(list[q].path, list[q].line);
}
