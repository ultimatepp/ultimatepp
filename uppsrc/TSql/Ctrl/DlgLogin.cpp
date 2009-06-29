//////////////////////////////////////////////////////////////////////
// DlgLogin: login dialog.

#include "TSqlCtrl.h"
#pragma hdrstop

NAMESPACE_UPP

String Garble(const char *s)
{
	int c = 0xAA;
	String result;
	while(*s) {
		result.Cat(*s++ ^ (char)c);
		if((c <<= 1) & 0x100)
			c ^= 0x137;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////
// DlgLoginSetPass::

static int CharFilterToUpper(int i) { return ToUpper(i); }

class DlgLoginSetPass : public WithSetPasswordLayout<TopWindow>
{
public:
	DlgLoginSetPass();

	String      Run(SqlSession& session, Gate2<SqlSession&, String> setpwd);
};

RegisterHelpTopicObjectTitle(DlgLoginSetPass, t_("Change password"))

DlgLoginSetPass::DlgLoginSetPass()
{
	user.SetReadOnly();
	password.Password();
//	password.SetFilter(&CharFilterToUpper);
	password2.Password();
//	password2.SetFilter(&CharFilterToUpper);
	CtrlLayoutOKCancel(*this, DlgLoginSetPassHelpTitle());
	HelpTopic("DlgLoginSetPass");
}

String DlgLoginSetPass::Run(SqlSession& session, Gate2<SqlSession&, String> setpwd)
{
	user = session.GetUser().ToWString();
	ActiveFocus(password);
	while(TopWindow::Run() == IDOK) {
		if(~password != ~password2)
			Exclamation(t_("The entered passwords differ. Please check typing errors."));
		else if(setpwd(session, ~password))
			return ~password;
		else
			Exclamation(NFormat(t_("Error changing user password.\n[* \1%s\1]"), session.GetLastError()));
	}
	return String::GetVoid();
}

//////////////////////////////////////////////////////////////////////
// DlgLogin::

RegisterHelpTopicObjectTitle(DlgLogin, t_("System login"))

static const Display& StdGradientDisplay()
{
	static GradientDisplay gd(Point(0, 0), White, Point(-1, -1), Yellow);
	return gd;
}

DlgLogin::DlgLogin()
{
	CtrlLayoutOKCancel(*this, DlgLoginHelpTitle());
	HelpTopic("DlgLogin");
	Background(StdGradientDisplay());
	Zoom screen(1, 1);
	app.SetZoom(screen);
	version.SetZoom(screen);
	date.SetZoom(screen);
	copyright.SetZoom(screen);
//	app.Font(Roman(1).Bold().Italic());
//	version.Font(GuiFont().Bold());
//	user.SetFilter(&CharFilterToUpper);
	password.Password();
//	password.SetFilter(&CharFilterToUpper);
//	database.SetFilter(&CharFilterToUpper);
//	server.SetFilter(&CharFilterToUpper);
	SetStyle(PROGRESS_STYLE);
	abort <<= THISBACK(OnAbort);
	ClearCanceled();
	WhenSetPassword = THISBACK(StdSetPassword);
}

One<SqlSession> DlgLogin::Login(LoginProc loginproc, String& connect_string)
{
	SetStyle(EDIT_STYLE);
	ClearCanceled();

	session_type_tag.Show(session_type.GetCount());
	session_type.Show(session_type.GetCount());
//	session_type.SetEditable(session_type.GetCount() > 1);

	if(!connect_string.IsEmpty()) {
		const char *p = connect_string, *b = p;
		while(*p && *p != ':' && *p != '/' && *p != '@')
			p++;
		if(*p == ':') {
			session_type <<= String(b, p);
			b = ++p;
		}
		while(*p && *p != '/' && *p != '@')
			p++;
		user <<= String(b, p);
		if(*p == '/') {
			b = ++p;
			while(*p && *p != '@')
				p++;
			password <<= String(b, p);
		}
		if(*p == '@') {
			b = ++p;
			while(*p && *p != '/')
				p++;
			if(*p == '/') {
				database <<= String(b, p);
				b = ++p;
			}
			server <<= b;
		}
	}

	if(password.GetText().IsEmpty() && !user.GetText().IsEmpty())
		ActiveFocus(password);
	else
		ActiveFocus(user);

	set_password <<= Acceptor(IDRETRY);

	for(;;) {
		int code = TopWindow::Run();
		if(code == IDCANCEL)
			return NULL;
		String t = ~session_type, u = ~user, p = ~password, d = ~database, s = ~server;
		String connect;
		if(!IsNull(t))
			connect << t << ':';
		connect << u;
		if(!IsNull(p))
			connect << '/' << p;
		if(!IsNull(s) || !IsNull(d)) {
			connect << '@';
			if(!IsNull(d))
				connect << d << '/';
			connect << s;
		}

		One<SqlSession> session;

		{
			WaitCursor wait;
			loginproc(connect, session);
		}

		if(!session || !session -> IsOpen()) {
			Exclamation(NFormat(t_("Error creating database connection.&[* \1%s\1]&User name: [* \1%s\1]&Database: [* \1%s\1]&Server: [* \1%[(default)]~s\1]&Connection type: [* \1%[(default)]~s\1]"),
				(session ? session->GetLastError() : String(t_("(unknown error)"))),
				u, d, s, t));
			continue;
		}

		if(code == IDRETRY) { // set password
			String pwd = DlgLoginSetPass().Run(*session, WhenSetPassword);
			if(IsNull(pwd))
				continue;
			else
				password = pwd.ToWString();
		}

		connect_string = connect;
		return session;
	}
	return 0;
}

void DlgLogin::Serialize(Stream& stream)
{
	int version = 2;
	String pwd;
	if(save_password && stream.IsStoring())
		pwd = Garble(password.GetText().ToString());
	stream / version % user % pwd;
	if(version >= 2)
		stream % database;
	stream % server % save_password % session_type;
	if(save_password && stream.IsLoading())
		password = Garble(pwd).ToWString();
}

void DlgLogin::SetStyle(STYLE s)
{
	style = s;
	bool ed = (s == EDIT_STYLE);
	bool pg = (s == PROGRESS_STYLE);
	user.Enable(ed);
	password.Enable(ed);
	database.Enable(ed);
	server.Enable(ed);
	session_type.Enable(ed);
	save_password.Enable(ed);
	ok.Show(ed);
	cancel.Show(ed);
	set_password.Show(ed);
	progress.Show(pg);
	abort.Show(pg);
	if(ed)
		ActiveFocus(user);
	else
		ActiveFocus(abort);
	next_ticks = 0;
}

void DlgLogin::Progress(String text)
{
	if(style != PROGRESS_STYLE)
		SetStyle(PROGRESS_STYLE);

	progress.SetLabel(NFormat(action_format = text, 0));
	ProcessEvents();
}

bool DlgLogin::ProgressCanceled(String text)
{
	Progress(text);
	return OnProgress(0, 0);
}

bool DlgLogin::OnProgress(int done, int total)
{
	if((int)(GetTickCount() - next_ticks) >= 0) {
		progress.SetLabel(NFormat(action_format, done));
		ProcessEvents();
		next_ticks = GetTickCount() + 200;
	}
	return canceled;
}

void DlgLogin::OnAbort()
{
	canceled = true;
}

bool DlgLogin::StdSetPassword(SqlSession& session, String new_pwd)
{
	String cmd;
	switch(session.GetDialect())
	{
	case ORACLE:
		cmd = NFormat("alter user %s identified by %s", session.GetUser(), new_pwd);
		break;

	case MY_SQL:
	case SQLITE3:
		cmd = NFormat("set password for %s=PASSWORD('%s')", session.GetUser(), new_pwd);
		break;

	default:
		session.SetError(t_("Password setting not supported by active database connection."), "DlgLogin/SetPassword");
		return false;
	}
	Sql cursor(session);
	return cursor.Execute(cmd);
}

END_UPP_NAMESPACE
