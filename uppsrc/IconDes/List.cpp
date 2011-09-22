#include "IconDes.h"

NAMESPACE_UPP

static String sFormatImageName(const String& name, const Image& img, bool exp)
{
	Size sz = img.GetSize();
	String r;
	r << name << " (" << sz.cx << " x " << sz.cy << ')';
	if(exp)
		r << " ex.";
	return r;
}

void IconDes::SyncList()
{
	int a = list.GetCursorSc();
	int ii = list.GetCursor();
	list.Clear();
	for(int i = 0; i < slot.GetCount(); i++) {
		Slot& c = slot[i];
		list.Add(sFormatImageName(c.name, c.image, c.exp), c.image);
	}
	if(ii >= 0) {
		list.SetCursor(ii);
		list.ScCursor(a);
	}
}

static int sCharFilterCid(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void IconDes::PlaceDlg(TopWindow& dlg)
{
	Rect r = list.GetScreenRect();
	Size sz = dlg.GetSize();
	dlg.NoCenter().SetRect(max(0, r.left + (r.Width() - sz.cx) / 2), r.bottom + 32, sz.cx, sz.cy);
}

void IconDes::PrepareImageDlg(WithImageLayout<TopWindow>& dlg)
{
	CtrlLayoutOKCancel(dlg, "New image");
	dlg.cx <<= 16;
	dlg.cy <<= 16;
	if(IsCurrent()) {
		Size sz = GetImageSize();
		dlg.cx <<= sz.cx;
		dlg.cy <<= sz.cy;
	}
	dlg.name.SetFilter(sCharFilterCid);
}

bool CheckName(WithImageLayout<TopWindow>& dlg)
{
	String n = dlg.name;
	CParser p(n);
	if(p.IsId()) return true;
	Exclamation("Invalid name!");
	return false;
}

void IconDes::InsertRemoved(int q)
{
	if(q >= 0 && q < removed.GetCount()) {
		int ii = list.IsCursor() ? list.GetCursor() : 0;
		slot.Insert(ii) = removed[q];
		removed.Remove(q);
		SyncList();
		list.SetCursor(ii);
	}
}

void IconDes::ImageInsert(const String& name, const Image& m, bool exp)
{
	int ii = list.IsCursor() ? list.GetCursor() : 0;
	if(ii == list.GetCount() - 1)
		ii = list.GetCount();
	Slot& c = slot.Insert(ii);
	c.name = name;
	c.image = m;
	c.exp = exp;
	SyncList();
	list.SetCursor(ii);
}

void IconDes::InsertImage()
{
	WithImageLayout<TopWindow> dlg;
	PrepareImageDlg(dlg);
	do {
		if(dlg.Run() != IDOK)
			return;
	}
	while(!CheckName(dlg));
	ImageInsert(~dlg.name, CreateImage(Size(~dlg.cx, ~dlg.cy), Null), dlg.exp);
}

void IconDes::Duplicate()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	ImageInsert(c.name, c.image);
	EditImage();
}

void IconDes::InsertPaste()
{
	Image m = ReadClipboardImage();
	if(IsNull(m)) {
		Exclamation("Clipboard does not contain an image.");
		return;
	}
	ImageInsert("", m);
	EditImage();
}

struct FileImage : ImageMaker {
	String filename;
	Size   size;
	
	virtual String Key() const { return filename + '/' + AsString(size); }
	virtual Image Make() const {
		if(GetFileLength(filename) > 1024 * 1024 * 20)
			return Null;
		Image m = StreamRaster::LoadFileAny(filename);
		Size sz = m.GetSize();
		if(sz.cx > size.cx || sz.cy > size.cy) {
			if(sz.cx * size.cy > sz.cy * size.cx)
				sz = GetRatioSize(sz, size.cx, 0);
			else
				sz = GetRatioSize(sz, 0, size.cy);
			return Rescale(m, sz);
		}
		return m;
	}
};

struct ImgPreview : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		if(!IsNull(q)) {
			FileImage im;
			im.size = r.GetSize();
			im.filename = q;
			w.DrawRect(r, SColorPaper);
			Image m = MakeImage(im);
			Point p = r.CenterPos(m.GetSize());
			w.DrawImage(p.x, p.y, m);
		}
	}
};

FileSel& IconDes::ImgFile()
{
	static FileSel sel;
	ONCELOCK {
		sel.Type("Image files", "*.png *.bmp *.jpg *.jpeg *.gif");
		sel.AllFilesType();
		sel.Multi();
		sel.Preview(Single<ImgPreview>());
	}
	return sel;
}

int CharFilterImageId(int c)
{
	return IsAlNum(c) ? c : '_';
}

void IconDes::InsertFile()
{
	if(!ImgFile().ExecuteOpen()) return;
	for(int i = 0; i < ImgFile().GetCount(); i++) {
		String fn = ImgFile()[i];
		Image m = StreamRaster::LoadFileAny(fn);
		if(IsNull(m))
			Exclamation(DeQtf(fn) + " not an image.");
		ImageInsert(Filter(GetFileTitle(fn), CharFilterImageId), m);
	}
}

void IconDes::ExportPngs()
{
	String dir = SelectDirectory();
	for(int i = 0; i < GetCount(); i++)
		PNGEncoder().SaveFile(AppendFileName(dir, GetName(i) + ".png"), GetImage(i));
}

void IconDes::InsertIml()
{
	Array<ImlImage> m;
	int f;
	if(LoadIml(SelectLoadFile("Iml files\t*.iml"), m, f))
		for(int i = 0; i < m.GetCount(); i++) {
			ImageInsert(m[i].name, m[i].image, m[i].exp);
			list.SetCursor(list.GetCursor() + 1);
		}
}

void IconDes::ListCursor()
{
	SyncImage();
}

void IconDes::EditImage()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	WithImageLayout<TopWindow> dlg;
	PrepareImageDlg(dlg);
	dlg.Breaker(dlg.cx);
	dlg.Breaker(dlg.cy);
	Image img = c.image;
	dlg.cx <<= img.GetWidth();
	dlg.cy <<= img.GetHeight();
	dlg.name <<= c.name;
	dlg.exp <<= c.exp;
	for(;;) {
		switch(dlg.Run()) {
		case IDCANCEL:
			c.image = img;
			Reset();
			return;
		case IDOK:
			if(!CheckName(dlg)) break;
			c.name = ~dlg.name;
			c.exp = ~dlg.exp;
			list.Set(0, sFormatImageName(c.name, c.image, c.exp));
			Reset();
			return;
		}
		c.image = CreateImage(Size(minmax((int)~dlg.cx, 1, 8192), minmax((int)~dlg.cy, 1, 8192)), Null);
		UPP::Copy(c.image, Point(0, 0), img, img.GetSize());
		Reset();
	}
}

void IconDes::RemoveImage()
{
	if(!IsCurrent() || !PromptYesNo("Remove current image?"))
		return;
	int ii = list.GetCursor();
	while(removed.GetCount() > 12)
		removed.Remove(0);
	Slot& r = removed.Add();
	r = slot[ii];
	if(r.image.GetWidth() <= 128 && r.image.GetHeight() <= 128)
		r.base_image = Rescale(r.image, Size(16, 16));
	else
		r.base_image = IconDesImg::LargeImage();
	slot.Remove(ii);
	list.KillCursor();
	SyncList();
	if(ii < slot.GetCount())
		list.SetCursor(ii);
	else
		list.GoEnd();
}

void IconDes::MoveSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = list.GetCursor();
	d = c + d;
	if(d >= 0 && d < slot.GetCount()) {
		slot.Swap(c, d);
		SyncList();
		list.SetCursor(d);
	}
}

void IconDes::ChangeSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = list.GetCursor();
	d = c + d;
	if(d >= 0 && d < slot.GetCount())
		list.SetCursor(d);
}

void IconDes::ListMenu(Bar& bar)
{
	using namespace IconDesKeys;
	bar.Add(AK_INSERT_IMAGE, IconDesImg::Insert(), THISBACK(InsertImage));
	bar.Add(IsCurrent(), AK_IMAGE, IconDesImg::Edit(), THISBACK(EditImage));
	bar.Add(IsCurrent(), AK_REMOVE_IMAGE, IconDesImg::Remove(), THISBACK(RemoveImage));
	bar.Add(IsCurrent(), AK_DUPLICATE, IconDesImg::Duplicate(), THISBACK(Duplicate));
	bar.Add(AK_INSERT_CLIP, IconDesImg::InsertPaste(), THISBACK(InsertPaste));
	bar.Add(AK_INSERT_FILE, IconDesImg::InsertFile(), THISBACK(InsertFile));
	bar.Add(AK_INSERT_IML, IconDesImg::InsertIml(), THISBACK(InsertIml));
	bar.Add(AK_EXPORT_PNGS, IconDesImg::ExportPngs(), THISBACK(ExportPngs));
	bar.Separator();
	bar.Add(IsCurrent() && list.GetCursor() > 0, AK_MOVE_UP, IconDesImg::MoveUp(),
	        THISBACK1(MoveSlot, -1));
	bar.Add(IsCurrent() && list.GetCursor() < slot.GetCount() - 1, AK_MOVE_DOWN, IconDesImg::MoveDown(),
	        THISBACK1(MoveSlot, 1));
	if(removed.GetCount()) {
		bar.Separator();
		for(int i = removed.GetCount() - 1; i >= 0; i--) {
			Slot& r = removed[i];
			bar.Add("Insert " + sFormatImageName(r.name, r.image, r.exp), r.base_image,
			        THISBACK1(InsertRemoved, i));
		}
	}
	bar.Separator();
	EditBar(bar);
	ListMenuEx(bar);
}

void IconDes::ListMenuEx(Bar& bar) {}


void IconDes::Clear()
{
	list.Clear();
	slot.Clear();
	Reset();
}

void IconDes::AddImage(const String& name, const Image& image, bool exp)
{
	Slot& c = slot.Add();
	c.name = name;
	c.image = image;
	c.exp = exp;
	list.Add(sFormatImageName(c.name, c.image, c.exp), c.image);
	list.GoBegin();
}

int IconDes::GetCount() const
{
	return slot.GetCount();
}

Image IconDes::GetImage(int ii) const
{
	const Slot& c = slot[ii];
	Image m = slot[ii].image;
	return c.supersampling ? DownSample3x(m) : m;
}

String IconDes::GetName(int ii) const
{
	return slot[ii].name;
}

String IconDes::GetCurrentName() const
{
	if(list.IsCursor())
		return slot[list.GetCursor()].name;
	return String();
}

bool IconDes::GetExport(int ii) const
{
	return slot[ii].exp;
}

END_UPP_NAMESPACE
