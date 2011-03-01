#include <Core/Core.h>

using namespace Upp;

#include "Spreadsheet.h"

class ExcelSpreadsheet : public SpreadsheetAPI {
private:
	String filename;
	
public:
	Spreadsheet_API
	ExcelSpreadsheet() {
		puts("ExcelSpreadsheet new");
	}
	~ExcelSpreadsheet() {
		puts("ExcelSpreadsheet closing " + filename);		
		puts("ExcelSpreadsheet delete");
	}
};


INITBLOCK {
	PluginRegister(Spreadsheet, ExcelSpreadsheet, "Excel");
}

bool ExcelSpreadsheet::Open(const char *_filename) {
	filename = _filename;	
	puts(Format("ExcelSpreadsheet::Open(%s)", filename));
	return false; 
}

void ExcelSpreadsheet::SetData(int row, int col, Value val) {
	puts(Format("ExcelSpreadsheet::SetData(%d, %d, %s)", row, col, val.ToString()));
}