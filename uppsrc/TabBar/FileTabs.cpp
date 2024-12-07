#include "TabBar.h"

namespace Upp {

String FileTabs::GetFileGroup(const String &file)
{
	return GetFileDirectory(file);
}

String FileTabs::GetStackId(const Tab &a)
{
	String s = a.value;
	return GetFileTitle(s);
}

hash_t FileTabs::GetStackSortOrder(const Tab &a)
{
	String s = a.value;
	return GetFileExt(s).GetHashValue();
}

void FileTabs::ComposeTab(Tab& tab, const Font &font, Color ink, int style)
{
	if(PaintIcons() && tab.HasIcon())
	{
		tab.AddImage(tab.img);
		tab.AddSpace(DPI(TB_SPACEICON));
	}

	WString txt = IsString(tab.value) ? tab.value : StdConvert().Format(tab.value);
	int extpos = txt.ReverseFind('.');
	tab.AddText(extpos >= 0 ? txt.Left(extpos) : txt, font, filecolor);

	if (extpos >= 0) {
		tab.AddText(txt.Right(txt.GetLength() - extpos), font, extcolor);
	}
}

void FileTabs::ComposeStackedTab(Tab& tab, const Tab& stacked_tab, const Font &font, Color ink, int style)
{
	tab.AddImage(TabBarImg::STSEP);

	if (stackedicons && tab.HasIcon()) {
		tab.AddImage(style == CTRL_HOT ? stacked_tab.img : (greyedicons ? DisabledImage(stacked_tab.img) : stacked_tab.img))
			.Clickable();
	}
	else {
		WString txt = IsString(stacked_tab.value) ? stacked_tab.value : StdConvert().Format(stacked_tab.value);
		int extpos = txt.ReverseFind('.');
	
		Color c = (style == CTRL_HOT) ? extcolor : SColorDisabled();
		if (extpos >= 0) {
			tab.AddText(
				txt.Mid(extpos + 1),
				font,
				c
			).Clickable();
		}
		else {
			tab.AddText("-", font, c).Clickable();
		}
	}
}

Size FileTabs::GetStackedSize(const Tab &t)
{
	if (stackedicons && t.HasIcon())
		return min(t.img.GetSize(), Size(DPI(TB_ICON), DPI(TB_ICON))) + Size(DPI(TB_SPACEICON), 0) + DPI(5);

	WString txt = IsString(t.value) ? t.value : StdConvert().Format(t.value);
	int extpos = txt.ReverseFind('.');
	txt = extpos >= 0 ? txt.Mid(extpos + 1) : "-";
	return GetTextSize(txt, GetStyle().font) + Size(TabBarImg::STSEP().GetSize().cx, 0);
}

void FileTabs::Serialize(Stream& s)
{
	TabBar::Serialize(s);
	if(s.IsLoading() && icons)
	{
		for(int i = 0; i < tabs.GetCount(); i++)
			tabs[i].img = NativePathIcon(String(tabs[i].value));
	}
}

void FileTabs::AddFile(const WString &file, bool make_active)
{
	AddFile(file, NativePathIcon(file.ToString()), make_active);
}

void FileTabs::AddFile(const WString &file, Image img, bool make_active)
{
	InsertFile(GetCount(), file, img, make_active);
}

void FileTabs::InsertFile(int ix, const WString &file, bool make_active)
{
	InsertFile(ix, file, NativePathIcon(file.ToString()), make_active);
}

void FileTabs::InsertFile(int ix, const WString &file, Image img, bool make_active)
{
	String s = file.ToString();
	TabBar::InsertKey(ix, file, GetFileName(s), img, GetFileGroup(s), make_active);
}

void FileTabs::AddFiles(const Vector<String> &files, bool make_active)
{
	AddFiles(files, Vector<Image>(), make_active);
}

void FileTabs::AddFiles(const Vector<String> &files, const Vector<Image> &img, bool make_active)
{
	InsertFiles(GetCount(), files, img, make_active);
}

void FileTabs::InsertFiles(int ix, const Vector<String> &files, bool make_active)
{
	InsertFiles(ix, files, Vector<Image>(), make_active);
}

void FileTabs::InsertFiles(int ix, const Vector<String> &files, const Vector<Image> &img, bool make_active)
{
	if (!files.GetCount()) return;
	bool useimg = img.GetCount() == files.GetCount();
	for (int i = files.GetCount() - 1; i > 0; i--) {
		TabBar::InsertKey0(ix, files[i].ToWString(), GetFileName(files[i]), 
			useimg ? img[i] : NativePathIcon(files[i]), GetFileGroup(files[i]));	
	}
	InsertFile(ix, files[0].ToWString(), useimg ? img[0] : NativePathIcon(files[0]), make_active);
}

void FileTabs::RenameFile(const WString &from, const WString &to, Image icon)
{
	int n = FindKey(from);
	if (n >= 0)
		Set(n, to, GetFileName(to.ToString()), IsNull(icon) ? NativePathIcon(to.ToString()) : icon);
}

FileTabs& FileTabs::FileIcons(bool normal, bool stacked, bool stacked_greyedout)
{
	stackedicons = stacked;
	greyedicons = stacked_greyedout;
	TabBar::Icons(normal); 
	return *this;	
}

Vector<String> FileTabs::GetFiles() const
{
	Vector<String> files;
	files.SetCount(tabs.GetCount());
	for (int i = 0; i < tabs.GetCount(); i++)
		files[i] = tabs[i].key;
	return files;	
}

FileTabs& FileTabs::operator<<(const FileTabs &src)
{
	Clear();
	Stacking(false);
	SortGroups(false);
	Grouping(false);
	AddFiles(src.GetFiles(), src.GetIcons(), false);
	CopySettings(src);	
	stackedicons = src.stackedicons;
	greyedicons = src.greyedicons;
	
	if (src.HasCursor())
		SetData(~src);
	sc.SetPos(src.GetScrollPos());
	Refresh();
	return *this;
}

FileTabs::FileTabs() :
stackedicons(false), 
greyedicons(true),
filecolor(SColorLabel)
{
	static SColor se([] { return IsDark(SColorFace()) ? Blend(White, LtBlue) : LtBlue; });
	extcolor = se;
}

}
