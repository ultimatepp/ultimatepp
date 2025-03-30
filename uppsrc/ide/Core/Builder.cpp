#include "Core.h"

void Builder::ChDir(const String& path)
{
	host->ChDir(path);
}

String TrimSlash(String s)
{
	while(findarg(*s.Last(), '/', '\\') >= 0)
		s.Trim(s.GetCount() - 1);
	return s;
}

String Builder::GetPathQ(const String& path) const
{
	return '\"' + TrimSlash(path) + '\"';
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

VectorMap<String, String> Builder::cmdx_cache;

Time Builder::HdependFileTime(const String& path)
{
	dependencies.Clear();
	return onefile.GetCount() ? path == onefile ? GetSysTime() : Time::Low()
	                          : HdependGetFileTime(path, &dependencies);
}

String Builder::CmdX(const char *s)
{ // expand ` character delimited sections by executing them as commands
	String r, cmd;
	bool cmdf = false;
	for(; *s; s++)
		if(*s == '`') {
			if(cmdf) {
				int q = cmdx_cache.Find(cmd);
				if(q >= 0)
					r << cmdx_cache[q];
				else {
					String h = Sys(cmd);
					r << h;
					cmdx_cache.Add(cmd, h);
					IdeProcessEvents();
				}
				cmd.Clear();
			}
			cmdf = !cmdf;
		}
		else
			(cmdf ? cmd : r).Cat(*s);
	int q = r.Find(' ');
#ifdef PLATFORM_BSD
	const int limit = 1000000;
#else
	const int limit = 8000;
#endif
	if(r.GetCount() > limit && q >= 0) {
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
