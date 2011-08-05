#include "ide.h"

void Ide::ExportMakefile(const String& ep)
{
	SaveMakeFile(AppendFileName(ep, "Makefile"), true);
}

void Ide::ExportProject(const String& ep, bool all, bool gui)
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
		used.FindAdd(SourcePath(pn, "init"));
	}
	if(FileExists(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " is existing file.&"
		                "Do you want to delete it?")) return;
		FileDelete(ep);
	}
	if(DirectoryExists(ep)) {
		if(gui && !PromptYesNo(DeQtf(ep) + " is existing directory.&"
		                "Do you want to replace it?")) return;
		DeleteFolderDeep(ep);
	}

	Progress pi("Exporting project");
	pi.SetTotal(wspc.GetCount());
	for(int i = 0; i < wspc.GetCount(); i++) {
		if(gui && pi.StepCanceled())
			return;
		CopyFolder(AppendFileName(ep, wspc[i]), PackageDirectory(wspc[i]), used, all);
	}
	ExportMakefile(ep);
}
