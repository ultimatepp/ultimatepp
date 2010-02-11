#include <Core/Core.h>

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include <winnls.h>

#include "Functions4U/Functions4U.h"
#include "OfficeAutomationBase.h"
#include "OfficeAutomation.h"

////////////////////////////////////////////////////////////////////////////////////////
VariantOle::VariantOle()
{
    VariantInit(&var);
    VariantClear(&var); 
}    
VariantOle::~VariantOle()
{
    if (var.vt == VT_BSTR)
        SysFreeString(var.bstrVal);
}
void VariantOle::Bool(bool val)
{
    var.vt = VT_BOOL;
    var.boolVal = val;
}
void VariantOle::Int(int val)
{
    var.vt = VT_I4;
    var.intVal = val;
}
void VariantOle::Int4(long val)
{
    var.vt = VT_I4;
    var.lVal = val;
}
void VariantOle::Real4(float val)
{
    var.vt = VT_R4;
    var.fltVal = val;
}
void VariantOle::Real8(double val)
{
    var.vt = VT_R8;
    var.dblVal = val;
}
void VariantOle::BString(String str)
{
	WCHAR wfileName[1024*sizeof(WCHAR)];

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wfileName, sizeof(wfileName)/sizeof(wfileName[0]));
	var.vt = VT_BSTR;
	var.bstrVal = ::SysAllocString(wfileName);
}
void VariantOle::Optional()
{
    var.vt = VT_ERROR; 
    var.scode = DISP_E_PARAMNOTFOUND;	
}
void VariantOle::ObjectOle(::ObjectOle val)
{
	var.vt = VT_DISPATCH;
    var.pdispVal = val;
}
void VariantOle::Time(Upp::Time t)
{
    var.vt = VT_DATE;
    SYSTEMTIME stime;
   	stime.wDay = t.day;
  	stime.wMonth = t.month;
   	stime.wYear = t.year;
    stime.wHour = t.hour; 
  	stime.wMinute = t.minute;
   	stime.wSecond = t.second;	
    SystemTimeToVariantTime(&stime, &(var.date));
}
void VariantOle::ArrayDim(int sizeX)
{
    var.vt = VT_ARRAY | VT_VARIANT;
    SAFEARRAYBOUND sab[1];		// 1 dimension
	sab[0].lLbound = 1; sab[0].cElements = sizeX;
	var.parray = SafeArrayCreate(VT_VARIANT, 1, sab);
}
void VariantOle::ArrayDim(int sizeX, int sizeY)
{
    var.vt = VT_ARRAY | VT_VARIANT;
    SAFEARRAYBOUND sab[2];		// 2 dimension
	sab[0].lLbound = 1; sab[0].cElements = sizeY;
	sab[1].lLbound = 1; sab[1].cElements = sizeX;
	var.parray = SafeArrayCreate(VT_VARIANT, 2, sab);
}
void VariantOle::ArraySetValue(int x, ::Value value)
{
	VariantOle tmp;
	tmp.Value(value);
	long indices[] = {x};
	SafeArrayPutElement(var.parray, indices, (void *)&tmp);
}
void VariantOle::ArraySetVariant(int x, VariantOle &value)
{
	long indices[] = {x};
	SafeArrayPutElement(var.parray, indices, (void *)&value);
}
void VariantOle::ArraySetValue(int x, int y, ::Value value)
{
	VariantOle tmp;
	tmp.Value(value);
	long indices[] = {y, x};
	SafeArrayPutElement(var.parray, indices, (void *)&tmp);
}
void VariantOle::ArraySetVariant(int x, int y, VariantOle &value)
{
	long indices[] = {y, x};
	SafeArrayPutElement(var.parray, indices, (void *)&value);
}
void VariantOle::Value(::Value value)
{
	if(value.Is<bool>()) {
		bool v = value;
		Bool(v);
	} else if(value.Is<int>()) {
		int v = value;
		Int(v);
	} else if(value.Is<int64>()) {
		int64 v = value;
		Int4((long)v);
	//} else if(value.Is<float>()) {
	//	float v = value;
	//	Real4(v);
	}  else if(value.Is<double>()) {
		double v = value;
		Real8(v);
	} else if(value.Is<String>()) {
		String v = value;
		BString(v);
	} else if(IsDateTime(value)) {
		Upp::Time v = value;
		Time(v);
	} else {
		String v = value;
		BString(v);
	}        
}

////////////////////////////////////////////////////////////////////////////////////////
class Ole
{
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

int Ole::numOleInit = 0;

void Ole::Init()
{	// Initialize COM for this thread
	if (numOleInit == 0) {
		OleInitialize(NULL);
		CoInitialize(NULL);
		numOleInit++;
	}
}
bool Ole::Close()
{	// Uninitialize COM for this thread
	numOleInit--;
	if (numOleInit == 0) 
		CoUninitialize();
	if (numOleInit < 0)	{	// This is a mistake
		numOleInit = 0;
		return false;
	} else
		return true;
}
// Invoke() - Automation helper function
bool Ole::Invoke(int autoType, VARIANT *pvResult, IDispatch *pDisp, String name, int cArgs ...)
{
    // Begin variable-argument lis
    va_list marker;
    va_start(marker, cArgs);

    if(!pDisp) {
        MessageBox(NULL, t_("OfficeAutomation internal error. NULL IDispatch passed to AutoWrap()"), t_("Error"), 0x10010);
        return false;
    }
    // Variables used
    DISPPARAMS dp = { NULL, NULL, 0, 0 };
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    DISPID dispID;
    HRESULT hr;
    char buf[200];
    char szName[200];
    WString wname = name.ToWString();
	WStringBuffer ptName(wname);

    // Convert down to ANSI
    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

    // Get DISPID for name passed
    hr = pDisp->GetIDsOfNames(IID_NULL, (LPOLESTR *)&ptName, 1, LOCALE_USER_DEFAULT, &dispID);
    if(FAILED(hr)) {
        //sprintf(buf, "Command IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
        sprintf(buf, t_("OfficeAutomation internal error. Command \"%s\" not found for object or problem when running it"), szName);
        MessageBox(NULL, buf, t_("Ole error"), 0x10010);
        return false;
    }
    // Allocate memory for arguments
    VARIANT *pArgs = new VARIANT[cArgs+1];
    // Extract arguments
    for(int i = 0; i < cArgs; i++) 
        pArgs[i] = va_arg(marker, VARIANT);
    
    // Build DISPPARAMS
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs;

    // Handle special-case for property-puts!
    if(autoType & DISPATCH_PROPERTYPUT) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }
    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);

    // End variable-argument section
    va_end(marker);

    delete [] pArgs;

    if(FAILED(hr)) {
        //sprintf(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
		//MessageBox(NULL, buf, "Ole::Invoke()", 0x10010);
        return false;
    }
    return true;
}
ObjectOle Ole::CreateObject(String application)
{
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(application.ToWString(), &clsid);	// Get CLSID for our server
	if(FAILED(hr)) {
		//::MessageBox(NULL, "CLSIDFromProgID() failed", "Error", 0x10010);
		return NULL;
	}
	ObjectOle app;
	// Start server and get IDispatch
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&app);
	if(FAILED(hr)) {
		::MessageBox(NULL, t_("OfficeAutomation internal error. Application not registered properly"), t_("Error"), 0x10010);
		return NULL;
	}
	return app;
}
ObjectOle Ole::GetObject(ObjectOle from, String which)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 0))
		return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::GetObject(ObjectOle from, String which, VariantOle &value)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 1, value.var))
		return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::GetObject(ObjectOle from, String which, VariantOle &value, VariantOle &value2)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 2, value.var, value2.var))
		return NULL;
	else
		return result.pdispVal;
}
Value Ole::GetValue(ObjectOle from, String which)
{
	VARIANT result;
	VariantInit(&result);	
	if(!Ole::Invoke(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &result, from, which, 0))
		return "";
	
	return GetVARIANT(result);
}
bool Ole::SetValue(ObjectOle from, String which, VariantOle &value)
{
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 1, value.var);
}
bool Ole::SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2)
{
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 2, value.var, value2.var);
}
bool Ole::SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3)
{
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 3, value.var, value2.var, value3.var);
}
// result var is not used but OpenOffice automation requires it (I do not why)
bool Ole::Method(ObjectOle from, String which)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 0);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 1, value.var);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 2, value.var, value2.var);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var, value4.var);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var, value4.var, value5.var);
}
bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5, VariantOle &value6, VariantOle &value7, VariantOle &value8, VariantOle &value9, VariantOle &value10, VariantOle &value11)
{
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 11, value.var, value2.var, value3.var, value4.var, value5.var, value6.var, value7.var, value8.var, value9.var, value10.var, value11.var);
}
ObjectOle Ole::MethodGet(ObjectOle from, String which)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 0))
	   	return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 1, value.var))
	   	return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 2, value.var, value2.var))
	   	return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var))
	   	return NULL;
	else
		return result.pdispVal;
}
ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4)
{
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 4, value.var, value2.var, value3.var, value4.var))
	   	return NULL;
	else
		return result.pdispVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MSSheet::MSSheet()
{
	App = Books = Book = Sheet = Range = NULL;
	quit = false;

	App = Ole::CreateObject("Excel.Application");
	if (!(Books = Ole::GetObject(App, "Workbooks"))) {
		::MessageBox(NULL, t_("Excel workbooks not loaded properly"), t_("Error"), 0x10010);
		return;
	}
}
MSSheet::~MSSheet()
{
	if (Range)
		Range->Release();
	if (Sheet)
		Sheet->Release();
	if (Book)
		Book->Release();
	if (Books)
		Books->Release();
	if (App) {
		Quit();
		App->Release();
	}
}
bool MSSheet::IsAvailable()
{
	ObjectOle app = Ole::CreateObject("Excel.Application");
	
	if (!app) 
		return false;
	else {
		app->Release();
		return true;
	}
}
bool MSSheet::SetVisible(bool visible)
{
	if (!Books)
		return false;

	VariantOle vVisible;
	vVisible.Int4(visible? 1: 0);
	return Ole::SetValue(App, "Visible", vVisible);
}
bool MSSheet::SetSaved(bool saved)
{
	if (!Books && !Book)
		return false;

   // Set .Saved property of workbook to TRUE so we aren't prompted
   // to save when we tell Excel to quit...
	VariantOle vSaved;
	vSaved.Int4(saved? 1: 0);
	return Ole::SetValue(Book, "Saved", vSaved);
}
bool MSSheet::Quit()
{
	if (!quit) {
		quit = true;
		return Ole::Method(App, "Quit");
	}
	return true;
}
bool MSSheet::AddSheet(bool visible)
{
	if (!Books)
		return false;
	
	if (!(Book = Ole::GetObject(Books, "Add")))
		return false;
	if (!(Sheet = Ole::GetObject(App, "ActiveSheet")))
		return false;

	SetVisible(visible);
	return true;
}
bool MSSheet::OpenSheet(String fileName, bool visible)
{
	if (!Books)
		return false;

	VariantOle vFileName;
	vFileName.BString(fileName);	
	if (!(Book = Ole::GetObject(Books, "Open", vFileName)))
		return false;
	if (!(Sheet = Ole::GetObject(App, "ActiveSheet")))
		return false;

	SetVisible(visible);
	return true;
}
bool MSSheet::Select(String range)
{
	VariantOle vRange;
	vRange.BString(range);
	
	if (!(Range = Ole::GetObject(Sheet, "Range", vRange)))
		return false;	
	return true;
}
bool MSSheet::Select(int fromX, int fromY, int toX, int toY)
{
	String range = OfficeSheet::ColRowToCell(fromX, fromY) + ":" + OfficeSheet::ColRowToCell(toX, toY);
	VariantOle vRange;
	vRange.BString(range);
	
	if (!(Range = Ole::GetObject(Sheet, "Range", vRange)))
		return false;	
	return true;
}
bool MSSheet::Select()
{
	if (!(Range = Ole::GetObject(Sheet, "Cells")))		// ActiveSheet.Cells.Select
		return false;
	return Ole::Method(Range, "Select");
}
void MSSheet::DefMatrix(int width, int height)
{
	Matrix.ArrayDim(width, height);
}
void MSSheet::SetMatrixValue(int x, int y, ::Value value)
{
	Matrix.ArraySetValue(x, y, value);
}
bool MSSheet::FillSelectionMatrix()
{
	if (!Range)
		return false;

	bool ret = Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, Range, "Value", 1, Matrix.var);

	return ret;
}

bool MSSheet::SetValue(String cell, Value value)	// cell in textual format like "B14" 
{
	int col, row;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return MSSheet::SetValue(col, row, value);
}
bool MSSheet::SetValue(int col, int row, Value value)
{
	if (!Sheet)
		return false;

	VariantOle x, y, val;
	x.Int4(col);
	y.Int4(row);
	val.Value(value);
		
	return Ole::SetValue(Sheet, "Cells", val, x, y);
}
bool MSSheet::SetValue(Value value)
{
	if (!Sheet)
		return false;

	if (!Range)
		return false;

	VariantOle val;
	val.Value(value);
	
	return Ole::SetValue(Range, "Value", val);
}
bool MSSheet::Replace(Value search, Value replace)
{
	if (!App)
		return false;

	ObjectOle selection;								// Call Application.Selection.Replace(search, replace)
	if (!(selection = Ole::GetObject(App, "Selection")))	
		return false;	
	
	VariantOle vSearch, vReplace;
	vSearch.Value(search);
	vReplace.Value(replace);
		
	return Ole::Method(selection, "Replace", vReplace, vSearch);
}
Value MSSheet::GetValue(int col, int row)
{
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	return Ole::GetValue(Ole::GetObject(Sheet, "Cells", x, y), "value");
}
Value MSSheet::GetValue(String cell)
{
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetValue(col, row);
}
bool MSSheet::SaveAs(String fileName, String type)
{
	if (!Book)
		return false;

	fileName = ForceExt(fileName, "." + type);
	
	VariantOle vFileName, vType;
	
	vFileName.BString(fileName);
	if (type == "csv")
		vType.Int(6);
	else if (type == "html")
		vType.Int(44);
	else if (type == "txt")
		vType.Int(20);
	else if (type == "xlsx")
		vType.Int(51);
	else				// xls
		vType.Int(-4143);

	bool opened = false;
	if (FileExists(fileName)) {
		if (!FileToTrashBin(fileName)) 
			opened = true;
	} 
	int ret;
	if (opened)
		ret = Ole::Method(Book, "Save");
	else
		ret = Ole::Method(Book, "SaveAs", vType, vFileName);

	return (bool)ret;
}
bool MSSheet::SetBold(String cell, bool bold)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, bold);
}
bool MSSheet::SetBold(int col, int row, bool bold)
{
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle val;
	val.Bool(bold);	
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font"), "Bold", val);
}
bool MSSheet::SetBold(bool bold)
{
	if (!Range)
		return false;

	VariantOle val;
	val.Bool(bold);	
	return Ole::SetValue(Ole::GetObject(Range, "Font"), "Bold", val);
}
bool MSSheet::SetFont(String cell, String name, int size)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetFont(col, row, name, size);
}
bool MSSheet::SetFont(int col, int row, String name, int size)
{
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	ObjectOle font = Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font");
	if (!font)
		return false;

	VariantOle vName;
	vName.BString(name);	
	if(!Ole::SetValue(font, "Name", vName))
		return false;
	VariantOle vSize;
	vSize.Int(size);	
	if(!Ole::SetValue(font, "Size", vSize))
		return false;
	return true;
}
bool MSSheet::SetFont(String name, int size)
{
	if (!Range)
		return false;

	ObjectOle font = Ole::GetObject(Range, "Font");
	if (!font)
		return false;

	VariantOle vName;
	vName.BString(name);	
	if(!Ole::SetValue(font, "Name", vName))
		return false;
	VariantOle vSize;
	vSize.Int(size);	
	if(!Ole::SetValue(font, "Size", vSize))
		return false;
	return true;
}
bool MSSheet::Print()
{
	if (!Sheet)
		return false;
	
	bool ret = Ole::Method(Sheet, "PrintOut");
	
	return ret;
}

enum XLSheetTypes {xlWorksheet = -4167, xlChart = -4109, xlExcel4MacroSheet = 3, xlExcel4IntlMacroSheet = 4};

bool MSSheet::InsertTab(String name)	// Insert tab after the last
{
	if (!App)
		return false;
	
	if (!(Sheet = Ole::GetObject(Ole::GetObject(App, "Sheets"), "Add")))
		return false;
	VariantOle vName;
	vName.BString(name);	
	if(!Ole::SetValue(Sheet, "Name", vName))
		return false;
	//return true;

	VariantOle vOptional;		
	vOptional.Optional();
	
	ObjectOle lastSheet;
	VariantOle vId;
	vId.Int(GetNumTabs());	
	if (!(lastSheet = Ole::GetObject(App, "Sheets", vId)))
		return false;
	VariantOle vAfter;
	vAfter.ObjectOle(lastSheet);
	
	return Ole::Method(Sheet, "Move", vAfter, vOptional);	// Apending tab after last tab
}
bool MSSheet::ChooseTab(String name)
{
	if (!App)
		return false;

    VariantOle vName;
	vName.BString(name);
	if (!(Sheet = Ole::GetObject(App, "Sheets", vName)))
		return false;
	
	return Ole::Method(Sheet, "select");	
}
bool MSSheet::ChooseTab(int index)
{
	if (!App)
		return false;	
	
    VariantOle vId;
	vId.Int(index+1);	
	if (!(Sheet = Ole::GetObject(App, "Sheets", vId)))
		return false;
	
	return Ole::Method(Sheet, "select");
}
bool MSSheet::RemoveTab(String name)
{
	if (!App)
		return false;

	ObjectOle DelSheet;
    VariantOle vName;
	vName.BString(name);
	if (!(DelSheet = Ole::GetObject(App, "Sheets", vName)))
		return false;
	
	return Ole::Method(DelSheet, "delete");	
}
bool MSSheet::RemoveTab(int index)
{
	if (!App)
		return false;	
	
	ObjectOle DelSheet;
    VariantOle vId;
	vId.Int(index+1);	
	if (!(DelSheet = Ole::GetObject(App, "Sheets", vId)))
		return false;
	
	return Ole::Method(DelSheet, "delete");
}
int MSSheet::GetNumTabs()
{
	if (!App)
		return -1;

	return Ole::GetValue(Ole::GetObject(App, "Sheets"), "count");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MSDoc::MSDoc()
{
	App = Docs = Doc = Selection = NULL;
	quit = false;
	
	App = Ole::CreateObject("Word.Application");
	if (!(Docs = Ole::GetObject(App, "Documents"))) {
		::MessageBox(NULL, t_("Word document not loaded properly"), t_("Error"), 0x10010);
		return;
	}
}
MSDoc::~MSDoc()
{
	if (Selection)
		Selection->Release();
	if (Doc)
		Doc->Release();
	if (Docs)
		Docs->Release();
	if (App) {
		Quit();
		App->Release();
	}
	Ole::Close();
}
bool MSDoc::SetVisible(bool visible)
{
	if (!Docs)
		return false;

	VariantOle vVisible;
	vVisible.Int4(visible? 1: 0);
	return Ole::SetValue(App, "Visible", vVisible);
}
bool MSDoc::IsAvailable()
{
	ObjectOle app = Ole::CreateObject("Word.Application");
	
	if (!app) 
		return false;
	else {
		app->Release();
		return true;
	}
}
bool MSDoc::SetSaved(bool saved)
{
	if (!Doc)
		return false;

	VariantOle vSaved;
	vSaved.Int4(saved? 1: 0);
	return Ole::SetValue(Doc, "Saved", vSaved);
}
bool MSDoc::Quit()
{
	if (!quit) {
		quit = true;
		return Ole::Method(App, "Quit");
	}
	return true;
}
bool MSDoc::AddDoc(bool visible)
{
	if (!Docs)
		return false;
	
	if (!(Doc = Ole::MethodGet(Docs, "Add")))
		return false;

	if(!(Selection = Ole::GetObject(App, "Selection")))
		return false;
	
	SetVisible(visible);
	return true;
}
bool MSDoc::OpenDoc(String fileName, bool visible)
{
	if (!Docs)
		return false;

	VariantOle vFileName;
	vFileName.BString(fileName);	
	if (!(Doc = Ole::MethodGet(Docs, "Open", vFileName)))
		return false;
		
	if(!(Selection = Ole::GetObject(App, "Selection")))
		return false;	
	
	SetVisible(visible);
	return true;
}
bool MSDoc::SaveAs(String fileName, String type)
{
	if (!Doc)
		return false;

	fileName = ForceExt(fileName, "." + type);
	
	VariantOle vFileName, vType;
	
	vFileName.BString(fileName);
	if (type == "rtf")
		vType.Int(6);
	else if (type == "html")
		vType.Int(8);
	else if (type == "txt")
		vType.Int(2);
	else if (type == "docx")
		vType.Int(13);
	else				// doc
		vType.Int(0);

	int ret = Ole::Method(Doc, "SaveAs", vType, vFileName);

	return (bool)ret;
}
bool MSDoc::WriteText(String value)
{
	if (!Selection)
		return false;
	
	VariantOle val;
	val.BString(value);	
	return Ole::Method(Selection, "TypeText", val);
}
bool MSDoc::SetFont(String type, int size)
{
	ObjectOle font;
	
	if(!(font = Ole::GetObject(Selection, "Font")))
		return false;
	
	VariantOle vType;
	vType.BString(type);	
	if(!Ole::SetValue(font, "Name", vType))
		return false;
	VariantOle vSize;
	vSize.Int(size);	
	if(!Ole::SetValue(font, "Size", vSize))
		return false;

	return true;	
}
bool MSDoc::SetBold(bool bold)
{
	VariantOle vBold;
	vBold.Bool(bold);	
	if(!Ole::SetValue(Ole::GetObject(Selection, "Font"), "Bold", vBold))
		return false;
	
	return true;	
}
bool MSDoc::SetItalic(bool italic)
{
	VariantOle vItalic;
	vItalic.Bool(italic);	
	if(!Ole::SetValue(Ole::GetObject(Selection, "Font"), "Italic", vItalic))
		return false;
	
	return true;	
}
bool MSDoc::Print()
{
	if (!Docs && !Doc)
		return false;
	
	bool ret = Ole::Method(Doc, "PrintOut");
	
	return ret;
}
bool MSDoc::Select()
{
	return Ole::Method(Selection, "WholeStory");		
}
// Some tricks as Ole Replace does not work properly for texts to replace longer than 
// about 250 chars and for some special chars
bool MSDoc::Replace(String search, String _replace)
{
	String replace = CleanString(_replace);
    replace = ::Replace(replace, "^", " ");
    replace = ::Replace(replace, "\r", "");  // To remove squares
    
    String replaceSubset;
    while (replace.GetCount() > 200) {
        replaceSubset = replace.Left(200);
        replaceSubset.Cat(search);
        replace = replace.Right(replace.GetCount() - 200);
        
        replaceSubset = ::Replace(replaceSubset, "\n", "^l");  
        if (!ReplaceSubset(search, replaceSubset))
        	return false;
    }
    replace = ::Replace(replace, "\n", "^l");  
    return ReplaceSubset(search, replace);
}
String MSDoc::CleanString(String str)	// Clean chars in String
{
    String ret;
    String valid = ">=<ºª%€&()$1234567890áéíóúÁÉÍÓÚñÑçÇ,.,:-_/¿?+*[]{}'\"!¡ \r\n";
    for (int i = 0; i < str.GetCount(); ++i) {
        int s = str[i];        
        if (IsLetter(s) || valid.Find(s) >= 0)
            ret.Cat(s);
    }
    return ret;
}
bool MSDoc::ReplaceSubset(String search, String replace)
{
	ObjectOle find;
	if (!(find = Ole::GetObject(Selection, "Find")))
		return false;
	
	if(!Ole::Method(find, "ClearFormatting"))		// Selection.Find.ClearFormatting
		return false;
	
	VariantOle vFindText;
	vFindText.BString(search);
	
	VariantOle vMatchCase;
	vMatchCase.Bool(false);
	
	VariantOle vMatchWholeWord;
	vMatchWholeWord.Bool(false);

	VariantOle vMatchWildcards;
	vMatchWildcards.Bool(false);
	
	VariantOle vMatchSoundsLike;
	vMatchSoundsLike.Bool(false);

	VariantOle vMatchAllWordForms;
	vMatchAllWordForms.Bool(false);
	
	VariantOle vForward;
	vForward.Bool(true);

	VariantOle vWrap;
	vWrap.Int(wdFindContinue);

	VariantOle vFormat;
	vFormat.Bool(false);
	
	VariantOle vReplaceWith;
	vReplaceWith.BString(replace);

	VariantOle vReplace;
	vReplace.Int(wdReplaceAll);
	
	return Ole::Method(find, "Execute", vReplace, vReplaceWith, vFormat, vWrap, vForward, vMatchAllWordForms, vMatchSoundsLike, vMatchWildcards, vMatchWholeWord, vMatchCase, vFindText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OOo
{
public:
	static ObjectOle MakePropertyValue(String name, ::Value value)
	{
    	ObjectOle ServiceManager;
    	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
    		return NULL;
    	
		ObjectOle Struct;
		VariantOle vstr;
		vstr.BString("com.sun.star.beans.PropertyValue");
		if (!(Struct = Ole::MethodGet(ServiceManager, "Bridge_GetStruct", vstr)))
    		return NULL;
		
		VariantOle vname, vvalue;
		vname.BString(name);
		if (!Ole::SetValue(Struct, "Name", vname))
			return NULL;
		vvalue.Value(value);
		if (!Ole::SetValue(Struct, "Value", vvalue))
			return NULL;
		return Struct;		
	}
	static String ConvertToUrl(String strFile) 
	{
		String ret;
		String c;
		
		for (int i = 0; i < strFile.GetCount(); ++i) {
			if (strFile[i] >= 123)
				c = "%" + Format("%x", strFile[i]);
			else {
				switch (strFile[i]) {
				case 9:		c = "%09";	break;
	      		case 13:	c = "%0d";	break;
	      		case 10:	c = "%0a";	break;
	      		case 32: case 33: case 34: case 35: case 37: case 38: case 39: case 40: case 41: case 43: 
	      		case 44: case 59: case 60: case 61: case 62: case 63: case 91: case 93: case 94: case 96:	
	      					c = "%" + Format("%x", strFile[i]); break;
				case '\\':	c = "/";	break;		
				case ':':	c = "|";	break;
				default:	c = strFile.Mid(i, 1);
				}
			}
			ret.Cat(c);
		}
		return "file:///" + ret;
	}
	static ObjectOle CreateUnoService(String strServiceName) 
	{
    	ObjectOle ServiceManager;
    	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
    		return NULL;
    	
    	VariantOle vService;
    	vService.BString(strServiceName);
    	return Ole::MethodGet(ServiceManager, "createInstance", vService);    	
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OPENSheet::OPENSheet()
{
	ServiceManager = CoreReflection = Desktop = Document = Sheets = Sheet = Cell = Range = NULL;
	selectedAll = false;
	quit = false;
	
	// The service manager is always the starting point
	// If there is no office running then an office is started up
	ObjectOle ServiceManager;
	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return;

	// Create the CoreReflection service that is later used to create structs
	ObjectOle CoreReflection;
	VariantOle vInstance;
	vInstance.BString("com.sun.star.reflection.CoreReflection");	
	if (!(CoreReflection = Ole::MethodGet(ServiceManager, "createInstance", vInstance)))
		return;

	// Create the Desktop
	VariantOle vDesktop;
	vDesktop.BString("com.sun.star.frame.Desktop");	
	if (!(Desktop = Ole::MethodGet(ServiceManager, "createInstance", vDesktop)))
		return;
}

OPENSheet::~OPENSheet()
{
	if (Cell)
		Cell->Release();
	if (Range)
		Range->Release();
	if (Sheet)
		Sheet->Release();
	if (Sheets)
		Sheets->Release();
	if (Document)
		Document->Release();
	if (Desktop) {
		Quit();
		Desktop->Release();
	}
	if (CoreReflection)
		CoreReflection->Release();
	if (ServiceManager)
		ServiceManager->Release();
}
bool OPENSheet::IsAvailable()
{
	ObjectOle serviceManager;
	if (!(serviceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return false;
	else {
		serviceManager->Release();
		return true;
	}
}
bool OPENSheet::SetSaved(bool saved)
{
	VariantOle vval;
	vval.Bool(!saved);	// Modified is the opposite to saved
	return Ole::Method(Document, "setModified", vval);
}
bool OPENSheet::Quit()
{
	if (!quit) {
		quit = true;
		return Ole::Method(Desktop, "Terminate");
	}
	return true;
}
bool OPENSheet::InsertTab(String name)
{
	if (!Sheets)
		return false;
	VariantOle vStr;
	vStr.BString("com.sun.star.sheet.Spreadsheet");
	if (!(Sheet = Ole::MethodGet(Document, "createInstance", vStr)))
		return false;
	
	vStr.BString(name);
	
	VariantOle vsheet;
	vsheet.ObjectOle(Sheet);
	
	if (!(Ole::Method(Sheets, "insertByName", vsheet, vStr)))
		return false;		
	
	// Set tab as active
	ObjectOle View;
	if (!(View = Ole::MethodGet(Document, "getCurrentController")))
		return false;
	
	VariantOle vSheet;
	vSheet.ObjectOle(Sheet);
	return Ole::SetValue(View, "ActiveSheet", vSheet); 	
}
bool OPENSheet::ChooseTab(String name)
{
	if (!Sheets)
		return false;
	VariantOle vStr;
	vStr.BString(name);
	if (!(Sheet = Ole::MethodGet(Sheets, "getByName", vStr)))
		return false;
	
	// Set tab as active
	ObjectOle View;
	if (!(View = Ole::MethodGet(Document, "getCurrentController")))
		return false;
	
	VariantOle vSheet;
	vSheet.ObjectOle(Sheet);
	return Ole::SetValue(View, "ActiveSheet", vSheet); 	
}
bool OPENSheet::ChooseTab(int index)
{
	if (!Sheets)
		return false;
	VariantOle vId;
	vId.Int(index);
	if (!(Sheet = Ole::MethodGet(Sheets, "getByIndex", vId)))
		return false;	
	
	// Set tab as active
	ObjectOle View;
	if (!(View = Ole::MethodGet(Document, "getCurrentController")))
		return false;
	
	VariantOle vSheet;
	vSheet.ObjectOle(Sheet);
	return Ole::SetValue(View, "ActiveSheet", vSheet); 
}
bool OPENSheet::RemoveTab(String name)
{
	if (!Sheets)
		return false;
	VariantOle vStr;
	vStr.BString(name);
	if (!Ole::Method(Sheets, "removeByName", vStr))
		return false;
	return true;
}
bool OPENSheet::RemoveTab(int index)
{
	if (!Sheets)
		return false;
	VariantOle vId;
	vId.Int(index);
	if (!Ole::Method(Sheets, "removeByIndex", vId))
		return false;	
	return true;
}
int OPENSheet::GetNumTabs()
{
	if (!Sheets)
		return -1;
	Value count;
	count = Ole::GetValue(Sheets, "getCount");
	return count; 
}
bool OPENSheet::AddSheet(bool visible)
{
	if (!Desktop)
		return false;
	
	VariantOle vArray;
	vArray.ArrayDim(1);
	VariantOle vobject;
	vobject.ObjectOle(OOo::MakePropertyValue("Hidden", true));
	vArray.ArraySetVariant(1, vobject);

	VariantOle vFile;
	vFile.BString("_blank");	
	VariantOle vSize;
	vSize.Int4(0);	
	VariantOle vDocument;
	vDocument.BString("private:factory/scalc");	
	if (!(Document = Ole::MethodGet(Desktop, "loadComponentFromURL", vArray, vSize, vFile, vDocument)))
		return false;

	if (!(Sheets = Ole::MethodGet(Document, "getSheets")))
		return false;
	
	if (!visible)
		SetVisible(visible);
	return true;	//ChooseTab(0);	// Select the first tab
}
bool OPENSheet::OpenSheet(String fileName, bool visible)
{
	if (!Desktop)
		return false;
	
	VariantOle vArray;
	vArray.ArrayDim(0);

	VariantOle vFile;
	vFile.BString("_blank");	
	VariantOle vSize;
	vSize.Int4(0);	
	VariantOle vDocument;
	vDocument.BString(OOo::ConvertToUrl(fileName));	
	if (!(Document = Ole::MethodGet(Desktop, "loadComponentFromURL", vArray, vSize, vFile, vDocument))) 
		return false;

	if (!(Sheets = Ole::MethodGet(Document, "getSheets")))
		return false;

	if (!visible)
		SetVisible(visible);	
	return ChooseTab(0);	// Select the first tab
}
// Problem: It does hidden but not set visible again the window !!!
bool OPENSheet::SetVisible(bool visible)
{
	if (!Document)
		return false;
	ObjectOle DocCtrl;
	if (!(DocCtrl = Ole::MethodGet(Document, "getCurrentController")))
		return false;
	ObjectOle DocFrame;
	if (!(DocFrame = Ole::MethodGet(DocCtrl, "getFrame")))
		return false;
	ObjectOle DocWindow;
	if (!(DocWindow = Ole::MethodGet(DocFrame, "getContainerWindow")))
		return false;

	VariantOle vvisible;
	vvisible.Bool(visible);   
	return Ole::Method(DocWindow, "setVisible", vvisible); 
}
bool OPENSheet::Select(String range)
{
	if (!Sheet)
		return false;
	VariantOle vrange;
	vrange.BString(range);	
	if (!(Range = Ole::MethodGet(Sheet, "getCellRangeByName", vrange)))
		return false;
	return true;
}
bool OPENSheet::Select(int fromX, int fromY, int toX, int toY)
{
	if (!Sheet)
		return false;
	VariantOle vX1, vY1, vX2, vY2;
	vX1.Int(fromX-1);	// Comienza en 0
	vY1.Int(fromY-1);	
	vX2.Int(toX-1);	// Comienza en 0
	vY2.Int(toY-1);	
	if (!(Range = Ole::MethodGet(Sheet, "getCellRangeByPosition", vY2, vX2, vY1, vX1)))
		return false;
	selectedAll = false;
	return true;
}
bool OPENSheet::Select()
{
	if (!Sheet)
		return false;
	selectedAll = true;
	return true;
}
/*
void OPENSheet::DefMatrix(int width, int height)
{
	return;
}
void OPENSheet::SetMatrixValue(int x, int y, ::Value value)
{
	return;
}
bool OPENSheet::FillSelectionMatrix()
{
	return false;
}
*/
bool OPENSheet::Replace(Value search, Value replace)
{
	if (!Sheet)
		return false;
	ObjectOle Search;
	if (!(Search = Ole::MethodGet(Sheet, "createReplaceDescriptor")))
		return false;		
	VariantOle vsearch, vreplace;
	vsearch.BString(search);
	Ole::Method(Search, "setSearchString", vsearch);
	vreplace.BString(replace);
	Ole::Method(Search, "setReplaceString", vreplace);
	VariantOle vsrep;
	vsrep.ObjectOle(Search);
	
	ObjectOle *data;
	if (selectedAll) {
		if (!Sheet)
			return false;
		data = &Sheet;
	} else {
		if (!Range)
			return false;
		data = &Range;
	}
	if (!Ole::Method(*data, "replaceAll", vsrep))
		return false;
	return true;
}
Value OPENSheet::GetValue(int col, int row)
{
	if (!SelCell(col, row))
		return false;
	return Ole::GetValue(Cell, "getFormula");	// Also valid getValue and getString
}
Value OPENSheet::GetValue(String cell)
{
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetValue(col, row);
}
bool OPENSheet::SaveAs(String fileName, String type)
{
	if (!Document)
		return false;

	fileName = ForceExt(fileName, "." + type);
	VariantOle vFileName;
	vFileName.BString(OOo::ConvertToUrl(fileName));
	
	String filter;
	if (type == "xls")
		filter = "MS Excel 97";
	else if (type == "pxl")
		filter = "Pocket Excel";
	else if (type == "html")
		filter = "HTML (StarCalc)";
	else if (type == "txt")
		filter = "Text - txt - csv (StarCalc)";
	else if (type == "pdf")
		filter = "calc_pdf_Export";
	else if (type == "ods")
		filter = "";
	else
		return false;
	
	VariantOle vArraySave;
	if (filter.IsEmpty()) {
		vArraySave.ArrayDim(0);
		return Ole::Method(Document, "storeToURL", vArraySave, vFileName);
	} else {	
		VariantOle vArg;
		vArg.ObjectOle(OOo::MakePropertyValue("FilterName", filter));
		vArraySave.ArrayDim(1);
		vArraySave.ArraySetVariant(1, vArg);
		return Ole::Method(Document, "storeToURL", vArraySave, vFileName);	
	}
}

bool OPENSheet::SelCell(int x, int y)
{
	if (!Sheet)
		return false;
	VariantOle vX, vY;
	vX.Int(x-1);	// Comienza en 0
	vY.Int(y-1);	
	if (!(Cell = Ole::MethodGet(Sheet, "getCellByPosition", vY, vX)))
		return false;
	return true;
}
	
bool OPENSheet::SetValue(String cell, Value value)	// cell in textual format like "B14" 
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetValue(col, row, value);
}
bool OPENSheet::SetValue(int col, int row, Value value)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Cell, "setFormula", vText);		
	else
		return Ole::Method(Cell, "setValue", vText);		
}
bool OPENSheet::SetValue(Value value)
{
	if(!Range)
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Range, "setFormula", vText);		
	else
		return Ole::Method(Range, "setValue", vText);		
}

bool OPENSheet::SetItalic(String cell, bool italic)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetItalic(col, row, italic);
}
bool OPENSheet::SetItalic(int col, int row, bool italic)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharPosture");
	if (italic)
		vvalue.Int(2);	
	else
		vvalue.Int(0);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetItalic(bool italic)
{
	VariantOle vvalue;
	if (italic)
		vvalue.Int(2);	
	else
		vvalue.Int(0);	
	return Ole::SetValue(Range, "CharPosture", vvalue);
}
bool OPENSheet::SetBold(String cell, bool bold)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, bold);
}
bool OPENSheet::SetBold(int col, int row, bool bold)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharBold");
	vvalue.Bool(bold);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetBold(bool bold) 
{
	VariantOle vvalue;
	if (bold)
		vvalue.Int(150);	
	else
		vvalue.Int(100);	
	return Ole::SetValue(Range, "CharWeight", vvalue);
}
bool OPENSheet::SetUnderline(String cell, bool underline)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, underline);
}
bool OPENSheet::SetUnderline(int col, int row, bool underline)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharUnderline");
	vvalue.Bool(underline);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetUnderline(bool underline)
{
	VariantOle vvalue;
	if (underline)
		vvalue.Int(1);	// 2 is doble underline
	else
		vvalue.Int(0);	
	return Ole::SetValue(Range, "CharUnderline", vvalue);
}
// 2 Centered, 0 Normal, 1 left, 3 right
bool OPENSheet::SetHorizJustify(String cell, int justify)	
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetHorizJustify(col, row, justify);
}
bool OPENSheet::SetHorizJustify(int col, int row, int justify)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("HoriJustify");
	vvalue.Bool(justify);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
// 2 Centered
bool OPENSheet::SetVertJustify(String cell, int justify)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetVertJustify(col, row, justify);
}
bool OPENSheet::SetVertJustify(int col, int row, int justify)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("VertJustify");
	vvalue.Bool(justify);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetFont(String cell, String name, int size)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetFont(col, row, name, size);
}
bool OPENSheet::SetFont(int col, int row, String name, int size)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharFontName");
	vvalue.BString(name);
	Ole::Method(Cell, "setPropertyValue", vvalue, vproperty);
	
	vproperty.BString("CharHeight");
	vvalue.Int(size);
	return Ole::Method(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetFont(String name, int size)
{
	VariantOle vvalue;
	vvalue.BString(name);
	Ole::SetValue(Range, "CharFontName", vvalue);
	vvalue.Int(size);
	return Ole::SetValue(Range, "CharHeight", vvalue);
}
bool OPENSheet::SetCellBackColor(String cell, Color color)	// 2 Centered
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetCellBackColor(col, row, color);
}
bool OPENSheet::SetCellBackColor(int col, int row, Color color)
{
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CellBackColor");
	vvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}
bool OPENSheet::SetFormat(String format)
{
	int value;
	
	if (format == "integer")
		value = 1;
	else if (format == "float")
		value = 2;
	else if (format == "date")
		value = 37;
	else if (format == "time")
		value = 41;
	else if (format == "datetime")
		value = 51;
	else
		return false;
	
	VariantOle vvalue;
	vvalue.Int(value);
	return Ole::SetValue(Range, "NumberFormat", vvalue);	
}
bool OPENSheet::SetColWidth(int col, int width)
{
    ObjectOle Columns;
    if (!(Columns = Ole::MethodGet(Sheet, "getColumns")))
		return false;
    ObjectOle Column;
    VariantOle vvalue;
    vvalue.Int(col);
    if (!(Column = Ole::MethodGet(Columns, "getByIndex", vvalue)))
		return false;
    vvalue.Int(width);
    return Ole::SetValue(Column, "Width", vvalue);    	
}
	
bool OPENSheet::SetRowHeight(int row, int height)
{
    ObjectOle Rows;
    if (!(Rows = Ole::MethodGet(Sheet, "getRows")))
		return false;
    ObjectOle Row;
    VariantOle vvalue;
    vvalue.Int(row);
    if (!(Row = Ole::MethodGet(Rows, "getByIndex", vvalue)))
		return false;
    vvalue.Int(height);
    return Ole::SetValue(Row, "Height", vvalue); 
}
bool OPENSheet::Print()
{
	VariantOle vArrayPrint;
	vArrayPrint.ArrayDim(1);
	VariantOle vArg;
	vArg.ObjectOle(OOo::MakePropertyValue("CopyCount", 1));	
	vArrayPrint.ArraySetVariant(1, vArg);

	return Ole::Method(Document, "print", vArrayPrint);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OPENDoc::OPENDoc()
{
	ServiceManager = CoreReflection = Desktop = Document = Text = Cursor = NULL;
	quit = false;
	
	// The service manager is always the starting point
	// If there is no office running then an office is started up
	ObjectOle ServiceManager;
	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return;

	// Create the CoreReflection service that is later used to create structs
	ObjectOle CoreReflection;
	VariantOle vInstance;
	vInstance.BString("com.sun.star.reflection.CoreReflection");	
	if (!(CoreReflection = Ole::MethodGet(ServiceManager, "createInstance", vInstance)))
		return;

	// Create the Desktop
	VariantOle vDesktop;
	vDesktop.BString("com.sun.star.frame.Desktop");	
	if (!(Desktop = Ole::MethodGet(ServiceManager, "createInstance", vDesktop)))
		return;	
}
OPENDoc::~OPENDoc()
{
	if (Cursor)
		Cursor->Release();
	if (Text)
		Text->Release();
	if (Document)
		Document->Release();
	if (Desktop) {
		Quit();
		Desktop->Release();
	}
	if (CoreReflection)
		CoreReflection->Release();
	if (ServiceManager)
		ServiceManager->Release();
}
	
bool OPENDoc::IsAvailable()
{
	ObjectOle serviceManager;
	if (!(serviceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return false;
	else {
		serviceManager->Release();
		return true;
	}
}
bool OPENDoc::AddDoc(bool visible)
{
	if (!Desktop)
		return false;
	
	VariantOle vArray;
	vArray.ArrayDim(1);
	VariantOle vobject;
	vobject.ObjectOle(OOo::MakePropertyValue("Hidden", true));
	vArray.ArraySetVariant(1, vobject);

	VariantOle vFile;
	vFile.BString("_blank");	
	VariantOle vSize;
	vSize.Int4(0);	
	VariantOle vDocument;
	vDocument.BString("private:factory/swriter");	
	if (!(Document = Ole::MethodGet(Desktop, "loadComponentFromURL", vArray, vSize, vFile, vDocument)))
		return false;
	
	if (!visible)
		SetVisible(visible);

	// Create a text object
	if (!(Text = Ole::MethodGet(Document, "getText")))
		return false;
	
	// Create a cursor object
	if (!(Cursor = Ole::MethodGet(Text, "createTextCursor")))
		return false;
	return true;
}
bool OPENDoc::OpenDoc(String fileName, bool visible)
{
	if (!Desktop)
		return false;
	
	VariantOle vArray;
	vArray.ArrayDim(1);
	VariantOle vobject;
	vobject.ObjectOle(OOo::MakePropertyValue("Hidden", true));
	vArray.ArraySetVariant(1, vobject);

	VariantOle vFile;
	vFile.BString("_blank");	
	VariantOle vSize;
	vSize.Int4(0);	
	VariantOle vDocument;
	vDocument.BString(OOo::ConvertToUrl(fileName));	
	if (!(Document = Ole::MethodGet(Desktop, "loadComponentFromURL", vArray, vSize, vFile, vDocument))) 
		return false;

	if (!visible)
		SetVisible(visible);	

	// Create a text object
	if (!(Text = Ole::MethodGet(Document, "getText")))
		return false;
	
	// Create a cursor object
	if (!(Cursor = Ole::MethodGet(Text, "createTextCursor")))
		return false;
	return true;
}
bool OPENDoc::SetVisible(bool visible)
{
	if (!Document)
		return false;
	ObjectOle DocCtrl;
	if (!(DocCtrl = Ole::MethodGet(Document, "getCurrentController")))
		return false;
	ObjectOle DocFrame;
	if (!(DocFrame = Ole::MethodGet(DocCtrl, "getFrame")))
		return false;
	ObjectOle DocWindow;
	if (!(DocWindow = Ole::MethodGet(DocFrame, "getContainerWindow")))
		return false;

	VariantOle vvisible;
	vvisible.Bool(visible);   
	return Ole::Method(DocWindow, "setVisible", vvisible); 
}
bool OPENDoc::SetColor(Color col)
{
	VariantOle vColor;
	vColor.BString("CharColor");
	VariantOle vValue;
	vValue.Int4(RGB(col.GetR(), col.GetG(), col.GetB()));
	return Ole::Method(Cursor, "setPropertyValue", vValue, vColor);
}
bool OPENDoc::SetFont(String font, int size)
{
	VariantOle vFont;
	vFont.BString("CharFontName");
	VariantOle vValue;
	vValue.BString(font);
	Ole::Method(Cursor, "setPropertyValue", vValue, vFont);
	VariantOle vHeight;
	vHeight.BString("CharHeight");
	vValue.Int(size);
	return Ole::Method(Cursor, "setPropertyValue", vValue, vHeight);
}
bool OPENDoc::SetBold(bool bold)
{
	VariantOle vvalue;
	if (bold)
		vvalue.Int(150);	
	else
		vvalue.Int(100);	
	return Ole::SetValue(Cursor, "CharWeight", vvalue);
}
bool OPENDoc::SetItalic(bool italic)
{
	VariantOle vvalue;
	if (italic)
		vvalue.Int(2);	
	else
		vvalue.Int(0);	
	return Ole::SetValue(Cursor, "CharPosture", vvalue);
}
bool OPENDoc::WriteText(String str)
{
	VariantOle vFalse;
	vFalse.Bool(false);	
	VariantOle vText;
	vText.BString(str);
	VariantOle vCursor;
	vCursor.ObjectOle(Cursor);
	return Ole::Method(Text, "insertString", vFalse, vText, vCursor);
}
	
bool OPENDoc::Select()	// Not used here
{
	return true;
}
	
bool OPENDoc::Replace(String search, String replace)
{
	ObjectOle Search;
	if (!(Search = Ole::MethodGet(Document, "createReplaceDescriptor")))
		return false;		
	VariantOle vsearch;
	vsearch.BString(search);
	Ole::Method(Search, "setSearchString", vsearch);
	VariantOle vreplace;
	vreplace.BString(replace);
	Ole::Method(Search, "setReplaceString", vreplace);
	
	VariantOle vsrep;
	vsrep.ObjectOle(Search);
	return Ole::Method(Document, "replaceAll", vsrep);
}
	
bool OPENDoc::Print()
{
	/*	Sorry. This does not run
	VariantOle vArrayPrintConf;
	vArrayPrintConf.ArrayDim(3);

	vArg.ObjectOle(OOo::MakePropertyValue("Printer", "Default"));	// Pages "1-15; 18"
	vArrayPrintConf.ArraySetVariant(1, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("PaperOrientation", "com.sun.star.view.PaperOrientation.PORTRAIT"));
	vArrayPrintConf.ArraySetVariant(2, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("PaperFormat", "com.sun.star.view.PaperFormat.A4"));
	vArrayPrintConf.ArraySetVariant(3, vArg);
	
	if (!(Ole::SetValue(Document, "SetPrinter", vArrayPrintConf)))
		return false;
	*/
	VariantOle vArrayPrint;
	vArrayPrint.ArrayDim(1);
	VariantOle vArg;
	vArg.ObjectOle(OOo::MakePropertyValue("CopyCount", 1));	
	vArrayPrint.ArraySetVariant(1, vArg);
	// It runs
	// vArg.ObjectOle(OOo::MakePropertyValue("Pages", "2"));	// Pages "1-15; 18"
	// vArrayPrint.ArraySetVariant(2, vArg);

	return Ole::Method(Document, "print", vArrayPrint);
}
	
bool OPENDoc::SetSaved(bool saved)
{
	VariantOle vval;
	vval.Bool(!saved);	// Modified is the opposite to saved
	return Ole::Method(Document, "setModified", vval);
}
bool OPENDoc::SaveAs(String fileName, String type)
{
	if (!Document)
		return false;

	fileName = ForceExt(fileName, "." + type);
	VariantOle vFileName;
	vFileName.BString(OOo::ConvertToUrl(fileName));
	
	String filter;
	if (type == "doc")
		filter = "MS Word 97";
	else if (type == "rtf")
		filter = "Rich Text Format";
	else if (type == "psw")
		filter = "Pocket Word File";
	else if (type == "html")
		filter = "HTML (StarDoc)";
	else if (type == "txt")
		filter = "Text";
	else if (type == "pdf")
		filter = "writer_pdf_Export";
	else if (type == "odt")
		filter = "";
	else
		return false;
	
	VariantOle vArraySave;
	if (filter.IsEmpty()) 
		vArraySave.ArrayDim(0);
	else {	
		VariantOle vArg;
		vArg.ObjectOle(OOo::MakePropertyValue("FilterName", filter));
		vArraySave.ArrayDim(1);
		vArraySave.ArraySetVariant(1, vArg);
	}	
	return Ole::Method(Document, "storeToURL", vArraySave, vFileName);
}
bool OPENDoc::Quit()
{
	if (!quit) {
		quit = true;
		return Ole::Method(Document, "Dispose");
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

OfficeSheet::OfficeSheet() 	
{
	data = NULL;
	Ole::Init();
}
OfficeSheet::~OfficeSheet()	
{
	if (!data)
		return;
	if (type == SheetOPEN)
		delete (OPENSheet*)data;
	else if (type == SheetMS)
		delete (MSSheet*)data;		
	
	Ole::Close();
}
void OfficeSheet::CellToColRow(const char *cell, int &col, int &row)
{
	String s_col;
	int i;
	int lenCell = strlen(cell);
	for (i = 0; (i < lenCell) && (cell[i] >= 'A') && (cell[i] <= 'Z'); ++i)
		s_col.Cat(cell[i]);
	
	row = atoi(cell + i);
	
	int num_az = 'Z' - 'A' + 1;
	col = 0;
	int len = s_col.GetCount();
	int p = (int)pow((double)num_az, len-1);
	for (int j = 0; j < s_col.GetCount(); ++j) {
		col += (s_col[j] - 'A' + 1)*p;
		p /= num_az;
	}
}
void OfficeSheet::CellToColRow(const char *cell, Cell &cellPos)
{
	CellToColRow(cell, cellPos.col, cellPos.row);
}
String OfficeSheet::ColRowToCell(const int col, const int row)
{
	String cell;
	int num_az = 'Z' - 'A' + 1;
	int firstLetter = col/num_az;
	int secondLetter = col - firstLetter*num_az;
	int len = firstLetter > 0 ? 2: 1;
		
	StringBuffer bCell(len);
	if (len == 2) {
		bCell[0] = 'A' + firstLetter - 1;
		bCell[1] = 'A' + secondLetter - 1;
	} else if (len == 1)
		bCell[0] = 'A' + secondLetter - 1;
	else
		return "Error";
	bCell.Cat(AsString(row));
	cell = bCell;
	
	return cell;
}
String OfficeSheet::ColRowToCell(const Cell &cellPos)
{
	return ColRowToCell(cellPos.col, cellPos.row);
}		
bool OfficeSheet::IsAvailable(String strtype)
{
	if (strtype == "Open")
		return OPENSheet::IsAvailable();
	else if (strtype == "Microsoft")
		return MSSheet::IsAvailable();
	else
		return false;
}
bool OfficeSheet::Init(String strtype)
{
	if (data) {
		if (type == SheetOPEN)
			delete (OPENSheet*)data;
		else if (type == SheetMS)
			delete (MSSheet*)data;	
	}
	if (strcmp(strtype, "Open") >= 0) {
		type = SheetOPEN;
		data = new OPENSheet();
	} else if (strcmp(strtype, "Microsoft") >= 0) {
		type = SheetMS;
		data = new MSSheet();
	} else
		return false;
	return true;
}
bool OfficeSheet::AddSheet(bool visible)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->AddSheet(visible);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->AddSheet(visible);
	else
		return false;
}
bool OfficeSheet::OpenSheet(String fileName, bool visible)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->OpenSheet(fileName, visible);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->OpenSheet(fileName, visible);
	else
		return false;
}
bool OfficeSheet::SetValue(int col, int row, Value value)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(col, row, value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(col, row, value);
	else
		return false;
}
bool OfficeSheet::SetValue(String cell, Value value)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(cell, value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(cell, value);
	else
		return false;
}
bool OfficeSheet::SetValue(Value value)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetValue(value);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetValue(value);
	else
		return false;
}
Value OfficeSheet::GetValue(String cell)
{
	if (!data)
		return Null;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetValue(cell);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetValue(cell);
	else
		return Null;
}
Value OfficeSheet::GetValue(int col, int row)
{
	if (!data)
		return Null;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetValue(col, row);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetValue(col, row);
	else
		return Null;
}
bool OfficeSheet::Replace(Value search, Value replace)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Replace(search, replace);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Replace(search, replace);
	else
		return false;
}
bool OfficeSheet::SetBold(int col, int row, bool bold)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(col, row, bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(col, row, bold);
	else
		return false;
}
bool OfficeSheet::SetBold(String cell, bool bold)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(cell, bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(cell, bold);
	else
		return false;
}
bool OfficeSheet::SetBold(bool bold)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetBold(bold);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetBold(bold);
	else
		return false;
}
bool OfficeSheet::SetFont(int col, int row, String name, int size)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(col, row, name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(col, row, name, size);
	else
		return false;
}
bool OfficeSheet::SetFont(String cell, String name, int size)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(cell, name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(cell, name, size);
	else
		return false;
}
bool OfficeSheet::SetFont(String name, int size)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->SetFont(name, size);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->SetFont(name, size);
	else
		return false;
}
bool OfficeSheet::Select(String range)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select(range);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select(range);
	else
		return false;
}
bool OfficeSheet::Select(int fromX, int fromY, int toX, int toY)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select(fromX, fromY, toX, toY);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select(fromX, fromY, toX, toY);
	else
		return false;
}
bool OfficeSheet::Select()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Select();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Select();
	else
		return false;
}
/*
void OfficeSheet::DefMatrix(int width, int height)
{
	if (!data)
		return;
	if (type == SheetOPEN) 
		((OPENSheet*)data)->DefMatrix(width, height);  
	else if (type == SheetMS) 
		((MSSheet*)data)->DefMatrix(width, height);
	else
		return;
}
bool OfficeSheet::FillSelectionMatrix()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->FillSelectionMatrix();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->FillSelectionMatrix();
	else
		return false;
}
void OfficeSheet::SetMatrixValue(int i, int j, ::Value value)
{
	if (!data)
		return;
	if (type == SheetOPEN) 
		((OPENSheet*)data)->SetMatrixValue(i, j, value);  
	else if (type == SheetMS) 
		((MSSheet*)data)->SetMatrixValue(i, j, value);
	else
		return;
}	
*/
bool OfficeSheet::SaveAs(String fileName, String _type)
{
	if (!data)
		return false;
	if (type == SheetOPEN) {
		int ret;
		ret = ((OPENSheet*)data)->SaveAs(fileName, _type);  
		((OPENSheet*)data)->SetSaved(true);
		return ret;
	} else if (type == SheetMS) {
		((MSSheet*)data)->SetSaved(true);
		return ((MSSheet*)data)->SaveAs(fileName, _type);
	} else
		return false;
}

bool OfficeSheet::Quit()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Quit();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Quit();
	else
		return false;
}
bool OfficeSheet::Print()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->Print();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->Print();
	else
		return false;
}
bool OfficeSheet::InsertTab(String name)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->InsertTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->InsertTab(name);
	else
		return false;
}
bool OfficeSheet::ChooseTab(String name)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->ChooseTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->ChooseTab(name);
	else
		return false;
}
bool OfficeSheet::ChooseTab(int index)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->ChooseTab(index);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->ChooseTab(index);
	else
		return false;
}	
bool OfficeSheet::RemoveTab(String name)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->RemoveTab(name);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->RemoveTab(name);
	else
		return false;
}
bool OfficeSheet::RemoveTab(int index)
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->RemoveTab(index);  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->RemoveTab(index);
	else
		return false;
}
int OfficeSheet::GetNumTabs()
{
	if (!data)
		return false;
	if (type == SheetOPEN) 
		return ((OPENSheet*)data)->GetNumTabs();  
	else if (type == SheetMS) 
		return ((MSSheet*)data)->GetNumTabs();
	else
		return false;
}	
	

/////////////////////////////////////////////////////////////////////////////////////////
OfficeDoc::OfficeDoc() 	
{
	data = NULL;
	Ole::Init();
}
OfficeDoc::~OfficeDoc()	
{
	if (!data)
		return;
	if (type == DocOPEN)
		delete (OPENDoc*)data;
	else if (type == DocMS)
		delete (MSDoc*)data;		
	
	Ole::Close();
}
bool OfficeDoc::IsAvailable(String strtype)
{
	if (strcmp(strtype, "Open") >= 0)
		return OPENDoc::IsAvailable();
	else if (strcmp(strtype, "Microsoft") >= 0)
		return MSDoc::IsAvailable();
	else
		return false;
}
bool OfficeDoc::Init(String strtype)
{
	if (data) {
		if (type == DocOPEN)
			delete (OPENDoc*)data;
		else if (type == DocMS)
			delete (MSDoc*)data;	
	}
	if (strcmp(strtype, "Open") >= 0) {
		type = DocOPEN;
		data = new OPENDoc();
	} else if (strcmp(strtype, "Microsoft") >= 0) {
		type = DocMS;
		data = new MSDoc();
	} else
		return false;
	return true;
}
bool OfficeDoc::AddDoc(bool visible)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->AddDoc(visible);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->AddDoc(visible);
	else
		return false;
}
bool OfficeDoc::OpenDoc(String fileName, bool visible)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->OpenDoc(fileName, visible);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->OpenDoc(fileName, visible);
	else
		return false;
}
bool OfficeDoc::SetFont(String font, int size)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetFont(font, size);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetFont(font, size);
	else
		return false;
}
bool OfficeDoc::SetBold(bool bold)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetBold(bold);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetBold(bold);
	else
		return false;
}
bool OfficeDoc::SetItalic(bool italic)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetItalic(italic);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetItalic(italic);
	else
		return false;
}
bool OfficeDoc::WriteText(String value)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->WriteText(value);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->WriteText(value);
	else
		return false;
}
bool OfficeDoc::Select()
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Select();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Select();
	else
		return false;
}
bool OfficeDoc::Replace(String search, String replace)
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Replace(search, replace);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Replace(search, replace);
	else
		return false;
}
bool OfficeDoc::Print()
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Print();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Print();
	else
		return false;
}
bool OfficeDoc::SaveAs(String fileName, String _type)
{
	if (!data)
		return false;
	if (type == DocOPEN) {
		int ret;
		ret = ((OPENDoc*)data)->SaveAs(fileName, _type);  
		((OPENDoc*)data)->SetSaved(true);
		return ret;
	} else if (type == DocMS) {
		((MSDoc*)data)->SetSaved(true);
		return ((MSDoc*)data)->SaveAs(fileName, _type);
	} else
		return false;
}

bool OfficeDoc::Quit()
{
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Quit();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Quit();
	else
		return false;
}




	
	
