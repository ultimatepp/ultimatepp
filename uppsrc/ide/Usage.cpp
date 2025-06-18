#include "ide.h"

static String         lpath;
static Vector<String> line;

void   Ide::ResetFileLine()
{
	lpath.Clear();
}

String Ide::GetFileLine(const String& path, int linei)
{ // slightly cached fetching of line from file
	if(path == editfile && !designer)
		return linei >= 0 && linei < editor.GetLineCount() ? editor.GetUtf8Line(linei)
		                                                   : String();
	if(path != lpath) {
		lpath = path;
		FileIn in(path);
		line.Clear();
		if(in.GetSize() < 10000000)
			while(!in.IsEof())
				line.Add(in.GetLine());
	}
	return linei >= 0 && linei < line.GetCount() ? line[linei] : String();
}

void Ide::AddReferenceLine(const String& path, Point mpos, const String& name, Index<String>& unique)
{
	String ln = GetFileLine(path, mpos.y);
	int count = 0;
	int pos = -1;
	if(name.GetCount()) {
		pos = FindId(ln.Mid(mpos.x), name);
		if(pos < 0) {
			pos = FindId(ln.Mid(mpos.x), "With" + name);
			if(pos >= 0) // special case for WithNameLayout
				pos += 4;
		}
		if(pos >= 0) {
			count = name.GetCount();
			pos += mpos.x;
		}
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
	return String(r);
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
		for(const AnnotationItem& lm : editor.locals)
			if(lm.id == id && lm.pos.y >= cm.pos.y && lm.pos.y <= li) {
				if(ref_pos == lm.pos) {
					local = true;
					break;
				}
			}
	}
	
	NewFFound();

	Index<String> unique;
	if(local) {
		AddReferenceLine(editfile, ref_pos, name, unique);
		for(const ReferenceItem& lm : editor.references)
			if(lm.id == id && lm.ref_pos == ref_pos)
				AddReferenceLine(editfile, lm.pos, name, unique);
	}
	else {
		bool isvirtual = false;
		bool isstatic = false; // to limit file static variables to single file
		bool istype = false;
		String cls;
		Progress pi("Indexing files");
		while(Indexer::IsRunning()) {
			if(pi.StepCanceled())
				break;
			GuiSleep(10);
		}
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& m : f.value.items) {
				if(m.id == id) {
					if(m.isvirtual) {
						isvirtual = true;
						cls = m.nest;
						break;
					}
					if(IsStruct(m.kind)) {
						istype = true;
						cls = m.nest;
						break;
					}
					if(m.id == id && m.isstatic && f.key == editfile &&
					   m.nest.GetCount() == m.nspace.GetCount()) // ignore class variables
						isstatic = true;
				}
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

		UsageId(name, id, ids, istype, isstatic, unique);
	}

	UsageFinish();
}

void Ide::UsageId(const String& name, const String& id, const Index<String>& ids, bool istype, bool isstatic, Index<String>& unique)
{
	// DLOG("UsageId " << name << " " << id << " " << ids << " istype: " << istype << " " << unique << " static " << isstatic);
	int q = id.ReverseFind("::");
	String constructor = id + "::" + (q >= 0 ? id.Mid(q + 2) : id) + "(";
	String destructor = id + "::~(";
	SortByKey(CodeIndex());
	int kind = Null;
	for(int src = 0; src < 2; src++)
		for(const auto& f : ~CodeIndex()) {
			if(!isstatic || f.key == editfile)
				if((findarg(GetFileExt(f.key), ".h", "") < 0) == src) { // headers first
					auto Add = [&](Point mpos) {
						AddReferenceLine(f.key, mpos, name, unique);
					};
					for(const AnnotationItem& m : f.value.items) {
						if(m.id == id)
							kind = m.kind;
						if(ids.Find(m.id) >= 0 || istype && (m.id.StartsWith(constructor) || m.id.StartsWith(destructor)))
							Add(m.pos);
					}
					for(const ReferenceItem& m : f.value.refs)
						if(ids.Find(m.id) >= 0)
							Add(m.pos);
				}
		}
	
	if(!IsNull(kind))
		FFoundSetIcon(CxxIcon(kind));
}

void Ide::UsageFinish()
{
	FFound().Sort(3, [](const Value& va, const Value& vb)->int {
		const ListLineInfo& a = va.To<ListLineInfo>();
		const ListLineInfo& b = vb.To<ListLineInfo>();
		return CombineCompare(GetFileName(a.file), GetFileName(b.file))
		                     (a.file, b.file)
		                     (a.lineno, b.lineno)
		                     (a.linepos, b.linepos);
	});
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
	if(designer)
		return;
	if(editfile.EndsWith(".lay")) {
		String layout, item;
		if(GetLayoutItem(layout, item)) {
			if(item.GetCount())
				FindDesignerItemReferences("With" + layout + "::" + item, item);
			else
				FindDesignerItemReferences("With" + layout, layout);
		}
		return;
	}
	if(editfile.EndsWith(".key")) {
		CParser p(editor.GetUtf8Line(editor.GetCursorLine()));
		try {
			p.PassId("KEY");
			p.PassChar('(');
			String id = "AK_" + p.ReadId();
			String path = NormalizePath(editfile);
			for(const auto& f : ~CodeIndex())
				for(const AnnotationItem& m : f.value.items)
					if(m.name == id) {
						Index<String> ids, unique;
						ids.Add(m.id);
						NewFFound();
						UsageId(id, id, ids, IsStruct(m.kind), false, unique);
						UsageFinish();
						return;
					}
		}
		catch(CParser::Error) {}
		return;
	}
	String name;
	Point ref_pos;
	String ref_id;
	for(int pass = 0; pass < 2; pass++) { // bit of heuristics - if stored info has correct id at current pos, do not wait for rescan
		ref_id = GetRefId(editor.GetCursor(), name, ref_pos);
		if(ref_id.GetCount())
			break;
		if(!editor.WaitCurrentFile())
			return;
	}
	Usage(ref_id, name, ref_pos);
}

void Ide::FindDesignerItemReferences(const String& id, const String& name)
{
	SaveFile();
	ResetFileLine();
	SortByKey(CodeIndex());
	String path = NormalizePath(editfile);
	int q = CodeIndex().Find(path);
	if(q < 0)
		return;
	
	auto DoUsage = [&](const AnnotationItem& m) {
		Index<String> ids, unique;
		ids.Add(m.id);
		NewFFound();
		UsageId(name, m.id, ids, IsStruct(m.kind), false, unique);
		UsageFinish();
	};

	for(const AnnotationItem& m : CodeIndex()[q].items) {
		if(m.id.EndsWith(id) &&
		   (m.id.GetCount() <= id.GetCount() || !iscid(m.id[m.id.GetCount() - id.GetCount() - 1]))) {
			DoUsage(m);
			return;
		}
	}
	
	// Probably untyped layout item, need to find all classes using the layout
	q = id.ReverseFind("::");
	if(q < 0) return;
	String bclst = id.Mid(0, q);
	String nbclst = ":" + id.Mid(0, q);
	String var = id.Mid(q + 2);
	Index<String> vars;
	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items) {
			if(IsStruct(m.kind) && m.bases.GetCount()) {
				for(String bcls : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; })) {
					if(bcls == bclst || bcls.EndsWith(nbclst)) {
						vars.FindAdd(m.id + "::" + var);
					}
				}
			}
		}

	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items) {
			if(IsVariable(m.kind) && vars.Find(m.id) >= 0) {
				DoUsage(m);
			}
		}
}
