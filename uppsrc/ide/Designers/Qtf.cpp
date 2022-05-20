#include "Designers.h"

struct IdeQtfEditPos : Moveable<IdeQtfEditPos> {
	Time               filetime;
	RichEdit::PosInfo  pos;
	RichEdit::UndoInfo uf;

	IdeQtfEditPos()           { filetime = Null; }
};

static VectorMap<String, IdeQtfEditPos>& sEP()
{
	static VectorMap<String, IdeQtfEditPos> x;
	return x;
}

void IdeQtfDes::SaveEditPos()
{
	if(filename.GetCount()) {
		IdeQtfEditPos& p = sEP().GetAdd(filename);
		p.filetime = FileGetTime(filename);;
		p.pos = GetPosInfo();
		p.uf = PickUndoInfo();
	}
}

bool IdeQtfDes::Load(const char *_filename)
{
	RichText txt;
	if(ParseQTF(txt, LoadFile(_filename))) {
		filename = _filename;
		Pick(pick(txt));
		IdeQtfEditPos& ep = sEP().GetAdd(filename);
		if(ep.filetime == FileGetTime(filename)) {
			SetPosInfo(ep.pos);
			SetPickUndoInfo(pick(ep.uf));
			ep.uf.Clear();
		}
		return true;
	}
	return false;
}

void IdeQtfDes::Save()
{
	SaveFile(filename, GetQTF());
}

void IdeQtfDes::EditMenu(Bar& menu)
{
	EditTools(menu);
}

void IdeQtfDes::Serialize(Stream& s)
{
	SerializeSettings(s);
}

void SerializeQtfDesPos(Stream& s)
{
	VectorMap<String, IdeQtfEditPos>& filedata = sEP();
	s.Magic();
	s.Magic(0);
	if(s.IsStoring()) {
		for(int i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty()) {
				FindFile ff(fn);
				IdeQtfEditPos& ep = filedata[i];
				if(ff && ff.GetLastWriteTime() == ep.filetime) {
					s % fn;
					s % ep.filetime;
					s % ep.pos;
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
			IdeQtfEditPos& ep = filedata.GetAdd(fn);
			s % ep.filetime;
			s % ep.pos;
		}
	}
}

bool IsQtfFile(const char *path)
{
	return ToLower(GetFileExt(path)) == ".qtf";
}

struct QtfDesModule : public IdeModule {
	virtual String       GetID() { return "QtfDesModule"; }

	virtual Image FileIcon(const char *path) {
		return IsQtfFile(path) ? IdeCommonImg::Qtf() : Null;
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsQtfFile(path)) {
			IdeQtfDes *d = new IdeQtfDes;
			LoadFromGlobal(*d, "qtfdes-ctrl");
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
		SerializeQtfDesPos(s);
	}
};

INITIALIZER(Qtf)
{
	RegisterIdeModule(Single<QtfDesModule>());
	RegisterGlobalConfig("qtfdes-ctrl");
}
