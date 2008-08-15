#ifndef Setup_h
#define Setup_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CtrlCore.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

#define LAYOUTFILE <E011Setup/setup.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS AppImg
#define IMAGEFILE <E011Setup/setup.iml>
#include <Draw/iml_header.h>

#include <MySupport/support.h>
//#include <MyDbase/dbase.h>
#include <E011Setup/version.h>
#include <E011Setup/WinUtils.h>

String installPath;
int version, revision;

struct DisplayProgramFolderIcon : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword) const {
		w.DrawRect(r, paper);
		Image m = AppImg::PrgFolder_1_16();
		w.DrawImage(r.CenterRect(m.GetSize()), m);
	}
};

class SelectFolder : public WithMySelectFolderLay<TopWindow> {
public:
	bool Perform();
	void BrowsePath();
	
	typedef SelectFolder CLASSNAME;
	
public:
  SelectFolder();	
};

class SelectFolderGroup : public WithMyInfoOptionLay<TopWindow> {
public:
	void Perform();
	void SelectFolder();
	
	typedef SelectFolderGroup CLASSNAME;
	
public:
  SelectFolderGroup();	
};

#endif
