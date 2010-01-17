#include "TabBar.h"

NAMESPACE_UPP

String FileTabs::GetFileGroup(const String &file)
{
	return GetFileDirectory(file);
}

String FileTabs::GetStackId(const Tab &a)
{
	String s = a.value;
	return GetFileTitle(s);
}

unsigned FileTabs::GetStackSortOrder(const Tab &a)
{
	String s = a.value;
	return GetFileExt(s).GetHashValue();
}

void FileTabs::PaintTab(Draw& w, const Rect &r, const Tab& tab, const Font &font, Color ink, dword style)
{
	const Value &q = tab.value;
	WString txt = IsString(q) ? q : StdConvert().Format(q);

	Point p;
	int text_offset = TB_MARGIN;
	if (PaintIcons() && tab.HasIcon()) {
		Size isz = min(tab.img.GetSize(), Size(TB_ICON, TB_ICON));
		p = GetImagePosition(r, isz.cx, isz.cy, text_offset, LEFT);
		w.DrawImage(p.x, p.y, isz.cx, isz.cy, tab.img);
		text_offset += TB_ICON + 2;
	}

	int extpos = txt.ReverseFind('.');
	Size tsz = GetTextSize(txt, font, extpos);
	int ang = GetTextAngle();
	
	p = GetTextPosition(r, tsz.cy, text_offset);
	w.DrawText(p.x, p.y, ang, txt, font, filecolor, extpos);

	if (extpos >= 0) {
		int extlen = txt.GetLength() - extpos;
		p = GetTextPosition(r, tsz.cy, tsz.cx + text_offset);
		w.DrawText(p.x, p.y, ang, txt.GetIter(extpos), font, extcolor, extlen);
	}
}

void FileTabs::PaintStackedTab(Draw& w, const Rect &r, const Tab& tab, const Font &font, Color ink, dword style)
{
	Point p;
	int ang = GetTextAngle();
	int tcy = GetTextSize("|", font, 1).cy;
	
	p = GetTextPosition(r, tcy, 0);
	w.DrawText(p.x, p.y, ang, "|", font, ink, 1);

	if (stackedicons && tab.HasIcon()) {
		Size isz = min(tab.img.GetSize(), Size(TB_ICON, TB_ICON));
		p = GetImagePosition(r, isz.cx, isz.cy, TB_SPACEICON, LEFT);
		w.DrawImage(p.x, p.y, isz.cx, isz.cy, (style == CTRL_HOT) ? tab.img : (greyedicons ? DisabledImage(tab.img) : tab.img));
	}
	else {
		const Value &q = tab.value;
		WString txt = IsString(q) ? q : StdConvert().Format(q);
		int extpos = txt.ReverseFind('.');
		if (extpos >= 0) {
			int extlen = txt.GetLength() - extpos;
			p = GetTextPosition(r, tcy, TB_SPACEICON);
			w.DrawText(p.x, p.y, ang, txt.GetIter(extpos), font, (style == CTRL_HOT) ? extcolor : SColorDisabled(), extlen);				
		}
	}
}

Size FileTabs::GetStackedSize(const Tab &t)
{
	if (stackedicons && t.HasIcon())
		return min(t.img.GetSize(), Size(TB_ICON, TB_ICON)) + Size(TB_SPACEICON, 0);
	String s = t.value;
	s = GetFileExt(s);
	return GetTextSize(s.ToWString(), GetStyle().font) + Size(TB_SPACEICON, 0); 
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
	for (int i = files.GetCount()-1; i > 0; i--) {
		TabBar::InsertKey0(ix, files[i].ToWString(), GetFileName(files[i]), 
			useimg ? img[i] : NativePathIcon(files[i]), GetFileGroup(files[i]));	
	}
	InsertFile(ix, files[0].ToWString(), useimg ? img[0] : NativePathIcon(files[0]), make_active);
}

void FileTabs::RenameFile(WString &from, WString &to, Image icon)
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
	GroupSort(false);
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
normalicons(true), 
stackedicons(false), 
greyedicons(true),
filecolor(SColorText),
extcolor(LtBlue)
{
	
}

END_UPP_NAMESPACE
