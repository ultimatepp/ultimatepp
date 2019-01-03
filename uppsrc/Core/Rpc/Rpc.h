// NOTE: Because of clash with Windows rpc.h, Core/Core.h has to be included before this header
// For the same reason implementation files are in 'imp' subdirectory

#ifndef Rpc_Rpc_h
#define Rpc_Rpc_h

namespace Upp {

enum {
	RPC_SERVER_JSON_ERROR = -32700,    // Parse error
	RPC_SERVER_JSON_REQUEST_ERROR = -32600, // Invalid Request
	RPC_UNKNOWN_METHOD_ERROR = -32601, // Method not found
	RPC_SERVER_PARAM_ERROR = -32602, // Invalid params
	RPC_SERVER_PROCESSING_ERROR = -32000, // Server error

	RPC_SERVER_XML_ERROR,

	RPC_CLIENT_HTTP_ERROR = -1000000,
	RPC_CLIENT_XML_ERROR,
	RPC_CLIENT_JSON_ERROR,
	RPC_CLIENT_RESPONSE_ERROR,
};

struct ValueTypeMismatch {};

String FormatIso8601(Time t);
Time   ScanIso8601(const String& p);
Value  JsonRpcData(const Value& v);

struct RawJsonText {
	String json;
};

void ValueCheck(bool b);

void ValueGet(int& n, const Value& v);
void ValueGet(int64& n, const Value& v);
void ValueGet(bool& b, const Value& v);
void ValueGet(String& s, const Value& v);
void ValueGet(double& x, const Value& v);
void ValueGet(Date& x, const Value& v);
void ValueGet(Time& x, const Value& v);
void ValueGet(Value& t, const Value& v);
void ValueGet(ValueArray& va, const Value& v);
void ValueGet(ValueMap& vm, const Value& v);

template <class T>
void ValueGet(Array<T>& x, const Value& v)
{
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
void ValueGet(Vector<T>& x, const Value& v)
{
	ValueCheck(IsValueArray(v));
	ValueArray va = v;
	x.SetCount(va.GetCount());
	for(int i = 0; i < va.GetCount(); i++)
		ValueGet(x[i], va[i]);
}

template <class T>
void ValueGet(ArrayMap<String, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		ValueGet(x.Add(k[i]), va[i]);
}

template <class T>
void ValueGet(VectorMap<String, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		ValueGet(x.Add(k[i]), va[i]);
}

template <class T>
void ValueGet(ArrayMap<int, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		ValueGet(x.Add(atoi(AsString(k[i]))), va[i]);
}

template <class T>
void ValueGet(VectorMap<int, T>& x, const Value& v)
{
	ValueCheck(IsValueMap(v));
	ValueMap vm = v;
	const Index<Value>& k = vm.GetKeys();
	ValueArray va = vm.GetValues();
	x.Clear();
	for(int i = 0; i < k.GetCount(); i++)
		ValueGet(x.Add(atoi(AsString(k[i]))), va[i]);
}

void ValuePut(Value& v, int n);
void ValuePut(Value& v, int64 n);
void ValuePut(Value& v, const String& s);
void ValuePut(Value& v, const char *s);
void ValuePut(Value& v, double x);
void ValuePut(Value& v, bool x);
void ValuePut(Value& v, const Date& x);
void ValuePut(Value& v, const Time& x);
void ValuePut(Value& v, const Value& t);
void ValuePut(Value& v, const ValueArray& va);
void ValuePut(Value& v, const ValueMap& vm);
void ValuePut(Value& v, const Json& json);

template <class T>
Value AsXmlRpcValue(const T& x)
{
	Value vs;
	ValuePut(vs, x);
	return vs;
}

template <class T>
void ValuePut(Value& v, const Array<T>& x)
{
	ValueArray va;
	for(int i = 0; i < x.GetCount(); i++)
		va.Add(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Value& v, const Vector<T>& x)
{
	ValueArray va;
	for(int i = 0; i < x.GetCount(); i++)
		va.Add(AsXmlRpcValue(x[i]));
	v = va;
}

template <class T>
void ValuePut(Value& v, const ArrayMap<String, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(x.GetKey(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const VectorMap<String, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(x.GetKey(i), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const ArrayMap<int, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(AsString(x.GetKey(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

template <class T>
void ValuePut(Value& v, const VectorMap<int, T>& x)
{
	ValueMap vm;
	for(int i = 0; i < x.GetCount(); i++)
		if(!x.IsUnlinked(i))
			vm.Add(AsString(x.GetKey(i)), AsXmlRpcValue(x[i]));
	v = vm;
}

Value      ParseXmlRpcValue(XmlParser& p);
Value      ParseXmlRpcParam(XmlParser& p);
ValueArray ParseXmlRpcParams(XmlParser& p);

struct RpcGet {
	Value v;
	
	template <class T>
	operator T() const { T x; ValueGet(x, v); return x; }

	template <class T>
	T Get() const { T x; ValueGet(x, v); return x; } // Ugly workaround for MSC compiler bug

	String ToString() const { return v.ToString(); }
};

struct XmlRpcDo;

struct RpcData {
	String     peeraddr;
	ValueArray in;
	int        ii;
	ValueMap   in_map;
	ValueArray out;
	ValueMap   out_map;

	int      GetRemainingCount() const  { return in.GetCount() - ii; }
	bool     HasMore() const            { return GetRemainingCount() > 0; }

	Value Get()                         { if(ii >= in.GetCount()) return Value(); return in[ii++]; }
	
	template <class T>
	RpcData& operator>>(T& x)           { ValueGet(x, Get()); return *this; }
	
	RpcGet   operator++(int)            { RpcGet h; h.v = Get(); return h; }
	
	RpcGet   operator[](const char *id) { RpcGet h; h.v = in_map[id]; return h; }
	
	template <class T>
	RpcData& operator<<(const T& x)     { ASSERT(out_map.GetCount() == 0); Value v; ValuePut(v, x); out.Add(v); return *this; }

	template <class T>
	void        Set(int i, const T& x)  { ASSERT(out_map.GetCount() == 0); Value v; ValuePut(v, x); out.Set(i, v); }

	template <class T>
	void operator=(const T& x)          { out.Clear(); operator<<(x); }

	void        Reset()                 { in.Clear(); out.Clear(); ii = 0; }
	
	void        EndRpc();

	RpcData() { ii = 0; rpc = NULL; }

private:
	friend struct XmlRpcDo;

	XmlRpcDo *rpc;
};

String FormatXmlRpcValue(const Value& _v, bool supports_i8);
String FormatXmlRpcParam(const Value& param, bool supports_i8);
String FormatXmlRpcParams(const ValueArray& params, bool supports_i8);

String FormatXmlRpcError(int code, const char *text);

void   Register(const char *name, void (*method)(RpcData&), const char *group = NULL);

#define RPC_METHOD(name) void name(RpcData& rpc); INITBLOCK { Register(#name, name); } void name(RpcData& rpc)
#define RPC_GMETHOD(name, group) void name(RpcData& rpc); INITBLOCK { Register(#name, name, group); } void name(RpcData& rpc)

struct RpcError {
	int    code;
	String text;
};

void   SetRpcMethodFilter(String (*filter)(const String& methodname));
bool   RpcPerform(TcpSocket& http, const char *group);
bool   RpcServerLoop(int port, const char *group = NULL);

void   ThrowRpcError(int code, const char *s);
void   ThrowRpcError(const char *s);

class RpcRequest : public HttpRequest {
	bool       shorted;
	RpcData    data;
	String     method;
	String     error;
	String     faultString;
	int        faultCode;
	bool       shouldExecute;
	bool       json, notification;
	bool       supports_i8;
	String     protocol_version;
	void       Init();

public:
	RpcRequest& Method(const char *name);

	template <class T>
	RpcRequest& operator<<(const T& x)             { data << x; return *this; }
	template <class T>
	void        Set(int i, const T& x)             { data.Set(i, x); }
	template <class T>
	RpcRequest& Named(const char *id, const T& x)  { data.out_map.Add(id, x); return *this; }

	RpcGet      Execute();
	RpcGet      Retry();

	template <class T>
	bool operator>>(T& x)                          { if(Execute().v.IsError()) return false;
	                                                 try { data >> x; } catch(ValueTypeMismatch) { return false; } return true; }

	RpcRequest& operator()(const char *method)     { Method(method); return *this; }

#define E__Templ(I)  class COMBINE(T, I)
#define E__Decl(I)   const COMBINE(T, I)& COMBINE(p, I)
#define E__Param(I)  *this << COMBINE(p, I)
#define E__Body(I) \
	template <__List##I(E__Templ)> \
	RpcRequest& operator()(const char *method, __List##I(E__Decl)) { \
		Method(method); \
		__List##I(E__Param); \
		return *this; \
	}

	__Expand20(E__Body)

#undef E__Templ
#undef E__Decl
#undef E__Param
#undef E__Body

	String GetFaultString() const                               { return faultString; }
	int    GetFaultCode() const                                 { return faultCode; }
	String GetError() const                                     { return error; }
	String GetMethod() const                                    { return method; }
	void   ClearError();
	
	RpcRequest& Url(const char *url);
	
	RpcRequest& JsonRpc()                                       { json = true; return *this; }
	RpcRequest& Notification()                                  { notification = true; return *this; }
	RpcRequest& SupportsI8()                                    { supports_i8 = true; protocol_version = "2.1"; return *this; }
	
	RpcRequest(const char *url);
	RpcRequest();
};

struct XmlRpcRequest : RpcRequest {
	XmlRpcRequest(const char *url) : RpcRequest(url) {}
	XmlRpcRequest() {}
};

struct JsonRpcRequest : RpcRequest {
	JsonRpcRequest(const char *url) : RpcRequest(url) { JsonRpc(); }
	JsonRpcRequest() { JsonRpc(); }
};

struct JsonRpcRequestNamed : RpcRequest {
	template <class T>
	JsonRpcRequestNamed& operator()(const char *id, const T& x)   { Named(id, x); return *this; }
	JsonRpcRequestNamed& operator()(const char *method)           { Method(method); return *this; }
	
	JsonRpcRequestNamed(const char *url) : RpcRequest(url)        { JsonRpc(); }
	JsonRpcRequestNamed()                                         { JsonRpc(); }
};

void LogRpcRequests(bool b = true);
void LogRpcRequestsCompress(bool b);

void SetRpcServerTrace(Stream& s, int level = 1);
void SetRpcServerTraceCompress(bool compress);
void StopRpcServerTrace();
void SuppressRpcServerTraceForMethodCall();

#include "legacy.h"

}

#endif
