#include "Core.h"

void Builder::ChDir(const String& path)
{
	host->ChDir(path);
}

String Builder::GetHostPath(const String& path) const
{
	return host->GetHostPath(path);
}

String Builder::GetHostPathShort(const String& path) const
{
#ifdef PLATFORM_WIN32
	const dword SHORT_PATH_LENGTH = 2048;
	char short_path[SHORT_PATH_LENGTH];
	dword length = ::GetShortPathName((LPCTSTR) path, (LPTSTR) short_path, SHORT_PATH_LENGTH);
	if(length > 0)
		return String(short_path, length);
#endif
	return path;
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

String Builder::GetHostPathShortQ(const String& path) const
{
	return '\"' + TrimSlash(GetHostPathShort(path)) + '\"';
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

int Builder::Execute(const char *cmdline)
{
	return host->Execute(cmdline);
}

int Builder::Execute(const char *cl, Stream& out)
{
	return host->Execute(cl, out);
}

void Builder::DeleteFile(const Vector<String>& path)
{
	host->DeleteFile(path);
}

void Builder::DeleteFile(const String& path)
{
	host->DeleteFile(Vector<String>() << path);
}

void Builder::SaveFile(const String& path, const String& data)
{
	host->SaveFile(path, data);
}

String Builder::LoadFile(const String& path)
{
	return host->LoadFile(path);
}

bool Builder::FileExists(const String& path) const
{
	return !IsNull(GetFileInfo(path).length);
}
