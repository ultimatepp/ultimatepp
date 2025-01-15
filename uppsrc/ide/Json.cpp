#include "ide.h"

class JsonView : public TopWindow {
	typedef JsonView CLASSNAME;

public:
	TreeCtrl              tree;
	LineEdit              view;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	
public:
	JsonView();
	
	virtual bool Key(dword key, int count);
	virtual void Close();

	void Load(const String& json);
	void CopyPath();

	void Serialize(Stream& s);

private:
	void   Reset();
	String Load0(const String& json);
};

JsonView::JsonView()
{
	Title("JSON view");
	Sizeable().Zoomable();
	Icon(IdeCommonImg::json());
	
	Add(tree.SizePos());
	Add(view.SizePos());
	
	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());
	tree.NoRoot();
	SetupJsonTree(tree);
}

bool JsonView::Key(dword key, int count)
{
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return false;
}

void JsonView::Load(const String& json)
{
	Reset();
	
	String parsingError = Load0(json);
	if(parsingError.GetCount() > 0) {
		parsingError.Set(0, ToLower(parsingError[0]));
		
		error = "Json parsing error: \"" + parsingError + "\".";
		AddFrame(errorbg);
		view.Show();
		view <<= json;
		
		return;
	}
	
	tree.Show();
	tree.SetFocus();
}

void JsonView::CopyPath()
{
	::CopyJsonPath(tree);
}

void JsonView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
}

void JsonView::Close()
{
	StoreToGlobal(*this, "JSONview");
	TopWindow::Close();
}

void JsonView::Reset()
{
	RemoveFrame(errorbg);
	view.Clear();
	view.Hide();
	tree.Clear();
	tree.Hide();
}

String JsonView::Load0(const String& json)
{
	return LoadJson(tree, json);
}

void Ide::Json()
{
	JsonView& dlg = CreateNewWindow<JsonView>();
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	LoadFromGlobal(dlg, "JSONview");
	dlg.OpenMain();
}
