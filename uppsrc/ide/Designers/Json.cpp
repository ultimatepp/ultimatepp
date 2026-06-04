#include "Designers.h"
#include "JsonValidator.h"

void CopyJsonPath(TreeCtrl& tree)
{
	int id = tree.GetCursor();
	String path;
	while(id) {
		Value k = tree.Get(id);
		if(!IsNull(k)) {
			if(IsNumber(k) && tree.GetChildCount(id))
				path = "[" + AsString(k) + "]" + path;
			if(IsString(k))
				path = "[" + AsCString(String(k)) + "]" + path;
		}
		id = tree.GetParent(id);
	}
	WriteClipboardText(path);
}

void CopyJsonNode(TreeCtrl& tree)
{
	int id = tree.GetCursor();
	if(id >= 0)
		WriteClipboardText(ParseQTF(~tree.GetValue(id)).GetPlainText().ToString());
}

static
int sAddNode(TreeCtrl& tree, int parent_id, const Value& id, const String& name, const Value& v)
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
		parent_id = tree.Add(parent_id, IdeCommonImg::JsonStruct(), id, "[G1 [* " + name + ':');
		for(int i = 0; i < m.GetCount(); i++)
			sAddNode(tree, parent_id, m.GetKey(i), "[@B \1" + String(m.GetKey(i)) + "\1]", m.GetValue(i));
	}
	else
	if(v.Is<ValueArray>()) {
		parent_id = tree.Add(parent_id, IdeCommonImg::JsonArray(), id,
		                     "[G1 [* " + name + "]\1[" + AsString(v.GetCount()) + "]:\1");
		for(int i = 0; i < v.GetCount(); i++)
			sAddNode(tree, parent_id, i, "[@c " + AsString(i) + "]", v[i]);
	}
	else {
		String qtf = "[G1 [* " + name + ":]";
		Image img = IdeCommonImg::JsonNumber();
		if(IsString(v)) {
			img = IdeCommonImg::JsonString();
			if(IsNull(v))
				qtf << "[*@g  Null";
			else
				qtf << "[@r \1 " + AsCString(String(v));
		}
		else {
			if(v.Is<bool>())
				img = IdeCommonImg::JsonBool();
			if(IsNull(v))
				qtf << "[*@g  Null";
			else
				qtf << "\1 " + AsString(v);
		}
		parent_id = tree.Add(parent_id, img, id, qtf);
	}
	return parent_id;
}

String LoadJson(TreeCtrl& tree, const String& json)
{
	String parsingError;
	
	CParser p(json);
	try {
		tree.Open(sAddNode(tree, 0, Null, "JSON", ParseJSON(p)));
	}
	catch(const CParser::Error& e) {
		parsingError << e;
	}
	
	return parsingError;
}

void JsonTreeMenu(Bar& bar, TreeCtrl& tree)
{
	bar.Add(CtrlImg::copy(), "Copy", [=, &tree] { CopyJsonNode(tree); }).Key(K_CTRL_C);
	bar.Add("Copy path\t[double-click]", [=, &tree] { CopyJsonPath(tree); });
}

void SetupJsonTree(TreeCtrl& tree)
{
	tree.SetDisplay(QTFDisplay());
	tree.WhenLeftDouble = [=, &tree] { CopyJsonPath(tree); };
	tree.WhenBar = [=, &tree](Bar& bar) { JsonTreeMenu(bar, tree); };
}

void JsonViewDes::EditMenu(Bar& bar)
{
	JsonTreeMenu(bar, tree);
	bar.Add("Validate with schema..", [=] {
		String schema = SelectLoadFile("*.json\t*.*");
		if(schema.GetCount() == 0)
			return;
		errors.Clear();
		JsonSchemaChecker chk;
		chk.WhenError = [&](const String& error) {
			errors_frame.Show();
			String path;
			ValueArray va;
			for(Value v : chk.data_path) {
				MergeWith(path, "/", ~v);
				va << v;
			}
			errors.Add(path, error, va);
		};
		chk.Validate(ParseJSON(schema), ParseJSON(json));
		if(errors.GetCount() == 0)
			PromptOK("No errors found");
	});
}

JsonViewDes::JsonViewDes()
{
	SetupJsonTree(tree);
	AddFrame(errors_frame);
	errors_frame.Bottom(errors, GetStdFontCy() * 16);
	errors.AddColumn("Path");
	errors.AddColumn("Error", 2);
	errors_frame.Hide();
	errors.WhenSel = [=] {
		if(errors.IsCursor()) {
			ValueArray va = errors.Get(2);
			if(!tree.GetChildCount(0))
				return;
			int id = tree.GetChild(0, 0);
			for(Value m : va) {
				int n = tree.GetChildCount(id);
				bool found = false;
				for(int i = 0; i < n; i++) {
					int cid = tree.GetChild(id, i);
					if(cid > 0 && tree.Get(cid) == m) {
						found = true;
						id = cid;
						break;
					}
				}
				if(!found)
					break;
			}
			if(id > 0)
				tree.SetCursor(id);
		}
	};
}

void JsonViewDes::CopyPath()
{
	CopyJsonPath(tree);
}

String JsonViewDes::Load0(const String& json_)
{
	json = json_;
	return LoadJson(tree, json);
}

struct JsonDesModule : public IdeModule {
	virtual String       GetID() { return "JsonDesModule"; }

	virtual bool         AcceptsFile(const char *path) {
		return ToLower(GetFileExt(path)) == ".json";
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(!AcceptsFile(path))
			return NULL;
		JsonViewDes *d = new JsonViewDes;
		if(d->Load(path))
			return d;
		delete d;
		return NULL;
	}
};

INITIALIZER(JsonViewDes)
{
	RegisterIdeModule(Single<JsonDesModule>());
}
