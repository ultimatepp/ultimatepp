#include "XmlView.h"

#define IMAGEFILE  <XmlView/XmlView.iml>
#define IMAGECLASS XmlImg
#include <Draw/iml_source.h>

void XmlView::Load(int parent, XmlParser& p)
{
	if(p.IsTag()) {
		String txt = p.ReadTag();
		for(int i = 0; i < p.GetAttrCount(); i++)
			txt << ' ' << p.GetAttr(i) << "=\"" << p[i] << "\"";
		parent = xml.Add(parent, XmlImg::Tag(), txt);
		while(!p.End()) {
			if(p.IsEof())
				throw XmlError("");
			Load(parent, p);
		}
	}
	else
	if(p.IsText())
		xml.Add(parent, XmlImg::Text(), NormalizeSpaces(p.ReadText()));
	else
	if(p.IsPI())
		xml.Add(parent, XmlImg::PI(), NormalizeSpaces(p.ReadPI()));
	else
	if(p.IsDecl())
		xml.Add(parent, XmlImg::Decl(), NormalizeSpaces(p.ReadDecl()));
	else
	if(p.IsComment())
		xml.Add(parent, XmlImg::Comment(), NormalizeSpaces(p.ReadComment()));
	else
		NEVER();
}

void XmlView::Load(const char *filename)
{
	int64 l = GetFileLength(filename);
	if(l < 0 || l > 16000000)
		return;
	String txt = LoadFile(filename);
	XmlParser p(txt);
	xml.Clear();
	try {
		while(!p.IsEof())
			Load(0, p);
	}
	catch(XmlError e) {
		error = "XML parsing error: " + e;
		view.Show();
		view <<= txt;
		view.SetCursor(view.GetPos(p.GetLine() - 1, p.GetColumn() - 1));
		view.SetFocus();
		return;
	}
	xml.Show();
	xml.SetFocus();
}

void XmlView::LoadDir(const char *d)
{
	files.Clear();
	dir = d;
	Title(dir);
#ifdef PLATFORM_WIN32
	if(dir.GetLength())
#else
	if(dir.GetLength() > 1)
#endif
		files.Add("..", CtrlImg::DirUp(), StdFont(), SColorText(), true);
	::Load(files, dir, "*.*");
	SortByExt(files);
}

void XmlView::Enter()
{
	if(!files.IsCursor()) {
		Title(dir);
		return;
	}
	const FileList::File& f = files.Get(files.GetCursor());
	xml.Hide();
	view.Hide();
	if(f.name == "..") {
		Title(dir);
		return;
	}
	String p = AppendFileName(dir, f.name);
	Title(p);
	if(!f.isdir)
		Load(p);
}

void XmlView::DoDir()
{
	if(!files.IsCursor())
		return;
	const FileList::File& f = files.Get(files.GetCursor());
	if(!f.isdir)
		return;
	if(f.name == "..") {
		String n = DirectoryUp(dir);
		LoadDir(dir);
		files.FindSetCursor(n);
	}
	else
		LoadDir(AppendFileName(dir, f.name));
}

bool XmlView::Key(dword key, int)
{
	if(key == K_ENTER) {
		DoDir();
		return true;
	}
	return false;
}

void XmlView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	files.SerializeSettings(s);
	s % dir;
	s % splitter;
}

XmlView::XmlView()
{
	xml.NoRoot();

	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.AddFrame(errorbg);
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());

	xml.Hide();
	view.Hide();

	data.Add(xml.SizePos());
	data.Add(view.SizePos());

	splitter.Horz(files, data.SizePos());
	splitter.SetPos(2700);
	Add(splitter.SizePos());

	files.WhenEnterItem = THISBACK(Enter);
	files.WhenLeftDouble = THISBACK(DoDir);

	Sizeable().Zoomable();

	dir = GetCurrentDirectory();

	Icon(XmlImg::Icon());
}

GUI_APP_MAIN
{
	XmlView x;
	LoadFromFile(x);
	x.LoadDir();
	x.Run();
	StoreToFile(x);
}
