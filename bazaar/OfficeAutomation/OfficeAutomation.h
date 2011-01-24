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
	bool SetItalic(int col, int row, bool italic);
	bool SetItalic(String cell, bool italic);
	bool SetItalic(bool italic);
	bool SetUnderline(bool underline);
	bool SetUnderline(String cell, bool underline);
	bool SetUnderline(int col, int row, bool underline);	
	bool SetFont(int col, int row, String name, int size);
	bool SetFont(String cell, String name, int size);
	bool SetFont(String name, int size);
	bool SetColor(int col, int row, Color color);
	bool SetColor(String cell, Color color);
	bool SetColor(Color color);
	bool SetBackColor(int col, int row, Color color);
	bool SetBackColor(String cell, Color color);
	bool SetBackColor(Color color);
	
	bool SetRowHeight(int row, double height);
	bool SetColWidth(int col, double width);
	
	enum {LEFT = 0, CENTER, RIGHT, JUSTIFY, TOP, BOTTOM, MAX_JUSTIFY};
	
	bool SetHorizAlignment(String cell, int alignment);
	bool SetHorizAlignment(int col, int row, int alignment);
	bool SetVertAlignment(String cell, int alignment);
	bool SetVertAlignment(int col, int row, int alignment); 
	
	enum {BORDER_DIAG_DOWN = 0, BORDER_DIAG_UP, BORDER_LEFT, BORDER_TOP, BORDER_BOTTOM, BORDER_RIGHT};
	enum {HAIRLINE = 0, MEDIUM, THIN, THICK};
	enum {NONE = 0, CONTINUOUS, DASH, DASHDOT, DOT};
	
	bool SetBorder(int borderIndx, int lineStyle, int weight, Color color);
	bool SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color);
	
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
