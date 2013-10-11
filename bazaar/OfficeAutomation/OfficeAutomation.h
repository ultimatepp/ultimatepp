#ifndef _OfficeAutomation_h
#define _OfficeAutomation_h

#include <Functions4U/Functions4U.h>

using namespace Upp;

#include "OfficeAutomationMethods.h"

struct Cell {
	int col;
	int row;
};


class SheetPlugin {
public:
	virtual bool IsAvailable();			
	
	Sheet_METHOD_LIST
};

class OfficeSheet : public SheetPlugin, public StaticPlugin {
public:
	OfficeSheet();
	~OfficeSheet();
	
	bool Init(const char *name);
	virtual bool IsAvailable(const char *_name);	
	String Function(String name, String arg1 = "", String arg2 = "", String arg3 = "", String arg4 = "", String arg5 = "", String arg6 = "");
	
	Sheet_METHOD_LIST

	static void CellToColRow(const char *cell, int &col, int &row);
	static void CellToColRow(const char *cell, Cell &cellPos);
	static String ColRowToCell(const int col, const int row);
	static String ColRowToCell(const Cell &cellPos);
};

class DocPlugin {
public:
	virtual bool IsAvailable();			
	
	Doc_METHOD_LIST
};

class OfficeDoc : public DocPlugin, public StaticPlugin {
public:
	OfficeDoc();
	~OfficeDoc();

	bool Init(const char *name);	
	virtual bool IsAvailable(const char *_name);
	
	Doc_METHOD_LIST	
};


#endif
