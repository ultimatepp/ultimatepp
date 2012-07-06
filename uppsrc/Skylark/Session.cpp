#include "Skylark.h"

#define LLOG(x)    //DLOG(x)
#define LDUMPC(x)  //DDUMPC(x)
#define LDUMPM(x)  //DDUMPM(x)
#define LLOGHEX(x) //DLOGHEX(x)

namespace Upp {

SessionConfig::SessionConfig()
{
	cookie = "__skylark_session_cookie__";
	dir = ConfigFile("session");
	format = SESSION_FORMAT_BINARY;
	id_column = "ID";
	data_column = "DATA";
	lastwrite_column = "LASTWRITE";
	expire = 3600 * 24 * 365; // one year to expire the session
}

String Http::SessionFile(const String& sid)
{
	ONCELOCK
		RealizeDirectory(app.session.dir);
	return AppendFileName(app.session.dir, sid);
}

void Http::LoadSession()
{
	const SessionConfig& cfg = app.session;
	session_var.Clear();
	session_id = (*this)[cfg.cookie];
	if(IsNull(session_id))
		return;
	String data;
	if(cfg.table.IsNull())
		data = LoadFile(SessionFile(session_id));
	else
		data = SQLR % Select(cfg.data_column).From(cfg.table)
		              .Where(cfg.id_column == session_id);
	LLOGHEX(data);
	switch(cfg.format) {
	case SESSION_FORMAT_JSON:
		LoadFromJson(session_var, data);
		break;
	case SESSION_FORMAT_XML:
		LoadFromXML(session_var, data);
		break;
	case SESSION_FORMAT_BINARY:
		LoadFromString(session_var, data);
		break;
	}
	SKYLARKLOG("Loaded session: " << session_id);
	LDUMPM(session_var);
	for(int i = 0; i < session_var.GetCount(); i++)
		var.GetAdd(session_var.GetKey(i)) = session_var[i];
}

thread__ int s_exp;

void Http::SaveSession()
{
	const SessionConfig& cfg = app.session;
	SetCookie(cfg.cookie, session_id);
	if(IsNull(session_id))
		return;
	String data;
	switch(cfg.format) {
	case SESSION_FORMAT_JSON:
		data = StoreAsJson(session_var);
		break;
	case SESSION_FORMAT_XML:
		data = StoreAsXML(session_var, "session");
		break;
	case SESSION_FORMAT_BINARY:
		data = StoreAsString(session_var);
		break;
	}
	if(cfg.table.IsNull())
		SaveFile(SessionFile(session_id), data);
	else {
		SqlVal d = SqlBinary(data);
		Time tm = GetSysTime();
		SQL * Update(cfg.table)
				(cfg.data_column, d)
				(cfg.lastwrite_column, tm)
		      .Where(cfg.id_column == session_id);
		if(SQL.GetRowsProcessed() == 0)
			SQL * Insert(cfg.table)
			        (cfg.id_column, session_id)
					(cfg.data_column, d)
					(cfg.lastwrite_column, tm);
	}
	SKYLARKLOG("Stored session: " << session_id);
	LDUMPM(session_var);
	
	if((s_exp++ % 1000) == 0) {
		Time tm = GetSysTime() - cfg.expire;
		SKYLARKLOG("Expiring sessions older than " << tm);
		if(cfg.table.IsNull()) {
			FindFile ff(AppendFileName(cfg.dir, "*.*"));
			Vector<String> todelete;
			while(ff) {
				if(ff.GetLastWriteTime() < tm)
					todelete.Add(ff.GetPath());
				ff.Next();
			}
			for(int i = 0; i < todelete.GetCount(); i++)
				FileDelete(todelete[i]);
		}
		else
			SQL * Delete(cfg.table).Where(cfg.lastwrite_column < tm);
	}
}

Http& Http::ClearSession()
{
	session_var.Clear();
	session_id.Clear();
	return *this;
}

Http& Http::SessionSet(const char *id, const Value& value)
{
	LLOG("SessionSet " << id << " = " << value);
	if(IsNull(session_id))
		NewSessionId();
	session_var.GetAdd(id) = value;
	var.GetAdd(id) = value;
	session_dirty = true;
	return *this;
}

Http& Http::NewSessionId()
{
	session_id = AsString(Uuid::Create());
	session_dirty = true;
	return *this;
}

Http& Http::SetLanguage(int lang_)
{
	lang = lang_;
	Upp::SetLanguage(lang_);
	SessionSet("__lang__", lang);
	SessionSet("language", ToLower(LNGAsText(lang)));
	return *this;
}

};