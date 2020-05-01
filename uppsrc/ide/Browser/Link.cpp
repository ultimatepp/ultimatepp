#include "Browser.h"

struct TopicLinkDlg : WithTopicLinkLayout<TopWindow> {
	void   Package();
	void   Group();
	void   Topic();
	void   Label();
	String LinkString();
	String ActualDir();
	String PackageGroup(const char *name);

	typedef TopicLinkDlg CLASSNAME;
	TopicLinkDlg();
};

TopicLinkDlg::TopicLinkDlg()
{
	CtrlLayoutOKCancel(*this, "Hyperlink");
	package.WhenKillCursor = package.WhenEnterItem = THISBACK(Package);
	group.WhenKillCursor = group.WhenEnterItem = THISBACK(Group);
	topic.WhenKillCursor = topic.WhenEnterItem = THISBACK(Topic);
	label.WhenKillCursor = label.WhenEnterItem = THISBACK(Label);
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		package.Add(wspc[i]);
	insert << Breaker(IDYES);
}

String TopicLinkDlg::PackageGroup(const char *name)
{
	return AppendFileName(PackageDirectory(package.GetCurrentName()), String(name) + ".tpp");
}

void TopicLinkDlg::Package()
{
	String pg = group.GetCurrentName();
	group.Clear();
	if(!package.IsCursor())
		return;
	FindFile ff(PackageGroup("*"));
	while(ff) {
		if(ff.IsFolder())
			group.Add(GetFileTitle(ff.GetName()), TopicImg::Group());
		ff.Next();
	}
	topic.Clear();
	group.FindSetCursor(pg);
}

void TopicLinkDlg::Group()
{
	topic.Clear();
	
	if(package.IsCursor() && group.IsCursor()) {
		Vector<String> topics;
		LoadTopics(topics, PackageGroup(group.GetCurrentName()));
		
		FillTopicsList(topic, topics);
	}
}

String TopicLinkDlg::LinkString()
{
	return "topic://" + (String)package.GetCurrentName() + '/' + (String)group.GetCurrentName()
	       + '/' + (String)topic.GetCurrentName();
}

void TopicLinkDlg::Topic()
{
	if(package.IsCursor() && group.IsCursor() && topic.IsCursor()) {
		link <<= LinkString();
		RichText txt = ParseQTF(ReadTopic(LoadFile(
						NormalizePath(
							AppendFileName(PackageGroup(group.GetCurrentName()),
							topic.GetCurrentName() + ".tpp")
		               ))).text);
		Vector<String> ref = GatherLabels(txt);
		label.Clear();
		for(int i = 0; i < ref.GetCount(); i++)
			label.Add(ref[i]);
	}
}

void TopicLinkDlg::Label()
{
	if(package.IsCursor() && group.IsCursor() && topic.IsCursor() && label.IsCursor())
		link <<= LinkString() + '#' + label.GetCurrentName();
}

void TopicEditor::Hyperlink(String& link, WString& text)
{
	TopicLinkDlg d;
	String label;
	TopicLink tl;
	int q = link.ReverseFind('#');
	if(q >= 0) {
		label = link.Mid(q + 1);
		tl = ParseTopicLink(link.Mid(0, q));
	}
	else
		tl = ParseTopicLink(link);
	if(IsNull(tl.topic)) {
		d.package.FindSetCursor(GetFileTitle(GetFileFolder(grouppath))) &&
		d.group.FindSetCursor(GetFileTitle(grouppath)) &&
		d.topic.FindSetCursor(topics_list.GetCurrentName());
	}
	else {
		d.package.FindSetCursor(tl.package) &&
		d.group.FindSetCursor(tl.group) &&
		d.topic.FindSetCursor(tl.topic) &&
		d.label.FindSetCursor(label);
	}
	if(link.GetCount())
		d.link <<= link;
	int c = d.Run();
	if(c == IDCANCEL)
		return;
	link = ~d.link;
	if(c == IDOK)
		return;
	if(d.topic.IsCursor()) {
		String fn = AppendFileName(d.PackageGroup(d.group.GetCurrentName()),
		                           d.topic.GetCurrentName() + ".tpp");
		text = ReadTopic(LoadFile(fn)).title.ToWString();
	}
}
