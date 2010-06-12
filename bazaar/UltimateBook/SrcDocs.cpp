#include <Core/Core.h>
#include <ide/Browser/Browser.h>
#include <Functions4U/Functions4U.h>

using namespace Upp;

String FolderLinks(const String uppsrc, const String package, const String group, const String lang, GatherTpp &tt)
{
	String qtf;
	FindFile ff(AppendFileName(AppendFileName(AppendFileName(uppsrc, package), group + ".tpp"), "*.tpp"));
	while(ff) {
		if(ff.IsFile()) {
			if (ff.GetName().Find(lang) >= 0) {
				String title;
				String tl = "topic://" + package + '/' + group + '/' + GetFileTitle(ff.GetName());
				tt.GatherTopics(tl, title);
				qtf << "________[^" << tl << "^ " << DeQtf(Nvl(title, tl)) << "]&";
			}
		}
		ff.Next();
	}
	return qtf;
}

void SrcDocs(Index<String> &x, String& qtf, String uppsrc, const char *folder, String lang, GatherTpp &tt)
{
	if(x.Find(folder) >= 0)
		return;
	x.Add(folder);
	String srcdoc = FolderLinks(uppsrc, folder, "srcdoc", lang, tt);
	String src = FolderLinks(uppsrc, folder, "src", lang, tt);
	Package p;
	p.Load(AppendFileName(uppsrc, AppendFileName(folder, GetFileName(folder) + ".upp")));
	if(srcdoc.GetLength() || src.GetLength()) {
		qtf << "&&&[*4@b " << folder << "]&";
		if(!IsNull(p.description))
			qtf << "[2 " << p.description << "]&";
		if(srcdoc.GetCount()) {
			qtf << "&[3/* " + Format(t_("Using %s"), folder) << "]&";
			qtf << srcdoc;
		}
		if(src.GetCount()) {
			qtf << "&[3/* " << Format(t_("%s reference"), folder) << "]&";
			qtf << src;
		}
	}
}