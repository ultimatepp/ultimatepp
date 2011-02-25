#ifndef _OfficeAutomation_h
#define _OfficeAutomation_h

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
