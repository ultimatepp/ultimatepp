#include "Browser.h"

void TopicEditor::ListTemplates(Vector<String>& path, Vector<String>& name)
{
	Vector<String> ud = GetUppDirs();
	for(int i = 0; i < ud.GetCount(); i++) {
		String nest = ud[i];
		String tmpl = AppendFileName(nest, "_.tpp");
		FindFile ff(AppendFileName(tmpl, "*.tpp"));
		while(ff) {
			if(ff.IsFile()) {
				path.Add(AppendFileName(tmpl, ff.GetName()));
				name.Add(GetFileTitle(ff.GetName()) + " (in " + GetFileTitle(nest) + ")");
			}
			ff.Next();
		}
	}
	IndexSort(name, path);
}

String TopicEditor::ChooseTemplate(const char *title)
{
	WithTemplateListLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, title);
	Vector<String> path, name;
	ListTemplates(path, name);
	dlg.list.AddIndex();
	dlg.list.AddColumn();
	for(int i = 0; i < path.GetCount(); i++)
		dlg.list.Add(path[i], name[i]);
	dlg.list.GoBegin();
	if(dlg.Execute() != IDOK)
		return Null;
	return dlg.list.GetKey();
}

void TopicEditor::ApplyStylesheet()
{
	if(!topics_list.IsCursor())
		return;
	String t = ChooseTemplate("Apply template stylesheet");
	if(t.GetCount())
		editor.ApplyStylesheet(ParseQTF(LoadFile(t)));
}

void TopicEditor::ApplyStylesheetGroup()
{
	String t = ChooseTemplate("Apply template stylesheet to current group");
	if(IsNull(t))
		return;
	RichText ss = ParseQTF(LoadFile(t));
	int c = topics_list.GetCursor();
	Progress pi("Applying stylesheet");
	for(int i = 0; i < topics_list.GetCount(); i++) {
		topics_list.SetCursor(i);
		editor.ApplyStylesheet(ss);
	}
	topics_list.SetCursor(c);
}
