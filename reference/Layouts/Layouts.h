#ifndef _Layouts_Layouts_h_
#define _Layouts_Layouts_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Layouts/layoutfile1.lay> // Layouts will be visible in all files that #include Layouts.h
#include <CtrlCore/lay.h>

// FirstTabDlg will be visible in all files that #include Layouts.h
struct FirstTabDlg : WithFirstTabLayout<ParentCtrl> { // ParentCtrl is a good base class for tabs
	typedef FirstTabDlg CLASSNAME;
	FirstTabDlg();
};

#endif
