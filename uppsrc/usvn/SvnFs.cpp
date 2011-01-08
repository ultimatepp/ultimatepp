#include "usvn.h"

SvnSel::SvnSel()
{
	CtrlLayoutOKCancel(*this, "Select SVN url");
	list.WhenLeftDouble = THISBACK(Go);
	list.WhenSel = THISBACK(SyncResult);
	list.Columns(3);
	dirup.SetImage(CtrlImg::DirUp());
	dirup <<= THISBACK(DirUp);
	result.SetReadOnly();
	Sizeable().Zoomable();
	gourl <<= THISBACK(Url);
}

void SvnSel::SyncResult()
{
	String p = url;
	if(list.IsCursor()) {
		const FileList::File& f = list.Get(list.GetCursor());
		if(f.isdir)
			p << '/' << f.name;
	}
	result <<= p;
}

bool SvnSel::Load(const String& path)
{
	String h;
	if(Sys(SvnCmd("list", usr, pwd) + ' ' + path, h))
		return false;
	list.Clear();
	Vector<String> l = Split(h, CharFilterCrLf);
	for(int pass = 0; pass < 2; pass++)
		for(int i = 0; i < l.GetCount(); i++) {
			String fn = l[i];
			if(fn.GetLength()) {
				if(*fn.Last() == '/' || *fn.Last() == '\\') {
					fn.Trim(fn.GetLength() - 1);
					if(pass == 0)
						list.Add(fn, CtrlImg::Dir(), StdFont().Bold(), SColorText(), true);
				}
				else
					if(pass == 1)
						list.Add(fn, CtrlImg::File());
			}
	}
	url = path;
	list.KillCursor();
	SyncResult();
	return true;
}

void SvnSel::DirUp()
{
	int q = url.ReverseFind('/');
	if(q > 0 && url[q - 1] != '/')
		Load(url.Mid(0, q));
}

void SvnSel::Go()
{
	if(list.IsCursor()) {
		const FileList::File& f = list.Get(list.GetCursor());
		if(f.isdir)
			Load(url + '/' + f.name);
	}
}

bool SvnSel::NewUrl()
{
	WithNewUrlLayout<TopWindow> dlg;
	CtrlRetriever r;
	r(dlg.url, url)(dlg.usr, usr)(dlg.pwd, pwd);
	CtrlLayoutOKCancel(dlg, "SVN URL");
	if(dlg.Execute() != IDOK)
		return false;
	String busr = usr;
	String bpwd = pwd;
	r.Retrieve();
	if(!TryLoad((String)~dlg.url)) {
		usr = busr;
		pwd = bpwd;
		return false;
	}
	return true;
}

void SvnSel::Url()
{
	NewUrl();
}

bool SvnSel::TryLoad(const char *url)
{
	if(!Load(url)) {
		Exclamation("Invalid URL!");
		return false;
	}
	return true;
}

bool SvnSel::Select0()
{
	if(url.GetLength() == 0) {
		if(!NewUrl())
			return false;
	}
	return Execute();
}

bool SvnSel::Select()
{
	url.Clear();
	return Select0();
}

bool SvnSel::Select(const char *_url, const char *_usr, const char *_pwd)
{
	usr = _usr;
	pwd = _pwd;
	return TryLoad(url) && Select0();
}
