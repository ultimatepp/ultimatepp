#include "Skylark.h"

#if 0
#define LLOG(x)     DLOG(x)
#define LDUMP(x)    DDUMP(x)
#define LDUMPC(x)   DDUMPC(x)
#define LDUMPM(x)   DDUMPM(x)
#else
#define LLOG(x)
#define LDUMP(x)
#define LDUMPC(x)
#define LDUMPM(x)
#endif

#define LTIMING(x)  //RTIMING(x)

namespace Upp {

enum { DISPATCH_VARARGS = -1 };

struct DispatchNode : Moveable<DispatchNode> { // Single node in url hierarchy tree
	VectorMap<String, int> subnode;
	Callback1<Http&> handler; // Associated handler, if the dispatch ends at this node
	int              argpos;
	int              method; // Type of method - GET or POST
	bool             post_raw; // true for POST_RAW
	String           id; // Handler ID
	
	int (*progress)(int, Http&, int);
	
	enum { GET, POST };
	
	rval_default(DispatchNode);

	DispatchNode() { argpos = Null; method = GET; post_raw = false; progress = NULL;}
};

static Vector<DispatchNode>& sDispatchMap()
{
	static Vector<DispatchNode> x;
	return x;
}

static VectorMap<String, Vector<String> >& sLinkMap()
{
	static VectorMap<String, Vector<String> > x;
	return x;
}

static Index<uintptr_t>& sHandlerIndex()
{ // map of handler functions for Redirect
	static Index<uintptr_t> x; // use uintptr_t because it has defined hashing
	return x;
}

void DumpDispatchMap()
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	for(int i = 0; i < DispatchMap.GetCount(); i++) {
		LLOG("-------------");
		String sub;
		for(int j = 0; j < DispatchMap[i].subnode.GetCount(); j++)
			sub << DispatchMap[i].subnode.GetKey(j) << "->" << DispatchMap[i].subnode[j] << ", ";
		LLOG(i << " " << (bool)DispatchMap[i].handler << ": " << sub);
	}			
}

String SkylarkAppendPath__(const String& path_prefix, const String& path)
{
	String r = path_prefix;
	if(path_prefix.GetCount() || path.GetCount())
		r << '/';
	r << path;
	return r;
}

Http& DummyHttp()
{
	static Http http;
	return http;
}

Vector<String> *GetUrlViewLinkParts(const String& id)
{
	int q = sLinkMap().Find(id);
	if(q < 0)
		return NULL;
	return &sLinkMap()[q];
}

String MakeLink0(int q, const Vector<Value>& arg)
{
	if(q < 0)
		throw Exc("Invalid view");
	if(q < 0)
		return String();
	StringBuffer out;
	MakeLink(out, sLinkMap()[q], arg);
	return out;
}

String MakeLink(const HandlerId& id, const Vector<Value>& arg)
{
	return MakeLink0(id.handler ? sHandlerIndex().Find((uintptr_t)id.handler) : sLinkMap().Find(id.id), arg);
}

void RegisterView0(void (*fn)(Http&), Callback1<Http&> cb, const char *id, String path, int (*progress)(int, Http&, int), bool primary)
{
	SKYLARKLOG("Register Handler " << id << " -> " << path);
	ASSERT_(sLinkMap().Find(id) < 0, "duplicate handler id " + String(id));
	Vector<String>& linkpart = sLinkMap().GetAdd(id);
	ASSERT_(!fn || sHandlerIndex().Find((uintptr_t)fn) < 0, "duplicate view function registration " + String(id));
	sHandlerIndex().Add((uintptr_t)fn);
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	int method = DispatchNode::GET;
	bool post_raw = false;
	int q = path.Find(':');
	if(q >= 0) {
		if(path.Mid(q + 1) == "POST")
			method = DispatchNode::POST;
		if(path.Mid(q + 1) == "POST_RAW") {
			method = DispatchNode::POST;
			post_raw = true;
		}
		path = path.Mid(0, q);
	}
	Vector<String> h = Split(path, '/');
	if(DispatchMap.GetCount() == 0)
		DispatchMap.Add();
	q = 0;
	int linkargpos = 0;
	for(int i = 0; i < h.GetCount(); i++) {
		String s = h[i];
		LLOG(" Node " << h[i]);
		DispatchNode& n = DispatchMap[q];
		if(*s == '*') {
			int argpos = Null;
			if(IsDigit(s[1]))
				linkargpos = argpos = minmax(atoi(~s + 1), 0, 30);
			else
			if(s[1] == '*')
				argpos = DISPATCH_VARARGS;
			q = DispatchMap.GetCount();
			LLOG(" Adding arg " << argpos << ": " << q);
			n.subnode.Add(Null, q);
			DispatchMap.Add();
			DispatchMap[q].argpos = argpos;
			if(primary)
				linkpart.Add(String(linkargpos++, 1));
		}
		else {
			if(primary)
				linkpart.Add(s);
			q = n.subnode.Get(s, -1);
			if(q < 0) {
				q = DispatchMap.GetCount();
				LLOG(" Adding " << s << ": " << q);
				n.subnode.Add(s, q);
				DispatchMap.Add();
			}
		}
	}
	ASSERT_(!DispatchMap[q].handler, "duplicate view " + String(path));
	DispatchMap[q].handler = fn ? callback(fn) : cb;
	DispatchMap[q].method = method;
	DispatchMap[q].id = id;
	DispatchMap[q].post_raw = post_raw;
	DispatchMap[q].progress = progress;
//	DumpDispatchMap();
}

struct HandlerData { // temporary storage of handlers until FinalizeViews call
	void           (*fn)(Http&);
	Callback1<Http&> cb;
	String           id;
	String           path;
	
	int(*progress)(int, Http&, int);
	
	HandlerData() { fn = NULL; progress = NULL; }
};

static Array<HandlerData>& sHandlerData()
{
	static Array<HandlerData> x;
	return x;
}

void RegisterHandler(void (*fn)(Http&), const char *id, const char *path, int (*progress)(int, Http&, int))
{
	Array<HandlerData>& v = sHandlerData();
	HandlerData& w = v.Add();
	w.fn = fn;
	w.id = id;
	w.path = path;
	w.progress = progress;
}

void RegisterHandler(Callback1<Http&> cb, const char *id, const char *path, int (*progress)(int, Http&, int))
{
	Array<HandlerData>& v = sHandlerData();
	HandlerData& w = v.Add();
	w.cb = cb;
	w.id = id;
	w.path = path;
	w.progress = progress;
}

void SkylarkApp::FinalizeViews()
{// the reason for HandlerData/FinalizeViews is to have a chance to ReplaceVars
	Array<HandlerData>& w = sHandlerData();
	for(int i = 0; i < w.GetCount(); i++) {
		const HandlerData& v = w[i];
		String p = v.path;
		if(*p == '/')
			p = p.Mid(1);
		else
			p = root + '/' + p;
		Vector<String> h = Split(ReplaceVars(p, view_var, '$'), ';');
		for(int i = 0; i < h.GetCount(); i++)
			RegisterView0(v.fn, v.cb, v.id, h[i], v.progress, i == 0);
	}
	w.Clear();
}

struct BestDispatch { // Information about the best dispatch node for given path
	Callback1<Http&> handler;
	int              matched_parts;  // number of matched path elements
	int              matched_params; // number of '*' arguments extracted
	Vector<String>&  arg;            // arguments extracted
	String           id;             // id of handler
	bool             post_raw;       // handler is POST_RAW type
	int(*progress)(int, Http&, int);
	
	BestDispatch(Vector<String>& arg) : arg(arg) { matched_parts = -1; matched_params = 0; post_raw = false; progress = NULL;}
};

void GetBestDispatch(int method,
                     const Vector<String>& part, int ii, const DispatchNode& n, Vector<String>& arg,                     
                     BestDispatch& bd, int matched_parts, int matched_params)
{// find the best DispatchNode for given path, best is the one with most path elements matched, if equal, more '*' used (not '**')
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(ii >= part.GetCount()) { // we have reached the end of path
		if(n.handler && n.method == method &&
		   (matched_parts > bd.matched_parts ||
		    matched_parts == bd.matched_parts && matched_params > bd.matched_params)) { // node has handler
		    LLOG("Matched " << n.id << " parts " << matched_parts << " params " << matched_params);
			bd.arg <<= arg;
			bd.handler = n.handler;
			bd.matched_parts = matched_parts;
			bd.id = n.id;
			bd.post_raw = n.post_raw;
			bd.progress = n.progress;
		}
		if(!bd.handler) { // node does not have handler, try '**' subnode
			int q = n.subnode.Find(String());
			while(q >= 0) {
				const DispatchNode& an = DispatchMap[n.subnode[q]];
				if(an.argpos == DISPATCH_VARARGS && an.handler && an.method == method) {
					bd.handler = an.handler;
					bd.id = n.id;
					bd.arg.Clear();
					bd.progress = n.progress;
					break;
				}
				q = n.subnode.FindNext(q);
			}
		}
		return;
	}
	int qq = n.subnode.Get(part[ii], -1);
	if(qq >= 0) // path element matched, try subnodes first
		GetBestDispatch(method, part, ii + 1, DispatchMap[qq], arg, bd, matched_parts + 1, matched_params);
	int q = n.subnode.Find(String());
	while(q >= 0) {
		int qq = n.subnode[q];
		int ac = arg.GetCount();
		const DispatchNode& an = DispatchMap[qq];
		int apos = an.argpos;
		LLOG(" *" << qq << " apos: " << apos);
		if(apos == DISPATCH_VARARGS) {
			if(an.handler && an.method == method &&
			   (matched_parts > bd.matched_parts || matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
			    LLOG("Matched VARARGS " << an.id << " parts " << matched_parts << " params " << matched_params);
				bd.arg <<= arg;
				bd.arg.Append(part, ii, part.GetCount() - ii);
				bd.handler = an.handler;
				bd.matched_parts = matched_parts;
				bd.id = an.id;
				bd.progress = an.progress;
			}
		}
		else {
			String pv;
			if(IsNull(apos))
				arg.Add(part[ii]);
			else {
				String& at = arg.At(apos);
				pv = at;
				at = part[ii];
			}
			GetBestDispatch(method, part, ii + 1, an, arg, bd, matched_parts, matched_params + 1);
			if(!IsNull(apos))
				arg[apos] = pv;
		}
		arg.SetCount(ac);
		q = n.subnode.FindNext(q);
	}
}

void SkylarkApp::NotFound(Http& http)
{
	http << "Page not found";
	http.Response(404, "Not found");
}

void SkylarkApp::SqlError(Http& http, const SqlExc& e)
{
	http << "Internal server error<br>SQL ERROR: " << e;
	http.Response(500, "Internal server error");
}

void SkylarkApp::InternalError(Http& http, const Exc& e)
{
	http << "Internal server error<br>" << e;
	http.Response(500, "Internal server error");
}

void SkylarkApp::Unauthorized(Http& http, const AuthExc& e)
{
	http << e;
	http.Response(403, "Unauthorized");
}

void SkylarkApp::BadRequest(Http& http, const BadRequestExc& e)
{
	http << "Bad request";
	http.Response(400, "Bad request");
}

void Http::WaitHandler(int (*progress)(int, Http&, int), TcpSocket *socket)
{
	if(!progress)
		return;
	if(!(*progress)(PROGRESS_CONTENT, *this, socket->GetDone()))
		socket->Abort();
	if(session_dirty)
		SaveSession();
	session_dirty = false;
}

void Http::Dispatch(TcpSocket& socket)
{
	const Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(hdr.Read(socket)) {
		rsocket = &socket;
		LLOG("--------------------------------------------");
		SKYLARKLOG(hdr.GetMethod() << " " << hdr.GetURI());
		LDUMP(content);
		var.Clear();
		arg.Clear();
		LTIMING("Request processing");
		request_content_type = GetHeader("content-type");
		String rc = ToLower(request_content_type);
		bool post = hdr.GetMethod() == "POST";

		String uri = hdr.GetURI();
		int q = uri.Find('?');
		if(q >= 0) {
			ParseRequest(~uri + q + 1);
			uri.Trim(q);
		}
		var.GetAdd(".__identity__"); // To make StdLib.icpp GetIndentity work, this has to be at index 0
		var.GetAdd("static") = app.static_dir;
		for(int i = hdr.fields.Find("cookie"); i >= 0; i = hdr.fields.FindNext(i)) {
			const String& h = hdr.fields[i];
			int q = 0;
			for(;;) {
				int qq = h.Find('=', q);
				if(qq < 0)
					break;
				String id = ToLower(TrimBoth(h.Mid(q, qq - q)));
				qq++;
				q = h.Find(';', qq);
				if(q < 0) {
					var.Add('@' + id, UrlDecode(h.Mid(qq)));
					break;
				}
				var.Add('@' + id, UrlDecode(h.Mid(qq, q - qq)));
				q++;
			}
		}
		LDUMPM(var);
		Vector<String> part = Split(uri, '/');
		for(int i = 0; i < part.GetCount(); i++)
			part[i] = UrlDecode(part[i]);
		LDUMPC(part);
		Vector<String> a;
		BestDispatch bd(arg);
		if(DispatchMap.GetCount())
			GetBestDispatch(post ? DispatchNode::POST : DispatchNode::GET, part, 0, DispatchMap[0], a, bd, 0, 0);
		LDUMPC(arg);
		
		// we need session loaded before progress handler is called
		try {
			if(SQL.IsOpen())
				SQL.Begin();
			LoadSession();
			session_dirty = false;

			int len = GetLength();
			if(bd.progress)
			{
				(*bd.progress)( PROGRESS_HEADER, *this, len);
				if(session_dirty)
					SaveSession();
				session_dirty = false;
				socket.WhenWait = callback2(this, &Http::WaitHandler, bd.progress, rsocket);
			}
			content = socket.GetAll(len);
			if(bd.progress) {
				socket.WhenWait.Clear();
				(*bd.progress)(PROGRESS_END, *this, len);
			}
			if(post) {
				if(rc.StartsWith("application/x-www-form-urlencoded"))
					ParseRequest(content);
				else
				if(rc.StartsWith("multipart/"))
					ReadMultiPart(content);
			}
			response.Clear();
			if(bd.handler) {
				if(post && !bd.post_raw) {
					String id = Nvl((*this)["__post_identity__"], (*this)["__js_identity__"]);
					if(id != (*this)[".__identity__"])
						throw AuthExc("identity error");
				}
				lang = Nvl(Int(".__lang__"), LNG_ENGLISH);
				Upp::SetLanguage(lang);
				var.GetAdd(".__lang__") = lang;
				var.GetAdd(".language") = ToLower(LNGAsText(lang));
				handlerid = bd.id;
				LDUMP(handlerid);
				bd.handler(*this);
				if(session_dirty)
					SaveSession();
				if(SQL.IsOpen())
					SQL.Commit();
			}
			else
				app.NotFound(*this);
		}
		catch(SqlExc e) {
			if(SQL.IsOpen())
				SQL.Rollback();
			app.SqlError(*this, e);
		}
		catch(AuthExc e) {
			if(SQL.IsOpen())
				SQL.Rollback();
			app.Unauthorized(*this, e);
		}
		catch(BadRequestExc e) {
			if(SQL.IsOpen())
				SQL.Rollback();
			app.BadRequest(*this, e);
		}
		catch(Exc e) {
			if(SQL.IsOpen())
				SQL.Rollback();
			app.InternalError(*this, e);
		}
		Finalize();
	}	
}

void Http::Finalize()
{
    if(rsocket) {
        SKYLARKLOG("=== Response: " << code << ' ' << code_text);
        String r;

        // weird apache2 mod_scgi behaviour
        if(hdr.scgi)
            r << "Status: ";
        else
            r << "HTTP/1.1 ";

        if(redirect.GetCount()) {
            // for SCGI (at least on apache 2 mod_scgi), we need protocol inside url
            if(hdr.scgi && redirect.Find(":") < 0)
                redirect = "http:" + redirect;

            SKYLARKLOG("Redirect to: " << redirect);
            r << code << " Found\r\n";
            r << "Location: " << redirect << "\r\n";
            for(int i = 0; i < cookies.GetCount(); i++)
                r << cookies[i];
        }
        else {
            r <<
                code << ' ' << code_text << "\r\n" 
                "Date: " <<  WwwFormat(GetUtcTime()) << "\r\n" 
                "Content-Length: " << response.GetCount() << "\r\n" 
                "Content-Type: " << content_type << "\r\n";
            for(int i = 0; i < headers.GetCount(); i++)
                r << headers.GetKey(i) << ": " << headers[i] << "\r\n";
            for(int i = 0; i < cookies.GetCount(); i++)
                r << cookies[i];
        }
        r << "\r\n";
        rsocket->PutAll(r);
        rsocket->PutAll(response);
        rsocket = NULL;
    }
}

};
