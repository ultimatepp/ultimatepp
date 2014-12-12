#include <Core/Core.h>

using namespace Upp;

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include <winnls.h>

#include <Functions4U/Functions4U.h>
#include <SysInfo/SysInfo.h>
#include "OfficeAutomation.h"
#include "OfficeAutomationBase.h"

////////////////////////////////////////////////////////////////////////////////////////
VariantOle::VariantOle() {
    VariantInit(&var);
    VariantClear(&var); 
}    

VariantOle::~VariantOle() {
    if (var.vt == VT_BSTR)
        SysFreeString(var.bstrVal);
    else if ((var.vt == (VT_ARRAY | VT_VARIANT)) && (var.parray != 0))
        SafeArrayDestroy(var.parray);
}

void VariantOle::Bool(bool val) {
    var.vt = VT_BOOL;
    var.boolVal = val;
}

void VariantOle::Int(int val) {
    var.vt = VT_I4;
    var.intVal = val;
}

void VariantOle::Int4(long val) {
    var.vt = VT_I4;
    var.lVal = val;
}

void VariantOle::Real4(float val) {
    var.vt = VT_R4;
    var.fltVal = val;
}

void VariantOle::Real8(double val) {
    var.vt = VT_R8;
    var.dblVal = val;
}

void VariantOle::BString(String str) {
	WCHAR wfileName[1024*sizeof(WCHAR)];

	MultiByteToWideChar(CP_UTF8, 0, str, -1, wfileName, sizeof(wfileName)/sizeof(wfileName[0]));
	var.vt = VT_BSTR;
	var.bstrVal = ::SysAllocString(wfileName);
}

void VariantOle::Optional() {
    var.vt = VT_ERROR; 
    var.scode = DISP_E_PARAMNOTFOUND;	
}

void VariantOle::ObjectOle(::ObjectOle val) {
	var.vt = VT_DISPATCH;
    var.pdispVal = val;
}

void VariantOle::Time(Upp::Time t) {
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

bool VariantOle::ArrayDim(int sizeX) {
    var.vt = VT_ARRAY | VT_VARIANT;
    SAFEARRAYBOUND sab[1];		// 1 dimension
	sab[0].lLbound = 1; sab[0].cElements = sizeX;
	var.parray = SafeArrayCreate(VT_VARIANT, 1, sab);
	return var.parray != 0;
}

bool VariantOle::ArrayDim(int sizeX, int sizeY) {
    var.vt = VT_ARRAY | VT_VARIANT;
    SAFEARRAYBOUND sab[2];		// 2 dimension
	sab[0].lLbound = 1; sab[0].cElements = sizeY;
	sab[1].lLbound = 1; sab[1].cElements = sizeX;
	var.parray = SafeArrayCreate(VT_VARIANT, 2, sab);
	return var.parray != 0;
}

bool VariantOle::ArrayDestroy() { 
	bool ret = SafeArrayDestroy(var.parray) == S_OK;
	var.parray = 0;
	return ret;
}

bool VariantOle::ArraySetValue(int x, ::Value value) {
	VariantOle tmp;
	tmp.Value(value);
	long indices[] = {x};
	return S_OK == SafeArrayPutElement(var.parray, indices, (void *)&tmp);
}

bool VariantOle::ArraySetVariant(int x, VariantOle &value) {
	long indices[] = {x};
	return S_OK == SafeArrayPutElement(var.parray, indices, (void *)&value);
}

bool VariantOle::ArraySetValue(int x, int y, ::Value value) {
	VariantOle tmp;
	tmp.Value(value);
	long indices[] = {y, x};
	return S_OK == SafeArrayPutElement(var.parray, indices, (void *)&tmp);
}

bool VariantOle::ArraySetVariant(int x, int y, VariantOle &value) {
	long indices[] = {y, x};
	return S_OK == SafeArrayPutElement(var.parray, indices, (void *)&value);
}

void VariantOle::Value(::Value value) {
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


int Ole::numOleInit = 0;

void Ole::Init() {	// Initialize COM for this thread
	if (numOleInit == 0) {
		OleInitialize(NULL);
		CoInitialize(NULL);
		numOleInit++;
	}
}

bool Ole::Close() {	// Uninitialize COM for this thread
	numOleInit--;
	if (numOleInit == 0) 
		CoUninitialize();
	if (numOleInit < 0)	{	// This is a mistake
		numOleInit = 0;
		return false;
	} else
		return true;
}

// Automation helper function
bool Ole::Invoke(int autoType, VARIANT *pvResult, IDispatch *pDisp, String name, int cArgs, ...) {
    // Begin variable-argument list
    va_list marker;
    va_start(marker, cArgs);

    if(!pDisp) {
        LOG("Ole::Invoke() error. ObjectOle included is null");
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
        sprintf(buf, "Ole::Invoke(\"%s\") not found for object or problem when running it", szName);
        LOG(buf);
        return false;
    }
    VARIANT *pArgs = new VARIANT[cArgs+1];		// Allocate memory for arguments
    for(int i = 0; i < cArgs; i++) 				// Extract arguments
        pArgs[i] = va_arg(marker, VARIANT);
    
    dp.cArgs = cArgs;							// Build DISPPARAMS
    dp.rgvarg = pArgs;

    if(autoType & DISPATCH_PROPERTYPUT) {		// Handle special-case for property-puts!
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }
    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);

    va_end(marker);								// End variable-argument section

    delete [] pArgs;

    if(FAILED(hr)) {
        sprintf(buf, "Ole::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
		LOG(buf);
        return false;
    }
    return true;
}

ObjectOle Ole::CreateObject(String application) {
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(application.ToWString(), &clsid);	// Get CLSID for our server
	if(FAILED(hr)) {
		LOG("CLSIDFromProgID() failed");
		return NULL;
	}
	ObjectOle app;
	// Start server and get IDispatch
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&app);
	if(FAILED(hr)) {
		LOG("OfficeAutomation internal error. Application not registered properly");
		return NULL;
	}
	return app;
}

ObjectOle Ole::GetObject(ObjectOle from, String which) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 0))
		return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::GetObject(ObjectOle from, String which, VariantOle &value) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 1, value.var))
		return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::GetObject(ObjectOle from, String which, VariantOle &value, VariantOle &value2) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_PROPERTYGET, &result, from, which, 2, value.var, value2.var))
		return NULL;
	else
		return result.pdispVal;
}

Value Ole::GetValue(ObjectOle from, String which) {
	VARIANT result;
	VariantInit(&result);	
	if(!Ole::Invoke(DISPATCH_PROPERTYGET|DISPATCH_METHOD, &result, from, which, 0))
		return "";
	
	return GetVARIANT(result);
}

bool Ole::SetValue(ObjectOle from, String which, VariantOle &value) {
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 1, value.var);
}

bool Ole::SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2) {
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 2, value.var, value2.var);
}

bool Ole::SetValue(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3) {
	return Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, from, which, 3, value.var, value2.var, value3.var);
}

// result var is not used but OpenOffice automation requires it (I do not why)
bool Ole::Method(ObjectOle from, String which) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 0);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 1, value.var);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 2, value.var, value2.var);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var, value4.var);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var, value4.var, value5.var);
}

bool Ole::Method(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5, VariantOle &value6, VariantOle &value7, VariantOle &value8, VariantOle &value9, VariantOle &value10, VariantOle &value11) {
	VARIANT result;
	VariantInit(&result);
	return Ole::Invoke(DISPATCH_METHOD, &result, from, which, 11, value.var, value2.var, value3.var, value4.var, value5.var, value6.var, value7.var, value8.var, value9.var, value10.var, value11.var);
}

ObjectOle Ole::MethodGet(ObjectOle from, String which) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 0))
	   	return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 1, value.var))
	   	return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 2, value.var, value2.var))
	   	return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 3, value.var, value2.var, value3.var))
	   	return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 4, value.var, value2.var, value3.var, value4.var))
	   	return NULL;
	else
		return result.pdispVal;
}

ObjectOle Ole::MethodGet(ObjectOle from, String which, VariantOle &value, VariantOle &value2, VariantOle &value3, VariantOle &value4, VariantOle &value5) {
	VARIANT result;
	VariantInit(&result);
	if(!Ole::Invoke(DISPATCH_METHOD, &result, from, which, 4, value.var, value2.var, value3.var, value4.var, value5.var))
	   	return NULL;
	else
		return result.pdispVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INITBLOCK {
	PluginRegister(OfficeSheet, MSSheet, "Microsoft");
}

MSSheet::MSSheet() {
	App = Books = Book = Sheet = Range = NULL;
	quit = false;

	App = Ole::CreateObject("Excel.Application");
	if (!(Books = Ole::GetObject(App, "Workbooks"))) {
		LOG("Excel workbooks not loaded properly");
		return;
	}
	killProcess = false;
}

MSSheet::~MSSheet() {
	if (Range) {
		Range->Release();
		Range = 0;
	}
	if (Sheet) {
		Sheet->Release();
		Sheet = 0;
	}
	if (Book) {
		Book->Release();
		Book = 0;
	}
	if (Books) {
		Books->Release();
		Books = 0;
	}
	if (App) {
		HRESULT res;
		DWORD pid;
		if (killProcess)
			res = CoGetServerPID(App, &pid);
		Quit();
		App->Release();
		App = 0;
		if (killProcess && !FAILED(res)) 
			ProcessTerminate(pid);
	}
}

bool MSSheet::IsAvailable() {
	ObjectOle app = Ole::CreateObject("Excel.Application");
	
	if (!app) 
		return false;
	else {
		Ole::Method(app, "Quit");
		app->Release();
		return true;
	}
}

bool MSSheet::SetVisible(bool visible) {
	if (!Books)
		return false;

	VariantOle vVisible;
	vVisible.Int4(visible? 1: 0);
	return Ole::SetValue(App, "Visible", vVisible);
}

bool MSSheet::SetSaved(bool saved) {
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

bool MSSheet::AddSheet(bool visible) {
	killProcess = true;
	if (!Books)
		return false;
	
	if (!(Book = Ole::GetObject(Books, "Add")))
		return false;
	if (!(Sheet = Ole::GetObject(App, "ActiveSheet")))
		return false;

	SetVisible(visible);
	return true;
}

bool MSSheet::OpenSheet(String fileName, bool visible) {
	killProcess = true;
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

bool MSSheet::Select(String range) {
	VariantOle vRange;
	vRange.BString(range);
	
	if (!(Range = Ole::GetObject(Sheet, "Range", vRange)))
		return false;	
	return true;
}

bool MSSheet::Select(int fromX, int fromY, int toX, int toY) {
	String range = OfficeSheet::ColRowToCell(fromX, fromY) + ":" + OfficeSheet::ColRowToCell(toX, toY);
	VariantOle vRange;
	vRange.BString(range);
	
	if (!(Range = Ole::GetObject(Sheet, "Range", vRange)))
		return false;	
	return true;
}

bool MSSheet::Select() {
	if (!(Range = Ole::GetObject(Sheet, "Cells")))		// ActiveSheet.Cells.Select
		return false;
	return Ole::Method(Range, "Select");
}

bool MSSheet::EnableCommandVars(bool enable) {
	ObjectOle commandBars;
	if(!(commandBars = Ole::GetObject(App, "CommandBars")))
		return false;	

	VariantOle vEnabled;
	vEnabled.Int4(enable? 1: 0);
	return Ole::SetValue(App, "Enabled", vEnabled);
}

bool MSSheet::MatrixAllocate(int width, int height) {
	return Matrix.ArrayDim(width, height);
}

bool MSSheet::MatrixDelete() {
	return Matrix.ArrayDestroy();
}

bool MSSheet::MatrixSetValue(int x, int y, ::Value value) {
	return Matrix.ArraySetValue(x, y, value);
}

bool MSSheet::MatrixFillSelection() {
	if (!Range)
		return false;

	bool ret = Ole::Invoke(DISPATCH_PROPERTYPUT, NULL, Range, "Value", 1, VARIANT(Matrix.var));

	return ret;
}

bool MSSheet::MatrixFill(int fromX, int fromY, Vector<Vector<Value> > &data) {
	if (data.IsEmpty())
		return false;
	int height = data.GetCount();
	int width = data[0].GetCount();
	int toX = fromX + width - 1;	
	int toY = fromY + height - 1;
	if (!Select(fromX, fromY, toX, toY)) return false;
	if (!MatrixAllocate(width, height)) return false;
	try {
		for (int row = 0; row < height; ++row)
			for (int col = 0; col < width; ++col)
				if (!MatrixSetValue(col + 1, row + 1, data[row][col]))  throw;
		if (!MatrixFillSelection()) throw;
	} catch (...) {
		MatrixDelete();
		return false;
	}
	return MatrixDelete();
}

// cell in textual format like "B14" 
bool MSSheet::SetValue(String cell, Value value) {
	int col, row;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return MSSheet::SetValue(col, row, value);
}

bool MSSheet::SetValue(int col, int row, Value value) {
	if (!Sheet)
		return false;

	VariantOle x, y, val;
	x.Int4(col);
	y.Int4(row);
	val.Value(value);
		
	return Ole::SetValue(Sheet, "Cells", val, x, y);
}

bool MSSheet::SetValue(Value value) {
	if (!Sheet)
		return false;

	if (!Range)
		return false;

	VariantOle val;
	val.Value(value);
	
	return Ole::SetValue(Range, "Value", val);
}

bool MSSheet::Replace(Value search, Value replace) {
	if (!App)
		return false;

	ObjectOle selection;								// Call Application.Selection.Replace(search, replace)
	if (!(selection = Ole::GetObject(App, "Selection")))	
		return false;	
	
	VariantOle vSearch, vReplace;
	vSearch.Value(search);
	vReplace.Value(replace);
		
	bool ret = Ole::Method(selection, "Replace", vReplace, vSearch);
	selection->Release();
	return ret;
}

Value MSSheet::GetValue(int col, int row) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	return Ole::GetValue(Ole::GetObject(Sheet, "Cells", x, y), "value");
}

Value MSSheet::GetText(int col, int row) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	return Ole::GetValue(Ole::GetObject(Sheet, "Cells", x, y), "text");
}

Value MSSheet::GetValue(String cell) {
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetValue(col, row);
}

Value MSSheet::GetText(String cell) {
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetText(col, row);
}

// cell in textual format like "B14" 
bool MSSheet::SetHyperlink(String cell, String address, String value) {
	int col, row;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return SetHyperlink(col, row, address, value);
}

bool MSSheet::SetHyperlink(int col, int row, String address, String value) {
	if (!Sheet)
		return false;

	if (!Select(col, row, col, row))
	    return false;
	
	return SetHyperlink(address, value);
}

bool MSSheet::SetHyperlink(String address, String value) {
	if (!Sheet)
		return false;

	if (!Range)
		return false;

	ObjectOle hyperlinks = Ole::GetObject(Sheet, "Hyperlinks");
	
	VariantOle textToDisplay, screenTip, subAddress, saddress, range;
	textToDisplay.BString(value);
	screenTip.BString("");
	subAddress.BString("");
	saddress.BString(address);
	range.ObjectOle(Range);
	
	bool ret = Ole::MethodGet(hyperlinks, "Add", textToDisplay, screenTip, subAddress, saddress, range);
	hyperlinks->Release();
	return ret;
}

bool MSSheet::SaveAs(String fileName, String type) {
	if (!Book)
		return false;

	if (type[0] == '.')
		type = type.Mid(1);
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

bool MSSheet::SetBold(String cell, bool bold) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, bold);
}

bool MSSheet::SetBold(int col, int row, bool bold) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle val;
	val.Bool(bold);	
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font"), "Bold", val);
}

bool MSSheet::SetBold(bool bold) {
	if (!Range)
		return false;

	VariantOle val;
	val.Bool(bold);	
	return Ole::SetValue(Ole::GetObject(Range, "Font"), "Bold", val);
}

bool MSSheet::SetItalic(String cell, bool italic) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetItalic(col, row, italic);
}

bool MSSheet::SetItalic(int col, int row, bool italic) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle val;
	val.Bool(italic);	
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font"), "Italic", val);
}

bool MSSheet::SetItalic(bool italic) {
	if (!Range)
		return false;

	VariantOle val;
	val.Bool(italic);	
	return Ole::SetValue(Ole::GetObject(Range, "Font"), "Italic", val);
}

bool MSSheet::SetUnderline(String cell, bool underline) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetUnderline(col, row, underline);
}

bool MSSheet::SetUnderline(int col, int row, bool underline) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle val;
	val.Bool(underline);	
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font"), "Underline", val);
}

bool MSSheet::SetUnderline(bool underline) {
	if (!Range)
		return false;

	VariantOle val;
	val.Bool(underline);	
	return Ole::SetValue(Ole::GetObject(Range, "Font"), "Underline", val);
}

bool MSSheet::SetHorizAlignment(String cell, int alignment) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetHorizAlignment(col, row, alignment);
}

bool MSSheet::SetHorizAlignment(int col, int row, int alignment) {
	if (!Sheet)
		return false;
	if (alignment < 1 || alignment >= OfficeSheet::MAX_JUSTIFY)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	int justifyConst[] = {-4131, -4108, -4152, -4130, -4160, -4107};
		
	VariantOle cvalue;
	cvalue.Int(justifyConst[alignment]);
	return Ole::SetValue(Ole::GetObject(Sheet, "Cells", x, y), "HorizontalAlignment", cvalue);
}

bool MSSheet::SetVertAlignment(String cell, int alignment) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetVertAlignment(col, row, alignment);
}

bool MSSheet::SetVertAlignment(int col, int row, int alignment) {
	if (!Sheet)
		return false;
	if (alignment < 1 || alignment >= OfficeSheet::MAX_JUSTIFY)
		return false;
	
	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	int justifyConst[] = {-4131, -4108, -4152, -4130, -4160, -4107};

	VariantOle cvalue;
	cvalue.Int(justifyConst[alignment]);
	return Ole::SetValue(Ole::GetObject(Sheet, "Cells", x, y), "VerticalAlignment", cvalue);
}

bool MSSheet::SetBorder(int borderIndx, int lineStyle, int weight, Color color) {
	if (!Range)
		return false;

	ObjectOle borders = Ole::GetObject(Range, "Borders");
	if (!borders)
		return false;	
	
	bool ret = SetBorder(borders, borderIndx, lineStyle, weight, color);
	borders->Release();
	return ret;
}

bool MSSheet::SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	ObjectOle borders = Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Borders");
	if (!borders)
		return false;

	bool ret = SetBorder(borders, borderIndx, lineStyle, weight, color);
	borders->Release();
	return ret;
}

bool MSSheet::SetBorder(ObjectOle &borders, int borderIndx, int lineStyle, int weight, Color color) {
	int borderIndxConst[] = {5, 6, 7, 8, 9, 10};
	VariantOle item;
	item.Int4(borderIndxConst[borderIndx]);
	ObjectOle border = Ole::GetObject(borders, "Item", item);
	if (!border)
		return false;
	
	int lineStyles[] = {-4142, 1, -4115, 4, -4118};	
	VariantOle val;
	val.Int4(lineStyles[lineStyle]);	
	if(!Ole::SetValue(border, "LineStyle", val))
		return false;

	int weights[] = {1, -4138, 2, 4};	
	val.Int4(weights[weight]);	
	if(!Ole::SetValue(border, "Weight", val))
		return false;	
	
	VariantOle cvalue;
	cvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(border, "Color", cvalue);
}

bool MSSheet::SetFont(String cell, String name, int size)
{
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetFont(col, row, name, size);
}

bool MSSheet::SetFont(int col, int row, String name, int size) {
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
	bool ret = Ole::SetValue(font, "Size", vSize);
	font->Release();
	return ret;
}

bool MSSheet::SetFont(String name, int size) {
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
	bool ret = Ole::SetValue(font, "Size", vSize);
	font->Release();
	return ret;
}

bool MSSheet::Print() {
	if (!Sheet)
		return false;
	
	bool ret = Ole::Method(Sheet, "PrintOut");
	
	return ret;
}

enum XLSheetTypes {xlWorksheet = -4167, xlChart = -4109, xlExcel4MacroSheet = 3, xlExcel4IntlMacroSheet = 4};

bool MSSheet::InsertTab(String name) {	// Insert tab after the last
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
	
	bool ret =  Ole::Method(Sheet, "Move", vAfter, vOptional);	// Apending tab after last tab
	lastSheet->Release();
	return ret;
}

bool MSSheet::SetColor(String cell, Color color) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetColor(col, row, color);
}

bool MSSheet::SetColor(int col, int row, Color color) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle cvalue;
	cvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Font"), "Color", cvalue);
}

bool MSSheet::SetColor(Color color) {
	if (!Range)
		return false;

	VariantOle cvalue;
	cvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Ole::GetObject(Range, "Font"), "Color", cvalue);
}

double MmToPointsWidth(double mm) {
	return 0.51155*mm - 0.7502;
}

double MmToPointsHeight(double mm) {
	return 2.83018*mm;
}
        
bool MSSheet::SetRowHeight(int row, double height) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(1);
	y.Int4(row);
	
	VariantOle cvalue;
	cvalue.Int(int(MmToPointsHeight(height)));
	return Ole::SetValue(Ole::GetObject(Sheet, "Cells", x, y), "RowHeight", cvalue);
}

bool MSSheet::SetColWidth(int col, double width) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(1);
	
	VariantOle cvalue;
	cvalue.Int(int(MmToPointsWidth(width)));
	return Ole::SetValue(Ole::GetObject(Sheet, "Cells", x, y), "ColumnWidth", cvalue);
}

bool MSSheet::SetBackColor(String cell, Color color) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetColor(col, row, color);
}

bool MSSheet::SetBackColor(int col, int row, Color color) {
	if (!Sheet)
		return false;

	VariantOle x, y;
	x.Int4(col);
	y.Int4(row);
	
	VariantOle cvalue;
	cvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Ole::GetObject(Ole::GetObject(Sheet, "Cells", x, y), "Interior"), "Color", cvalue);
}

bool MSSheet::SetBackColor(Color color) {
	if (!Range)
		return false;

	VariantOle cvalue;
	cvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Ole::GetObject(Range, "Interior"), "Color", cvalue);
}

bool MSSheet::ChooseTab(String name) {
	if (!App)
		return false;

    VariantOle vName;
	vName.BString(name);
	if (!(Sheet = Ole::GetObject(App, "Sheets", vName)))
		return false;
	
	return Ole::Method(Sheet, "select");	
}

bool MSSheet::ChooseTab(int index) {
	if (!App)
		return false;	
	
    VariantOle vId;
	vId.Int(index+1);	
	if (!(Sheet = Ole::GetObject(App, "Sheets", vId)))
		return false;
	
	return Ole::Method(Sheet, "select");
}

bool MSSheet::RemoveTab(String name) {
	if (!App)
		return false;

	ObjectOle DelSheet;
    VariantOle vName;
	vName.BString(name);
	if (!(DelSheet = Ole::GetObject(App, "Sheets", vName)))
		return false;
	
	bool ret = Ole::Method(DelSheet, "delete");	
	DelSheet->Release();
	return ret;
}

bool MSSheet::RemoveTab(int index) {
	if (!App)
		return false;	
	
	ObjectOle DelSheet;
    VariantOle vId;
	vId.Int(index+1);	
	if (!(DelSheet = Ole::GetObject(App, "Sheets", vId)))
		return false;
	
	bool ret = Ole::Method(DelSheet, "delete");
	DelSheet->Release();
	return ret;
}

int MSSheet::GetNumTabs() {
	if (!App)
		return -1;

	return Ole::GetValue(Ole::GetObject(App, "Sheets"), "count");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INITBLOCK {
	PluginRegister(OfficeDoc, MSDoc, "Microsoft");
}

MSDoc::MSDoc() {
	App = Docs = Doc = Selection = NULL;
	quit = false;
	
	App = Ole::CreateObject("Word.Application");
	if (!(Docs = Ole::GetObject(App, "Documents"))) {
		LOG("Word document not loaded properly");
		return;
	}
}

MSDoc::~MSDoc() {
	if (Selection)
		Selection->Release();
	if (Doc) {
		Close();
		Doc->Release();
	}
	if (Docs) 
		Docs->Release();
	if (App) {
		Quit();
		App->Release();
	}
	Ole::Close();
}

bool MSDoc::SetVisible(bool visible) {
	if (!Docs)
		return false;

	VariantOle vVisible;
	vVisible.Int4(visible? 1: 0);
	return Ole::SetValue(App, "Visible", vVisible);
}

bool MSDoc::IsAvailable() {
	ObjectOle app = Ole::CreateObject("Word.Application");
	
	if (!app) 
		return false;
	else {
		Ole::Method(app, "Quit");
		app->Release();
		return true;
	}
}

bool MSDoc::SetSaved(bool saved) {
	if (!Doc)
		return false;

	VariantOle vSaved;
	vSaved.Int4(saved? 1: 0);
	return Ole::SetValue(Doc, "Saved", vSaved);
}

bool MSDoc::Close() {
	return Ole::Method(Doc, "Close"); 
}

bool MSDoc::Quit() {
	if (!quit) {
		quit = true;
		return Ole::Method(App, "Quit");
	}
	return true;
}

bool MSDoc::AddDoc(bool visible) {
	if (!Docs)
		return false;
	
	if (!(Doc = Ole::MethodGet(Docs, "Add")))
		return false;

	if(!(Selection = Ole::GetObject(App, "Selection")))
		return false;
	
	SetVisible(visible);
	return true;
}

bool MSDoc::OpenDoc(String fileName, bool visible) {
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

bool MSDoc::EnableCommandVars(bool enable) {
	ObjectOle commandBars;
	if(!(commandBars = Ole::GetObject(App, "CommandBars")))
		return false;	

	VariantOle vEnabled;
	vEnabled.Int4(enable? 1: 0);
	return Ole::SetValue(App, "Enabled", vEnabled);
}


bool MSDoc::SaveAs(String fileName, String type) {
	if (!Doc)
		return false;

	if (type[0] == '.')
		type = type.Mid(1);
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

bool MSDoc::WriteText(String value) {
	if (!Selection)
		return false;
	
	VariantOle val;
	val.BString(value);	
	return Ole::Method(Selection, "TypeText", val);
}

bool MSDoc::SetFont(String type, int size) {
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

bool MSDoc::SetBold(bool bold) {
	VariantOle vBold;
	vBold.Bool(bold);	
	if(!Ole::SetValue(Ole::GetObject(Selection, "Font"), "Bold", vBold))
		return false;
	
	return true;	
}

bool MSDoc::SetItalic(bool italic) {
	VariantOle vItalic;
	vItalic.Bool(italic);	
	if(!Ole::SetValue(Ole::GetObject(Selection, "Font"), "Italic", vItalic))
		return false;
	
	return true;	
}

bool MSDoc::Print() {
	if (!Docs && !Doc)
		return false;
	
	bool ret = Ole::Method(Doc, "PrintOut");
	
	return ret;
}

bool MSDoc::Select() {
	return Ole::Method(Selection, "WholeStory");		
}

// Some tricks as Ole Replace does not work properly for texts to replace longer than 
// about 250 chars and for some special chars
bool MSDoc::Replace(String search, String _replace) {
	String replace = CleanString(_replace);
    replace.Replace("^", " ");
    replace.Replace("\r", "");  // To remove squares
    
    String replaceSubset;
    while (replace.GetCount() > 200) {
        replaceSubset = replace.Left(200);
        replaceSubset.Cat(search);
        replace = replace.Right(replace.GetCount() - 200);
        
        replaceSubset.Replace("\n", "^l");  
        if (!ReplaceSubset(search, replaceSubset))
        	return false;
    }
    replace.Replace("\n", "^l");  
    return ReplaceSubset(search, replace);
}

String MSDoc::CleanString(String str) {	// Clean chars in String
    String ret;
    String valid = ">=<ºª%€&()$1234567890áéíóúÁÉÍÓÚñÑçÇ,.,:-_/¿?+*[]{}'\"!¡ \r\n";
    for (int i = 0; i < str.GetCount(); ++i) {
        int s = str[i];        
        if (IsLetter(s) || valid.Find(s) >= 0)
            ret.Cat(s);
    }
    return ret;
}

bool MSDoc::ReplaceSubset(String search, String replace) {
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
class OOo {
public:
	static ObjectOle MakePropertyValue(String name, ::Value value) {
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
	static ObjectOle MakePropertyValue(String name, VariantOle &vvalue) {
    	ObjectOle ServiceManager;
    	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
    		return NULL;
    	
		ObjectOle Struct;
		VariantOle vstr;
		vstr.BString("com.sun.star.beans.PropertyValue");
		if (!(Struct = Ole::MethodGet(ServiceManager, "Bridge_GetStruct", vstr)))
    		return NULL;
		
		VariantOle vname/*, vvalue*/;
		vname.BString(name);
		if (!Ole::SetValue(Struct, "Name", vname))
			return NULL;
		//vvalue.Value(value);
		if (!Ole::SetValue(Struct, "Value", vvalue))
			return NULL;
		return Struct;		
	}	
	static String ConvertToUrl(String strFile) {
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
	
	static ObjectOle CreateUnoService(String strServiceName) {
    	ObjectOle ServiceManager;
    	if (!(ServiceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
    		return NULL;
    	
    	VariantOle vService;
    	vService.BString(strServiceName);
    	return Ole::MethodGet(ServiceManager, "createInstance", vService);    	
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INITBLOCK {
	PluginRegister(OfficeSheet, OPENSheet, "Open");
}

OPENSheet::OPENSheet() {
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

OPENSheet::~OPENSheet() {
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

bool OPENSheet::IsAvailable() {
	ObjectOle serviceManager;
	if (!(serviceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return false;
	else {
		serviceManager->Release();
		return true;
	}
}

bool OPENSheet::SetSaved(bool saved) {
	VariantOle vval;
	vval.Bool(!saved);	// Modified is the opposite to saved
	return Ole::Method(Document, "setModified", vval);
}

bool OPENSheet::Quit() {
	if (!quit) {
		quit = true;
		if (Desktop) {
			//Ole::Method(Desktop, "Dispose");				Commented 12/2014
			return Ole::Method(Desktop, "Terminate");
		}
	}
	return true;
}

bool OPENSheet::InsertTab(String name) {
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

bool OPENSheet::ChooseTab(String name) {
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

bool OPENSheet::ChooseTab(int index) {
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

bool OPENSheet::RemoveTab(String name) {
	if (!Sheets)
		return false;
	VariantOle vStr;
	vStr.BString(name);
	if (!Ole::Method(Sheets, "removeByName", vStr))
		return false;
	return true;
}

bool OPENSheet::RemoveTab(int index) {
	if (!Sheets)
		return false;
	VariantOle vId;
	vId.Int(index);
	if (!Ole::Method(Sheets, "removeByIndex", vId))
		return false;	
	return true;
}

int OPENSheet::GetNumTabs() {
	if (!Sheets)
		return -1;
	Value count;
	count = Ole::GetValue(Sheets, "getCount");
	return count; 
}

bool OPENSheet::AddSheet(bool visible) {
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
	return ChooseTab(0);	// Select the first tab
}

bool OPENSheet::OpenSheet(String fileName, bool visible) {
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
bool OPENSheet::SetVisible(bool visible) {
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

bool OPENSheet::Select(String range) {
	if (!Sheet)
		return false;
	VariantOle vrange;
	vrange.BString(range);	
	if (!(Range = Ole::MethodGet(Sheet, "getCellRangeByName", vrange)))
		return false;
	return true;
}

bool OPENSheet::Select(int fromX, int fromY, int toX, int toY) {
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

bool OPENSheet::Select() {
	if (!Sheet)
		return false;
	selectedAll = true;
	return true;
}

bool OPENSheet::MatrixAllocate(int width, int height) {
	return false;
}

bool OPENSheet::MatrixDelete() {
	return false;
}

bool OPENSheet::MatrixSetValue(int x, int y, ::Value value) {
	return false;
}

bool OPENSheet::MatrixFillSelection() {
	return false;
}

bool OPENSheet::MatrixFill(int fromX, int fromY, Vector<Vector<Value> > &data) {
	if (data.IsEmpty())
		return false;
	int height = data.GetCount();
	int width = data[0].GetCount();
	for (int row = 0; row < height; ++row)
		for (int col = 0; col < width; ++col)
			SetValue(col + fromX, row + fromY, data[row][col]);	
	return true;
}

bool OPENSheet::Replace(Value search, Value replace) {
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

Value OPENSheet::GetValue(int col, int row) {
	if (!SelCell(col, row))
		return false;
	return Ole::GetValue(Cell, "getFormula");	// Also valid getValue and getString
}

Value OPENSheet::GetText(int col, int row) {
	if (!SelCell(col, row))
		return false;
	return Ole::GetValue(Cell, "getString");	// Also valid getValue and getString
}

Value OPENSheet::GetValue(String cell) {
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetValue(col, row);
}

Value OPENSheet::GetText(String cell) {
	int row, col;
	
	OfficeSheet::CellToColRow(cell, col, row);
	return GetText(col, row);
}

bool OPENSheet::SaveAs(String fileName, String type) {
	if (!Document)
		return false;

	if (type[0] == '.')
		type = type.Mid(1);
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
	bool ret;
	if (filter.IsEmpty()) {
		vArraySave.ArrayDim(0);
		ret = Ole::Method(Document, "storeToURL", vArraySave, vFileName);
	} else {	
		VariantOle vArg;
		vArg.ObjectOle(OOo::MakePropertyValue("FilterName", filter));
		vArraySave.ArrayDim(1);
		vArraySave.ArraySetVariant(1, vArg);
		ret = Ole::Method(Document, "storeToURL", vArraySave, vFileName);	
	}
	if (ret)
		SetSaved(true);
	return ret;
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
	
bool OPENSheet::SetValue(String cell, Value value) {	// cell in textual format like "B14" 
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetValue(col, row, value);
}

bool OPENSheet::SetValue(int col, int row, Value value) {
	if (!SelCell(col, row))
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Cell, "setFormula", vText);		
	else
		return Ole::Method(Cell, "setValue", vText);		
}

bool OPENSheet::SetValue(Value value) {
	if(!Range)
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Range, "setFormula", vText);		
	else
		return Ole::Method(Range, "setValue", vText);		
}

bool OPENSheet::SetHyperlink(String cell, String address, String text) {	// cell in textual format like "B14" 
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetHyperlink(col, row, address, text);
}

bool OPENSheet::SetHyperlink(int col, int row, String address, String text) {
/*	if (!SelCell(col, row))
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Cell, "setFormula", vText);		
	else
		return Ole::Method(Cell, "setValue", vText);		
*/
	return false;
}

bool OPENSheet::SetHyperlink(String address, String text) {
/*	if(!Range)
		return false;
	VariantOle vText;
	vText.Value(value); 
	if (value.Is<String>() || value.Is<Date>() || value.Is<Time>())
		return Ole::Method(Range, "setFormula", vText);		
	else
		return Ole::Method(Range, "setValue", vText);		
*/
	return false;
}

bool OPENSheet::EnableCommandVars(bool enable) {
/**/	
	return false;
}
	
bool OPENSheet::SetItalic(String cell, bool italic) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetItalic(col, row, italic);
}

bool OPENSheet::SetItalic(int col, int row, bool italic) {
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

bool OPENSheet::SetItalic(bool italic) {
	VariantOle vvalue;
	if (italic)
		vvalue.Int(2);	
	else
		vvalue.Int(0);	
	return Ole::SetValue(Range, "CharPosture", vvalue);
}

bool OPENSheet::SetBold(String cell, bool bold) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, bold);
}

bool OPENSheet::SetBold(int col, int row, bool bold) {
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharBold");
	vvalue.Bool(bold);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}

bool OPENSheet::SetBold(bool bold)  {
	VariantOle vvalue;
	if (bold)
		vvalue.Int(150);	
	else
		vvalue.Int(100);	
	return Ole::SetValue(Range, "CharWeight", vvalue);
}

bool OPENSheet::SetUnderline(String cell, bool underline) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBold(col, row, underline);
}

bool OPENSheet::SetUnderline(int col, int row, bool underline) {
	if (!SelCell(col, row))
		return false;
	VariantOle vproperty, vvalue;
	vproperty.BString("CharUnderline");
	vvalue.Bool(underline);	
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}

bool OPENSheet::SetUnderline(bool underline) {
	VariantOle vvalue;
	if (underline)
		vvalue.Int(1);	// 2 is doble underline
	else
		vvalue.Int(0);	
	return Ole::SetValue(Range, "CharUnderline", vvalue);
}
	
bool OPENSheet::SetHorizAlignment(String cell, int alignment) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetHorizAlignment(col, row, alignment);
}

bool OPENSheet::SetHorizAlignment(int col, int row, int alignment) {
	if (!SelCell(col, row))
		return false;
	if (alignment < 1 || alignment >= OfficeSheet::MAX_JUSTIFY)
		return false;
	
	int justifyConst[] = {1, 2, 3, 4, 1, 3};
	
	VariantOle vvalue;
	vvalue.Int(justifyConst[alignment]);	
	return Ole::SetValue(Cell, "HoriJustify", vvalue);
}

bool OPENSheet::SetVertAlignment(String cell, int alignment) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetVertAlignment(col, row, alignment);
}

bool OPENSheet::SetVertAlignment(int col, int row, int alignment) {
	if (!SelCell(col, row))
		return false;
	if (alignment < 1 || alignment >= OfficeSheet::MAX_JUSTIFY)
		return false;
	
	int justifyConst[] = {1, 2, 3, 4, 1, 3};
	
	VariantOle vvalue;
	vvalue.Int(justifyConst[alignment]);	
	return Ole::SetValue(Cell, "VertJustify", vvalue);
}

bool OPENSheet::SetBorder(int borderIndx, int lineStyle, int weight, Color color) {
	if (!Range)
		return false;

	return false;		// Not implemented yet :(
	
/*	This is an unsuccesful test
	VariantOle vBorder;
	vBorder.ArrayDim(4);
	vBorder.ArraySetValue(0, 0);
	vBorder.ArraySetValue(1, 0);
	vBorder.ArraySetValue(2, 80);
	vBorder.ArraySetValue(3, 0);

	VariantOle vNoBorder;
	vNoBorder.ArrayDim(4);
	vNoBorder.ArraySetValue(0, 0);
	vNoBorder.ArraySetValue(1, 0);
	vNoBorder.ArraySetValue(2, 0);
	vNoBorder.ArraySetValue(3, 0);
	
	VariantOle vArg;
	VariantOle vArraySave;
	vArraySave.ArrayDim(8);
	
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.LeftBorder", vBorder));
	vArraySave.ArraySetVariant(1, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.LeftDistance", 0));
	vArraySave.ArraySetVariant(2, vArg);

	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.RightBorder", vBorder));
	vArraySave.ArraySetVariant(3, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.RightDistance", 0));
	vArraySave.ArraySetVariant(4, vArg);
	
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.TopBorder", vBorder));
	vArraySave.ArraySetVariant(5, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.TopDistance", 0));
	vArraySave.ArraySetVariant(6, vArg);
	
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.BottomBorder", vBorder));
	vArraySave.ArraySetVariant(7, vArg);
	vArg.ObjectOle(OOo::MakePropertyValue("BorderOuter.BottomDistance", 0));
	vArraySave.ArraySetVariant(8, vArg);
	
	if (!Ole::Method(Range, "BorderOuter", vArraySave))
		return false;
*/	
}

bool OPENSheet::SetBorder(int col, int row, int borderIndx, int lineStyle, int weight, Color color) {
	if (!Sheet)
		return false;

	return false;		// Not implemented yet :(
}

bool OPENSheet::SetFont(String cell, String name, int size) {
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetFont(col, row, name, size);
}

bool OPENSheet::SetFont(int col, int row, String name, int size) {
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

bool OPENSheet::SetFont(String name, int size) {
	VariantOle vvalue;
	vvalue.BString(name);
	Ole::SetValue(Range, "CharFontName", vvalue);
	vvalue.Int(size);
	return Ole::SetValue(Range, "CharHeight", vvalue);
}

bool OPENSheet::SetColor(String cell, Color color) {	// 2 Centered 
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetColor(col, row, color);
}

bool OPENSheet::SetColor(int col, int row, Color color) {
	if (!SelCell(col, row))
		return false;
	return SetColor(color);
}

bool OPENSheet::SetColor(Color color) {
	VariantOle vproperty, vvalue;
	vproperty.BString("CellColor");
	vvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);	
}

bool OPENSheet::SetBackColor(String cell, Color color) {	// 2 Centered 
	int col, row;
	OfficeSheet::CellToColRow(cell, col, row);
	return SetBackColor(col, row, color);
}

bool OPENSheet::SetBackColor(int col, int row, Color color) {
	if (!SelCell(col, row))
		return false;
	return SetBackColor(color);
}

bool OPENSheet::SetBackColor(Color color) {
	VariantOle vproperty, vvalue;
	vproperty.BString("CellBackColor");
	vvalue.Int4(RGB(color.GetR(), color.GetG(), color.GetB()));
	return Ole::SetValue(Cell, "setPropertyValue", vvalue, vproperty);
}

bool OPENSheet::SetFormat(String format) {
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

bool OPENSheet::SetColWidth(int col, double width) {
    ObjectOle Columns;
    if (!(Columns = Ole::MethodGet(Sheet, "getColumns")))
		return false;
    ObjectOle Column;
    VariantOle vvalue;
    vvalue.Int(col);
    if (!(Column = Ole::MethodGet(Columns, "getByIndex", vvalue)))
		return false;
    vvalue.Int(int(width*100));
    return Ole::SetValue(Column, "Width", vvalue);    	
}
	
bool OPENSheet::SetRowHeight(int row, double height) {
    ObjectOle Rows;
    if (!(Rows = Ole::MethodGet(Sheet, "getRows")))
		return false;
    ObjectOle Row;
    VariantOle vvalue;
    vvalue.Int(row);
    if (!(Row = Ole::MethodGet(Rows, "getByIndex", vvalue)))
		return false;
    vvalue.Int(int(height*100));
    return Ole::SetValue(Row, "Height", vvalue); 
}

bool OPENSheet::Print() {
	VariantOle vArrayPrint;
	vArrayPrint.ArrayDim(1);
	VariantOle vArg;
	vArg.ObjectOle(OOo::MakePropertyValue("CopyCount", 1));	
	vArrayPrint.ArraySetVariant(1, vArg);

	return Ole::Method(Document, "print", vArrayPrint);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

INITBLOCK {
	PluginRegister(OfficeDoc, OPENDoc, "Open");
}

OPENDoc::OPENDoc() {
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

OPENDoc::~OPENDoc() {
	if (Cursor)
		Cursor->Release();
	if (Text)
		Text->Release();
	if (Document)
		Document->Release();
	if (Desktop) {
		//Quit();				Commented 12/2014
		Desktop->Release();
	}
	if (CoreReflection)
		CoreReflection->Release();
	if (ServiceManager)
		ServiceManager->Release();
}
	
bool OPENDoc::IsAvailable() {
	ObjectOle serviceManager;
	if (!(serviceManager = Ole::CreateObject("com.sun.star.ServiceManager")))
		return false;
	else {
		serviceManager->Release();
		return true;
	}
}

bool OPENDoc::AddDoc(bool visible) {
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

bool OPENDoc::OpenDoc(String fileName, bool visible) {
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

bool OPENDoc::SetVisible(bool visible) {
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

/*bool OPENDoc::SetColor(Color col) {
	VariantOle vColor;
	vColor.BString("CharColor");
	VariantOle vValue;
	vValue.Int4(RGB(col.GetR(), col.GetG(), col.GetB()));
	return Ole::Method(Cursor, "setPropertyValue", vValue, vColor);
}*/

bool OPENDoc::SetFont(String font, int size) {
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

bool OPENDoc::SetBold(bool bold) {
	VariantOle vvalue;
	if (bold)
		vvalue.Int(150);	
	else
		vvalue.Int(100);	
	return Ole::SetValue(Cursor, "CharWeight", vvalue);
}

bool OPENDoc::SetItalic(bool italic) {
	VariantOle vvalue;
	if (italic)
		vvalue.Int(2);	
	else
		vvalue.Int(0);	
	return Ole::SetValue(Cursor, "CharPosture", vvalue);
}

bool OPENDoc::WriteText(String str) {
	VariantOle vFalse;
	vFalse.Bool(false);	
	VariantOle vText;
	vText.BString(str);
	VariantOle vCursor;
	vCursor.ObjectOle(Cursor);
	return Ole::Method(Text, "insertString", vFalse, vText, vCursor);
}
	
bool OPENDoc::Select() {	// Not used here 
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
	
bool OPENDoc::Print() {
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

bool OPENDoc::EnableCommandVars(bool enable) {
/**/	
	return false;
}
	
bool OPENDoc::SetSaved(bool saved) {
	VariantOle vval;
	vval.Bool(!saved);	// Modified is the opposite to saved
	return Ole::Method(Document, "setModified", vval);
}

bool OPENDoc::SaveAs(String fileName, String type) {
	if (!Document)
		return false;

	if (type[0] == '.')
		type = type.Mid(1);
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

bool OPENDoc::Quit() {
	if (!quit) {
		quit = true;
		if (Document) {
			Ole::Method(Document, "Dispose");
			return Ole::Method(Document, "Terminate");
		}
	}
	return true;
}
