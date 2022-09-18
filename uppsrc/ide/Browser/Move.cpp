#include <ide/ide.h>

struct MoveTopicDlg : public WithMoveTopicLayout<TopWindow> {
	typedef MoveTopicDlg CLASSNAME;

public:

	void Package();

	MoveTopicDlg();
};

void MoveTopicDlg::Package()
{
	String g = ~group;
	group.Clear();
	group.Add("src", "Reference (src)");
	group.Add("srcdoc", "Documents (srcdoc)");
	group.Add("srcimp", "Implementation (srcimp)");
	FindFile ff(SourcePath(~package, "*.tpp"));
	while(ff) {
		if(ff.IsFolder()) {
			String h = GetFileTitle(ff.GetName());
			if(h != "src" && h != "srcdoc" && h != "srcimp")
				group.Add(h);
		}
		ff.Next();
	}
	if(group.HasKey(g))
		group <<= g;
	else
		group.GoBegin();
}

MoveTopicDlg::MoveTopicDlg()
{
	CtrlLayoutOKCancel(*this, "Move topic");

	topic.NotNull();
	topic.MaxLen(30);
	topic.SetFilter(CharFilterID);

	package <<= THISBACK(Package);
	const Workspace& w = GetIdeWorkspace();
	for(int i = 0; i < w.GetCount(); i++)
		package.Add(w[i]);
	package.GoBegin();
	Package();
}

void TopicEditor::MoveTopic()
{
	MoveTopicDlg dlg;
	String p = GetCurrentTopicPath();
	TopicLink tl = ParseTopicFilePath(p);
	dlg.package <<= tl.package;
	dlg.Package();
	dlg.group <<= tl.group;
	String tn;
	int    lng;
	ParseTopicFileName(p, tn, lng);
	dlg.topic <<= tn;
	dlg.lang <<= lng;
	if(dlg.Run() != IDOK)
		return;
	String np = AppendFileName(SourcePath(~dlg.package, (String)~dlg.group + ".tpp"),
	                           (String)~dlg.topic + "_" + ToLower(LNGAsText(~dlg.lang)) + ".tpp");
	if(FindFile(np)) {
		if(!PromptYesNo("Target file aready exists!&Do you want to overwrite it?"))
			return;
		FileDelete(np);
	}
	Flush();
	RealizeDirectory(GetFileFolder(np));
	String pi = ForceExt(p, ".tppi");
	if(!SaveFile(np, LoadFile(p))) {
		Exclamation("Operation failed!");
		TopicCursor();
		return;
	}
	SaveFile(ForceExt(np, ".tppi"), LoadFile(pi));
	serial++;
	FileDelete(p);
	FileDelete(pi);
	InvalidateTopicInfoPath(p);
	InvalidateTopicInfoPath(np);
	TheIde()->IdeFlushFile();
	TheIde()->IdeOpenTopicFile(np);
}
