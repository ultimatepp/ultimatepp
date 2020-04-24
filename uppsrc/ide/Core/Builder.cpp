#include "Core.h"

void Builder::ChDir(const String& path)
{
	host->ChDir(path);
}

String Builder::GetHostPath(const String& path) const
{
	return host->GetHostPath(path);
}

String TrimSlash(String s)
{
	while(findarg(*s.Last(), '/', '\\') >= 0)
		s.Trim(s.GetCount() - 1);
	return s;
}

String Builder::GetHostPathQ(const String& path) const
{
	return '\"' + TrimSlash(GetHostPath(path)) + '\"';
}

Vector<Host::FileInfo> Builder::GetFileInfo(const Vector<String>& path) const
{
	return host->GetFileInfo(path);
}

Host::FileInfo Builder::GetFileInfo(const String& path) const
{
	return GetFileInfo(Vector<String>() << path)[0];
}

Time Builder::GetFileTime(const String& path) const
{
	return GetFileInfo(path);
}

String Builder::CmdX(const char *s)
{ // expand ` character delimited sections by executing them as commands
	String r, cmd;
	bool cmdf = false;
	for(; *s; s++)
		if(*s == '`') {
			if(cmdf) {
				r << Sys(cmd);
				cmd.Clear();
			}
			cmdf = !cmdf;
		}
		else
			(cmdf ? cmd : r).Cat(*s);
	int q = r.Find(' ');
	if(r.GetCount() > 8000 && q >= 0) {
		String rn = CatAnyPath(outdir, AsString(tmpfilei.GetAdd(outdir, 0)++) + ".cmd");
		PutVerbose("Generating response file: " << rn);
		PutVerbose(r);
		r.Replace("\\", "/"); // clang win32 needs this
		SaveFile(rn, r.Mid(q + 1));
		r = r.Mid(0, q) + " @" + rn;
	}
	return r;
}

int Builder::Execute(const char *cmdline)
{
	return host->Execute(CmdX(cmdline));
}

int Builder::Execute(const char *cl, Stream& out)
{
	return host->Execute(CmdX(cl), out);
}

void Builder::DeleteFile(const Vector<String>& path)
{
	host->DeleteFile(path);
}

void Builder::DeleteFile(const String& path)
{
	host->DeleteFile(Vector<String>() << path);
}

bool Builder::RealizeDir(const String& path)
{
	return host->RealizeDir(path);
}

bool Builder::SaveFile(const String& path, const String& data)
{
	return host->SaveFile(path, data);
}

String Builder::LoadFile(const String& path)
{
	return host->LoadFile(path);
}

bool Builder::FileExists(const String& path) const
{
	return !IsNull(GetFileInfo(path).length);
}
