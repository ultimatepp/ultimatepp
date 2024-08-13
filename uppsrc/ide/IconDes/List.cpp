#include "IconDes.h"

String IconDes::FormatImageName(const Slot& c)
{
	Size sz = c.image.GetSize();
	String r;
	r << c.name << " " << sz.cx << " x " << sz.cy;
	if(c.flags & IML_IMAGE_FLAG_FIXED)
		r << " Fxd";
	else {
		if(c.flags & IML_IMAGE_FLAG_FIXED_COLORS)
			r << " Clr";
		if(c.flags & IML_IMAGE_FLAG_FIXED_SIZE)
			r << " Sz";
	}
	if(c.flags & IML_IMAGE_FLAG_UHD)
		r << " HD";
	if(c.flags & IML_IMAGE_FLAG_DARK)
		r << " Dk";
	if(c.flags & IML_IMAGE_FLAG_S3)
		r << " S3";
	if(c.exp)
		r << " X";
	return r;
}

void IconDes::SyncList()
{
	if(syncinglist)
		return;
	syncinglist++;
	int sc = ilist.GetScroll();
	int q = ilist.GetKey();
	ilist.Clear();
	String s = ToUpper((String)~search);
	for(int i = 0; i < slot.GetCount(); i++) {
		Slot& c = slot[i];
		if(ToUpper(c.name).Find(s) >= 0)
			ilist.Add(i, FormatImageName(c), RawToValue(MakeTuple(c.image, c.flags)));
	}
	ilist.ScrollTo(sc);
	ilist.FindSetCursor(q);
	syncinglist--;
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

		dword flags = IsCurrent() ? Current().flags : 0;
		dlg.fixed <<= !!(flags & IML_IMAGE_FLAG_FIXED);
		dlg.fixed_colors <<= !!(flags & IML_IMAGE_FLAG_FIXED_COLORS);
		dlg.fixed_size <<= !!(flags & IML_IMAGE_FLAG_FIXED_SIZE);
		
		dlg.uhd <<= !!(flags & IML_IMAGE_FLAG_UHD);
		dlg.dark <<= !!(flags & IML_IMAGE_FLAG_DARK);
		
		dlg.s3 <<= !!(flags & IML_IMAGE_FLAG_S3);
		
		for(Ctrl& q : dlg)
			if(dynamic_cast<Option *>(&q))
				q << [&] { dlg.Break(-1000); };
	}
	dlg.name.SetFilter(sCharFilterCid);
}

void IconDes::SyncDlg(WithImageLayout<TopWindow>& dlg)
{
	bool b = !dlg.fixed;
	dlg.fixed_colors.Enable(b);
	dlg.fixed_size.Enable(b);
}

dword IconDes::GetFlags(WithImageLayout<TopWindow>& dlg)
{
	dword flags = 0;
	if(dlg.fixed)
		flags |= IML_IMAGE_FLAG_FIXED;
	if(dlg.fixed_colors)
		flags |= IML_IMAGE_FLAG_FIXED_COLORS;
	if(dlg.fixed_size)
		flags |= IML_IMAGE_FLAG_FIXED_SIZE;
	if(dlg.uhd)
		flags |= IML_IMAGE_FLAG_UHD;
	if(dlg.dark)
		flags |= IML_IMAGE_FLAG_DARK;
	if(dlg.s3)
		flags |= IML_IMAGE_FLAG_S3;
	return flags;
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
	String n = ~dlg.name;
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

IconDes::Slot& IconDes::ImageInsert(int ii, const String& name, const Image& m, bool exp)
{
	Slot& c = slot.Insert(ii);
	c.name = name;
	c.image = m;
	c.exp = exp;
	search <<= Null;
	SyncList();
	GoTo(ii);
	return c;
}

IconDes::Slot& IconDes::ImageInsert(const String& name, const Image& m, bool exp)
{
	int ii = ilist.IsCursor() ? (int)ilist.GetKey() : 0;
	if(ii == slot.GetCount() - 1)
		ii = slot.GetCount();
	return ImageInsert(ii, name, m, exp);
}

void IconDes::InsertImage()
{
	WithImageLayout<TopWindow> dlg;
	PrepareImageDlg(dlg);
	for(;;) {
		SyncDlg(dlg);
		int c = dlg.Run();
		if(c == IDCANCEL)
			return;
		if(c == IDOK && CheckName(dlg))
			break;
	}
	Image m = CreateImage(Size(~dlg.cx, ~dlg.cy), Null);
	ImageInsert(~dlg.name, m, dlg.exp).flags = GetFlags(dlg);
	SyncList();
}

void IconDes::Slice()
{
	if(!IsCurrent())
		return;
	Image src = Current().image;
	Size isz = src.GetSize();
	int cc = min(isz.cx, isz.cy);
	if(!cc)
		return;
	WithImageLayout<TopWindow> dlg;
	PrepareImageDlg(dlg);
	dlg.name <<= Current().name;
	dlg.cx <<= cc;
	dlg.cy <<= cc;
	dlg.Title("Slice image");
	for(;;) {
		SyncDlg(dlg);
		int c = dlg.Run();
		if(c == IDCANCEL)
			return;
		if(c == IDOK && CheckName(dlg))
			break;
	}
	String s = ~dlg.name;
	int n = 0;
	int ii = ilist.GetKey();
	for(int y = 0; y < isz.cy; y += (int)~dlg.cy)
		for(int x = 0; x < isz.cx; x += (int)~dlg.cx) {
			Image m = Crop(src, x, y, ~dlg.cx, ~dlg.cy);
			ImageInsert(++ii, s + AsString(n++), m, ~dlg.exp).flags = GetFlags(dlg);
		}
	SyncList();
}

void IconDes::Duplicate()
{
	if(!IsCurrent())
		return;
	Slot& c = Current();
	dword flags = c.flags;
	int ii = ilist.GetCursor();
	ii = ii >= 0 ? ii + 1 : 0;
	ImageInsert(ii, c.name, c.image);
	Current().flags = flags;
	SyncList();
	SyncShow();
	Refresh();
	EditImage();
}

int CharFilterImageId(int c)
{
	return IsAlNum(c) ? c : '_';
}

String FixImageName(String name)
{
	if(IsNull(name))
		return name;
	int q = name.Find("\n");
	if(q >= 0)
		name.Trim(q);
	if(name.GetCount() > 32)
		name.Trim(32);
	String id = Filter(name, CharFilterImageId);
	if(!IsAlpha(*id) && *id != '_')
		id = '_' + id;
	return id;
}

void IconDes::InsertPaste()
{
	Image m = ReadClipboardImage();
	if(IsNull(m)) {
		Exclamation("Clipboard does not contain an image.");
		return;
	}
	ImageInsert(FixImageName(ReadClipboardText()), m);
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

FileSel& IconDes::ImgFile()
{
	static FileSel sel;
	ONCELOCK {
		sel.Type("Image files", "*.iml *.png *.bmp *.jpg *.jpeg *.gif *.ico");
		sel.AllFilesType();
		sel.Multi();
		sel.WhenIconLazy = sLoadImage;
		sel.Preview(Single<ImgPreview>());
	}
	return sel;
}

void IconDes::InsertFile()
{
	if(!ImgFile().ExecuteOpen()) return;
	String errors;
	for(int i = 0; i < ImgFile().GetCount(); i++) {
		String fn = ImgFile()[i];
		Array<ImlImage> ml;
		String ext = ToLower(GetFileExt(fn));
		String id = FixImageName(GetFileTitle(fn));
		if(ext == ".iml") {
			int f;
			LoadIml(LoadFile(fn), ml, f);
		}
		else
		if(ext == ".ico") {
			Index<Image> im;
			for(Image m : ReadIcon(LoadFile(fn), true))
				im.FindAdd(m);
			int ii = 0;
			for(Image mm : im) {
				ImlImage& m = ml.Add();
				m.image = mm;
				m.name = id;
				if(ii)
					m.name << "_" << ii;
				ii++;
			}
		}
		else {
			Image mm = StreamRaster::LoadFileAny(fn);
			if(!IsNull(mm)) {
				ImlImage& m = ml.Add();
				m.image = mm;
				m.name = id;
			}
		}
		if(ml.GetCount() == 0)
			errors << fn << '\n';
		int ii = ilist.IsCursor() ? (int)ilist.GetKey() : 0;
		if(ii == slot.GetCount() - 1)
			ii = slot.GetCount();
		for(ImlImage m : ml) {
			ImageInsert(ii, m.name, m.image, m.exp).flags = m.flags;
			GoTo(ii++);
		}
	}
	if(errors.GetCount())
		Exclamation("Failed to load:&&\1" + errors);
}

void IconDes::ExportPngs()
{
	String dir = SelectDirectory();
	if(!dir.IsEmpty())
		for(int i = 0; i < GetCount(); i++) {
			dword f = GetFlags(i);
			String n = GetName(i);
			if(f & IML_IMAGE_FLAG_UHD)
				n << ".uhd";
			if(f & IML_IMAGE_FLAG_DARK)
				n << ".dark";
			PNGEncoder().SaveFile(AppendFileName(dir, n + ".png"), GetImage(i));
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
			SyncList();
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
	dlg.Title("Image");
	dlg.Breaker(dlg.cx);
	dlg.Breaker(dlg.cy);
	Image img = c.image;
	dword flags = c.flags;
	bool exp = c.exp;
	String name = c.name;

	Vector<ImageIml> bimg;
	ImageIml mm;
	mm.image = c.image;
	bimg << mm;
	dlg.estimated_size = FormatFileSize(PackImlData(bimg).GetCount());
	
	dlg.cx <<= img.GetWidth();
	dlg.cy <<= img.GetHeight();
	dlg.name <<= c.name;
	dlg.exp <<= c.exp;
	for(;;) {
		SyncDlg(dlg);
		switch(dlg.Run()) {
		case IDCANCEL:
			c.image = img;
			c.flags = flags;
			c.exp = exp;
			c.name = name;
			Reset();
			return;
		case IDOK:
			if(CheckName(dlg)) {
				c.name = ~dlg.name;
				c.exp = ~dlg.exp;
				c.flags = GetFlags(dlg);
				ilist.Set(1, FormatImageName(c));
				int q = ilist.GetKey();
				Reset();
				SyncList();
				GoTo(q);
				return;
			}
		}
		c.name = ~dlg.name;
		c.flags = GetFlags(dlg);
		c.image = CreateImage(Size(minmax((int)~dlg.cx, 1, 8192), minmax((int)~dlg.cy, 1, 8192)), Null);
		c.exp = ~dlg.exp;
		UPP::Copy(c.image, Point(0, 0), img, img.GetSize());
		SetHotSpots(c.image, img.GetHotSpot(), img.Get2ndSpot());
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
		bar.Add(AK_INSERT_CLIP, IconDesImg::InsertPaste(), THISBACK(InsertPaste));
		bar.Add(AK_INSERT_FILE, CtrlImg::open(), THISBACK(InsertFile));
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
				bar.Add("Insert " + FormatImageName(r), r.base_image, THISBACK1(InsertRemoved, i));
			}
		}
	}
	bar.Separator();
	EditBar(bar);
	ListMenuEx(bar);
}

void IconDes::Clear()
{
	ilist.Clear();
	slot.Clear();
	Reset();
}

IconDes::Slot& IconDes::AddImage(const String& name, const Image& image, bool exp)
{
	int q = slot.GetCount();
	Slot& c = slot.Add();
	c.name = name;
	c.image = image;
	c.exp = exp;
	ilist.Add(q, FormatImageName(c), RawToValue(MakeTuple(c.image, c.flags)));
	ilist.GoBegin();
	return c;
}

int IconDes::GetCount() const
{
	return slot.GetCount();
}

Image IconDes::GetImage(int ii) const
{
	return slot[ii].image;
}

String IconDes::GetName(int ii) const
{
	return slot[ii].name;
}

dword IconDes::GetFlags(int ii) const
{
	return slot[ii].flags;
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

void IconDes::DnDInsert(int line, PasteClip& d)
{
	if(GetInternalPtr<ArrayCtrl>(d, "icondes-icon") == &ilist && IsCurrent() &&
	   line >= 0 && line <= slot.GetCount() && d.Accept()) {
		int c = ilist.GetKey();
		slot.Move(c, line);
		if(c <= line)
			line--;
		search <<= Null;
		SyncList();
		GoTo(line);
	}
}

void IconDes::Drag()
{
	ilist.DoDragAndDrop(InternalClip(ilist, "icondes-icon"), ilist.GetDragSample(), DND_MOVE);
}
