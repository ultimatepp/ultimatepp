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
	
	SetupXmlTree(xml);
}

void XmlView::Load(const String& txt)
{
	Reset();

	XmlParser p(txt);
	String parsingError = XmlLoadTree(xml, txt);
	if(parsingError.GetCount() > 0) {
		parsingError.Set(0, ToLower(parsingError[0]));
		
		error = "XML parsing error: " + parsingError + ".";
		AddFrame(errorbg);
		view.Show();
		view <<= txt;
		view.SetCursor(view.GetPos64(p.GetLine() - 1, p.GetColumn() - 1));
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
	CopyXmlPath(xml);
}

void XmlView::Close()
{
	StoreToGlobal(*this, "XML view");
	TopWindow::Close();
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
	XmlView& dlg = CreateNewWindow<XmlView>();
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	LoadFromGlobal(dlg, "XMLview");
	dlg.OpenMain();
}
