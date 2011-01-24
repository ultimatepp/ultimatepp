#ifndef _OfficeAutomationBase_h
#define _OfficeAutomationBase_h

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
    void Optional();
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
		
	bool SetHorizAlignment(String cell, int alignment);
	bool SetHorizAlignment(int col, int row, int alignment);
	bool SetVertAlignment(String cell, int alignment);
	bool SetVertAlignment(int col, int row, int alignment); 
	
	bool SetBorder(int borderIndx, int lineStyle, int weight, Color color);	
	bool SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color);
			
	bool Select(String range);
	bool Select(int fromX, int fromY, int toX, int toY);
	bool Select();
	
	bool Print();
	
	bool SetSaved(bool);
	bool SaveAs(String fileName, String type = "xls");
	bool Quit();
	
	bool InsertTab(String name);
	bool ChooseTab(String name);
	bool ChooseTab(int index);
	bool RemoveTab(String name);
	bool RemoveTab(int index);	
	int GetNumTabs();
	
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
	bool SetBorder(ObjectOle &borders, int borderIndx, int lineStyle, int weight, Color color);
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
	bool SetValue(Value value);
	Value GetValue(int col, int row);
	Value GetValue(String cell);
	bool Replace(Value search, Value replace);
		
	bool SetBold(int col, int row, bool bold);
	bool SetBold(String cell, bool bold);
	bool SetBold(bool bold);
	bool SetItalic(String cell, bool italic);
	bool SetItalic(int col, int row, bool italic);
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
	
	bool SetColWidth(int col, double width);
	bool SetRowHeight(int row, double height);
	
	bool SetHorizAlignment(String cell, int alignment);
	bool SetHorizAlignment(int col, int row, int alignment);
	bool SetVertAlignment(String cell, int alignment);
	bool SetVertAlignment(int col, int row, int alignment); 
	
	bool SetBorder(int borderIndx, int lineStyle, int weight, Color color);	
	bool SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color);
	
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
	
	bool InsertTab(String name);
	bool ChooseTab(String name);
	bool ChooseTab(int index);
	bool RemoveTab(String name);
	bool RemoveTab(int index);	
	int GetNumTabs();
	
	// New functs for next versions
	//bool SetCellBackColor(Color color);
	//bool SetCellBackColor(String cell, Color color);
	//bool SetCellBackColor(int col, int row, Color color);
	bool SetFormat(String format);
	
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


class Ole {
public:
	static bool Invoke(int autoType, VARIANT *pvResult, IDispatch *pDisp, String name, int cArgs...);
	static void Init();
	static bool Close();
	static ObjectOle CreateObject(String application);
	static ObjectOle GetObject(ObjectOle from, String what);
	static ObjectOle GetObject(ObjectOle from, String which, VariantOle &value);
	static ObjectOle GetObject(ObjectOle from, String which, VariantOle &value, VariantOle &value2);
	static bool SetValue(ObjectOle from, String which, VariantOle &value);
	static bool SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2);
	static bool SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3);
	static Value GetValue(ObjectOle from, String which);
	static Value GetValue(ObjectOle from, String which, VariantOle &value);
	static Value GetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2);
	static bool Method(ObjectOle from, String which);
	static bool Method(ObjectOle from, String which, VariantOle &value);
	static bool Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2);
	static bool Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3);
	static bool Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4);
	static bool Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5);
	static bool Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5, VariantOle &value6, VariantOle &value7, VariantOle &value8, VariantOle &value9, VariantOle &value10, VariantOle &value11);
	static ObjectOle MethodGet(ObjectOle from, String which);
	static ObjectOle MethodGet(ObjectOle from, String which, VariantOle &value);
	static ObjectOle MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2);
	static ObjectOle MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3);
	static ObjectOle MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4);
	static bool IsOn() {return numOleInit > 0;};
private:
	static int numOleInit;
};

#endif
