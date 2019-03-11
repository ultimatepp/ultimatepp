#include "Browser.h"

ArrayMap<String, TopicEditor::FileInfo> TopicEditor::editstate;
VectorMap<String, String>               TopicEditor::grouptopic; // the last topic edited in group

String     TopicEditor::lasttemplate;
int        TopicEditor::lastlang;
bool       TopicEditor::allfonts;

struct ListOrder : FileList::Order {
public:
	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		return ToUpper(a.name) < ToUpper(b.name);
	}
};

bool LoadTopics(Vector<String>& topics, const String& grouppath)
{
	bool renamed = false;
	
	for(int pass = 0; pass < 2; pass++) {
		topics.Clear();

		FindFile ff(AppendFileName(grouppath, "*.*"));
		for(; ff; ff.Next()) {
			if(ff.IsFile() && GetFileExt(ff.GetName()) == ".tppi") {
				String n = ff.GetName();
				int q = n.ReverseFind('$');
				if(q < 0)
					q = n.ReverseFind('@');
				if(q >= 0 && q > n.GetLength() - 12) {
					String nn = n;
					n.Set(q, '_');
					FileMove(AppendFileName(grouppath, nn), AppendFileName(grouppath, n));
					renamed = true;
				}
			}
			if(ff.IsFile() && GetFileExt(ff.GetName()) == ".tpp") {
				String n = ff.GetName();
				int q = n.ReverseFind('$');
				if(q < 0)
					q = n.ReverseFind('@');
				if(q >= 0 && q > n.GetLength() - 12) {
					String nn = n;
					n.Set(q, '_');
					FileMove(AppendFileName(grouppath, nn), AppendFileName(grouppath, n));
					renamed = true;
				}
				
				topics.Add(GetFileTitle(n));
			}
		}
		if(!renamed)
			break;
	}
	
	return renamed;
}

void FillTopicsList(FileList& list, const Vector<String>& topics)
{
	list.Clear();
	
	for (const auto& topic : topics)
		list.Add(topic, TopicImg::Topic());
	
	list.Sort(ListOrder());
	list.Enable();
	
}

void TopicEditor::Open(const String& group_path)
{

	topics_search.Clear();
	
	grouppath = group_path;
	if(FileExists(grouppath))
		DeleteFile(grouppath);
	DirectoryCreate(grouppath);
	
	if(LoadTopics(topics, grouppath))
		SaveInc();
	FillTopicsList(topics_list, topics);
	
	int q = grouptopic.Find(grouppath);
	if(q >= 0)
		topics_list.FindSetCursor(grouptopic[q]);
	else
		topics_list.SetCursor(0);
}

void TopicEditor::OpenFile(const String& path)
{
	grouppath.Clear();
	singlefilepath = path;
	topics_list.Clear();
	topics_list.Add(GetFileTitle(path), TopicImg::Topic());
	topics_list.Enable();
	topics_list.SetCursor(0);
}

void TopicEditor::OnSearch()
{
	auto current_topic = topics_list.GetCurrentName();
	auto topic_list_sel_callback = topics_list.WhenSel;
	topics_list.WhenSel = {};
	
	DoSearch();
	
	auto idx = topics_list.Find(current_topic);
	if (idx >= 0) {
		topics_list.SetCursor(idx);
	}
	
	topics_list.WhenSel = topic_list_sel_callback;
}

void TopicEditor::DoSearch()
{
	auto search_phase = ToLower(topics_search.GetData().ToString());
		
	auto found_topics = Vector<String>();
	for (const auto& topic : topics) {
		const auto normalized_topic = ToLower(topic);
		if (normalized_topic.Find(search_phase) >= 0) {
			found_topics.Add(topic);
		}
	}
	FillTopicsList(topics_list, found_topics);
}

String TopicEditor::GetCurrentTopicPath()
{
	if(topics_list.IsCursor())
		return NormalizePath(AppendFileName(grouppath, topics_list.GetCurrentName() + ".tpp"));
	else
		return Null;
}

void TopicEditor::ShowTopic(bool b)
{
	title.Enable(b);
	title.Show(b);
	editor.Enable(b);
	editor.Show(b);
}

void TopicEditor::TopicCursor()
{
	String h;
	HideTopic();
	if(IsNull(grouppath)) {
		ShowTopic();
		h = singlefilepath;
	}
	else {
		if(!topics_list.IsCursor())
			return;
		h = GetCurrentTopicPath();
	}
	if(h != topicpath)
		Load(h);
	else
		ShowTopic();
}

void TopicEditor::Load(const String& fn)
{
	Flush();

	Topic t = ReadTopic(LoadFile(fn));
	if(t.text.IsVoid()) {
		Exclamation("Error loading the topic file:&[* " + DeQtf(fn));
		topics_list.KillCursor();
		return;
	}

	title <<= t.title;
	editor <<= t.text;
	topicpath = fn;

	int q = editstate.Find(fn);
	if(q >= 0) {
		FileInfo& fi = editstate[q];
		if(fi.time == FileGetTime(fn)) {
			editor.SetPickUndoInfo(pick(fi.undo));
			fi.time = Time(1, 1, 1);
			editor.SetPosInfo(fi.pos);
		}
	}

	grouptopic.GetAdd(grouppath) = GetFileTitle(fn);

	ShowTopic();

	editor.SetFocus();
	editor.ClearModify();
	title.ClearModify();
}

int TopicEditor::serial;

int TopicEditor::GetSerial()
{
	return serial;
}

void TopicEditor::SaveTopic()
{
	if(IsNull(topicpath))
		return;
	if(IsNull(~title)) {
		const RichText& txt = editor.Get();
		if(txt.IsPara(0)) {
			RichPara para = txt.Get(0);
			WString t;
			for(int i = 0; i < para.GetCount(); i++)
				if(para[i].IsText())
					for(const wchar *s = para[i].text; *s; s++) {
						if(*s == '\t' || *s == 160)
							t.Cat(' ');
						else
						if(*s >= ' ')
							t.Cat(*s);
					}
			if(!IsNull(t))
				title <<= t;
		}
	}
	if(!editor.IsModified() && !title.IsModified())
		return;
	String r = WriteTopic((String)~title, editor.Get());
	if(LoadFile(topicpath) != r) {
		serial++;
		SaveFile(topicpath, r);
		if(FileExists(AppendFileName(grouppath, "all.i")))
			SaveFile(ForceExt(topicpath, ".tppi"), WriteTopicI((String)~title, editor.Get()));
		TopicLink tl = ParseTopicFilePath(topicpath);
		if(tl)
			SyncTopicFile(editor.Get(), TopicLinkString(tl), topicpath, ~title);
	}
}

void TopicEditor::Flush()
{
	if(IsNull(topicpath))
		return;
	SaveTopic();
	FileInfo& fi = editstate.GetAdd(topicpath);
	fi.time = FileGetTime(topicpath);
	fi.pos = editor.GetPosInfo();
	fi.undo = editor.PickUndoInfo();
	topicpath.Clear();
	editor.Clear();
	HideTopic();
}

void TopicEditor::SaveInc()
{
	SaveGroupInc(grouppath);
}

TopicLink ParseTopicFilePath(const String& path)
{
	TopicLink tl;
	tl.topic = GetFileTitle(path);
	String q = GetFileFolder(path);
	tl.group = GetFileTitle(q);
	q = GetFileFolder(q);
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		if(PathIsEqual(PackageDirectory(wspc[i]), q)) {
			tl.package = wspc[i];
			return tl;
		}
	return TopicLink();
}

String TopicFilePath(const TopicLink& tl)
{
	return AppendFileName(AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"),
	                      tl.topic + ".tpp");
}
