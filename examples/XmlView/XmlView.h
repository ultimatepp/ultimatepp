#ifndef _XmlView_XmlView_h
#define _XmlView_XmlView_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGEFILE  <XmlView/XmlView.iml>
#define IMAGECLASS XmlImg
#include <Draw/iml_header.h>

class XmlView : public TopWindow {
public:
	virtual bool Key(dword key, int);
private:
	TreeCtrl              xml;
	LineEdit              view;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	FileList              files;
	ParentCtrl            data;
	Splitter              splitter;
	String                dir;

	void Load(int parent, XmlParser& p);
	void Load(const char *filename);
	void Enter();
	void DoDir();

public:
	typedef XmlView CLASSNAME;

	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	XmlView();
};

#endif
