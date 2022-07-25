#include "ide.h"

// libclang is unable to parse include files so we create fake .cpp content

void AssistEditor::SyncHeaders()
{
//	hdepend.CacheTime();
	TIMESTOP("SyncHeaders");
	hdepend.TimeDirty();
	hdepend.SetDirs(theide->GetCurrentIncludePath() + ";" + GetClangInternalIncludes());
	master_source.Clear();
	String editfile = NormalizePath(theide->editfile);
	if(editfile.GetCount() && IsCHeaderFile(editfile))
		master_source = FindMasterSource(hdepend, GetIdeWorkspace(), editfile);
}

bool AssistEditor::DoIncludeTrick(Index<String>& visited, int level, StringBuffer& out, String path, const String& target_path, int& line_delta)
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
			String ipath = hdepend.FindIncludeFile(tl, filedir);
			if(ipath.GetCount()) {
				if(NormalizePath(ipath) == NormalizePath(target_path))
					return true;
				int q = out.GetCount();
				int qq = line_delta;
				if(FindIndex(HdependGetDependencies(ipath), target_path) >= 0 && visited.Find(ipath) < 0 && level < 10
				   && DoIncludeTrick(visited, level + 1, out, ipath, target_path, line_delta))
					return true;
				out.SetCount(q);
				line_delta = qq;
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

void AssistEditor::MakeIncludeTrick(CurrentFileContext& cfx, int& line_delta)
{ // create pseudo source file for autocomplete in include file
	Index<String> visited;
	StringBuffer out;
	if(DoIncludeTrick(visited, 0, out, master_source, cfx.filename, line_delta))
		cfx.content = String(out) + cfx.content;
	else
		cfx.content.Clear();
}
