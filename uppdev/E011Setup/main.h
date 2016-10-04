#ifndef _Setup_Setup_h
#define _Setup_Setup_h

#include <src/support/support.h>
//#include <E011/release.h>
#include <src/Unzip/Unzip.h>

#define IMAGEFILE <E011Setup/setup.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <E011Setup/setup.lay>
#include <CtrlCore/lay.h>

#include "utils.h"

String installPath;
int version, revision;

class SelectFolder : public WithSelectFolderLay<TopWindow> {
public:
	bool Perform();
	void BrowsePath();

	typedef SelectFolder CLASSNAME;

public:
  SelectFolder();
};

#define IMAGEFILE <E011Setup/setup.iml>
#include <Draw/iml_source.h>

#endif
