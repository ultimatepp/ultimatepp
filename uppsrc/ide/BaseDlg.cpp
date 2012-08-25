#include "ide.h"

static VectorMap<String, PackageInfo> sPi;

void InvalidatePackageInfo(const String& name)
{
	int q = sPi.Find(name);
	if(q >= 0)
		sPi[q].path.Clear();
}

PackageInfo GetPackageInfo(const String& name)
{
	String path = PackagePath(name);
	Time tm = FileGetTime(path);
	int q = sPi.Find(name);
	if(q >= 0) {
		if(path == sPi[q].path && tm == sPi[q].stamp)
			return sPi[q];
	}
	else {
		q = sPi.GetCount();
		sPi.Add(name);
	}
	PackageInfo& pi = sPi[q];
	pi.path = path;
	pi.stamp = tm;
	Package p;
	p.Load(path);
	pi.ink = p.ink;
	pi.italic = p.italic;
	pi.bold = p.bold;
	return pi;
}

bool BaseSetup(String& vars) { return BaseSetupDlg().Run(vars); }

BaseSetupDlg::BaseSetupDlg()
: browse_out("Output & intermediate files")
{
	CtrlLayoutOKCancel(*this, "Assembly setup");
	browse_upp.SetImage(CtrlImg::right_arrow());
	browse_upp <<= THISBACK(OnBrowseUpp);
	upp.AddFrame(browse_upp);
	upp <<= THISBACK(OnUpp);
	browse_out.Attach(output);
}

bool BaseSetupDlg::Run(String& vars)
{
	upp <<= GetVar("UPP");
	output <<= GetVar("OUTPUT");
	base <<= vars;
	new_base = IsNull(vars);
	while(TopWindow::Run() == IDOK)
	{
		String varname = ~base;
		String varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(FileExists(varfile) && !PromptOKCancel(NFormat("Overwrite existing assembly [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(NFormat("Error writing assmbly [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("UPP", ~upp);
		SetVar("OUTPUT", ~output);
		Vector<String> paths = SplitDirs(upp.GetText().ToString());
		for(int i = 0; i < paths.GetCount(); i++)
			RealizeDirectory(paths[i]);
		RealizeDirectory(output);
		vars = varname;
		return true;
	}
	return false;
}

void BaseSetupDlg::OnBrowseUpp()
{
	String s = ~upp;
	int b, e;
	if(upp.HasFocus())
		upp.GetSelection(b, e);
	else
		e = s.GetLength();
	b = e;
	while(b > 0 && s[b - 1] != ';')
		b--;
	while(e < s.GetLength() && s[e] != ';')
		e++;
	FileSel fsel;
	String pre = s.Left(b), post = s.Mid(e);
	fsel.ActiveDir(s.Mid(b, e - b));
	if(fsel.ExecuteSelectDir()) {
		String newdir = ~fsel;
		upp <<= pre + newdir + Nvl(post, ";");
		upp.SetWantFocus();
		OnUpp();
	}
}

void BaseSetupDlg::OnUpp()
{
	if(new_base) {
		String s = ~upp;
		int f = s.Find(';');
		if(f >= 0) s.Trim(f);
		base <<= GetFileTitle(s);
	}
}
