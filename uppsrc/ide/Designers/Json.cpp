#include "Designers.h"
#include "JsonValidator.h"

Vector<Value> GetJsonPath(TreeCtrl& tree)
{
	int id = tree.GetCursor();
	Vector<Value> path;
	while(id) {
		Value k = tree.Get(id);
		if(!IsNull(k)) {
			if(IsNumber(k) && tree.GetChildCount(id) || IsString(k))
				path << k;
		}
		id = tree.GetParent(id);
	}
	Reverse(path);
	return path;
}

void CopyJsonPath(TreeCtrl& tree)
{
	String path;
	for(Value v : GetJsonPath(tree)) {
		path << '[';
		if(IsNumber(v))
			path << v;
		else
			path << AsCString(~v);
		path << ']';
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

bool JsonViewDes::Validate(const String& path)
{
	String schema = LoadFile(path);
	if(schema.GetCount() == 0)
		return true;
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
	return errors.GetCount() == 0;
}

void JsonViewDes::EditMenu(Bar& bar)
{
	JsonTreeMenu(bar, tree);
	bar.Separator();
	bar.Add("Validate with schema..", [=] {
		String p = SelectFileOpen("*.json\t*.schema.json\t*.*");
		if(IsNull(p))
			return;
		schema_path.GetAdd(filename) = p;
		if(Validate(p))
			PromptOK("No errors found");
		else
			errors_frame.Show();
	});
}

void JsonViewDes::GoTo(const Vector<Value>& path)
{
	if(!tree.GetChildCount(0))
		return;
	int id = tree.GetChild(0, 0);
	for(Value m : path) {
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

ArrayMap<String, String> JsonViewDes::schema_path;

JsonViewDes::JsonViewDes()
{
	SetupJsonTree(tree);
	AddFrame(errors_frame);
	errors_frame.Bottom(errors, GetStdFontCy() * 16);
	LoadFromGlobal(errors_frame, "JsonViewDes-frame");
	errors.AddColumn("Path");
	errors.AddColumn("Error", 2);
	errors_frame.Hide();
	errors.WhenSel = [=] {
		if(errors.IsCursor()) {
			ValueArray va = errors.Get(2);
			GoTo(va.Get());
		}
	};
	close_errors.SetImage(IdeImg::SmallClose());
	errors.HeaderObject() << close_errors.VSizePos(DPI(1), DPI(1)).RightPos(DPI(1), DPI(16));
	close_errors << [=] {
		errors_frame.Hide();
		schema_path.GetAdd(filename) = Null;
	};
}

JsonViewDes::~JsonViewDes()
{
	StoreToGlobal(errors_frame, "JsonViewDes-frame");
}

INITBLOCK {
	RegisterGlobalConfig("JsonViewDes-frame");
}

void JsonViewDes::CopyPath()
{
	CopyJsonPath(tree);
}

String JsonViewDes::Load0(const String& json_)
{
	json = json_;
	String s = LoadJson(tree, json);
	if(s.GetCount())
		return s;
	String p = schema_path.Get(filename, Null);
	if(p.GetCount()) {
		if(!Validate(p))
			errors_frame.Show();
	}
	return Null;
}

struct JsonDesModule : public IdeModule {
	String       GetID() override { return "JsonDesModule"; }
	
	bool         AcceptsFile(const char *path) override {
		return ToLower(GetFileExt(path)) == ".json";
	}

	IdeDesigner *CreateDesigner(const char *path, byte) override {
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
