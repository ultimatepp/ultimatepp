#include "ide.h"

#define LLOG(x) // DLOG(x)

void Ide::FindUnitializedMemberVariables(int scope)
{
	struct NavItem : AnnotationItem {
		String path;
	};

	struct ClassInfo {
		bool           has_constructors;
		Array<NavItem> vars;
		Array<NavItem> methods;
	};
	
	ArrayMap<String, ClassInfo> nests;
	VectorMap<String, Vector<int>> ranges;
	
	auto IsFundamental = [&](const char *s) {
		try {
			CParser p(s);
			return p.Id("signed") || p.Id("unsigned") || p.Id("int") || p.Id("long") || p.Id("char") || p.Id("bool") ||
			       p.Id("double") || p.Id("float") || p.Id("word") || p.Id("dword") || p.Id("qword") ||
			       p.Id("int64") || p.Id("uint64");
		}
		catch(CParser::Error) {}
		return false;
	};

	String nest_dir = GetPathNest(editfile);
	String package_dir = GetActivePackageDir();
	
	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items) {
			ranges.GetAdd(f.key).Add(m.pos.y);
		#if 0
			if(m.kind == CXCursor_FieldDecl || m.kind == CXCursor_Constructor || 1) {
				LLOG("=======");
				DDUMP(m.id);
				DDUMP(m.type);
				DDUMP(m.isstatic);
				DDUMP(m.pretty);
				DDUMP(m.nest);
				DDUMP(m.nspace);
				DDUMP(m.pretty0);
				DDUMP(m.name);
				DDUMP(m.pretty0.Find('='));
				DDUMP(m.pos);
			}
		#endif
			auto Add = [&](NavItem& mm) {
				mm.path = f.key;
				(AnnotationItem&)mm = m;
			};
			if(m.kind == CXCursor_FieldDecl && IsFundamental(m.type) && !m.isstatic && m.pretty0.Find('=') < 0 &&
			   (scope == 2 || scope == 1 && f.key.StartsWith(nest_dir) || scope == 0 && f.key.StartsWith(package_dir))) {
				LLOG("Candidate " << m.id);
				Add(nests.GetAdd(m.nest).vars.Add());
			}
			if(IsFunction(m.kind) && m.definition) {
				LLOG("Function " << m.id);
				if(m.definition)
					Add(nests.GetAdd(m.nest).methods.Add());
			}
			if(m.kind == CXCursor_Constructor && m.access == CX_CXXPublic) {
				LLOG("Constructor " << m.id);
				nests.GetAdd(m.nest).has_constructors = true;
			}
		}

	for(auto& v : ranges)
		Sort(v);

	NewFFound();
	
	FFoundSetText("uninitialized");

	Index<String> unique;
	
	Progress pi("Analyzing", nests.GetCount());
	
	for(const auto& n : nests) {
		if(pi.StepCanceled()) break;
		if(n.has_constructors)
			for(const auto& m : n.vars) {
				bool init_in_method = false;
				for(const auto& c : n.methods) {
					String path = c.path;
					int q = ranges.Find(path);
					if(q >= 0) {
						const Vector<int>& v = ranges[q];
						int endl = INT_MAX;
						for(int l : v)
							if(l > c.pos.y) {
								endl = l;
								break;
							}
						String code;
						for(int i = c.pos.y; i < endl; i++) {
							String s = GetFileLine(path, i);
							if(s.IsVoid())
								break;
							code << s << '\n';
						}
						
						try {
							CParser p(code);
							while(!p.IsEof()) {
								if(p.Id(m.name) && p.Char('(')) {
									LLOG("Initialization of " << m.name << " found in initializer list " << c.id);
									init_in_method = true;
									break;
								}
								else
								if(p.Char(';') || p.Char('{')) {
									while(!p.IsEof()) {
										if((p.Id("this") && p.Char2('-', '>') || !p.Char('.')&& !p.Char2('-', '>') && !p.Char2(':', ':'))
										   && p.Id(m.name) && p.Char('=')) {
											LLOG("Initialization of " << m.name << " found in constructor body " << c.id);
											init_in_method = true;
											break;
										}
										else
											p.Skip();
									}
									break;
								}
								else
									p.Skip();
							}
						}
						catch(CParser::Error) {}
					}
					if(init_in_method)
						break;
				}
				if(!init_in_method)
					AddReferenceLine(m.path, m.pos, m.name, unique);
		}
	}

	FFoundFinish();
}
