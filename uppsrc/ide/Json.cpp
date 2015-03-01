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
	
	int AddNode(int parent_id, const Value& id, const String& name, const Value& v);
};

JsonView::JsonView()
{
	Title("JSON view");
	Sizeable().Zoomable();
	Icon(IdeCommonImg::xml());
	
	Add(tree.SizePos()); 	
	Add(view.SizePos());
	
	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());
	tree.SetDisplay(QTFDisplay());
	tree.NoRoot();
	tree.WhenLeftDouble = THISBACK(CopyPath);
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
	int id = tree.GetCursor();
	String path;
	while(id) {
		Value k = tree.Get(id);
		if(!IsNull(k)) {
			if(IsNumber(k))
				path = "[" + AsString(k) + "]" + path;
			if(IsString(k))
				path = "[" + AsCString(String(k)) + "]" + path;
		}
		id = tree.GetParent(id);
	}
	WriteClipboardText(path);
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
	String parsingError;
	
	try {
		tree.Open(AddNode(0, Null, "JSON", ParseJSON(json)));
	}
	catch(const Exc& e) {
		parsingError = e;
	}
	
	return parsingError;
}

int JsonView::AddNode(int parent_id, const Value& id, const String& name, const Value& v)
{
	if(IsError(v)) {
		// TODO: Replace with JsonExc or something that is more accurate in this situation.
		String errorText = GetErrorText(v);
		errorText.Remove(0, errorText.Find(" ") + 1);
		throw Exc(errorText);
	}
	else
	if(v.Is<ValueMap>()) {
		ValueMap m = v;
		parent_id = tree.Add(parent_id, IdeImg::JsonStruct(), id, "[G1 [* " + name);
		for(int i = 0; i < m.GetCount(); i++)
			AddNode(parent_id, m.GetKey(i), "[@B \1" + String(m.GetKey(i)) + "\1:]", m.GetValue(i));
	}
	else
	if(v.Is<ValueArray>()) {
		parent_id = tree.Add(parent_id, IdeImg::JsonArray(), id, "[G1 [* " + name);
		for(int i = 0; i < v.GetCount(); i++)
			AddNode(parent_id, i, "[@c " + AsString(i) + ":]", v[i]);
	}
	else {
		String qtf = "[G1 [* " + name + "]";
		Image img = IdeImg::JsonNumber();
		if(IsString(v)) {
			img = IdeImg::JsonString();
			if(IsNull(v))
				qtf << "[*@g  Null";
			else
				qtf << "[@r \1 " + AsCString(String(v));
		}
		else {
			if(v.Is<bool>())
				img = IdeImg::JsonBool();
			if(IsNull(v))
				qtf << "[*@g  Null";
			else
				qtf << "\1 " + AsString(v);
		}
		parent_id = tree.Add(parent_id, img, id, qtf);
	}
	return parent_id;
}

void Ide::Json()
{
	static JsonView dlg;
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	if(!dlg.IsOpen()) {
		LoadFromGlobal(dlg, "JSONview");
		dlg.OpenMain();
	}
	else
		dlg.SetForeground();
}
