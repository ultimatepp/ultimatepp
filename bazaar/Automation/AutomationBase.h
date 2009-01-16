#ifndef _AutomationBase_h
#define _AutomationBase_h

using namespace Upp;

typedef IDispatch* ObjectOle;

class VariantOle
{
public:
	VariantOle();
    ~VariantOle();
    void Bool(bool val);
    void Int(int val);
    void Int4(long val);
    void Real4(float val);
    void Real8(double val);
    void BString(String str);
    void ObjectOle(::ObjectOle var);
    void Time(Upp::Time t);
    void ArrayDim(int sizeX);
    void ArrayDim(int sizeX, int sizeY);
    void ArraySetValue(int x, ::Value value);
    void ArraySetVariant(int x, VariantOle &value);
    void ArraySetValue(int x, int y, ::Value value);
    void ArraySetVariant(int x, int y, VariantOle &value);
    void Value(::Value value);

public:    
    VARIANT var;
};

class MSSheet
{
public:
	MSSheet();
	~MSSheet();
	
	static bool IsAvailable();
	
	bool AddSheet(bool visible);
	bool OpenSheet(String fileName, bool visible);
	
	bool SetValue(int col, int row, Value value);
	bool SetValue(String cell, Value value);
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
	
	bool Print();
	
	bool SetSaved(bool);
	bool SaveAs(String fileName, String type = "xls");
	bool Quit();
	
	// New functs for next versions
	void DefMatrix(int width, int height);
	bool FillSelectionMatrix();
	void SetMatrixValue(int i, int j, ::Value value);
	
private:
	ObjectOle App;
	ObjectOle Books;
	ObjectOle Book;
	ObjectOle Sheet;
	ObjectOle Range;
	
	VariantOle Matrix;
	
	bool quit;	
	
	bool SetVisible(bool visible);
};

class MSDoc
{
public:
	MSDoc();
	~MSDoc();
	
	static bool IsAvailable();
	
	bool AddDoc(bool visible);
	bool OpenDoc(String fileName, bool visible);

	bool SetFont(String font, int size);
	bool SetBold(bool bold);
	bool SetItalic(bool italic);
	bool WriteText(String value);
	
	bool Select();
	
	bool Replace(String search, String replace);
	
	bool Print();
	
	bool SetSaved(bool);
	bool SaveAs(String fileName, String type = "doc");
	bool Quit();
	
private:
	ObjectOle App;	
	ObjectOle Docs;
	ObjectOle Doc;
	ObjectOle Selection;
	
	bool ReplaceSubset(String search, String replace);
	static String CleanString(String str);
	bool quit;
	
	bool SetVisible(bool visible);
};

#define wdFindContinue 	1
#define wdReplaceAll	2


class OPENSheet
{
public:
	OPENSheet();
	~OPENSheet();
	
	static bool IsAvailable();
	
	bool AddSheet(bool visible);
	bool OpenSheet(String fileName, bool visible);
	
	bool SetValue(int col, int row, Value value);
	bool SetValue(String cell, Value value);
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
	/* // New functs for next versions
	void DefMatrix(int width, int height);
	bool FillSelectionMatrix();
	void SetMatrixValue(int i, int j, ::Value value);
	*/
	bool Print();
	
	bool SetSaved(bool);
	bool SaveAs(String fileName, String type = "xls");
	bool Quit();
	
	// New functs for next versions
	bool InsertTab(String name);
	bool ChooseTab(String name);
	bool ChooseTab(int index);
	int GetNumTabs();
	bool SetItalic(String cell, bool italic);
	bool SetItalic(int col, int row, bool italic);
	bool SetItalic(bool italic);
	bool SetUnderline(bool underline);
	bool SetUnderline(String cell, bool underline);
	bool SetUnderline(int col, int row, bool underline);
	bool SetHorizJustify(int justify);
	bool SetHorizJustify(String cell, int justify);
	bool SetHorizJustify(int col, int row, int justify);
	bool SetVertJustify(String cell, int justify);
	bool SetVertJustify(int col, int row, int justify);
	bool SetCellBackColor(Color color);
	bool SetCellBackColor(String cell, Color color);
	bool SetCellBackColor(int col, int row, Color color);
	bool SetFormat(String format);
	bool SetColWidth(int col, int width);
	bool SetRowHeight(int row, int height);
	
private:
	ObjectOle ServiceManager;
	ObjectOle CoreReflection;
	ObjectOle Desktop;
	ObjectOle Document;
	ObjectOle Sheets;
	ObjectOle Sheet;
	ObjectOle Cell;
	ObjectOle Range;
	
	bool selectedAll;
	bool SelCell(int x, int y);
	bool SetVisible(bool visible);
	bool quit;
};

class OPENDoc
{
public:
	OPENDoc();
	~OPENDoc();
	
	static bool IsAvailable();
	
	bool AddDoc(bool visible);
	bool OpenDoc(String fileName, bool visible);

	bool SetFont(String font, int size);
	bool SetBold(bool bold);
	bool SetItalic(bool italic);
	bool WriteText(String value);
	
	bool Select();
	
	bool Replace(String search, String replace);
	
	bool Print();
	
	bool SetSaved(bool);
	bool SaveAs(String fileName, String type = "doc");
	bool Quit();

	// New functs for next versions	
	bool SetColor(Color col);
	
private:
	ObjectOle ServiceManager;
	ObjectOle CoreReflection;
	ObjectOle Desktop;
	ObjectOle Document;
	ObjectOle Text;
	ObjectOle Cursor;
	bool quit;
	
	bool SetVisible(bool visible);
};

#endif
