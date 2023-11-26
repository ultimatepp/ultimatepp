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

byte passkey_sha256[32];

bool HasCredentialsPasskey()
{
	for(int i = 0; i < 32; i++)
		if(passkey_sha256[i])
			return true;
	return false;
}

String CredentialsCrypt(const String& src)
{
	if(!HasCredentialsPasskey())
		return src;
	int ci = 0;
	byte c[32];
	memcpy(c, passkey_sha256, 32);
	String r;
	for(char b : src) {
		if(ci >= 32) { // poor mans acceptable encryption
			SHA256(c, c, 32);
			ci = 0;
		}
		r.Cat(b ^ c[ci++]);
	}
	return r;
}

struct PasskeyDlg : WithPasskeyLayout<TopWindow> {
	PasskeyDlg();

	void Sync() { passkey1.Password(!show_passkey); passkey2.Password(!show_passkey); }
};

PasskeyDlg::PasskeyDlg()
{
	CtrlLayoutOK(*this, "Passkey");
	
	show_passkey << [=] { Sync(); };
	
	Sync();
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

bool LoadCredentials0(Array<Credential>& r)
{
	r.Clear();
	Value v = ParseJSON(CredentialsCrypt(LoadFile(CredentialsFile())));
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

struct GetPasskeyDlg : WithGetPasskeyLayout<TopWindow> {
	GetPasskeyDlg();
	
	void Sync() { passkey.Password(!show_passkey); }
};

GetPasskeyDlg::GetPasskeyDlg()
{
	CtrlLayoutOK(*this, "Passkey");
	
	show_passkey << [=] { Sync(); };
	
	Sync();
}

bool LoadCredentials(Array<Credential>& r)
{
	if(!FileExists(CredentialsFile()))
		return false;
	if(LoadCredentials0(r)) return true;
	if(!HasCredentialsPasskey()) {
		for(;;) {
			GetPasskeyDlg dlg;
			dlg.Run();
			SHA256(passkey_sha256, ~dlg.passkey);
			if(LoadCredentials0(r)) return true;
			WithFailedPasskeyLayout<TopWindow> p;
			CtrlLayoutOK(p, "Passkey");
			p.Breaker(p.clear, IDEXIT);
			if(p.Run() == IDEXIT) {
				memset(passkey_sha256, 0, 32);
				FileDelete(CredentialsFile());
				break;
			}
		}
	}
	return false;
}

bool GetCredentials(const String& url, const String& dir, String& username, String& password)
{
	Array<Credential> cr;
	if(LoadCredentials(cr)) {
		for(const Credential& c : cr) {
			if(url.StartsWith(c.url) || dir.StartsWith(c.url)) {
				username = c.username;
				password = c.password;
			}
		}
	}
	return false;
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
	
	void Passkey();

	typedef CredentialsDlg CLASSNAME;
	CredentialsDlg(const Vector<String>& url_hints);
};

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
	passkey << [=] { Passkey(); };
}

void CredentialsDlg::Passkey()
{
	PasskeyDlg dlg;
	String k;
	for(;;) {
		if(dlg.Run() == IDCANCEL)
			return;
		k = ~dlg.passkey1;
		if(k == ~dlg.passkey2) {
			if(k.GetCount() || PromptYesNo("Store credentials as plain text?"))
				break;
		}
		else
			Exclamation("Fields do not match!");
	}
	if(k.GetCount())
		SHA256(passkey_sha256, k);
	else
		memset(passkey_sha256, 0, 32);
}

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
		SaveFile(CredentialsFile(), CredentialsCrypt(AsJSON(va)));
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

void EditCredentials(const Vector<String>& url_hints)
{
	CredentialsDlg dlg(url_hints);
	dlg.Load();
	if(dlg.Execute() == IDOK)
		dlg.Save();
}
