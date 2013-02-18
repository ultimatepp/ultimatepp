#include "Skylark.h"

#define LLOG(x)    //DLOG(x)
#define LDUMPC(x)  //DDUMPC(x)
#define LDUMPM(x)  //DDUMPM(x)
#define LLOGHEX(x) //DLOGHEX(x)

namespace Upp {

namespace Ini {
INI_STRING(session_cookie, "__skylark_session_cookie__", "Skylark session cookie ID");
INI_STRING(session_dir, ConfigFile("session"), "Directory that contains Skylark session files");
INI_STRING(session_format, "BINARY", "Skylark session format ('BINARY', 'JSON', 'XML')");
INI_STRING(session_table, "", "SQL table used to store Skylark sessions; if empty, sessions are stored in files");
INI_STRING(session_id_column, "ID", "Primary key of SQL table used to store Skylark sessions");
INI_STRING(session_data_column, "DATA", "Name of SQL 'text' column used to store Skylark sessions data");
INI_STRING(session_lastwrite_column, "LASTWRITE", "Name of SQL timestamp column used to store Skylark session last update time");
INI_INT(session_expire, 3600 * 24 * 365, "Number of seconds after which Skylark session expires and can be deleted");
};

SkylarkSessionConfig::SkylarkSessionConfig()
{
	cookie = Ini::session_cookie;
	dir = Ini::session_dir;
	format = Ini::session_format == "JSON" ? SESSION_FORMAT_JSON :
	         Ini::session_format == "XML" ?  SESSION_FORMAT_XML :
	                                         SESSION_FORMAT_BINARY;
	id_column = (String)Ini::session_id_column;
	data_column = (String)Ini::session_data_column;
	lastwrite_column = (String)Ini::session_lastwrite_column;
	expire = Ini::session_expire;
	table = (String)Ini::session_table;
}

String Http::SessionFile(const String& sid)
{
	ONCELOCK
		RealizeDirectory(app.session.dir);
	return AppendFileName(app.session.dir, sid);
}

void Http::LoadSession()
{
	const SkylarkSessionConfig& cfg = app.session;
	session_var.Clear();
	session_id = (*this)['@' + cfg.cookie];
	if(IsNull(session_id))
		return;
	String data;
	if(cfg.table.IsNull())
		data = LoadFile(SessionFile(session_id));
	else
		data = SQLR % Select(cfg.data_column).From(cfg.table)
		              .Where(cfg.id_column == session_id);
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
		var.GetAdd('.' + session_var.GetKey(i)) = session_var[i];
}

thread__ int s_exp;

static int  s_last_expiration_check;

StaticMutex expire_mutex;

void Http::SaveSession()
{
	const SkylarkSessionConfig& cfg = app.session;
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
		if(SQL.GetRowsProcessed() == 0 // MySql returns zero even if row exist when columns are equal, therefore:
		   && IsNull(SQL % Select(cfg.id_column).From(cfg.table).Where(cfg.id_column == session_id)))
				SQL * Insert(cfg.table)
				        (cfg.id_column, session_id)
						(cfg.data_column, d)
						(cfg.lastwrite_column, tm);
	}
	SKYLARKLOG("Stored session: " << session_id);
	LDUMPM(session_var);
	
	if((s_exp++ % 1000) == 0) {
		bool expire_sessions = false;
		{
			Mutex::Lock __(expire_mutex);
			if((dword)msecs(s_last_expiration_check) > 1000 * 60 * 10) {
				expire_sessions = true;
				s_last_expiration_check = msecs();
			}
		}
		if(expire_sessions) {
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
}

Http& Http::ClearSession()
{
	session_var.Clear();
	session_id.Clear();
	session_dirty = true;
	return *this;
}

void Http::SessionSet0(const char *id, const Value& value)
{
	LLOG("SessionSet0 " << id << " = " << value);
	if(IsNull(session_id))
		NewSessionId();
	session_var.GetAdd(id) = value;
	session_dirty = true;
}

Http& Http::SessionSet(const char *id, const Value& value)
{
	LLOG("SessionSet " << id << " = " << value);
	if(*id == '.')
		id++;
	SessionSet0(id, value);
	var.GetAdd('.' + String(id)) = value;
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
	Renderer::SetLanguage(lang_);
	Upp::SetLanguage(lang_);
	SessionSet("__lang__", lang);
	SessionSet("language", ToLower(LNGAsText(lang)));
	return *this;
}

};