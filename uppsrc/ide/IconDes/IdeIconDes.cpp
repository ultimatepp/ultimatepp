#include <ide/ide.h>

bool IconDes::Load(const char *_filename)
{
	Clear();
	filename = _filename;
	filetime = FileGetTime(filename);
	Array<ImlImage> iml;
	int f;
	String s = LoadFile(filename);
	EOL = GetLineEndings(s);
	if(!LoadIml(s, iml, f))
		return false;
	format = f;
	for(const ImlImage& m : iml)
		AddImage(m.name, m.image, m.exp).flags = m.flags;
	return true;
}

void IconDes::Save()
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
		c.flags = GetFlags(i);
		if(c.exp) {
			Size sz = c.image.GetSize();
			exp.GetAdd(sz) = c.image;
			PNGEncoder png;
			SaveChangedFile(AppendFileName(folder, String().Cat() << "icon" << sz.cx << 'x' << sz.cy << ".png"),
			                png.SaveString(c.image));
		}
	}
	String d = SaveIml(m, format, EOL);
	if(!SaveChangedFileFinish(filename, d))
		return;
	filetime = FileGetTime(filename);
	if(exp.GetCount())
		SaveChangedFile(AppendFileName(folder, "icon.ico"), WriteIcon(exp.GetValues()));
}

void IconDes::ToolEx(Bar& bar)
{
	bar.Separator();
	if(!IsSingleMode()) {
		bar.Add("Usage..", IdeCommonImg::Cpp(), [=] {
			String name = GetCurrentName();
			TheIde()->FindDesignerItemReferences(name + "()", name);
		});
//		bar.Add("File properties..", IconDesImg::FileProperties(), THISBACK(FileProperties));
	}
}

void IconDes::FileProperties()
{
	WithFilePropertiesLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "File properties");
	dlg.format <<= format;
	if(dlg.Run() == IDOK)
		format = ~dlg.format;
}

void IconDes::Serialize(Stream& s)
{
	SerializeSettings(s);
}

String IconDes::GetFileName() const
{
	return filename;
}

void IconDes::EditMenu(Bar& bar)
{
	EditBar(bar);
	ToolEx(bar);
	bar.Add(IsSingleMode() ? "PNG" : "List", THISBACK(ListMenu));
	bar.Add("Selection", THISBACK(SelectBar));
	bar.Add("Image", THISBACK(ImageBar));
	bar.Add("Draw", THISBACK(DrawBar));
	bar.Add("Settings", THISBACK(SettingBar));
}
