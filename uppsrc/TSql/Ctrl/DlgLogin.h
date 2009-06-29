#ifndef __TSql_Ctrl_DlgLogin_h__
#define __TSql_Ctrl_DlgLogin_h__

NAMESPACE_UPP

#define LAYOUTFILE <TSql/Ctrl/DlgLogin.lay>
#include <CtrlCore/lay.h>

class DlgLogin : public WithLoginLayout<TopWindow>
{
public:
	typedef DlgLogin CLASSNAME;

	DlgLogin();

	typedef Callback2<String /* session_type:user/pwd@server */, One<SqlSession>&> LoginProc;
	typedef Gate2<SqlSession&, String> PasswordProc;

	One<SqlSession>   Login(LoginProc login_proc, String& connect);
	void              Progress(String text);
	bool              ProgressCanceled(String text);

	enum STYLE { EDIT_STYLE, PROGRESS_STYLE };
	void              SetStyle(STYLE style);
	void              ClearCanceled() { canceled = false; }

	bool              OnProgress(int done, int total);
	bool              IsCanceled() const { return canceled; }
	operator          Gate2<int, int> () { return THISBACK(OnProgress); }
	operator          Gate1<String> ()   { return THISBACK(ProgressCanceled); }

	void              Serialize(Stream& stream);

public:
	PasswordProc      WhenSetPassword;

protected:
	void              OnAbort();
	bool              StdSetPassword(SqlSession& session, String new_pwd);

protected:
	String            action_format;
	int               next_ticks;
	bool              canceled;
	STYLE             style;
};

END_UPP_NAMESPACE

#endif
