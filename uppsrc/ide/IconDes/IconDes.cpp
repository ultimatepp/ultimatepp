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
			SaveChangedFile(AppendFileName(folder, String().Cat() << "icon" << sz.cx << 'x' << sz.cy << ".png"),
			                png.SaveString(c.image));
		}
	}
	String d = SaveIml(m, format);
	if(!SaveChangedFileFinish(filename, d))
		return;
	filetime = FileGetTime(filename);
	if(exp.GetCount())
		SaveChangedFile(AppendFileName(folder, "icon.ico"), WriteIcon(exp.GetValues()));
}

void IdeIconDes::ToolEx(Bar& bar)
{
	bar.Separator();
	if(!IsSingleMode())
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

String IdeIconDes::GetFileName() const
{
	return filename;
}

void IdeIconDes::EditMenu(Bar& bar)
{
	EditBar(bar);
	ToolEx(bar);
	bar.Add(IsSingleMode() ? "PNG" : "List", THISBACK(ListMenu));
	bar.Add("Selection", THISBACK(SelectBar));
	bar.Add("Image", THISBACK(ImageBar));
	bar.Add("Draw", THISBACK(DrawBar));
	bar.Add("Settings", THISBACK(SettingBar));
}
