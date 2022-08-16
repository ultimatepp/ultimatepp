#include <ide/ide.h>

bool IsTopicFile(const char *path)
{
	return ToLower(GetFileExt(path)) == ".tpp" && FileExists(path);
}

bool IsTopicGroup(const char *path)
{
	return ToLower(GetFileExt(path)) == ".tpp" && (!IsFullPath(path) || !FileExists(path));
}

struct TopicModule : public IdeModule {
	virtual String       GetID() { return "TopicModule"; }

	virtual Image FileIcon(const char *path) {
		return IsTopicFile(path) ? TopicImg::Topic() : IsTopicGroup(path) ? TopicImg::Group() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsTopicGroup(path)) {
			TopicEditor *d = new TopicEditor;
			d->PersistentFindReplace(TheIde()->IsPersistentFindReplace());
			d->Open(path);
			return d;
		}
		if(IsTopicFile(path)) {
			TopicEditor *d = new TopicEditor;
			d->PersistentFindReplace(TheIde()->IsPersistentFindReplace());
			d->OpenFile(path);
			return d;
		}
		return NULL;
	}
	virtual void Serialize(Stream& s) {
		TopicEditor::SerializeEditPos(s);
	}
};

void InitializeTopicModule()
{
	RegisterIdeModule(Single<TopicModule>());
}

INITIALIZER(CodeBase)
{
	void InitializeTopicModule();
	InitializeTopicModule();
}
