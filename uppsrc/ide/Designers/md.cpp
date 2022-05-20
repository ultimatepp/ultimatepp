#include "Designers.h"
#include <plugin/md/Markdown.h>

struct IdeMDEditPos : Moveable<IdeMDEditPos> {
	Time               filetime = Null;
	LineEdit::EditPos  editpos;
	LineEdit::UndoData undodata;
};

static VectorMap<String, IdeMDEditPos>& sEPmd()
{
	static VectorMap<String, IdeMDEditPos> x;
	return x;
}

IdeMDDes::IdeMDDes()
{
	editor << [=] { delay.KillSet(250, [=] { Preview(); }); };
	splitter.Horz(editor, preview);
	Add(splitter.SizePos());
	if(TheIde())
		editor.SetFont(((Ide *)TheIde())->editorfont);
}

void IdeMDDes::Preview()
{
	int sc = preview.GetSb();
	preview <<= MarkdownConverter().Tables().ToQtf(editor.Get());
	preview.SetSb(sc);
}

void IdeMDDes::SaveEditPos()
{
	if(filename.GetCount()) {
		IdeMDEditPos& p = sEPmd().GetAdd(filename);
		p.filetime = FileGetTime(filename);;
		p.undodata = editor.PickUndoData();
		p.editpos = editor.GetEditPos();
	}
}

bool IdeMDDes::Load(const char *filename_)
{
	filename = filename_;
	FileIn in(filename);
	if(in) {
		editor.Load(in, CHARSET_UTF8);
		IdeMDEditPos& ep = sEPmd().GetAdd(filename);
		if(ep.filetime == FileGetTime(filename)) {
			editor.SetEditPos(ep.editpos);
			editor.SetPickUndoData(pick(ep.undodata));
		}
		Preview();
		return true;
	}
	return false;
}

void IdeMDDes::Save()
{
	FileOut out(filename);
	editor.Save(out, CHARSET_UTF8);
}

void IdeMDDes::EditMenu(Bar& menu)
{
//	EditTools(menu);
}

void IdeMDDes::GotFocus()
{
	editor.SetFocus();
}

void IdeMDDes::Serialize(Stream& s)
{
}

void SerializeMDDesPos(Stream& s)
{
	VectorMap<String, IdeMDEditPos>& filedata = sEPmd();
	s.Magic();
	s.Magic(0);
	if(s.IsStoring()) {
		for(int i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty()) {
				FindFile ff(fn);
				IdeMDEditPos& ep = filedata[i];
				if(ff && ff.GetLastWriteTime() == ep.filetime) {
					s % fn;
					s % ep.filetime;
					s % ep.editpos;
				}
			}
		}
		String end;
		s % end;
	}
	else {
		String fn;
		filedata.Clear();
		for(;;) {
			s % fn;
			if(fn.IsEmpty()) break;
			IdeMDEditPos& ep = filedata.GetAdd(fn);
			s % ep.filetime;
			s % ep.editpos;
		}
	}
}

bool IsMDFile(const char *path)
{
	return ToLower(GetFileExt(path)) == ".md";
}

struct MDDesModule : public IdeModule {
	virtual String       GetID() { return "MDDesModule"; }

	virtual Image FileIcon(const char *path) {
		return IsMDFile(path) ? IdeCommonImg::MD() : Null;
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsMDFile(path)) {
			IdeMDDes *d = new IdeMDDes;
			LoadFromGlobal(*d, "mddes-ctrl");
			if(d->Load(path)) {
				return d;
			}
			delete d;
			return NULL;
		}
		return NULL;
	}

	virtual void Serialize(Stream& s) {
		int version = 0;
		s / version;
		SerializeMDDesPos(s);
	}
};

INITIALIZER(MD)
{
	RegisterIdeModule(Single<MDDesModule>());
	RegisterGlobalConfig("mddes-ctrl");
}
