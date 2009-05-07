#ifndef _DateFmtCtrl_DateFmtCtrl_h
#define _DateFmtCtrl_DateFmtCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DateFmtCtrl/DateFmtCtrl.lay>
#include <CtrlCore/lay.h>

class DateFmtCtrl : public WithDateFmtCtrlLayout<TopWindow> {
public:
	void Print();
	void Language();

	typedef DateFmtCtrl CLASSNAME;
	DateFmtCtrl();
};

#endif
