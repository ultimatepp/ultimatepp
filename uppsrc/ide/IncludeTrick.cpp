#include "ide.h"

#define LLOG(x)

// libclang is unable to parse include files so we create fake .cpp content

void AssistEditor::SyncMaster()
{
	if(!theide)
		return;
	master_source.Clear();
	String editfile = NormalizePath(theide->editfile);
	if(editfile.GetCount() && IsCHeaderFile(editfile)) {
		ppi.Dirty();
		ppi.SetIncludes(theide->GetCurrentIncludePath() + ";" + GetClangInternalIncludes());
		master_chain = clone(FindMasterSourceCached(ppi, GetIdeWorkspace(), editfile, ms_cache).GetKeys());
		master_source = master_chain.GetCount() ? master_chain.Top() : String();
	}

	if(AssistDiagnostics) {
		PutConsole("Master source " << editfile << " -> " << master_source);
		PutConsole("Master chain " << AsString(master_chain));
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
		CParser p(l);
		if(!comment && p.Char('#') && p.Id("include")) {
			String tl = TrimBoth(p.GetPtr());
			String ipath = ppi.FindIncludeFile(tl, filedir);
			if(ipath.GetCount()) {
				if(NormalizePath(ipath) == NormalizePath(target_path))
					return true;
				int q = out.GetCount();
				int qq = line_delta;
				if(level < master_chain.GetCount() - 1 && master_chain[master_chain.GetCount() - 2 - level] == ipath &&
				   visited.Find(ipath) < 0 && level < 10
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
	if(DoIncludeTrick(visited, 0, out, master_source, cfx.filename, cfx.line_delta)) {
		bool comment = false;
		String filedir = GetFileDirectory(cfx.filename);
		StringStream in(cfx.content);
		StringBuffer out2;
		while(!in.IsEof()) {
			String l = in.GetLine();
			String tl = TrimLeft(l);
			bool processed = false;
			if(!comment && tl.TrimStart("#include") && (*tl == ' ' || *tl == '\t')) {
				tl = TrimBoth(tl);
				String ipath = ppi.FindIncludeFile(tl, filedir);
				if(ipath.GetCount())
					if(*tl == '\"') {
						out2 << "#include <" << ipath << ">\n";
						processed = true;
					}
			}
			if(!processed) {
				out2 << l << '\n';
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
		cfx.content = String(out) + String(out2);
	}
	else
		cfx.content.Clear();
}
