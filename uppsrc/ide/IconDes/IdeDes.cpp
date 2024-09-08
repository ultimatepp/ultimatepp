#include "IconDes.h"

static VectorMap<String, IdeIconEditPos>& sEP()
{
	static VectorMap<String, IdeIconEditPos> x;
	return x;
}

void IconDes::SaveEditPos()
{
	IdeIconEditPos& ep = sEP().GetAdd(filename);
	ep.filetime = filetime;
	(EditPos&)ep = GetEditPos();
}

void SerializeIconDesPos(Stream& s)
{
	VectorMap<String, IdeIconEditPos>& filedata = sEP();
	if(s.IsStoring()) {
		for(int i = 0; i < filedata.GetCount(); i++) {
			String fn = filedata.GetKey(i);
			if(!fn.IsEmpty()) {
				FindFile ff(fn);
				IdeIconEditPos& ep = filedata[i];
				if(ff && ff.GetLastWriteTime() == ep.filetime) {
					s % fn;
					s % ep.filetime;
					s % ep.cursor;
					s % ep.sc;
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
			IdeIconEditPos& ep = filedata.GetAdd(fn);
			s % ep.filetime;
			s % ep.cursor;
			s % ep.sc;
		}
	}
}

void IconDes::RestoreEditPos()
{
	IdeIconEditPos& ep = sEP().GetAdd(filename);
	if(ep.filetime == filetime)
		SetEditPos(ep);
}

bool IsImlFile(const char *path)
{
	return ToLower(GetFileExt(path)) == ".iml";
}

struct IconDesModule : public IdeModule {
	virtual String       GetID() { return "IconDesModule"; }
	virtual Image FileIcon(const char *path) {
		return IsImlFile(path) ? IconDesImg::FileIcon() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsImlFile(path)) {
			IconDes *d = new IconDes;
			LoadFromGlobal(*d, "icondes-ctrl");
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
		SerializeIconDesPos(s);
	}
};

void RegisterIconDes()
{
	RegisterIdeModule(Single<IconDesModule>());
}

void IconDes::CopyId(const String& n)
{
	ClearClipboard();
	AppendClipboardText(n);
}

void IconDes::FindId(const String& id)
{
	FindName(id);
}

void IconDes::ListMenuEx(Bar& bar)
{
	if(IsSingleMode())
		return;
	String n = GetCurrentName();
	String c = GetFileTitle(filename);
	c.Set(0, ToUpper(c[0]));
	c = c.EndsWith("Img") ? c : c + "Img";
	c << "::" << n << "()";
	bar.Separator();
	bar.Add(n.GetCount(), "Copy '" + c + '\'', CtrlImg::copy(), THISBACK1(CopyId, c));
}

INITIALIZER(IconDes) {
	RegisterGlobalConfig("icondes-ctrl");
}
