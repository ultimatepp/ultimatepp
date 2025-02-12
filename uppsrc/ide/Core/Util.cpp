#include "Core.h"

int GetRepo(String& path)
{
	if(IsNull(path))
		return NOT_REPO_DIR;
	if(DirectoryExists(AppendFileName(path, ".svn")) || DirectoryExists(AppendFileName(path, "_svn")))
		return SVN_DIR;
	for(;;) {
		String git = AppendFileName(path, ".git");
		if(DirectoryExists(git) || FileExists(git))
			return GIT_DIR;
		if(DirectoryExists(AppendFileName(path, ".svn")))
			return SVN_DIR;
		String path0 = path;
		path = GetFileFolder(path);
		if(path == path0)
			break;
	}
	return NOT_REPO_DIR;
}

int GetRepoKind(const String& p)
{
	String pp = p;
	return GetRepo(pp);
}

String GetGitPath()
{
#ifdef PLATFORM_WIN32
	static String path;
	ONCELOCK {
		path = AppendFileName(GetExeFolder(), "\\bin\\mingit\\cmd\\git.exe");
		path = FileExists(path) ? "\"" + path + "\"" : "git";
	}
	return path;
#else
	return "git";
#endif
}

bool IsGLSLExt(const String& ext)
{
	return findarg(ext, ".vert", ".tesc", ".tese", ".geom", ".frag", ".comp") >= 0;
}
