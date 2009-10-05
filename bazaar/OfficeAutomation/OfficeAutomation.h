#ifndef _OfficeAutomation_h
#define _OfficeAutomation_h

using namespace Upp;

struct Cell {
	int col;
	int row;
};

class OfficeSheet
{
public:
	OfficeSheet();
	~OfficeSheet();
	
	static bool IsAvailable(String type);
	
	bool Init(String type);
	
	bool AddSheet(bool visible);
	bool OpenSheet(String fileName, bool visible);
	
	bool SetValue(int col, int row, Value value);
	bool SetValue(String cell, Value value);
	bool SetValue(Value value);
	Value GetValue(int col, int row);
	Value GetValue(String cell);
	bool Replace(Value search, Value replace);
		
	bool SetBold(int col, int row, bool bold);
	bool SetBold(String cell, bool bold);
	bool SetBold(bool bold);
	bool SetFont(int col, int row, String name, int size);
	bool SetFont(String cell, String name, int size);
	bool SetFont(String name, int size);
	
	bool Select(String range);
	bool Select(int fromX, int fromY, int toX, int toY);
	bool Select();
	/*// New functs for next versions
	void DefMatrix(int width, int height);
	bool FillSelectionMatrix();
	void SetMatrixValue(int i, int j, ::Value value);
	*/	
	
	bool Print();
	
	bool SaveAs(String fileName, String type = "xls");
	bool Quit();
	
	bool InsertTab(String name);
	bool ChooseTab(String name);
	bool ChooseTab(int index);
	bool RemoveTab(String name);
	bool RemoveTab(int index);
	int GetNumTabs();
	
	String GetType() 
	{
		if (type == SheetOPEN)
			return "Open";
		else
			return "Microsoft"; 
	}
	static void CellToColRow(const char *cell, int &col, int &row);
	static void CellToColRow(const char *cell, Cell &cellPos);
	static String ColRowToCell(const int col, const int row);
	static String ColRowToCell(const Cell &cellPos);
	
private:
	void *data;
	enum {SheetOPEN = 1, SheetMS}; 
	int type;
};

class OfficeDoc
{
public:
	OfficeDoc();
	~OfficeDoc();
	
	static bool IsAvailable(String type);
	
	bool Init(String type);
	
	bool AddDoc(bool visible);
	bool OpenDoc(String fileName, bool visible);

	bool SetFont(String font, int size);
	bool SetBold(bool bold);
	bool SetItalic(bool italic);
	bool WriteText(String value);
	
	bool Select();
	
	bool Replace(String search, String replace);
	
	bool Print();
	
	bool SetSaved(bool saved);
	bool SaveAs(String fileName, String type = "doc");
	bool Quit();
	
	String GetType() 
	{
		if (type == DocOPEN)
			return "Open";
		else
			return "Microsoft"; 
	}
	
private:
	void *data;
	enum {DocOPEN = 1, DocMS}; 
	int type;
};


#endif
