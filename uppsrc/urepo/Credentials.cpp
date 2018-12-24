#include "urepo.h"

String GetSvnDir(const String& p)
{
	String dir = p;
	while(dir.GetCount() > 4) {
		if(DirectoryExists(AppendFileName(dir, ".svn")) || DirectoryExists(AppendFileName(dir, "_svn")))
			return dir;
		dir = GetFileFolder(dir);
	}
	return Null;
}

struct CredentialsDlg : WithCredentialsLayout<TopWindow> {
	RepoSync& rs;

	void SyncShowPwds();
	void GetResults();

	typedef CredentialsDlg CLASSNAME;
	CredentialsDlg(RepoSync& rs);
};

void CredentialsDlg::GetResults()
{
	for(int i = 0; i < list.GetCount(); i++) {
		String dir = list.Get(i, 0);
		String usr = list.Get(i, 1);
		String pwd = list.Get(i, 2);
		rs.svn_credentials.GetAdd(dir) = MakeTuple(usr, pwd);
	}
}

CredentialsDlg::CredentialsDlg(RepoSync& ars)
:	rs(ars)
{
	CtrlLayoutOKCancel(*this, "SVN credentials");
	list.AddColumn("SVN directory");
	list.AddColumn("Username").Ctrls<EditString>();
	list.AddColumn("Password").Ctrls<EditString>();
	list.ColumnWidths("500 200 200");
	list.NoCursor().EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy() + Zy(4), Zy(20)));
	
	for(const String& dir : rs.svndir) {
		Tuple<String, String> h = rs.svn_credentials.Get(dir, Tuple<String, String>());
		list.Add(dir, h.a, h.b);
	}
	
	SyncShowPwds();
	
	remember <<= rs.remember_credentials;
	show_passwords << [=] { SyncShowPwds(); };
	clear << [=] {
		if(PromptYesNo("Clear all?")) {
			rs.svn_credentials.Clear();
			for(int i = 0; i < list.GetCount(); i++) {
				list.Set(i, 1, Null);
				list.Set(i, 2, Null);
			}
		}
	};
}

void CredentialsDlg::SyncShowPwds()
{
	for(int i = 0; i < list.GetCount(); i++) {
		EditString *es = dynamic_cast<EditString *>(list.GetCtrl(i, 2));
		if(es)
			es->Password(!show_passwords);
	}
}

void EditCredentials(RepoSync& rs)
{
	CredentialsDlg dlg(rs);
	if(dlg.Execute() == IDOK) {
		rs.remember_credentials = ~dlg.remember;
		dlg.GetResults();
	}
}
