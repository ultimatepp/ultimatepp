#include "ide.h"

// libclang is unable to parse include files so we create fake .cpp content

bool DoIncludeTrick(Index<String>& visited, int level, StringBuffer& out, String path, const String& target_path, int& line_delta)
{
	String filedir = GetFileDirectory(path);
	bool comment = false;
	if(GetFileLength(path) > 200000 || out.GetCount() > 200000)
		return false;
	visited.Add(path);
	FileIn in(path);
	while(!in.IsEof()) {
		String l = in.GetLine();
		String tl = TrimLeft(l);
		if(!comment && tl.TrimStart("#include") && (*tl == ' ' || *tl == '\t')) {
			String ipath = FindIncludeFile(tl, filedir);
			if(ipath.GetCount()) {
				if(NormalizePath(ipath) == NormalizePath(target_path))
					return true;
				if(FindIndex(HdependGetDependencies(ipath), target_path) >= 0 && visited.Find(ipath) < 0 && level < 10
				   && DoIncludeTrick(visited, level + 1, out, ipath, target_path, line_delta))
					return true;
				out << "#include <" << ipath << ">\n";
				line_delta++;
			}
			else {
				out << l << '\n';
				line_delta++;
			}
		}
		else {
			out << l << '\n';
			line_delta++;
			for(const char *s = l; *s; s++) {
				if(s[0] == '/' && s[1] == '*') {
					comment = true;
					s++;
				}
				if(s[0] == '*' && s[1] == '/') {
					comment = false;
					s++;
				}
			}
		}
	}
	return false;
}

bool MakeIncludeTrick(const Package& pk, const String pk_name, CurrentFileContext& cfx, int& line_delta)
{ // create pseudo source file for autocomplete in include file
	HdependTimeDirty();
	for(int i = 0; i < pk.file.GetCount(); i++) {
		String path = SourcePath(pk_name, pk.file[i]);
		if(!PathIsEqual(cfx.filename, path) && IsSourceFile(path)) {
			if(FindIndex(HdependGetDependencies(path), cfx.filename) >= 0 && GetFileLength(path) < 200000) {
				StringBuffer out;
				Index<String> visited;
				if(DoIncludeTrick(visited, 0, out, path, cfx.filename, line_delta))
					cfx.content = String(out) + cfx.content;
				else
					cfx.content.Clear();
				return true;
			}
		}
	}
	return false;
}
