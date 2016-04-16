#include "ide.h"

class XmlView : public TopWindow {
	typedef XmlView CLASSNAME;

public:
	TreeCtrl              xml;
	LineEdit              view;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	ParentCtrl            data;
	
	XmlView();
	
	virtual bool Key(dword key, int count);
	virtual void Close();

	void Load(const String& txt);
	void CopyPath();

	void Serialize(Stream& s);

private:
	String Load0(XmlParser& p);
	void   Load0(int parent, XmlParser& p);
	
	void   Reset();
};

bool XmlView::Key(dword key, int count)
{
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return false;
}

XmlView::XmlView()
{
	Title("XML view");
	Sizeable().Zoomable();
	Icon(IdeCommonImg::xml());

	xml.NoRoot();

	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());

	Add(xml.SizePos());
	Add(view.SizePos());

	xml.WhenLeftDouble = THISBACK(CopyPath);
}

void XmlView::Load(const String& txt)
{
	Reset();
	
	XmlParser p(txt);
	String parsingError = Load0(p);
	if(parsingError.GetCount() > 0) {
		parsingError.Set(0, ToLower(parsingError[0]));
		
		error = "XML parsing error: " + parsingError + ".";
		AddFrame(errorbg);
		view.Show();
		view <<= txt;
		view.SetCursor(view.GetPos(p.GetLine() - 1, p.GetColumn() - 1));
		view.SetFocus();
		return;
	}
	
	xml.Show();
	xml.SetFocus();
}

void XmlView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
}

void XmlView::CopyPath()
{
	String path;
	int id = xml.GetCursor();
	while(id >= 0) {
		String tag = xml.Get(id);
		if(tag.GetCount())
			path = "[" + AsCString(tag) + "]" + path;
		id = xml.GetParent(id);
	}
	WriteClipboardText(path);
}

void XmlView::Close()
{
	StoreToGlobal(*this, "XML view");
	TopWindow::Close();
}

String XmlView::Load0(XmlParser& p) {
	String parsingError;
	
	try {
		while(!p.IsEof())
			Load0(0, p);
	}
	catch(const XmlError& e) {
		parsingError = e;
	}
	
	if(!parsingError.GetCount() && !xml.GetChildCount(0))
		parsingError = "Not found any XML tags";
	
	return parsingError;
}

void XmlView::Load0(int parent, XmlParser& p)
{
	if(p.IsTag()) {
		String tag = p.ReadTag();
		String txt = tag;
		for(int i = 0; i < p.GetAttrCount(); i++)
			txt << ' ' << p.GetAttr(i) << "=\"" << p[i] << "\"";
		parent = xml.Add(parent, IdeImg::XmlTag(), tag, txt);
		while(!p.End()) {
			if(p.IsEof())
				throw XmlError("Unexpected end of text");
			Load0(parent, p);
		}
	}
	else
	if(p.IsText())
		xml.Add(parent, IdeImg::XmlText(), Null, NormalizeSpaces(p.ReadText()));
	else
	if(p.IsPI())
		xml.Add(parent, IdeImg::XmlPI(), Null, NormalizeSpaces(p.ReadPI()));
	else
	if(p.IsDecl())
		xml.Add(parent, IdeImg::XmlDecl(), Null, NormalizeSpaces(p.ReadDecl()));
	else
	if(p.IsComment())
		xml.Add(parent, IdeImg::XmlComment(), Null, NormalizeSpaces(p.ReadComment()));
	else
		throw XmlError("Unexpected input");
}

void XmlView::Reset()
{
	RemoveFrame(errorbg);
	
	xml.Clear();
	xml.Hide();
	view.Clear();
	view.Hide();
}

void Ide::Xml()
{
	static XmlView dlg;
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	if(!dlg.IsOpen()) {
		LoadFromGlobal(dlg, "XMLview");
		dlg.OpenMain();
	}
	else
		dlg.SetForeground();
}
