#ifndef _PluginDemo_Spreadsheet_h_
#define _PluginDemo_Spreadsheet_h_

#include <Functions4U/Functions4U.h>

#define Spreadsheet_API							\
	virtual bool Open(const char *filename);	\
	virtual void SetData(int row, int col, Value val);	
	
	
class SpreadsheetAPI {
public:
	Spreadsheet_API
};


class Spreadsheet : public SpreadsheetAPI, public StaticPlugin {
public:
	Spreadsheet_API
};


#endif
