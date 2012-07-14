void MakeLink(StringBuffer& out, const Vector<String>& part, const Vector<Value>& arg);

class Renderer {
protected:
	VectorMap<String, Value>  var;
	int                       lang;

	Renderer& Link(const char *id, void (*view)(Http&), const Vector<Value>& arg);
	const One<Exe>& GetTemplate(const char *template_name);

public:	
	Renderer& operator()(const char *id, const Value& v)  { var.Add(id, v); return *this; }
	Renderer& operator()(const ValueMap& map);
	Renderer& operator()(const char *id, void (*view)(Http&));
	Renderer& operator()(const char *id, void (*view)(Http&), const Value& arg1);
	Renderer& operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2);

	Renderer& operator()(const Sql& sql);
	Renderer& operator()(Fields rec);
	Renderer& operator()(const SqlSelect& row_sel);
	Renderer& operator()(const char *id, const SqlSelect& sel);
	SqlUpdate Update(SqlId table);
	SqlInsert Insert(SqlId table);
	
	Value     operator[](const char *id) const           { return var.Get(id, Null); }

	String    RenderString(const String& template_name);
	Value     Render(const String& template_name)        { return Raw(RenderString(template_name)); }
	
	Renderer& Render(const char *id, const String& template_name);
	
	Renderer()               { lang = LNG_ENGLISH; }
	virtual ~Renderer();
};

class Http : public Renderer {
	SkylarkApp& app;
	HttpHeader  hdr;
	
	String content;
	String viewid;
	
	Vector<String>            arg;
	String                    session_id;
	VectorMap<String, Value>  session_var;
	bool                      session_dirty;
	
	String redirect;
	int    code;
	String code_text;
	String response;
	String content_type;
	String request_content_type;
	
	VectorMap<String, String> cookies;
	
	void   ParseRequest(const char *s);
	void   ReadMultiPart(const String& content);
	
	String SessionFile(const String& sid);

	void   LoadSession();
	void   SaveSession();

public:
	Http&  operator()(const char *id, const Value& v)  { var.Add(id, v); return *this; }
	Http&  operator()(const ValueMap& map)             { Renderer::operator()(map); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&)) { Renderer::operator()(id, view); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&), const Value& arg1) { Renderer::operator()(id, view, arg1); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2) { Renderer::operator()(id, view, arg1, arg2); return *this; }

	Http&  operator()(const Sql& sql)                  { Renderer::operator()(sql); return *this; }
	Http&  operator()(Fields rec)                      { Renderer::operator()(rec); return *this; }
	Http&  operator()(const SqlSelect& row_sel)        { Renderer::operator()(row_sel); return *this; }
	Http&  operator()(const char *id, const SqlSelect& sel) { Renderer::operator()(id, sel); return *this; }
	Http&  Render(const char *id, const String& template_name) { Renderer::Render(id, template_name); return *this; }
	Value  Render(const String& template_name)         { return Renderer::Render(template_name); }

	String GetHeader(const char *s) const              { return hdr[s]; }
	int    GetLength() const                           { return atoi(GetHeader("content-length")); }

	String GetViewId() const                           { return viewid; }

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

	Http&  ClearSession();
	Http&  SessionSet(const char *id, const Value& value);
	
	Http&  NewIdentity()                              { SessionSet("__identity__", Null); return *this; }
	Http&  NewSessionId();
	Http&  SetLanguage(int lang);
	
	Http&  Response(int code_, const String& ctext)   { code = code_; code_text = ctext; return *this; }
	
	Http&  RenderResult(const char *template_name);
	Http&  Redirect(const char *url, int code_ = 302) { code = code_; redirect = url; return *this; }
	Http&  Redirect(void (*view)(Http&), const Vector<Value>& arg);
	Http&  Redirect(void (*view)(Http&));
	Http&  Redirect(void (*view)(Http&), const Value& v1);
	Http&  Redirect(void (*view)(Http&), const Value& v1, const Value& v2);

	Http&  Ux(const char *id, const String& text);
	Http&  UxRender(const char *id, const char *template_name);
	Http&  UxSet(const char *id, const String& value);
	Http&  UxRun(const String& js_code);
	
	String GetResponse() const                        { return response; }

	void   Dispatch(TcpSocket& socket);
	
	const SkylarkApp& App() const                     { return app; }

	Http(SkylarkApp& app);
};

void RegisterHandler(void (*handler)(Http& http), const char *id, const char *path);

#define SKYLARK(name, path)  void name(Http& http); INITBLOCK { UPP::RegisterHandler(name, #name, path); } void name(Http& http)

Vector<String> *GetUrlViewLinkParts(const String& id);

String MakeLink(void (*view)(Http&), const Vector<Value>& arg);

enum {
	SESSION_FORMAT_BINARY, SESSION_FORMAT_JSON, SESSION_FORMAT_XML
};
