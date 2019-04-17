#include "Designers.h"

void XmlViewDes::CopyPath()
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

String XmlViewDes::Load0(const String& data) {
	XmlParser p(data);

	String parsingError;
	
	try {
		while(!p.IsEof())
			Load0(0, p);
	}
	catch(const XmlError& e) {
		parsingError = e;
	}
	
	if(!parsingError.GetCount() && !tree.GetChildCount(0))
		parsingError = "Not found any XML tags";
	
	return parsingError;
}

void XmlViewDes::Load0(int parent, XmlParser& p)
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
			Load0(parent, p);
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
