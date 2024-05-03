#include "ide.h"

#define LLOG(x)  // DLOG(x)

void Ide::SwapS()
{
	if(designer)
		return;
	if(!editor.WaitCurrentFile())
		return;
	if(editfile.EndsWith(".lay")) {
		EditUsingDesigner();
		return;
	}
	AnnotationItem cm = editor.FindCurrentAnnotation();
	PutAssist("Swap: " + cm.id);
	Cycle(cm, cm.pos.y, false);
}

void Ide::Cycle(const AnnotationItem& cm, int liney, bool navigate)
{
	if(IsNull(cm.id))
		return;
	
	struct Sf : Moveable<Sf> {
		String path;
		Point  pos;
		bool   definition;
	};
	Vector<Sf> set, list;
	for(int pass = 0; pass < 4; pass++) {
		set.Clear();
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& m : f.value.items) {
				if(findarg(pass, 0, 3) >= 0 ? m.id == cm.id : // 3 in case nothing was found in pass 2
				   pass == 1 ? m.nest == cm.nest && m.name == cm.name :
				               m.nest == cm.nest && IsStruct(m.kind) && cm.nest.GetCount()) {
					Sf& sf = set.Add();
					sf.path = f.key;
					sf.pos = m.pos;
					sf.definition = m.definition;
				}
				if(set.GetCount() > 20) // sanity
					break;
			}
		if(set.GetCount() > 1 || set.GetCount() && navigate)
			break;
	}
	
	if(set.GetCount() == 0)
		return;

	Sort(set, [](const Sf& a, const Sf& b) {
		return CombineCompare(a.path, b.path)(a.pos.y, b.pos.y)(a.pos.x, b.pos.x) < 0;
	});
	
	bool deff = true;
	
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
	int q = FindMatch(list, [&](const Sf& a) { return a.path == editfile && a.pos.y == liney; });
	q = q < 0 ? 0 : (q + 1) % list.GetCount();
	GotoPos(list[q].path, list[q].pos);
}

void Ide::SwitchHeader()
{
	int c = filelist.GetCursor();
	if(c < 0) return;
	String currfile = filelist[c];
	const char *ext = GetFileExtPos(currfile);
	if(!stricmp(ext, ".h") || !stricmp(ext, ".hpp")
	|| !stricmp(ext, ".lay") || !stricmp(ext, ".iml")) {
		int f = filelist.Find(ForceExt(currfile, ".cpp"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".c"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".cc"));
		if(f >= 0) filelist.SetCursor(f);
	}
}
