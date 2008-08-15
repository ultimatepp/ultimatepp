#ifndef _EditField_EditField_h
#define _EditField_EditField_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <EditField/EditField.lay>
#include <CtrlCore/lay.h>



class EditFieldApp : public WithEditFieldLayout<TopWindow> {
public:
	void Set();

	typedef EditFieldApp CLASSNAME;
	EditFieldApp();
};

#endif
