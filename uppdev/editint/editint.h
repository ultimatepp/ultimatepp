#ifndef _editint_editint_h
#define _editint_editint_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <editint/editint.lay>
#include <CtrlCore/lay.h>



class editint : public WitheditintLayout<TopWindow> {
public:
	typedef editint CLASSNAME;
	editint();
};

#endif
