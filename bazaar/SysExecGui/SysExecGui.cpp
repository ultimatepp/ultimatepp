#include "SysExecGui.h"

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

#define IMAGECLASS SysExecGuiImg
#define IMAGEFILE <SysExecGui/SysExecGui.iml>
#include <Draw/iml.h>

#define LAYOUTFILE <SysExecGui/SysExecGui.lay>
#include <CtrlCore/lay.h>

///////////////////////////////////////////////////////////////////////////////////////
// password dialog
class PasswordDlg : public WithPasswordDlgLayout<TopWindow>
{
	private:
		String user;
		StaticRect r;
	
	protected:
	
	public:
	
		typedef PasswordDlg CLASSNAME;

		// constructor
		PasswordDlg();
		
		String GetPassword(void) { return ~pwd; }

		// starts dialog, return true on success
		bool Run(String const &u = "root");
		
};

PasswordDlg::PasswordDlg()
{
	Add(r.SizePos());
	r.Color(LtCyan);
	CtrlLayout(*this, GetExeTitle());
	pwd.Password();
	lock.SetImage(SysExecGuiImg::lock());
	title1 <<= t_("Please enter password to execute administrative tasks");
	title2 <<= Format(t_("[1 '%s' application can change important parts of your system]"), GetExeTitle());
	okBtn.Ok() <<= Acceptor(IDOK);
	cancelBtn.Cancel() <<= Rejector(IDCANCEL);
	VCenterPos().HCenterPos();
}

bool PasswordDlg::Run(String const &u)
{
	// ask for password
	user = u;
	while(true)
	{
		pwd.Clear();
		PopUp(NULL, false, false, true, true);
		pwd.SetFocus();
		switch(Execute())
		{
			case IDOK:
				// check the password running pwd
				if(SysExecUser(user, ~pwd, "/bin/pwd", ""))
					return true;
				PromptOK(DeQtf(t_("Wrong password -- Try again")));
				break;
			case IDCANCEL:
				return false;
			default:
				NEVER();
		}
	}
	return true;
}

static PasswordDlg &pDlg()
{
	static PasswordDlg p;
	return p;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysStartAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
//	PasswordDlg *pp = &(pDlg());
	if(!pDlg().Run())
		return false;
	return SysStartAdmin(pDlg().GetPassword(), command, args, Environ);
}

bool SysStartAdmin(String const &command, String const &args)
{
	if(!pDlg().Run())
		return false;
	return SysStartAdmin(pDlg().GetPassword(), command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it without waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysStartUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	if(!pDlg().Run(user))
		return false;
	return SysStartUser(user, pDlg().GetPassword(), command, args, Environ);
}

bool SysStartUser(String const &user, String const &command, String const &args)
{
	if(!pDlg().Run(user))
		return false;
	return SysStartUser(user, pDlg().GetPassword(), command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysExecAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	if(!pDlg().Run())
		return false;
	return SysExecAdmin(pDlg().GetPassword(), command, args, Environ);
}

bool SysExecAdmin(String const &command, String const &args)
{
	if(!pDlg().Run())
		return false;
	return SysExecAdmin(pDlg().GetPassword(), command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysExecUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	if(!pDlg().Run(user))
		return false;
	return SysExecUser(user, pDlg().GetPassword(), command, args, Environ);
}

bool SysExecUser(String const &user, String const &command, String const &args)
{
	if(!pDlg().Run(user))
		return false;
	return SysExecUser(user, pDlg().GetPassword(), command, args);
}

#else

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysStartAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	return SysStartAdmin("", command, args, Environ);
}

bool SysStartAdmin(String const &command, String const &args)
{
	return SysStartAdmin("", command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it without waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysStartUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	return SysStartUser(user, "", command, args, Environ);
}

bool SysStartUser(String const &user, String const &command, String const &args)
{
	return SysStartUser(user, "", command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysExecAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	return SysExecAdmin("", command, args, Environ);
}

bool SysExecAdmin(String const &command, String const &args)
{
	return SysExecAdmin("", command, args);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysExecUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	return SysExecUser(user, "", command, args, Environ);
}

bool SysExecUser(String const &user, String const &command, String const &args)
{
	return SysExecUser(user, "", command, args);
}

#endif

END_UPP_NAMESPACE
