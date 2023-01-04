#include "ide.h"

static String         lpath;
static Vector<String> line;

void   Ide::ResetFileLine()
{
	lpath.Clear();
}

String Ide::GetFileLine(const String& path, int linei)
{
	if(path == editfile)
		return linei >= 0 && linei < editor.GetLineCount() ? editor.GetUtf8Line(linei)
		                                                   : String();
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

String ScopeWorkaround(const char *s)
{ // we are sometimes getting incorrect signatures with missing param qualifiers ([Upp::]CodeEditor::MouseTip
	StringBuffer r;
	while(*s) {
		const char *b = s;
		if(iscib(*s)) {
			s++;
			while(iscid(*s))
				s++;
			if(s[0] == ':' && s[1] == ':')
				s += 2;
			else
				r.Cat(b, s);
		}
		else {
			while(*s && !iscib(*s))
				s++;
			r.Cat(b, s);
		}
	}
	return r;
}

void GatherBaseVirtuals(const String& cls, const String& signature, Index<String>& ids, Index<String>& visited)
{ // find all ancestor classes that contain signature
	if(IsNull(cls) || visited.Find(cls) >= 0)
		return;
	visited.Add(cls);
	for(const auto& f : ~CodeIndex()) // check base classes
		for(const AnnotationItem& m : f.value.items)
			if(IsStruct(m.kind) && m.id == cls)
				// we cheat with With..<TopWindow> by splitting it to With... and TopWindow as
				// two bases
				for(String bcls : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
					GatherBaseVirtuals(bcls, signature, ids, visited);
	

	for(const auto& f : ~CodeIndex()) // now check virtual methods of this cls
		for(const AnnotationItem& m : f.value.items) {
			if(m.nest == cls && IsFunction(m.kind) && m.isvirtual && ScopeWorkaround(m.id.Mid(m.nest.GetCount())) == signature) {
				ids.FindAdd(cls); // found virtual method in the class
				return;
			}
		}
}

void GatherVirtuals(const VectorMap<String, String>& bases, const String& cls,
                    const String& signature, Index<String>& ids, Index<String>& visited)
{ // find all virtual methods with the same signature
	if(IsNull(cls) || visited.Find(cls) >= 0)
		return;

	visited.Add(cls);

	for(int q = bases.Find(cls); q >= 0; q = bases.FindNext(q)) {
		GatherVirtuals(bases, bases[q], signature, ids, visited);
	}

	for(const auto& f : ~CodeIndex()) // now check virtual methods of this cls
		for(const AnnotationItem& m : f.value.items) {
			if(m.nest == cls && IsFunction(m.kind) && m.isvirtual && ScopeWorkaround(m.id.Mid(m.nest.GetCount())) == signature) {
				ids.FindAdd(m.id); // found virtual method in the class
				return;
			}
		}
}

void Ide::Usage(const String& id, const String& name, Point ref_pos)
{
	if(IsNull(id))
		return;

	ResetFileLine();

	int li = editor.GetCursorLine();

	bool local = false;
	AnnotationItem cm = editor.FindCurrentAnnotation(); // what function body are we in?
	if(IsFunction(cm.kind)) { // do local variables
		for(const AnnotationItem& lm : editor.locals) {
			int ppy = -1;
			if(lm.id == id && lm.pos.y >= cm.pos.y && lm.pos.y <= li && lm.pos.y > ppy) {
				if(ref_pos == lm.pos) {
					local = true;
					break;
				}
			}
		}
	}
	
	SetFFound(ffoundi_next);
	FFound().Clear();

	Index<String> unique;
	if(local) {
		AddReferenceLine(editfile, ref_pos, name, unique);
		for(const ReferenceItem& lm : editor.references)
			if(lm.id == id && lm.ref_pos == ref_pos)
				AddReferenceLine(editfile, lm.pos, name, unique);
	}
	else {
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
			Index<String> visited;
			String signature = ScopeWorkaround(id.Mid(cls.GetCount()));
			Index<String> base_id;
			GatherBaseVirtuals(cls, signature, base_id, visited);
			

			VectorMap<String, String> bases;
			for(const auto& f : ~CodeIndex()) // check derived classes
				for(const AnnotationItem& m : f.value.items)
					if(IsStruct(m.kind))
						for(String bcls : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
							bases.Add(bcls, m.id);

			

			visited.Clear();
			for(const String& cls : base_id)
				GatherVirtuals(bases, cls, signature, ids, visited);
			
		}
		
		SortByKey(CodeIndex());
		for(int src = 0; src < 2; src++)
			for(const auto& f : ~CodeIndex())
				if((findarg(GetFileExt(f.key), ".h", "") < 0) == src) { // headers first
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
	Usage(cm.id, cm.name, cm.pos);
}

void Ide::IdUsage()
{
	String name;
	Point ref_pos;
	String ref_id = GetRefId(editor.GetCursor(), name, ref_pos);
	Usage(ref_id, name, ref_pos);
}
