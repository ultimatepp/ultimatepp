#include "ide.h"

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

void GatherVirtuals(const String& cls, const String& signature, Index<String>& ids, Index<String>& visited)
{ // find all virtual methods with the same id
	if(IsNull(cls) || visited.Find(cls) >= 0)
		return;
	visited.Add(cls);
	for(const auto& f : ~CodeIndex()) // find base and derived classes
		for(const AnnotationItem& m : f.value.items)
			if(IsStruct(m.kind)) {
				if(m.id == cls) // Find base classes
					// we cheat with With..<TopWindow> by splitting it to With... and TopWindow
					for(String bcls : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
						GatherVirtuals(bcls, signature, ids, visited);
			}
	

	for(const auto& f : ~CodeIndex()) // now gather virtual methods of this class
		for(const AnnotationItem& m : f.value.items) {
			if(m.nest == cls && IsFunction(m.kind) && m.isvirtual && m.id.Mid(m.nest.GetCount()) == signature) {
				ids.FindAdd(m.id); // found virtual method in the class
				for(const auto& f : ~CodeIndex()) // check derived classes for overrides
					for(const AnnotationItem& m : f.value.items)
						if(IsStruct(m.kind) && visited.Find(m.id) < 0) {
							for(String bcls : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
								if(bcls == cls) // Find derived classes
									GatherVirtuals(m.id, signature, ids, visited);
						}
				return;
			}
		}
}

void Ide::Usage(const String& id, const String& name)
{
	if(IsNull(id))
		return;

	bool isvirtual = false;
	String cls;
	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items)
			if(m.id == id && m.isvirtual) {
				isvirtual = true;
				cls = m.nest;
				break;
			}
	
	Index<String> ids;
	ids.FindAdd(id);
	
	if(isvirtual) {
		String sgn = id;
		sgn.TrimStart(cls);
		Index<String> visited;
		GatherVirtuals(cls, id.Mid(cls.GetCount()), ids, visited);
	}

	SetFFound(ffoundi_next);
	FFound().Clear();
	SortByKey(CodeIndex());
	Index<String> unique;
	for(const auto& f : ~CodeIndex()) {
		auto Add = [&](Point mpos) {
			AddReferenceLine(f.key, mpos, name, unique);
		};
		for(const AnnotationItem& m : f.value.items)
			if(ids.Find(m.id) >= 0)
				Add(m.pos);
		for(const ReferenceItem& m : f.value.refs)
			if(ids.Find(m.id) >= 0)
				Add(m.pos);
	}

	FFoundFinish();
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

void Ide::IdUsage()
{
	String name;
	String ref_id = GetRefId(editor.GetCursor(), name);
	Usage(ref_id, name);
}
