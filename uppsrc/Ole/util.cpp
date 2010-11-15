#include "Ole.h"

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////
// GUID

GUID GetCoClassGUID(const char *name, bool prog_id)
{
	OleBstr uni = String(name);
	GUID guid;
	Zero(guid);
	if(prog_id && *name != '{')
		CLSIDFromProgID(~uni, &guid);
	else
		CLSIDFromString(~uni, &guid);
	return guid;
}

unsigned GetHashValue(const GUID& guid) {
	const dword *p = reinterpret_cast<const dword *>(&guid);
	return CombineHash(p[0], p[1], p[2], p[3]);
}

String Format(const GUID& guid) {
	String result;
	wchar_t buffer[128];
	int length;
	if((length = StringFromGUID2(guid, buffer, __countof(buffer))) > 0)
		result = AsString(buffer, length - 1);
	return result;
}

String CFormat(const GUID& guid) {
	String value = Format("{ 0x%08x, 0x%04x, 0x%04x, { ", (int)guid.Data1, (int)guid.Data2, (int)guid.Data3);
	for(int i = 0; i < 8; i++)
		value << Format("0x%02x, ", guid.Data4[i]);
	return value << "} }";
}

String GetInterfaceName(const GUID& guid) {
	if(IsNull(guid))
		return Null;
	String goo = Format(guid);
	return Nvl(GetWinRegString(NULL, "Interface\\" + goo, HKEY_CLASSES_ROOT), goo);
}

String GetCoClassName(const GUID& guid) {
	if(IsNull(guid))
		return Null;
	String goo = Format(guid);
	return Nvl(GetWinRegString(NULL, "CLSID\\" + goo + "\\ProgID", HKEY_CLASSES_ROOT), goo);
}

String GetDisplayName(const GUID& guid) {
	if(IsNull(guid))
		return Null;
	String goo = Format(guid);
	return Nvl(GetWinRegString(NULL, "CLSID\\" + goo, HKEY_CLASSES_ROOT), goo);
}

bool IsNull(const GUID& guid) {
	const dword *p = reinterpret_cast<const dword *>(&guid);
	return (p[0] | p[1] | p[2] | p[3]) == 0;
}

//////////////////////////////////////////////////////////////////////
// VARIANT

Value AsValue(const VARIANT& variant)
{
	switch(variant.vt) {
		case VT_ERROR: return ErrorValue(GetErrorMessage(variant.scode));
		case VT_EMPTY: return Value(); // void
		case VT_NULL:  return Null;

		case VT_BOOL:
		case VT_UI1:   return (int)variant.bVal;
		case VT_I2:    return (int)variant.iVal;
		case VT_I4:    return (int)variant.lVal;

		case VT_R4:    return variant.fltVal;
		case VT_R8:    return variant.dblVal;

		case VT_CY:    return GetCurrency(variant.cyVal);

		case VT_DATE:  return FromDATE(variant.date);

		case VT_BSTR:  return BSTRToWString(variant.bstrVal);

		case VT_DECIMAL: {
			const DECIMAL& d = variant.decVal;
			double h = (d.Lo32 + d.Mid32 * (65536.0 * 65536.0)) * pow(0.1, d.scale);
			return d.sign ? -h : h;
		}

		case VT_ARRAY | VT_UI1: {
			int nelem = variant.parray->rgsabound[0].cElements;
			void *bits = 0;
			if(SUCCEEDED(SafeArrayAccessData(variant.parray, &bits)) && bits) {
				String out((const char *)bits, nelem);
				SafeArrayUnaccessData(variant.parray);
				return out;
			}
			break;
		}
	}
	return ErrorValue(Format("Unknown variant type %d", variant.vt));
}

OleVariant AsVariant(Value value) {
	OleVariant out(VT_NULL);
	if(value.IsNull())
		return out;
	switch(value.GetType()) {
	case BOOL_V:    out.vt = VT_UI1; out.bVal = (byte)(int)value; break;
	case INT_V:     out.vt = VT_I4; out.lVal = (int)value; break;
	case DOUBLE_V:  out.vt = VT_R8; out.dblVal = value; break;
	case STRING_V:  out.vt = VT_BSTR; out.bstrVal = StringToBSTR(value); break;
	case WSTRING_V: out.vt = VT_BSTR; out.bstrVal = WStringToBSTR(value); break;
	case DATE_V:    
	case TIME_V:    out.vt = VT_DATE; out.date = ToDATE((Time)value);
	// todo: VALUEARRAY_V ??
	default:        break;
	}
	return out;
}

double GetCurrency(const win32_CY_& currency) {
	return (currency.Hi * (65536.0 * 65536.0) + currency.Lo) / 10000.0;
}

OleVariant ValueToVariant(Value v)
{
	if(IsTypeRaw< IRef<IDispatch> >(v))
	{
		OleVariant out(VT_DISPATCH);
		IRef<IDispatch> disp = ValueTo< IRef<IDispatch> >(v);
		if(!!disp) disp -> AddRef();
		out.pdispVal = ~disp;
		return out;
	}
	if(IsTypeRaw< IRef<IUnknown> >(v))
	{
		OleVariant out(VT_UNKNOWN);
		IRef<IUnknown> unk = ValueTo< IRef<IUnknown> >(v);
		if(!!unk) unk -> AddRef();
		out.punkVal = ~unk;
		return out;
	}
	return AsVariant(v);
}

void ReturnVariant(VARIANT *var, const Value& v)
{
	*var = AsVariant(v);
}

//////////////////////////////////////////////////////////////////////
// BSTR

String BSTRToString(BSTR bstr)
{
	int l;
	if(!bstr || !(l = SysStringLen(bstr)))
		return Null;
	byte cs = GetDefaultCharset();
	if(cs == CHARSET_UTF8)
 		return ToUtf8(TWchar(bstr));
	StringBuffer out(l);
	FromUnicode(out, TWchar(bstr), l, cs);
	return out;
}

BSTR StringToBSTR(String s)
{
	int len = s.GetLength();
	if(len == 0)
		return NULL;
	byte cs = GetDefaultCharset();
	if(cs == CHARSET_UTF8)
		return WStringToBSTR(FromUtf8(s));
	BSTR bstr = SysAllocStringLen(NULL, len);
	ToUnicode(TWchar(bstr), s, len, cs);
	return bstr;
}

void ReturnString(BSTR *dest, String s)
{
	if(*dest)
		SysFreeString(*dest);
	*dest = StringToBSTR(s);
}

HRESULT CheckReturnString(BSTR *bstr, String s)
{
	if(!bstr) return E_INVALIDARG;
	ReturnString(bstr, s);
	return S_OK;
}

void ReturnWString(BSTR *dest, WString s)
{
	if(*dest)
		SysFreeString(*dest);
	*dest = WStringToBSTR(s);
}

HRESULT CheckReturnWString(BSTR *bstr, WString s)
{
	if(!bstr) return E_INVALIDARG;
	ReturnWString(bstr, s);
	return S_OK;
}

static ValueArray SAFEARRAYToValueArrayPart(SAFEARRAY *array, long *indices, int dim_index)
{
	Vector<Value> dim_array;
	int nelem = array->rgsabound[dim_index].cElements;
	dim_array.SetCount(nelem);
	for(int e = 0; e < nelem; e++) {
		indices[dim_index] = e;
		if(dim_index > 0)
			dim_array[e] = SAFEARRAYToValueArrayPart(array, indices, dim_index - 1);
		else {
			OleVariant var;
			HRESULT hr = SafeArrayGetElement(array, indices, &var);
			if(SUCCEEDED(hr))
				dim_array[e] = AsValue(var);
			else {
				String dims;
				for(int i = 0; i < dim_index; i++)
					dims << (i ? ", " : "") << indices[i];
				dim_array[e] = ErrorValue(NFormat("SafeArrayGetElement(%s): error %08lx", dims, hr));
			}
		}
	}
	return ValueArray(dim_array);
}

ValueArray SAFEARRAYToValueArray(SAFEARRAY *array)
{
	int ndims = array->cDims;
	Vector<long> indices;
	indices.SetCount(ndims, -1);
	return SAFEARRAYToValueArrayPart(array, indices.Begin(), ndims - 1);
}

Vector<WString> SAFEARRAYToWStringVector(SAFEARRAY *array)
{
	Vector<WString> out;
	out.SetCount(array->rgsabound[0].cElements);
	Vector<long> indices;
	indices.SetCount(array->cDims, 0);
	for(int i = 0; i < out.GetCount(); i++) {
		OleBstr s;
		indices[0] = i;
		HRESULT hr = SafeArrayGetElement(array, indices.Begin(), s.Set());
		out[i] = s;
	}
	return out;
}

SAFEARRAY *ToSAFEARRAY(const ValueArray& varray)
{
	SAFEARRAY *a = SafeArrayCreateVector(VT_VARIANT, 0, varray.GetCount());
	if(!a)
		return NULL;
	for(long index = 0; index < varray.GetCount(); index++) {
		OleVariant var = AsVariant(varray[(int)index]);
		SafeArrayPutElement(a, &index, &var);
	}
	return a;
}

SAFEARRAY *ToSAFEARRAY(const Vector<WString>& vstr)
{
	SAFEARRAY *a = SafeArrayCreateVector(VT_BSTR, 0, vstr.GetCount());
	if(!a)
		return NULL;
	for(long index = 0; index < vstr.GetCount(); index++) {
		OleBstr var = vstr[(int)index];
		SafeArrayPutElement(a, &index, ~var);
	}
	return a;
}

//////////////////////////////////////////////////////////////////////
// special types

Color PackColor(long rgb)
{
	if(rgb < 0)
		return Null;
	return Color((rgb >> 16) & 255, (rgb >> 8) & 255, (rgb >> 0) & 255);
}

long UnpackColor(Color c)
{
	if(IsNull(c))
		return -1;
	return (c.GetR() << 16) | (c.GetG() << 8) | (c.GetB() << 0);
}

HRESULT CheckReturnColor(long *ptr, Color c)
{
	if(!ptr)
		return E_INVALIDARG;
	*ptr = UnpackColor(c);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// errors

OleExc::OleExc(HRESULT hresult)
: Exc(NFormat("COM[%08x]: %s", (int)hresult, GetErrorMessage(hresult)))
, hresult(hresult)
{
}

OleExc::OleExc(HRESULT hresult, const char *text)
: Exc(NFormat("COM[%08x]: %s: %s", (int)hresult, GetErrorMessage(hresult), text))
, hresult(hresult)
{
	LOG("OleExc::OleExc " << *this);
}

HRESULT LogResult(HRESULT hr)
{
	LOGF("<= return 0x%08x: %s\r\n", hr, GetErrorMessage(hr));
	return hr;
}

HRESULT LogError(HRESULT hr)
{
	if(FAILED(hr))
		LogResult(hr);
	return hr;
}

//////////////////////////////////////////////////////////////////////
// interfaces

Value DispatchToValue(IDispatch *disp)
{
	return RawPickToValue(IRef<IDispatch>(disp));
}

Value UnknownToValue(IUnknown *unk)
{
	return RawPickToValue(IRef<IUnknown>(unk));
}

//////////////////////////////////////////////////////////////////////
// OleStream::

OleStream::OleStream(IUnknown *stream)
{
	Open(stream);
}

bool OleStream::Open(IUnknown *stream)
{
	Close();
	if(!stream)
		return false;
	if(FAILED(stream -> QueryInterface(IID_IStream, (void **)&istream)))
		return false;
	STATSTG stat;
	Zero(stat);
//	HRESULT statres = istream -> Stat(&stat, STATFLAG_NONAME);
	int mode = READWRITE;
//	if(SUCCEEDED(statres) && (stat.grfMode & (STGM_READ | STGM_WRITE | STGM_READWRITE)) == STGM_READ)
//		mode = READ;

	ULARGE_INTEGER pos = AsULarge(0), len;
	HRESULT hr;
	if(FAILED(hr = istream -> Seek(AsLarge(0), STREAM_SEEK_CUR, &pos))
	|| FAILED(hr = istream -> Seek(AsLarge(0), STREAM_SEEK_END, &len)))
		SetError(hr);
	current_offset = len.QuadPart;
	OpenInit(mode, len.QuadPart);
	Seek(pos.QuadPart);
	return true;
}

void OleStream::Close()
{
	if(!(bool)istream)
		return;
	int64 pos = GetPos();
	Flush();
	HRESULT hr;
	if(FAILED(hr = istream -> Seek(AsLarge(pos), STREAM_SEEK_SET, 0)))
		SetError(hr);
	istream = 0;
}

void OleStream::SetStreamSize(dword size)
{
	ASSERT((bool)istream && (GetStyle() & STRM_WRITE));
	HRESULT hr;
	if(FAILED(hr = istream -> SetSize(AsULarge(size))))
		SetError(hr);
	ULARGE_INTEGER pos = AsULarge(0);
	if(FAILED(hr = istream -> Seek(AsLarge(0), STREAM_SEEK_CUR, &pos)))
		SetError(hr);
	current_offset = pos.LowPart;
}

dword OleStream::Read(int64 at, void *ptr, dword size)
{
	ASSERT((bool)istream && (GetStyle() & STRM_READ));
	RawSeek(at);
	dword done = 0;
	HRESULT hr;
	if(FAILED(hr = istream -> Read(ptr, size, &done)))
		SetError(hr);
	else
		current_offset += done;
	return done;
}

void OleStream::Write(int64 at, const void *data, dword size)
{
	ASSERT((bool)istream && (GetStyle() & STRM_WRITE));
	RawSeek(at);
	dword done = 0;
	HRESULT hr;
	if(FAILED(hr = istream -> Write(data, size, &done)) || done != size)
		SetError(hr);
	current_offset += done;
}

void OleStream::RawSeek(int64 pos)
{
	if(current_offset == pos)
		return;
	ULARGE_INTEGER new_pos = AsULarge(pos);
	HRESULT hr;
	if(FAILED(hr = istream -> Seek(AsLarge(pos), STREAM_SEEK_SET, &new_pos)))
		SetError(hr);
	pos = new_pos.LowPart;
}

//////////////////////////////////////////////////////////////////////

END_UPP_NAMESPACE

#endif
