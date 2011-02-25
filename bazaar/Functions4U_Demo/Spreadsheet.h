#ifndef _PluginDemo_Spreadsheet_h_
#define _PluginDemo_Spreadsheet_h_

#include <Functions4U/Functions4U.h>

#define Spreadsheet_METHOD_LIST					\
	virtual bool Open(const char *filename);	\
	virtual void SetData(int row, int col, Value val);	
	
	
class SpreadsheetPlugin {
public:
	Spreadsheet_METHOD_LIST
};


class Spreadsheet : public SpreadsheetPlugin, public StaticPlugin {
public:
	Spreadsheet_METHOD_LIST
};


#endif
