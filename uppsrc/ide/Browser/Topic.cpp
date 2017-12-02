#include "Browser.h"

ArrayMap<String, TopicEditor::FileInfo> TopicEditor::editstate;
VectorMap<String, String>               TopicEditor::grouptopic;

String     TopicEditor::lasttemplate;
int        TopicEditor::lastlang;
bool       TopicEditor::allfonts;

struct ListOrder : FileList::Order {
public:
	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		return ToUpper(a.name) < ToUpper(b.name);
	}
};

bool LoadTopics(FileList& topic, const String& grouppath)
{
	bool renamed = false;
	for(int pass = 0; pass < 2; pass++) {
		topic.Clear();
		FindFile ff(AppendFileName(grouppath, "*.*"));
		while(ff) {
			if(ff.IsFile() && GetFileExt(ff.GetName()) == ".tppi") {
				String n = ff.GetName();
				int q = n.ReverseFind('$');
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
				topic.Add(GetFileTitle(n), TopicImg::Topic());
			}
			ff.Next();
		}
		if(!renamed)
			break;
	}
	topic.Sort(ListOrder());
	topic.Enable();
	return renamed;
}

void TopicEditor::Open(const String& _grouppath)
{
	grouppath = _grouppath;
	if(FileExists(grouppath))
		DeleteFile(grouppath);
	DirectoryCreate(grouppath);
	if(LoadTopics(topic, grouppath))
		SaveInc();
	int q = grouptopic.Find(grouppath);
	if(q >= 0)
		topic.FindSetCursor(grouptopic[q]);
	else
		topic.SetCursor(0);
}

void TopicEditor::OpenFile(const String& path)
{
	grouppath.Clear();
	singlefilepath = path;
	topic.Clear();
	topic.Add(GetFileTitle(path), TopicImg::Topic());
	topic.Enable();
	topic.SetCursor(0);
}

String TopicEditor::GetCurrentTopicPath()
{
	if(topic.IsCursor())
		return NormalizePath(AppendFileName(grouppath, topic.GetCurrentName() + ".tpp"));
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
		if(!topic.IsCursor())
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
		topic.KillCursor();
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
