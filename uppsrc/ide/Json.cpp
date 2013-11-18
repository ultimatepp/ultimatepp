#include "ide.h"

struct JsonView : TopWindow {
	TreeCtrl  tree;
	
	virtual bool Key(dword key, int count);

	int  AddNode(int parent_id, const Value& id, const String& name, const Value& v);
	void Load(const char *json);
	void CopyPath();

	void Serialize(Stream& s);

	typedef JsonView CLASSNAME;
	
	JsonView();
};

bool JsonView::Key(dword key, int count)
{
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return false;
}

int JsonView::AddNode(int parent_id, const Value& id, const String& name, const Value& v)
{
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

void JsonView::Load(const char *json)
{
	tree.Open(AddNode(0, Null, "JSON", ParseJSON(json)));
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

JsonView::JsonView()
{
	Title("JSON view");
	Add(tree.SizePos()); tree.SetDisplay(QTFDisplay()); tree.NoRoot();	Sizeable().Zoomable();
	Icon(IdeCommonImg::xml());
	tree.WhenLeftDouble = THISBACK(CopyPath);
}

void Ide::Json()
{
	JsonView dlg;
	LoadFromGlobal(dlg, "JSONview");
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	dlg.Execute();
	StoreToGlobal(dlg, "JSONview");
}
