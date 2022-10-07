#include "ide.h"

#define LLOG(x)  // DLOG(x)

void Ide::SwapS()
{
	if(designer)
		return;
	if(!editor.WaitCurrentFile())
		return;
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

String GetFileLine(const String& path, int linei)
{
	static String         lpath;
	static Vector<String> line;
	if(path != lpath) {
		lpath = path;
		FileIn in(path);
		line.Clear();
		if(in.GetSize() < 1000000)
			while(!in.IsEof())
				line.Add(in.GetLine());
	}
	return linei >= 0 && linei < line.GetCount() ? line[linei] : String();
}

void Ide::AddReferenceLine(const String& path, Point mpos, const String& name, Index<String>& unique)
{
	String ln = GetFileLine(path, mpos.y);
	int count = 0;
	int pos = 0;
	if(name.GetCount()) {
		pos = FindId(ln.Mid(mpos.x), name);
		if(pos >= 0) {
			count = name.GetCount();
			pos += mpos.x;
		}
		else
			pos = 0;
	}
	String h = String() << path << '\t' << mpos.y << '\t' << ln << '\t' << pos << '\t' << count;
	if(unique.Find(h) < 0) {
		unique.Add(h);
		AddFoundFile(path, mpos.y + 1, ln, pos, count);
	}
}

void Ide::Usage()
{
	if(designer)
		return;
	if(!editor.WaitCurrentFile())
		return;
	AnnotationItem cm = editor.FindCurrentAnnotation();
	Usage(cm.id, cm.name);
}

void Ide::Usage(const String& id, const String& name)
{
	if(IsNull(id))
		return;
	SetFFound(ffoundi_next);
	FFound().Clear();
	SortByKey(CodeIndex());
	Index<String> unique;
	for(const auto& f : ~CodeIndex()) {
		auto Add = [&](Point mpos) {
			AddReferenceLine(f.key, mpos, name, unique);
		};
		for(const AnnotationItem& m : f.value.items)
			if(m.id == id)
				Add(m.pos);
		for(const ReferenceItem& m : f.value.refs)
			if(m.id == id)
				Add(m.pos);
	}

	FFoundFinish();
}

void Ide::IdUsage()
{
	String name;
	String ref_id = GetRefId(editor.GetCursor(), name);
	Usage(ref_id, name);
}
