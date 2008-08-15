#ifndef _AtlantisStat_AtlantisStat_h
#define _AtlantisStat_AtlantisStat_h
//#include <iostream>
//#include "SQLiteWrapper.h"
#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <AtlantisStat/AtlantisStat.lay>
#include <CtrlCore/lay.h>



class AtlantisStat : public WithAtlantisStatLayout<TopWindow> {
public:
	FileSelector fs;
	String  filename;
	void Quit();
	void Open();
	void FileMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void DoQuery();
	typedef AtlantisStat CLASSNAME;
	AtlantisStat();
};

#endif
