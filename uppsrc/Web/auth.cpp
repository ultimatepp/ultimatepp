#include "Web.h"

NAMESPACE_UPP

enum { EXPIRE = 60 * 30 }; // 30 minutes inactivity expires token

static Array<WebUser>          user_map;
static CriticalSection         user_lock;
static Index<String>           token_map;
static Vector<Time>            token_expire;
static Vector<String>          token_args;
static CriticalSection         token_lock;

//////////////////////////////////////////////////////////////////////
// WebUser::

void WebUser::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version != 1)
	{
		stream.SetError();
		return;
	}
	stream % name;
	String p;
	if(stream.IsStoring())
		p = Garble(pass);
	stream % p;
	if(stream.IsLoading())
		pass = Garble(p);
	stream % args;
}

//////////////////////////////////////////////////////////////////////

String WebUser::CheckToken(String token)
{
	CriticalSection::Lock __(token_lock);
	int i = token_map.Find(token);
	if(i < 0)
		return String::GetVoid();
	Time time = GetSysTime();
	if(time <= token_expire[i])
	{
		token_expire[i] = time + EXPIRE;
		return token_args[i];
	}

	token_map.Remove(i);
	token_expire.Remove(i);
	token_args.Remove(i);
	return String::GetVoid();
}

//////////////////////////////////////////////////////////////////////

String WebUser::CreateToken(String args)
{
	String token;
	for(;;)
	{
		char buffer[128];
		int len = 30 + rand() % 10;
		for(int i = 0; i < len; i++)
			buffer[i] = 'A' + rand() % ('Z' - 'A' + 1);
		token = String(buffer, len);
		token_lock.Enter();
		if(token_map.Find(token) < 0)
			break;
		token_lock.Leave();
	}
	token_map.Add(token);
	token_expire.Add(GetSysTime() + EXPIRE);
	token_args.Add(args);
	token_lock.Leave();
	return token;
}

//////////////////////////////////////////////////////////////////////

void WebUser::DeleteToken(String token)
{
	CriticalSection::Lock guard(token_lock);
	int i = token_map.Find(token);
	if(i >= 0)
	{
		token_map.Remove(i);
		token_expire.Remove(i);
		token_args.Remove(i);
	}
}

//////////////////////////////////////////////////////////////////////

void WebUser::SetMap(const Array<WebUser>& map)
{
	CriticalSection::Lock guard(user_lock);
	user_map <<= map;
}

//////////////////////////////////////////////////////////////////////

Array<WebUser> WebUser::GetMap()
{
	CriticalSection::Lock guard(user_lock);
	return Array<WebUser>(user_map, 0);
}

//////////////////////////////////////////////////////////////////////

void WebUser::SerializeMap(String& s, bool write)
{
	StringStream stream;
	if(!write)
		stream.Open(Garble(Decode64(s)));

	int version = StreamHeading(stream, 1, 1, 1, "WebUser");
	if(version >= 1)
	{
		CriticalSection::Lock guard(user_lock);
		stream % user_map;
	}

	if(write)
	{
		String st = stream.GetResult();
		LOG("stream > " << st.GetLength());
		st = Garble(st);
		LOG("Garble > " << st.GetLength());
		s = Encode64(st);
		LOG("Encode = " << s);
	}
}

//////////////////////////////////////////////////////////////////////

void WebUser::ClearMap()
{
	CriticalSection::Lock guard(user_lock);
	user_map.Clear();
}

//////////////////////////////////////////////////////////////////////

void WebUser::SetUser() const
{
	CriticalSection::Lock guard(user_lock);
	for(int i = 0; i < user_map.GetCount(); i++)
		if(user_map[i].name == name)
		{
			user_map[i].pass = pass;
			user_map[i].args = args;
		}
	WebUser& u = user_map.Add();
	u.name = name;
	u.pass = pass;
	u.args = args;
}

//////////////////////////////////////////////////////////////////////

String WebUser::GetArgs() const
{
	CriticalSection::Lock guard(user_lock);
	LOG("name = " << name << ", pass = " << pass);
	LOG("WebUser::GetArgs; user map # = " << user_map.GetCount());
	for(int t = 0; t < user_map.GetCount(); t++)
		LOG(user_map[t].name << "/" << user_map[t].pass << ": " << (user_map[t].name == name));
	for(int i = 0; i < user_map.GetCount(); i++)
		if(user_map[i].name == name) {
			if(user_map[i].pass != pass) break;
			LOG("index found = " << i);
			return user_map[i].args;
		}
	return String::GetVoid();
}

//////////////////////////////////////////////////////////////////////

static String GetConnectPage(HttpQuery& out_map, const HttpQuery& map, const String& error)
{
	out_map.Set("AUTH", "1");

	Htmls body;
	body << HtmlRow() % (HtmlCell() / t_("Username:&nbsp;")
		+ HtmlCell() / HtmlEdit("USER", 20, 30, map.GetString("USER")))
		<< HtmlRow() % (HtmlCell() / t_("Password:&nbsp;")
		+ HtmlCell() / HtmlEdit("PASS", 20, 30, map.GetString("PASS"), true))
		<< HtmlRow() % (HtmlCell() / "" + HtmlCell() / HtmlButton("LOGIN", "  Pripojit  "));

	Htmls out;
	if(!IsNull(error))
		out << HtmlWarn() / error << "\n<p>\n";
	out << HtmlTable() % body;
	return out;
}

//////////////////////////////////////////////////////////////////////

static String GetSetPassPage(HttpQuery& out_map, const HttpQuery& map, const String &error)
{
	out_map.Set("USER", map.GetString("USER"));
	out_map.Set("AUTH", map.GetString("AUTH"));
	Htmls out;
	out << HtmlTag("H2") / t_("Connection succeeded")
		<< t_("\n\n<p>Connection to remote computer has succeeded. ")
		<< t_("Access code will loose validity after half hour of client inactivity or on explicit request.\n")
		<<
		"\n"
		"<p>";
	if(!IsNull(error))
		out << HtmlWarn() / error << "\n<p>\n";
	out
		<< HtmlTable() % (HtmlRow() % (HtmlCell() / t_("Change password:&nbsp;")
		+ HtmlCell() / HtmlEdit("NEW_PASS", 20, 30, map.GetString("NEW_PASS"), true))
		+ HtmlRow() % (HtmlCell() / t_("Repeat password:&nbsp;")
		+ HtmlCell() / HtmlEdit("RETYPE_PASS", 20, 30, map.GetString("RETYPE_PASS"), true))
		+ HtmlRow() % (HtmlCell() + HtmlCell() / (HtmlButton("SET_PASS", "Change password")
		+ "&nbsp;&nbsp;&nbsp;" + HtmlButton("CANCEL", t_(" Continue ")))))
		<< "\n";
	return out;
}

//////////////////////////////////////////////////////////////////////

String WebUser::Login(const HttpQuery& map, String& args, const char *iname, bool *save_cfg)
{
	String isapi_name;
	if(iname && *iname)
		isapi_name = iname;
	else
		isapi_name = map.GetString("$$PATH");
	HttpQuery out_map;
	String out;
	String auth = map.GetString("AUTH");
	if(auth.GetLength() == 1)
		auth = Null;
	args = CheckToken(auth);
	if(!args.IsVoid())
	{
		if(map.GetBool("SET_PASS"))
		{ // set password
			String p1 = map.GetString("NEW_PASS");
			String p2 = map.GetString("RETYPE_PASS");
			if(IsNull(p1))
				out = GetSetPassPage(out_map, map, t_("Please enter a new password."));
			else if(p1 != p2)
				out = GetSetPassPage(out_map, map, t_("Please enter the password twice."));
			else
			{ // set it now
				WebUser wu(map.GetString("USER"), p1, args);
				wu.SetUser();
				if(save_cfg)
					*save_cfg = true;
				return Null;
			}
		}
		else
			return Null;
	}

	if(IsNull(auth) && !map.IsEmpty("LOGIN"))
	{
		WebUser user;
		user.name = map.GetString("USER");
		user.pass = map.GetString("PASS");
		LOG("WID_LOGIN; name = " << user.name << ", pass = " << user.pass);
		String args;
		if(!IsNull(user.name) && !IsNull(user.pass) && !(args = user.GetArgs()).IsVoid())
		{
			out = GetSetPassPage(out_map, map, Null);
			out_map.Set("AUTH", WebUser::CreateToken(args));
		}
	}

	if(IsNull(out))
		out = GetConnectPage(out_map, map,
		!IsNull(auth) ? t_("Password has expired. Please login again.")
		: !map.IsEmpty("LOGIN") ? t_("Invalid password and/or username") : "");

	String hid = out_map.GetHidden();
	return HtmlForm(isapi_name) % (hid + out);
}

//////////////////////////////////////////////////////////////////////

END_UPP_NAMESPACE
