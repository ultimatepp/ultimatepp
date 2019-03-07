#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct SvgView : public TopWindow {
	virtual bool Key(dword key, int);
	virtual void Paint(Draw& w);

	String                svg;
	FileList              files;
	SplitterFrame         splitter;
	String                dir;
	FrameTop<Button>      dirup;

	void Load(const char *filename);
	void Enter();
	void DoDir();
	void DirUp();

	typedef SvgView CLASSNAME;

	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	SvgView();
};

void SvgView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, Gray());
	Image img = RenderSVGImage(sz, svg);
	Point p = Rect(sz).CenterPos(img.GetSize());
	w.DrawImage(p.x, p.y, img);
}

void SvgView::Load(const char *filename)
{
	svg = LoadFileBOM(filename);
	Refresh();
}

void SvgView::LoadDir(const char *d)
{
	files.Clear();
	dir = d;
	Title(dir);
	::Load(files, dir, "*.*");
	SortByExt(files);
}

void SvgView::DirUp()
{
	String n = DirectoryUp(dir);
	LoadDir(dir);
	files.FindSetCursor(n);
}

void SvgView::Enter()
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

void SvgView::DoDir()
{
	if(!files.IsCursor())
		return;
	const FileList::File& f = files.Get(files.GetCursor());
	if(!f.isdir)
		return;
	LoadDir(AppendFileName(dir, f.name));
}

bool SvgView::Key(dword key, int)
{
	if(key == K_ENTER) {
		DoDir();
		return true;
	}
	if(key == K_F5) {
		dir = GetDataFile("svg");
		LoadDir();
		return true;
	}
	return false;
}

void SvgView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	files.SerializeSettings(s);
	s % dir;
	s % splitter;
}

SvgView::SvgView()
{
	files.WhenEnterItem = THISBACK(Enter);
	files.WhenLeftDouble = THISBACK(DoDir);
	dirup.Height(max(CtrlImg::DirUp().GetSize().cy, Draw::GetStdFontCy() + 6));
	dirup.SetImage(CtrlImg::DirUp());
	dirup.NormalStyle();
	dirup <<= THISBACK(DirUp);
	files.AddFrame(dirup);

	Sizeable().Zoomable();
	
	AddFrame(splitter.Left(files, 200));

	dir = GetDataFile("svg");
}

GUI_APP_MAIN
{
	SvgView x;
	LoadFromFile(x);
	x.LoadDir();
	x.Run();
	StoreToFile(x);
}
