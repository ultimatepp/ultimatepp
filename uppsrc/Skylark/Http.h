// progress reasons
enum { PROGRESS_HEADER, PROGRESS_CONTENT, PROGRESS_END, PROGRESS_QUERY = -1 };

void MakeLink(StringBuffer& out, const Vector<String>& part, const Vector<Value>& arg);
void ServeStaticPage(Http& http);

struct HandlerId {
	void (*handler)(Http& http);
	String id;

	HandlerId(const char *id) : id(id) { handler = NULL; }
	HandlerId(void (*handler)(Http& http)) : handler(handler) {}

	HandlerId() { handler = NULL; }
};

class Renderer {
protected:
	VectorMap<String, Value>  var;
	int                       lang;

	Renderer& Link(const char *id, const HandlerId& handler, const Vector<Value>& arg);
	const One<Exe>& GetTemplate(const char *template_name);
	friend String GetIdentity(const Renderer *r);

public:	
	Renderer& operator()(const char *id, const char *v)   { var.GetAdd(id) = v; return *this; }
	Renderer& operator()(const char *id, const String& v) { var.GetAdd(id) = v; return *this; }
	Renderer& operator()(const char *id, const Value& v)  { var.GetAdd(id) = v; return *this; }
	Renderer& operator()(const ValueMap& map);
	Renderer& operator()(const char *id, const HandlerId& handler);
	Renderer& operator()(const char *id, const HandlerId& handler, const Value& arg1);
	Renderer& operator()(const char *id, const HandlerId& handler, const Value& arg1, const Value& arg2);
	Renderer& operator()(const char *id, void (*handler)(Http&));
	Renderer& operator()(const char *id, void (*handler)(Http&), const Value& arg1);
	Renderer& operator()(const char *id, void (*handler)(Http&), const Value& arg1, const Value& arg2);

	Renderer& operator()(const Sql& sql);
	Renderer& operator()(Fields rec);
	Renderer& operator()(const SqlSelect& row_sel);
	Renderer& operator()(const char *id, const SqlSelect& sel);

	Renderer& SetLanguage(int lang);

	SqlUpdate Update(SqlId table);
	SqlInsert Insert(SqlId table);

	Value     operator[](const char *id) const           { return var.Get(id, Null); }

	String    RenderString(const String& template_name);
	Value     Render(const String& template_name)        { return Raw(RenderString(template_name)); }
	
	Renderer& Render(const char *id, const String& template_name);
	
	const VectorMap<String, Value>& Variables() const    { return var; }
	
	Renderer()               { lang = LNG_ENGLISH; }
	virtual ~Renderer();
};

class Http : public Renderer {
	SkylarkApp& app;
	HttpHeader  hdr;
	
	String content;
	String handlerid;
	
	Vector<String>            arg;
	String                    session_id;
	VectorMap<String, Value>  session_var;
	TcpSocket                *rsocket;
	bool                      session_dirty;
	
	String redirect;
	int    code;
	String code_text;
	String response;
	String content_type;
	String request_content_type;
	
	VectorMap<String, String> cookies;
	VectorMap<String, String> headers;
	
	void   ParseRequest(const char *s);
	void   ReadMultiPart(const String& content);
	
	String SessionFile(const String& sid);

	void   LoadSession();
	void   SaveSession();

	void   SessionSet0(const char *id, const Value& value);
	friend String GetIdentity(const Renderer *r);
	
	void WaitHandler(int (*progress)(int, Http&, int), TcpSocket *socket);

public:
	Http&  operator()(const char *id, const char *v)   { var.GetAdd(id) = v; return *this; }
	Http&  operator()(const char *id, const String& v) { var.GetAdd(id) = v; return *this; }
	Http&  operator()(const char *id, const Value& v)  { var.GetAdd(id) = v; return *this; }
	Http&  operator()(const ValueMap& map)             { Renderer::operator()(map); return *this; }
	Http&  operator()(const char *id, const HandlerId& handler) { Renderer::operator()(id, handler); return *this; }
	Http&  operator()(const char *id, const HandlerId& handler, const Value& arg1) { Renderer::operator()(id, handler, arg1); return *this; }
	Http&  operator()(const char *id, const HandlerId& handler, const Value& arg1, const Value& arg2) { Renderer::operator()(id, handler, arg1, arg2); return *this; }
	Http&  operator()(const char *id, void (*handler)(Http&)) { Renderer::operator()(id, handler); return *this; }
	Http&  operator()(const char *id, void (*handler)(Http&), const Value& arg1) { Renderer::operator()(id, handler, arg1); return *this; }
	Http&  operator()(const char *id, void (*handler)(Http&), const Value& arg1, const Value& arg2) { Renderer::operator()(id, handler, arg1, arg2); return *this; }

	Http&  operator()(const Sql& sql)                  { Renderer::operator()(sql); return *this; }
	Http&  operator()(Fields rec)                      { Renderer::operator()(rec); return *this; }
	Http&  operator()(const SqlSelect& row_sel)        { Renderer::operator()(row_sel); return *this; }
	Http&  operator()(const char *id, const SqlSelect& sel) { Renderer::operator()(id, sel); return *this; }
	Http&  Render(const char *id, const String& template_name) { Renderer::Render(id, template_name); return *this; }
	Value  Render(const String& template_name)         { return Renderer::Render(template_name); }

	String GetHeader(const char *s) const              { return hdr[s]; }
	int    GetLength() const                           { return atoi(GetHeader("content-length")); }
	String GetPeerAddr() const;

	String GetRequestContent() const                   { return content; }

	String GetHandlerId() const                        { return handlerid; }

	Value  operator[](const char *id) const            { return Renderer::operator[](id); }
	int    Int(const char *id) const;

	String operator[](int i) const                     { return i >= 0 && i < arg.GetCount() ? arg[i] : String(); }
	int    Int(int i) const;
	
	int    GetParamCount() const                       { return arg.GetCount(); }

	Http&  ContentType(const char *s)                  { content_type = s; return *this; }

	Http&  Content(const char *s, const Value& data);
	Http&  operator<<(const Value& s);

	Http&  SetRawCookie(const char *id, const String& value,
	                    Time expires = Null, const char *path = NULL,
	                    const char *domain = NULL, bool secure = false, bool httponly = false);
	Http&  SetCookie(const char *id, const String& value,
	                 Time expires = Null, const char *path = NULL,
	                 const char *domain = NULL, bool secure = false, bool httponly = false);
	Http&  SetHeader(const char *header, const char *data);
	Http&  ClearHeader(const char *header);

	Http&  ClearSession();
	Http&  SessionSet(const char *id, const Value& value);
	Http&  NewSessionId();
	
	Http&  NewIdentity()                              { SessionSet("__identity__", Null); return *this; }

	Http&  SetLanguage(int lang);
	
	Http&  Response(int code_, const String& ctext)   { code = code_; code_text = ctext; return *this; }
	
	Http&  RenderResult(const char *template_name);
	Http&  Redirect(const char *url, int code_ = 302) { code = code_; redirect = url; return *this; }
	Http&  Redirect(const HandlerId& handler, const Vector<Value>& arg);
	Http&  Redirect(const HandlerId& handler);
	Http&  Redirect(const HandlerId& handler, const Value& v1);
	Http&  Redirect(const HandlerId& handler, const Value& v1, const Value& v2);

	Http&  Ux(const char *id, const String& text);
	Http&  UxRender(const char *id, const char *template_name);
	Http&  UxSet(const char *id, const String& value);
	Http&  UxRun(const String& js_code);
	
	String GetResponse() const                        { return response; }
	
	void   Finalize();

	void   Dispatch(TcpSocket& socket);
	
	const SkylarkApp& App() const                     { return app; }

	Http(SkylarkApp& app);
	Http();
};

void RegisterHandler(void (*handler)(Http& http), const char *id, const char *path, int (*progress)(int, Http&, int) = NULL);
void RegisterHandler(Callback1<Http&> handler, const char *id, const char *path, int (*progress)(int, Http&, int) = NULL);

#define SKYLARK(name, path)  void name(Http& http); INITBLOCK { UPP::RegisterHandler(name, #name, path); } void name(Http& http)
#define SKYLARK_PROGRESS(name, path, progressCb)  void name(Http& http); INITBLOCK { UPP::RegisterHandler(name, #name, path, progressCb); } void name(Http& http)

Vector<String> *GetUrlViewLinkParts(const String& id);

String MakeLink(const HandlerId& id, const Vector<Value>& arg);

enum {
	SESSION_FORMAT_BINARY, SESSION_FORMAT_JSON, SESSION_FORMAT_XML
};

struct SkylarkPack {
	String instance_id;
	String instance_path;
};

String SkylarkAppendPath__(const String& path_prefix, const String& path);

#define SKYLARK_USE(cls, id, path) \
static void cls##_##id##_##init(cls& id); \
cls id; \
INITBLOCK { \
	id.instance_id = #id; \
	id.instance_path = path; \
	cls##_##id##_##init(id); \
	id.Use(); \
} \
static void cls##_##id##_##init(cls& id)

#define SKYLARK_MEMBER(member, path) \
{ \
	member.instance_id = instance_id + ':' + #member; \
	member.instance_path = SkylarkAppendPath__(instance_path, path); \
	member.Use(); \
}

#define SKYLARK_METHOD(method, path) \
RegisterHandler(THISBACK(method), instance_id + ':' + #method, SkylarkAppendPath__(instance_path, path))

#define THISLINK(method)  (IGNORE_RESULT(&CLASSNAME::method), /* check that the method exists*/ \
                           HandlerId(instance_id + ':' + #method))

Http& DummyHttp(); // only for purposes of LINK

#define LINK(instance, method) ((void)(0 ? instance.method(DummyHttp()) : (void)0), /* check that the method exists */ \
                                HandlerId(instance.instance_id + ':' + #method))
