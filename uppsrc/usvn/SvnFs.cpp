#include "usvn.h"

SvnSel::SvnSel()
{
	CtrlLayoutOKCancel(*this, "Select SVN folder");
	list.WhenLeftDouble = THISBACK(Go);
}

void SvnSel::Load()
{
	list.Clear();
	String path = url;
	if(folder.GetCount()) {
		list.Add("..", CtrlImg::DirUp(), StdFont().Bold(), SColorText(), true);
		path << '/' << folder;
	}
	Vector<String> l = Split(Sys(SvnCmd("list", usr, pwd) + ' ' + path), CharFilterCrLf);
	for(int i = 0; i < l.GetCount(); i++) {
		String fn = l[i];
		if(fn.GetLength()) {
			bool isdir = false;
			if(*fn.Last() == '/' || *fn.Last() == '\\') {
				fn.Trim(fn.GetLength() - 1);
				list.Add(fn, CtrlImg::Dir(), StdFont().Bold(), SColorText(), true);
			}
			else
				list.Add(fn, CtrlImg::File());
		}
	}
}

void SvnSel::Go()
{
	if(list.IsCursor()) {
		const FileList::File& f = list.Get(list.GetCursor());
		if(f.isdir) {
			if(f.name == "..")
				folder.Trim(max(0, folder.Find(f.name)));
			else
				folder << '/' << f.name;
			Load();
		}
	}
}

String SvnSel::Select(const char *_url, const char *_usr, const char *_pwd)
{
	url = _url;
	usr = _usr;
	pwd = _pwd;
	Load();
	Execute();
	return url + '/' + folder;
}
