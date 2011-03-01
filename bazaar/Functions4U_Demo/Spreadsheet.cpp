#include <Core/Core.h>

using namespace Upp;

#include <Functions4U/Functions4U.h>
#include "Spreadsheet.h"


bool SpreadsheetAPI::Open(const char *filename) 				{return false;};
void SpreadsheetAPI::SetData(int row, int col, Value val) 	{};


bool Spreadsheet::Open(const char *filename) 			{return (static_cast<SpreadsheetAPI *>(GetData()))->Open(filename);}
void Spreadsheet::SetData(int row, int col, Value val)	{return (static_cast<SpreadsheetAPI *>(GetData()))->SetData(row, col, val);}


INITBLOCK {
	PluginRegister(Spreadsheet, SpreadsheetAPI, "");
}