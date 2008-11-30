#include "IconDes.h"

#include <plugin/bmp/bmp.h>
#include <plugin/png/png.h>

bool IdeIconDes::Load(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = FileGetTime(filename);
	Array<ImlImage> m;
	int f;
	if(!LoadIml(LoadFile(filename), m, f))
		return false;
	format = f;
	for(int i = 0; i < m.GetCount(); i++)
		AddImage(m[i].name, m[i].image, m[i].exp);
	return true;
}


void IdeIconDes::Save()
{
	if(format == 1) {
		for(int i = 0; i < GetCount(); i++) {
			Image m = GetImage(i);
			Point p = m.Get2ndSpot();
			if(m.GetKind() == IMAGE_ALPHA || p.x || p.y) {
				if(PromptYesNo("Legacy file format does not support images "
				               "with full alpha channel or 2nd hotspot - "
				               "the information would be lost.&"
				               "Do you wish to convert the file to the new format?")) {
					format = 0;
				}
				break;
			}
		}
	}
	StoreToGlobal(*this, "icondes-ctrl");
	Array<ImlImage> m;
	VectorMap<Size, Image> exp;
	String folder = GetFileFolder(filename);
	for(int i = 0; i < GetCount(); i++) {
		ImlImage& c = m.Add();
		c.name = GetName(i);
		c.image = GetImage(i);
		c.exp = GetExport(i);
		if(c.exp) {
			Size sz = c.image.GetSize();
			exp.GetAdd(sz) = c.image;
			PNGEncoder png;
			png.SaveFile(AppendFileName(folder, String().Cat() << "icon" << sz.cx << 'x' << sz.cy << ".png"), c.image);
		}
	}
	String d = SaveIml(m, format);
	if(!SaveChangedFileFinish(filename, d))
		return;
	filetime = FileGetTime(filename);
	if(exp.GetCount())
		SaveFile(AppendFileName(folder, "icon.ico"), WriteIcon(exp.GetValues()));
}

void IdeIconDes::ToolEx(Bar& bar)
{
	bar.Separator();
	bar.Add("File properties..", IconDesImg::FileProperties(), THISBACK(FileProperties));
}

void IdeIconDes::FileProperties()
{
	WithFilePropertiesLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "File properties");
	dlg.format <<= format;
	if(dlg.Run() == IDOK)
		format = ~dlg.format;
}

void IdeIconDes::Serialize(Stream& s)
{
	SerializeSettings(s);
}

struct IdeIconEditPos : IconDes::EditPos, Moveable<IdeIconEditPos> {
	Time filetime;

	IdeIconEditPos() { filetime = Null; }
};

static VectorMap<String, IdeIconEditPos>& sEP()
{
	static VectorMap<String, IdeIconEditPos> x;
	return x;
}

void IdeIconDes::SaveEditPos()
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

String IdeIconDes::GetFileName() const
{
	return filename;
}

bool IsImlFile(const char *path)
{
	return ToLower(GetFileExt(path)) == ".iml";
}

void IdeIconDes::EditMenu(Bar& bar)
{
	EditBar(bar);
	ToolEx(bar);
	bar.Add("List", THISBACK(ListMenu));
	bar.Add("Selection", THISBACK(SelectBar));
	bar.Add("Image", THISBACK(ImageBar));
	bar.Add("Draw", THISBACK(DrawBar));
	bar.Add("Settings", THISBACK(SettingBar));
}

struct IconDesModule : public IdeModule {
	virtual void CleanUsc() {}
	virtual bool ParseUsc(CParser& p) { return false; }
	virtual Image FileIcon(const char *path) {
		return IsImlFile(path) ? IconDesImg::FileIcon() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(IsImlFile(path)) {
			IdeIconDes *d = new IdeIconDes;
			LoadFromGlobal(*d, "icondes-ctrl");
			if(d->Load(path)) {
				IdeIconEditPos& ep = sEP().GetAdd(path);
				if(ep.filetime == d->filetime)
					d->SetEditPos(ep);
				return d;
			}
			delete d;
			return NULL;
		}
		return false;
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

void IdeIconDes::CopyId(const String& n)
{
	ClearClipboard();
	AppendClipboardText(n);
}

void IdeIconDes::ListMenuEx(Bar& bar)
{
	String n = GetCurrentName();
	String c = GetFileTitle(filename);
	c.Set(0, ToUpper(c[0]));
	c = c.EndsWith("Img") ? c : c + "Img";
	c << "::" << n << "()";
	bar.Separator();
	bar.Add(n.GetCount(), "Copy '" + c + '\'', CtrlImg::copy(), THISBACK1(CopyId, c));
}

INITBLOCK {
	RegisterGlobalConfig("icondes-ctrl");
}
