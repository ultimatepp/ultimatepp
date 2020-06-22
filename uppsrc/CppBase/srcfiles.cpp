#include "CppBase.h"
#include "Internal.h"

namespace Upp {

#define LTIMING(x) // DTIMING(x)

static VectorMap<String, String> sSrcFile;
static Index<uint64>             sIncludes;

String NormalizeSourcePath(const String& path, const String& currdir)
{
	LTIMING("NormalizeSourcePath");
#ifdef PLATFORM_WIN32
	return ToLower(NormalizePath(path, currdir));
#else
	return NormalizePath(path, currdir);
#endif
}

String NormalizeSourcePath(const String& path)
{
	return NormalizeSourcePath(path, GetCurrentDirectory());
}

void ClearSources()
{
	sSrcFile.Clear();
	sIncludes.Clear();
}

const Index<String>& GetAllSources()
{
	return sSrcFile.GetIndex();
}

const VectorMap<String, String>& GetAllSourceMasters()
{
	return sSrcFile;
}

void GatherSources(const String& master_path, const String& path_, Vector<int>& parents)
{
	String path = NormalizeSourcePath(path_);
	if(sSrcFile.Find(path) >= 0)
		return;
	int ii = sSrcFile.GetCount();
	for(int i = 0; i < parents.GetCount(); i++)
		sIncludes.Add(MAKEQWORD(parents[i], ii));
	sSrcFile.Add(path, master_path);
	parents.Add(ii);
	const PPFile& f = GetPPFile(path);
	Index<String> todo;
	for(int i = 0; i < f.includes.GetCount(); i++) {
		String p = GetIncludePath(f.includes[i], GetFileFolder(path));
		if(p.GetCount())
			todo.FindAdd(p);
	}
	MakePP(todo); // parse PP files in parallel to accelerate things...
	for(String p : todo)
		GatherSources(master_path, p, parents);
	parents.Drop();
}

void GatherSources(const String& master_path, const String& path)
{
	LTIMING("GatherSources");
	Vector<int> parents;
	GatherSources(NormalizeSourcePath(master_path), path, parents);
}

String GetMasterFile(const String& file)
{
	return sSrcFile.Get(file, Null);
}

bool   IncludesFile(const String& parent_path, const String& header_path)
{
	LTIMING("IncludesFile");
	int pi = sSrcFile.Find(parent_path);
	int i = sSrcFile.Find(header_path);
	return pi >= 0 && i >= 0 && sIncludes.Find(MAKEQWORD(pi, i)) >= 0;
}

}
