#ifndef _WRichEdit_WRichEdit_h
#define _WRichEdit_WRichEdit_h

#include <RichEdit/RichEdit.h>

#define LAYOUTFILE <WRichEdit/WRichEdit.lay>
#include <CtrlCore/lay.h>

class WRichEdit : public TopWindow {
public:
	RichEditWithToolBar editor;

	typedef WRichEdit CLASSNAME;
	WRichEdit();
};

#endif
