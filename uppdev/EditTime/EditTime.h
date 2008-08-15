#ifndef _EditTime_EditTime_h
#define _EditTime_EditTime_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <EditTime/EditTime.lay>
#include <CtrlCore/lay.h>

class EditTimeDlg : public WithEditTimeLayout<TopWindow> {
public:
	typedef EditTimeDlg CLASSNAME;
	EditTimeDlg();
};

#endif
