#include <Core/Core.h>

using namespace Upp;

#include <Functions4U/Functions4U.h>
#include "Spreadsheet.h"


bool SpreadsheetPlugin::Open(const char *filename) 				{return false;};
void SpreadsheetPlugin::SetData(int row, int col, Value val) 	{};


bool Spreadsheet::Open(const char *filename) 			{return (static_cast<SpreadsheetPlugin *>(GetData()))->Open(filename);}
void Spreadsheet::SetData(int row, int col, Value val)	{return (static_cast<SpreadsheetPlugin *>(GetData()))->SetData(row, col, val);}


INITBLOCK {
	PluginRegister(Spreadsheet, SpreadsheetPlugin, "");
}