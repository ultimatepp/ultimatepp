#ifndef _LabelBox_LabelBox_h
#define _LabelBox_LabelBox_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <LabelBox/LabelBox.lay>
#include <CtrlCore/lay.h>



class LabelBoxDlg : public WithLabelBoxLayout<TopWindow> {
public:
	typedef LabelBoxDlg CLASSNAME;
	LabelBoxDlg();
};

#endif
