#include "Skylark.h"

#define LLOG(x)     //DLOG(x)
#define LDUMP(x)    //DDUMP(x)
#define LDUMPC(x)   //DDUMPC(x)
#define LDUMPM(x)   //DDUMPM(x)
#define LTIMING(x)  //RTIMING(x)

namespace Upp {

enum { DISPATCH_VARARGS = -1 };

struct DispatchNode : Moveable<DispatchNode> {
	VectorMap<String, int> subnode;
	void   (*view)(Http&);
	int    argpos;
	int    method;
	bool   post_raw;
	String id;
	
	enum { GET, POST };

	DispatchNode() { view = NULL; argpos = Null; method = GET; post_raw = false; }
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

static Index<uintptr_t>& sViewIndex()
{
	static Index<uintptr_t> x;
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
		LLOG(i << " " << (bool)DispatchMap[i].view << ": " << sub);
	}			
}

Vector<String> *GetUrlViewLinkParts(const String& id)
{
	int q = sLinkMap().Find(id);
	if(q < 0)
		return NULL;
	return &sLinkMap()[q];
}

String MakeLink(void (*view)(Http&), const Vector<Value>& arg)
{
	int q = sViewIndex().Find((uintptr_t)view);
	if(q < 0)
		throw Exc("Invalid view");
	if(q < 0)
		return String();
	StringBuffer out;
	MakeLink(out, sLinkMap()[q], arg);
	return out;
}

void RegisterView0(void (*view)(Http&), const char *id, String path, bool primary)
{
	LLOG("RegisterView " << path);
	Vector<String>& linkpart = sLinkMap().GetAdd(id);
	sViewIndex().FindAdd((uintptr_t)view);
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
	ASSERT_(!DispatchMap[q].view, "duplicate view " + String(path));
	DispatchMap[q].view = view;
	DispatchMap[q].method = method;
	DispatchMap[q].id = id;
	DispatchMap[q].post_raw = post_raw;
//	DumpDispatchMap();
}

struct ViewData {
	void (*view)(Http&);
	String id;
	String path;
};

static Array<ViewData>& sViewData()
{
	static Array<ViewData> x;
	return x;
}

void RegisterHandler(void (*view)(Http&), const char *id, const char *path)
{
	Array<ViewData>& v = sViewData();
	ViewData& w = v.Add();
	w.view = view;
	w.id = id;
	w.path = path;
}

void SkylarkApp::FinalizeViews()
{
	Array<ViewData>& w = sViewData();
	for(int i = 0; i < w.GetCount(); i++) {
		const ViewData& v = w[i];
		ASSERT_(sViewIndex().Find((uintptr_t)v.view) < 0, "duplicate view function registration " + String(v.id));
		String p = v.path;
		if(*p == '/')
			p = p.Mid(1);
		else
			p = root + '/' + p;
		Vector<String> h = Split(ReplaceVars(p, view_var, '$'), ';');
		for(int i = 0; i < h.GetCount(); i++)
			RegisterView0(v.view, v.id, h[i], i == 0);
	}
	w.Clear();
}

struct BestDispatch {
	void            (*view)(Http&);
	int             matched_parts;
	int             matched_params;
	Vector<String>& arg;
	String          id;
	bool            post_raw;
	
	BestDispatch(Vector<String>& arg) : arg(arg) { matched_parts = -1; matched_params = 0; view = NULL; post_raw = false; }
};

void GetBestDispatch(int method,
                     const Vector<String>& part, int ii, const DispatchNode& n, Vector<String>& arg,                     
                     BestDispatch& bd, int matched_parts, int matched_params)
{
	Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(ii >= part.GetCount()) {
		if(n.view && n.method == method &&
		   (matched_parts > bd.matched_parts ||
		    matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
		    LLOG("Matched " << n.id << " parts " << matched_parts << " params " << matched_params);
			bd.arg <<= arg;
			bd.view = n.view;
			bd.matched_parts = matched_parts;
			bd.id = n.id;
			bd.post_raw = n.post_raw;
		}
		if(!bd.view) {
			int q = n.subnode.Find(String());
			while(q >= 0) {
				const DispatchNode& an = DispatchMap[n.subnode[q]];
				if(an.argpos == DISPATCH_VARARGS && an.view && an.method == method) {
					bd.view = an.view;
					bd.id = n.id;
					bd.arg.Clear();
					break;
				}
				q = n.subnode.FindNext(q);
			}
		}
		return;
	}
	int qq = n.subnode.Get(part[ii], -1);
	if(qq >= 0)
		GetBestDispatch(method, part, ii + 1, DispatchMap[qq], arg, bd, matched_parts + 1, matched_params);
	int q = n.subnode.Find(String());
	while(q >= 0) {
		int qq = n.subnode[q];
		int ac = arg.GetCount();
		const DispatchNode& an = DispatchMap[qq];
		int apos = an.argpos;
		LLOG(" *" << qq << " apos: " << apos);
		if(apos == DISPATCH_VARARGS) {
			if(an.view && an.method == method &&
			   (matched_parts > bd.matched_parts || matched_parts == bd.matched_parts && matched_params > bd.matched_params)) {
			    LLOG("Matched VARARGS " << an.id << " parts " << matched_parts << " params " << matched_params);
				bd.arg <<= arg;
				bd.arg.Append(part, ii, part.GetCount() - ii);
				bd.view = an.view;
				bd.matched_parts = matched_parts;
				bd.id = an.id;
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

void Http::Dispatch(TcpSocket& socket)
{
	const Vector<DispatchNode>& DispatchMap = sDispatchMap();
	if(hdr.Read(socket)) {
		int len = GetLength();
		content = socket.GetAll(len);
		LLOG("--------------------------------------------");
		SKYLARKLOG(hdr.GetMethod() << " " << hdr.GetURI());
		LDUMP(content);
		String r;
		var.Clear();
		arg.Clear();
		LTIMING("Request processing");
		request_content_type = GetHeader("content-type");
		String rc = ToLower(request_content_type);
		bool post = hdr.GetMethod() == "POST";
		if(post)
			if(rc.StartsWith("application/x-www-form-urlencoded"))
				ParseRequest(content);
			else
			if(rc.StartsWith("multipart/"))
				ReadMultiPart(content);
		String uri = hdr.GetURI();
		int q = uri.Find('?');
		if(q >= 0) {
			if(!post)
				ParseRequest(~uri + q + 1);
			uri.Trim(q);
		}
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
		var.GetAdd("static") = app.static_dir;
		var.GetAdd(".__identity__"); // To make StdLib.icpp GetIndentity work without changing preset stack positions
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
		response.Clear();
		if(bd.view) {
			try {
				if(SQL.IsOpen())
					SQL.Begin();
				LoadSession();
				session_dirty = false;
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
				LDUMP(viewid);
				(*bd.view)(*this);
				if(session_dirty)
					SaveSession();
				if(SQL.IsOpen())
					SQL.Commit();
			}
			catch(SqlExc e) {
				if(SQL.IsOpen())
					SQL.Rollback();
				response << "Internal server error<br>"
				         << "SQL ERROR: " << e;
				code = 500;
				code_text = "Internal server error";
				app.SqlError(*this);
			}
			catch(AuthExc e) {
				if(SQL.IsOpen())
					SQL.Rollback();
				response << e;
				code = 403;
				code_text = "Unauthorized";
				app.Unauthorized(*this);
			}
			catch(BadRequestExc e) {
				if(SQL.IsOpen())
					SQL.Rollback();
				response << "Bad request";
				code = 400;
				code_text = "Bad request";
				app.Unauthorized(*this);
			}
			catch(Exc e) {
				if(SQL.IsOpen())
					SQL.Rollback();
				response << "Internal server error<br>"
				         << e;
				code = 500;
				code_text = "Internal server error";
				app.InternalError(*this);
			}
		}
		else {
			response << "Page not found";
			code = 404;
			code_text = "Not found";
			app.NotFound(*this);
		}
		r.Clear();
		SKYLARKLOG("=== Response: " << code << ' ' << code_text);
		if(redirect.GetCount()) {
			SKYLARKLOG("Redirect to: " << redirect);
			r << "HTTP/1.1 " << code << " Found\r\n";
			r << "Location: " << redirect << "\r\n";
			for(int i = 0; i < cookies.GetCount(); i++)
				r << cookies[i];
		}
		else {
			r <<
				"HTTP/1.0 " << code << ' ' << code_text << "\r\n"
				"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
				"Server: U++\r\n"
				"Content-Length: " << response.GetCount() << "\r\n"
				"Connection: close\r\n"
				"Cache-Control: no-cache\r\n"
				"Content-Type: " << content_type << "\r\n";
			for(int i = 0; i < cookies.GetCount(); i++)
				r << cookies[i];
			r << "\r\n";
		}
		socket.PutAll(r);
		socket.PutAll(response);
	}
}

};