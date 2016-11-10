#include "ide.h"

void Ide::ExportMakefile(const String& ep)
{
	SaveMakeFile(AppendFileName(ep, "Makefile"), true);
}

void Ide::ExportProject(const String& ep, bool all, bool gui, bool deletedir)
{
	SaveFile(false);
	::Workspace wspc;
	wspc.Scan(main);
	Index<String> used;
	HdependClearDependencies();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& p = wspc.GetPackage(i);
		String pn = wspc[i];
		for(int j = 0; j < p.GetCount(); j++) {
			const Package::File& f = p[j];
			if(!f.separator) {
				String p = SourcePath(pn, f);
				used.FindAdd(p);
				Vector<String> d = HdependGetDependencies(p);
				for(int q = 0; q < d.GetCount(); q++)
					used.FindAdd(d[q]);
				for(int q = 0; q < f.depends.GetCount(); q++)
					used.FindAdd(SourcePath(pn, f.depends[q].text));
			}
		}
	}
	if(FileExists(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " is existing file.&"
		                "Do you want to delete it?")) return;
		FileDelete(ep);
	}
	if(deletedir && DirectoryExists(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " is existing directory.&"
		                "Do you want to replace it?")) return;
		DeleteFolderDeep(ep);
	}

	Progress pi("Exporting project");
	pi.SetTotal(wspc.GetCount());
	for(int i = 0; i < wspc.GetCount(); i++) {
		if(gui && pi.StepCanceled())
			return;
		CopyFolder(AppendFileName(ep, wspc[i]), PackageDirectory(wspc[i]), used, all, true);
	}
	Vector<String> upp = GetUppDirs();
	for(int i = 0; i < upp.GetCount(); i++) {
		if(gui && pi.StepCanceled())
			return;
		String d = upp[i];
		FindFile ff(AppendFileName(d, "*"));
		while(ff) {
			if(ff.IsFile()) {
				String fn = ff.GetName();
				String path = AppendFileName(d, fn);
				if(all || used.Find(path) >= 0)
					CopyFile(AppendFileName(ep, fn), path, true);
			}
			ff.Next();
		}
		CopyFolder(AppendFileName(ep, wspc[i]), PackageDirectory(wspc[i]), used, all, true);
	}
	ExportMakefile(ep);
}
