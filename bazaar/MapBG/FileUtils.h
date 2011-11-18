#ifndef MAP_BG__FILE_UTILS_H
#define MAP_BG__FILE_UTILS_H

#include <Core/Core.h>
using namespace Upp;

inline bool IsValidPath(const String& fp)
{
	return fp.Left(1) != "." && fp.Left(2) != "..";
}

inline bool IsFolder(const String& fp)
{
	return fp.GetCount() ? fp[fp.GetCount() - 1] == '/' : false;
}

static Vector<String> GetDirectoryFiles(const String& dir, const String& search = "*",
	String prefix = String())
{
	FindFile ff(AppendFileName(dir, search));
	Vector<String> r;

	while (ff)
	{
		if (ff.IsFolder())
		{
			prefix.IsEmpty()
			? r.Append( GetDirectoryFiles(AppendFileName(dir, ff.GetName()), ff.GetName()) )
			: r.Append( GetDirectoryFiles(AppendFileName(dir, ff.GetName()),
				AppendFileName(prefix, ff.GetName())) );
		}
		if (ff.IsFile())
		{
			prefix.IsEmpty()
			? r.Add(ff.GetName())
			: r.Add(AppendFileName(prefix, ff.GetName()));
		}
		ff.Next();
	}

	return r;
}

static Vector<String> GetSubDirectories(const String& dir, String prefix = String())
{
	FindFile ff(AppendFileName(dir, "*"));
	Vector<String> r;

	while (ff)
	{
		if (ff.IsFolder())
		{
			Vector<String> files = GetSubDirectories(AppendFileName(dir, ff.GetName()),
				ff.GetName());

			if (files.GetCount())
				r.Append(files);

			prefix.IsEmpty()
			? r.Add(ff.GetName())
			: r.Add(AppendFileName(prefix, ff.GetName()));
		}
		ff.Next();
	}

	return r;
}

static bool RemoveDirectory(const String& dir)
{
	if (!DirectoryExists(dir))
		return true;

	Vector<String> files = GetDirectoryFiles(dir);
	bool done = true;

	for (int i = files.GetCount() - 1; i >= 0; --i)
	{
		done &= FileDelete( AppendFileName(dir, files[i]) );
	}

	Vector<String> folders = GetSubDirectories(dir);
	for (int i = 0; i < folders.GetCount(); ++i)
	{
		done &= DirectoryDelete( AppendFileName(dir, folders[i]) );
	}

	return done &= DirectoryDelete(dir);
}

#endif
