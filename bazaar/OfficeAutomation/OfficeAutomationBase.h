#ifndef _OfficeAutomationBase_h
#define _OfficeAutomationBase_h

#include <Ole/Ole.h>

using namespace Upp;

#include "OfficeAutomationMethods.h"

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
    bool ArrayDim(int sizeX);
    bool ArrayDim(int sizeX, int sizeY);
    bool ArrayDestroy();
    bool ArraySetValue(int x, ::Value value);
    bool ArraySetVariant(int x, VariantOle &value);
    bool ArraySetValue(int x, int y, ::Value value);
    bool ArraySetVariant(int x, int y, VariantOle &value);
    void Value(::Value value);

    VARIANT var;
};
	
	
class MSSheet : public OfficeSheet
{
public:
	MSSheet();
	~MSSheet();
	
	virtual bool IsAvailable();					
	
	Sheet_METHOD_LIST
	
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
	
	bool killProcess;
};

class MSDoc : public OfficeDoc
{
public:
	MSDoc();
	~MSDoc();
	
	virtual bool IsAvailable();
	
	Doc_METHOD_LIST
	bool Close();
	
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


class OPENSheet : public OfficeSheet
{
public:
	OPENSheet();
	~OPENSheet();
	
	virtual bool IsAvailable();			
	
	Sheet_METHOD_LIST
	
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

class OPENDoc : public OfficeDoc
{
public:
	OPENDoc();
	~OPENDoc();
	
	virtual bool IsAvailable();
	
	Doc_METHOD_LIST

	// New functs for next versions	
	//bool SetColor(Color col);
	
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
	static ObjectOle MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5);
	static bool IsOn() {return numOleInit > 0;};
private:
	static int numOleInit;
};

HRESULT CoGetServerPID(IUnknown* punk, DWORD* pdwPID);

#endif
