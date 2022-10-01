#include "ide.h"

#define LLOG(x)

// libclang is unable to parse include files so we create fake .cpp content

void AssistEditor::SyncMaster()
{
	master_source.Clear();
	String editfile = NormalizePath(theide->editfile);
	if(editfile.GetCount() && IsCHeaderFile(editfile)) {
		ppi.Dirty();
		ppi.SetIncludes(theide->GetCurrentIncludePath() + ";" + GetClangInternalIncludes());
		master_source = FindMasterSource(ppi, GetIdeWorkspace(), editfile);
	}

	if(AssistDiagnostics) {
		PutConsole("Master source " << editfile << " -> " << master_source);
		ppi.WhenBlitzBlock = [=](const String& inc, const String& path) {
			PutConsole(String() << inc << " blocks BLITZ of " << path);
		};
		if(ppi.BlitzApproved(editfile))
			PutConsole(editfile + " BLITZ approved");
	}
}

bool AssistEditor::DoIncludeTrick(Index<String>& visited, int level, StringBuffer& out, String path, const String& target_path, int& line_delta)
{
	String filedir = GetFileDirectory(path);
	bool comment = false;
	if(GetFileLength(path) > 4000000 || out.GetCount() > 4000000)
		return false;
	visited.Add(path);
	FileIn in(path);
	while(!in.IsEof()) {
		String l = in.GetLine();
		String tl = TrimLeft(l);
		if(!comment && tl.TrimStart("#include") && (*tl == ' ' || *tl == '\t')) {
			tl = TrimBoth(tl);
			String ipath = ppi.FindIncludeFile(tl, filedir);
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
				if(*tl == '\"')
					out << "#include <" << ipath << ">\n";
				else
					out << l << '\n';
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

void AssistEditor::MakeIncludeTrick(CurrentFileContext& cfx)
{ // create pseudo source file for autocomplete in include file
	Index<String> visited;
	StringBuffer out;
	if(DoIncludeTrick(visited, 0, out, master_source, cfx.filename, cfx.line_delta))
		cfx.content = String(out) + cfx.content;
	else
		cfx.content.Clear();
}
