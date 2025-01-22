#include "Designers.h"

void CopyXmlPath(TreeCtrl& tree)
{
	String path;
	int id = tree.GetCursor();
	while(id >= 0) {
		String tag = tree.Get(id);
		if(tag.GetCount())
			path = "[" + AsCString(tag) + "]" + path;
		id = tree.GetParent(id);
	}
	WriteClipboardText(path);
}

void CopyXmlNode(TreeCtrl& tree)
{
	int id = tree.GetCursor();
	if(id >= 0)
		WriteClipboardText(~tree.GetValue(id));
}

void XmlViewDes::CopyPath()
{
	CopyXmlPath(tree);
}

void XmlLoadTree(TreeCtrl& tree, int parent, XmlParser& p)
{
	if(p.IsTag()) {
		String tag = p.ReadTag();
		String txt = tag;
		for(int i = 0; i < p.GetAttrCount(); i++)
			txt << ' ' << p.GetAttr(i) << "=\"" << p[i] << "\"";
		parent = tree.Add(parent, IdeCommonImg::XmlTag(), tag, txt);
		while(!p.End()) {
			if(p.IsEof())
				throw XmlError("Unexpected end of text");
			XmlLoadTree(tree, parent, p);
		}
	}
	else
	if(p.IsText())
		tree.Add(parent, IdeCommonImg::XmlText(), Null, NormalizeSpaces(p.ReadText()));
	else
	if(p.IsPI())
		tree.Add(parent, IdeCommonImg::XmlPI(), Null, NormalizeSpaces(p.ReadPI()));
	else
	if(p.IsDecl())
		tree.Add(parent, IdeCommonImg::XmlDecl(), Null, NormalizeSpaces(p.ReadDecl()));
	else
	if(p.IsComment())
		tree.Add(parent, IdeCommonImg::XmlComment(), Null, NormalizeSpaces(p.ReadComment()));
	else
		throw XmlError("Unexpected input");
}

String XmlLoadTree(TreeCtrl& tree, XmlParser& p) {
	String parsingError;
	
	try {
		while(!p.IsEof())
			XmlLoadTree(tree, 0, p);
	}
	catch(const XmlError& e) {
		parsingError = e;
	}
	
	if(!parsingError.GetCount() && !tree.GetChildCount(0))
		parsingError = "Not found any XML tags";
	
	return parsingError;
}

String XmlLoadTree(TreeCtrl& tree, const String& data) {
	XmlParser p(data);
	return XmlLoadTree(tree, p);
}

String XmlViewDes::Load0(const String& data) {
	return XmlLoadTree(tree, data);
}

void XmlViewDes::Load0(int parent, XmlParser& p)
{
	XmlLoadTree(tree, parent, p);
}

void XmlTreeMenu(Bar& bar, TreeCtrl& tree)
{
	bar.Add(CtrlImg::copy(), "Copy", [=, &tree] { CopyXmlNode(tree); }).Key(K_CTRL_C);
	bar.Add("Copy path\t[double-click]", [=, &tree] { CopyXmlPath(tree); });
}

void SetupXmlTree(TreeCtrl& tree)
{
	tree.NoRoot();
	tree.WhenLeftDouble = [=, &tree] { CopyJsonPath(tree); };
	tree.WhenBar = [=, &tree](Bar& bar) { XmlTreeMenu(bar, tree); };
}

void XmlViewDes::EditMenu(Bar& menu)
{
	XmlTreeMenu(menu, tree);
}

XmlViewDes::XmlViewDes()
{
	SetupXmlTree(tree);
}

struct XmlDesModule : public IdeModule {
	virtual String       GetID() { return "XmlDesModule"; }

	virtual bool         AcceptsFile(const char *path) {
		return ToLower(GetFileExt(path)) == ".xml";
	}

	virtual IdeDesigner *CreateDesigner(const char *path, byte) {
		if(!AcceptsFile(path))
			return NULL;
		XmlViewDes *d = new XmlViewDes;
		if(d->Load(path))
			return d;
		delete d;
		return NULL;
	}
};

INITIALIZER(XmlViewDes)
{
	RegisterIdeModule(Single<XmlDesModule>());
}
