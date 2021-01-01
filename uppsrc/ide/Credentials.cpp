#include "ide.h"

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

struct Credential {
	String url;
	String username;
	String password;
};

String CredentialsFile()
{
	return ConfigFile("repo.credentials");
}

bool LoadCredentials(Array<Credential>& r)
{
	r.Clear();
	String s = LoadFile(CredentialsFile());
	Value v = ParseJSON(s);
	if(IsError(v))
		return false;
	try {
		for(Value e : v) {
			Credential& c = r.Add();
			c.url = e["url"];
			c.username = e["username"];
			c.password = e["password"];
		}
	}
	catch(ValueTypeError) {
		r.Clear();
		return false;
	}
	return true;
}

bool GetCredentials(const String& url, const String& dir, String& username, String& password)
{
	Array<Credential> cr;
	int best = 0;
	if(LoadCredentials(cr)) {
		for(const Credential& c : cr) {
			for(int pass = 0; pass < 2; pass++) {
				const String& u = pass ? url : dir;
				int n = min(u.GetCount(), c.url.GetCount());
				int ml;
				for(ml = 0; ml < n; ml++)
					if(u[ml] != c.url[ml])
						break;
				if(ml > best) {
					best = ml;
					username = c.username;
					password = c.password;
				}
			}
		}
	}
	return best;
}

struct CredentialDlg : WithCredentialLayout<TopWindow> {
	void Hints(const Vector<String>& url_hints);
	void Sync() { password.Password(!show_password); }
	CredentialDlg();
};

void CredentialDlg::Hints(const Vector<String>& url_hints)
{
	for(String s : url_hints)
		url.AddList(s);
}

CredentialDlg::CredentialDlg()
{
	CtrlLayoutOKCancel(*this, "Credentials");
	Sync();
	show_password << [=] { Sync(); };
}

struct CredentialsDlg : WithCredentialsLayout<TopWindow> {
	const Vector<String>& url_hints;

	void Sync();
	void Add();
	void Edit();
	void Remove();

	void Load();
	void Save();

	typedef CredentialsDlg CLASSNAME;
	CredentialsDlg(const Vector<String>& url_hints);
};

void CredentialsDlg::Load()
{
	list.Clear();
	Array<Credential> cr;
	if(LoadCredentials(cr)) {
		for(const Credential& c : cr)
			list.Add(c.url, c.username, c.password);
		list.GoBegin();
	}
}

void CredentialsDlg::Save()
{
	if(list.GetCount()) {
		ValueArray va;
		for(int i = 0; i < list.GetCount(); i++)
			va << ValueMap()("url", list.Get(i, 0))
			                ("username", list.Get(i, 1))
			                ("password", list.Get(i, 2));
		SaveFile(CredentialsFile(), AsJSON(va));
	}
	else
		FileDelete(CredentialsFile());
}

void CredentialsDlg::Add()
{
	CredentialDlg dlg;
	dlg.Hints(url_hints);
again:
	if(dlg.Run() == IDOK) {
		String url = ~dlg.url;
		if(list.FindSetCursor(url)) {
			Exclamation("Duplicate entry!");
			goto again;
		}
		list.Add(url, ~dlg.username, ~dlg.password);
		list.Sort();
		list.FindSetCursor(url);
	}
}

void CredentialsDlg::Edit()
{
	if(!list.IsCursor())
		return;
	CredentialDlg dlg;
	dlg.Hints(url_hints);
	dlg.url <<= list.Get(0);
	dlg.username <<= list.Get(1);
	dlg.password <<= list.Get(2);
again:
	if(dlg.Run() == IDOK) {
		String url = ~dlg.url;
		int ii = list.Find(url);
		if(ii >= 0 && ii != list.GetCursor()) {
			Exclamation("Duplicate entry!");
			list.SetCursor(ii);
			goto again;
		}
		list.Set(0, url);
		list.Set(1, ~dlg.username);
		list.Set(2, ~dlg.password);
		list.Sort();
		list.FindSetCursor(url);
	}
}

void CredentialsDlg::Remove()
{
	list.DoRemove();
}

void CredentialsDlg::Sync()
{
	clear.Enable(list.GetCount());
	edit.Enable(list.IsCursor());
	remove.Enable(list.IsCursor());
	
	list.ColumnAt(2)
	    .SetConvert(show_passwords ? StdConvert()
	                               : LambdaConvert([](const Value& v) {
		                                 return String('*', (~v).GetCount());
		                             })
		);
}

CredentialsDlg::CredentialsDlg(const Vector<String>& url_hints)
:	url_hints(url_hints)
{
	CtrlLayoutOKCancel(*this, "Credentials");
	list.AddColumn("Url (or directory)");
	list.AddColumn("Username");
	list.AddColumn("Password");
	list.ColumnWidths("500 200 200");
	list.EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy() + Zy(4), Zy(20)));
	list.WhenSel = [=] { Sync(); };
	list.WhenLeftDouble = [=] { Edit(); };
	
	show_passwords << [=] { Sync(); };
	add << [=] { Add(); };
	edit << [=] { Edit(); };
	remove << [=] { Remove(); };
	clear << [=] {
		if(PromptYesNo("Remove all?"))
			list.Clear();
	};
}

void EditCredentials(const Vector<String>& url_hints)
{
	CredentialsDlg dlg(url_hints);
	dlg.Load();
	if(dlg.Execute() == IDOK)
		dlg.Save();
}
