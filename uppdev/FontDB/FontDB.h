#ifndef _FontDB_FontDB_h
#define _FontDB_FontDB_h

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define LAYOUTFILE <FontDB/FontDB.lay>
#include <CtrlCore/lay.h>



class FontDB : public TopWindow {
	ArrayCtrl list;

public:
	typedef FontDB CLASSNAME;
	
	void Perform();
	
	FontDB();
};

#endif

