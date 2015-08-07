#include "IconDes.h"

NAMESPACE_UPP

static String sFormatImageName(const String& name, const Image& img, bool exp)
{
	Size sz = img.GetSize();
	String r;
	r << name << " (" << sz.cx << " x " << sz.cy
	  << decode(img.GetResolution(), IMAGE_RESOLUTION_UHD, " UHD", IMAGE_RESOLUTION_NONE, " n.r.", "")
	  << ')';
	if(exp)
		r << " ex.";
	return r;
}

void IconDes::SyncList()
{
	int sc = ilist.GetScroll();
	int q = ilist.GetKey();
	ilist.Clear();
	String s = ToUpper((String)~search);
	for(int i = 0; i < slot.GetCount(); i++) {
		Slot& c = slot[i];
		if(ToUpper(c.name).Find(s) >= 0)
			ilist.Add(i, sFormatImageName(c.name, c.image, c.exp), c.image);
	}
	ilist.ScrollTo(sc);
	ilist.FindSetCursor(q);
}

void IconDes::Search()
{
	SyncList();
}

void IconDes::GoTo(int q)
{
	ilist.FindSetCursor(q);
	if(ilist.IsCursor())
		return;
	search <<= Null;
	SyncList();
	ilist.FindSetCursor(q);
}

static int sCharFilterCid(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void IconDes::PlaceDlg(TopWindow& dlg)
{
	Rect r = ilist.GetScreenRect();
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
		int resolution = IMAGE_RESOLUTION_STANDARD;
		if(IsCurrent())
			resolution = Current().image.GetResolution();
		dlg.resolution <<= decode(resolution, IMAGE_RESOLUTION_STANDARD, 0, IMAGE_RESOLUTION_UHD, 1, 2);
	}
	dlg.name.SetFilter(sCharFilterCid);
}

void IconDes::PrepareImageSizeDlg(WithImageSizeLayout<TopWindow>& dlg)
{
	CtrlLayoutOKCancel(dlg, "New image");
	dlg.cx <<= 16;
	dlg.cy <<= 16;
	if(IsCurrent()) {
		Size sz = GetImageSize();
		dlg.cx <<= sz.cx;
		dlg.cy <<= sz.cy;
	}
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
		int ii = ilist.IsCursor() ? (int)ilist.GetKey() : 0;
		slot.Insert(ii) = removed[q];
		removed.Remove(q);
		SyncList();
		GoTo(ii);
	}
}

void IconDes::ImageInsert(const String& name, const Image& m, bool exp)
{
	int ii = ilist.IsCursor() ? (int)ilist.GetKey() : 0;
	if(ii == slot.GetCount() - 1)
		ii = slot.GetCount();
	Slot& c = slot.Insert(ii);
	c.name = name;
	c.image = m;
	c.exp = exp;
	SyncList();
	GoTo(ii);
}

void SetRes(Image& m, int resolution)
{
	ImageBuffer ib(m);
	ib.SetResolution(decode(resolution, 0, IMAGE_RESOLUTION_STANDARD, 1, IMAGE_RESOLUTION_UHD, IMAGE_RESOLUTION_NONE));
	m = ib;
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
	Image m = CreateImage(Size(~dlg.cx, ~dlg.cy), Null);
	SetRes(m, ~dlg.resolution);
	ImageInsert(~dlg.name, m, dlg.exp);
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

#ifdef _MULTITHREADED
static void sLoadImage(const String& path, Image& result)
{
	if(findarg(ToLower(GetFileExt(path)), ".png", ".gif", ".jpeg", ".jpg") < 0)
		return;	
	FileIn in(path);
	if(!in)
		return;
	One<StreamRaster> r = StreamRaster::OpenAny(in);
	if(!r)
		return;
	Size sz = r->GetSize();
	if(sz.cx > 80 || sz.cy > 80)
		return;
	result = r->GetImage();
}
#endif

FileSel& IconDes::ImgFile()
{
	static FileSel sel;
	ONCELOCK {
		sel.Type("Image files", "*.png *.bmp *.jpg *.jpeg *.gif");
		sel.AllFilesType();
		sel.Multi();
#ifdef _MULTITHREADED
		sel.WhenIconLazy = sLoadImage;
#endif
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
		String id = Filter(GetFileTitle(fn), CharFilterImageId);
		if(!IsAlpha(*id) && *id != '_')
			id = '_' + id;
		ImageInsert(id, m);
	}
}

void IconDes::ExportPngs()
{
	String dir = SelectDirectory();
	if(!dir.IsEmpty()) 
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
			GoTo((int)ilist.GetKey() + 1);
		}
}

void IconDes::ListCursor()
{
	SyncImage();
}

void IconDes::EditImageSize()
{
	Slot& c = Current();
	WithImageSizeLayout<TopWindow> dlg;
	PrepareImageSizeDlg(dlg);
	dlg.Breaker(dlg.cx);
	dlg.Breaker(dlg.cy);
	Image img = c.image;
	dlg.cx <<= img.GetWidth();
	dlg.cy <<= img.GetHeight();
	for(;;) {
		switch(dlg.Run()) {
		case IDCANCEL:
			c.image = img;
			Reset();
			return;
		case IDOK:
			Reset();
			return;
		}
		c.image = CreateImage(Size(minmax((int)~dlg.cx, 1, 8192), minmax((int)~dlg.cy, 1, 8192)), Null);
		UPP::Copy(c.image, Point(0, 0), img, img.GetSize());
		Reset();
	}
}

void IconDes::EditImage()
{
	if(!IsCurrent())
		return;
	if(single_mode) {
		EditImageSize();
		return;
	}
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
			SetRes(c.image, ~dlg.resolution);
			ilist.Set(1, sFormatImageName(c.name, c.image, c.exp));
			int q = ilist.GetKey();
			Reset();
			SyncList();
			GoTo(q);
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
	int ii = ilist.GetKey();
	while(removed.GetCount() > 12)
		removed.Remove(0);
	Slot& r = removed.Add();
	r = slot[ii];
	if(r.image.GetWidth() <= 128 && r.image.GetHeight() <= 128)
		r.base_image = Rescale(r.image, Size(16, 16));
	else
		r.base_image = IconDesImg::LargeImage();
	slot.Remove(ii);
	ilist.KillCursor();
	SyncList();
	if(ii < slot.GetCount())
		GoTo(ii);
	else
		ilist.GoEnd();
}

void IconDes::MoveSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = ilist.GetKey();
	d = c + d;
	if(d >= 0 && d < slot.GetCount()) {
		slot.Swap(c, d);
		search <<= Null;
		SyncList();
		GoTo(d);
	}
}

void IconDes::ChangeSlot(int d)
{
	if(!IsCurrent())
		return;
	int c = ilist.GetCursor();
	d = c + d;
	if(d >= 0 && d < ilist.GetCount())
		ilist.SetCursor(d);
}

void IconDes::ListMenu(Bar& bar)
{
	using namespace IconDesKeys;
	if(single_mode)
		bar.Add(IsCurrent(), AK_RESIZE_SINGLE, IconDesImg::Edit(), THISBACK(EditImage));
	else {
		bar.Add(AK_INSERT_IMAGE, IconDesImg::Insert(), THISBACK(InsertImage));
		bar.Add(IsCurrent(), AK_IMAGE, IconDesImg::Edit(), THISBACK(EditImage));
		bar.Add(IsCurrent(), AK_REMOVE_IMAGE, IconDesImg::Remove(), THISBACK(RemoveImage));
		bar.Add(IsCurrent(), AK_DUPLICATE, IconDesImg::Duplicate(), THISBACK(Duplicate));
		bar.Add(AK_INSERT_CLIP, IconDesImg::InsertPaste(), THISBACK(InsertPaste));
		bar.Add(AK_INSERT_FILE, IconDesImg::InsertFile(), THISBACK(InsertFile));
		bar.Add(AK_INSERT_IML, IconDesImg::InsertIml(), THISBACK(InsertIml));
		bar.Add(AK_EXPORT_PNGS, IconDesImg::ExportPngs(), THISBACK(ExportPngs));
		bar.Separator();
		int q = ilist.GetKey();
		bar.Add(IsCurrent() && q > 0, AK_MOVE_UP, IconDesImg::MoveUp(),
		        THISBACK1(MoveSlot, -1));
		bar.Add(IsCurrent() && q < slot.GetCount() - 1, AK_MOVE_DOWN, IconDesImg::MoveDown(),
		        THISBACK1(MoveSlot, 1));
		if(removed.GetCount()) {
			bar.Separator();
			for(int i = removed.GetCount() - 1; i >= 0; i--) {
				Slot& r = removed[i];
				bar.Add("Insert " + sFormatImageName(r.name, r.image, r.exp), r.base_image,
				        THISBACK1(InsertRemoved, i));
			}
		}
	}
	bar.Separator();
	EditBar(bar);
	ListMenuEx(bar);
}

void IconDes::ListMenuEx(Bar& bar) {}


void IconDes::Clear()
{
	ilist.Clear();
	slot.Clear();
	Reset();
}

void IconDes::AddImage(const String& name, const Image& image, bool exp)
{
	int q = slot.GetCount();
	Slot& c = slot.Add();
	c.name = name;
	c.image = image;
	c.exp = exp;
	ilist.Add(q, sFormatImageName(c.name, c.image, c.exp), c.image);
	ilist.GoBegin();
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

bool IconDes::FindName(const String& name)
{
	for(int i = 0; i < slot.GetCount(); i++)
		if(slot[i].name == name) {
			GoTo(i);
			return true;
		}
	return false;
}

String IconDes::GetCurrentName() const
{
	if(ilist.IsCursor())
		return GetName(ilist.GetKey());
	return String();
}

bool IconDes::GetExport(int ii) const
{
	return slot[ii].exp;
}

END_UPP_NAMESPACE
