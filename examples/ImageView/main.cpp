#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class ImageView : public TopWindow {
public:
	virtual bool Key(dword key, int);

private:
	ImageCtrl             img;
	FileList              files;
	Splitter              splitter;
	String                dir;
	FrameTop<Button>      dirup;

	void Load(const char *filename);
	void Enter();
	void DoDir();
	void DirUp();

public:
	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	ImageView();
};

void ImageView::Load(const char *filename)
{
	Image m = StreamRaster::LoadFileAny(filename);
	if(IsNull(m))
		return;
	Size rsz = img.GetSize();
	Size isz = m.GetSize();
	if(isz.cx >= rsz.cx || isz.cy >= rsz.cy) {
		if(isz.cx * rsz.cx < rsz.cy * isz.cy)
			rsz.cx = isz.cx * rsz.cy / isz.cy;
		else
			rsz.cy = isz.cy * rsz.cx / isz.cx;
		m = Rescale(m, rsz);
		img.SetImage(Rescale(m, rsz));
	}

	img.SetImage(m);
}

void ImageView::LoadDir(const char *d)
{
	files.Clear();
	dir = d;
	Title(dir);
	::Load(files, dir, "*.*");
	SortByExt(files);
}

void ImageView::DirUp()
{
	String n = DirectoryUp(dir);
	LoadDir(dir);
	files.FindSetCursor(n);
}

void ImageView::Enter()
{
	if(!files.IsCursor()) {
		Title(dir);
		return;
	}
	const FileList::File& f = files.Get(files.GetCursor());
	if(f.name == "..") {
		Title(dir);
		return;
	}
	String p = AppendFileName(dir, f.name);
	Title(p);
	if(!f.isdir)
		Load(p);
}

void ImageView::DoDir()
{
	if(!files.IsCursor())
		return;
	const FileList::File& f = files.Get(files.GetCursor());
	if(!f.isdir)
		return;
	LoadDir(AppendFileName(dir, f.name));
}

bool ImageView::Key(dword key, int)
{
	if(key == K_ENTER) {
		DoDir();
		return true;
	}
	return false;
}

void ImageView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	files.SerializeSettings(s);
	s % dir;
	s % splitter;
}

ImageView::ImageView()
{
	splitter.Horz(files, img);
	splitter.SetPos(2700);
	Add(splitter.SizePos());

	files.WhenEnterItem = [=] { Enter(); };
	files.WhenLeftDouble = [=] { DoDir(); };
	dirup.Height(max(CtrlImg::DirUp().GetSize().cy, Draw::GetStdFontCy() + 6));
	dirup.SetImage(CtrlImg::DirUp());
	dirup.NormalStyle();
	dirup << [=] { DirUp(); };
	files.AddFrame(dirup);

	Sizeable().Zoomable();

	dir = GetCurrentDirectory();
}

GUI_APP_MAIN
{
	ImageView x;
	LoadFromFile(x);
	x.LoadDir();
	x.Run();
	StoreToFile(x);
}
