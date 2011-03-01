#include <Core/Core.h>

using namespace Upp;

#include "Spreadsheet.h"

class OpenSpreadsheet : public SpreadsheetAPI {
private:
	String filename;
	
public:
	Spreadsheet_API
	OpenSpreadsheet() {
		puts("OpenSpreadsheet new");
	}
	~OpenSpreadsheet() {
		puts("OpenSpreadsheet closing " + filename);
		puts("OpenSpreadsheet delete");
	}
};


INITBLOCK {
	PluginRegister(Spreadsheet, OpenSpreadsheet, "Open");
}

bool OpenSpreadsheet::Open(const char *_filename) {
	filename = _filename;
	puts(Format("OpenSpreadsheet::Open(%s)", filename));
	return false;
}

void OpenSpreadsheet::SetData(int row, int col, Value val) {
	puts(Format("OpenSpreadsheet::SetData(%d, %d, %s)", row, col, val.ToString()));
}
