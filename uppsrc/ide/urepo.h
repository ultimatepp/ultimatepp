#define LAYOUTFILE <ide/urepo.lay>
#include <CtrlCore/lay.h>

class UrepoConsole : public WithUrepoConsoleLayout<TopWindow> {
	typedef UrepoConsole CLASSNAME;
	
	Font font;
	void AddResult(const String& out);
	bool canceled = false;

public:
	int  System(const char *s);
	int  CheckSystem(const char *s);
	int  Git(const char *dir, const char *command);
	void Log(const Value& s, Color ink = SColorText());
	void Perform()	                            { exit.Show(); Execute(); }
	void Clear()                                { list.Clear(); }

	UrepoConsole();
};

String SvnCmd(const char *cmd);

enum { NOT_REPO_DIR = 0, SVN_DIR, GIT_DIR };

int    GetRepoKind(const String& p);
String GetSvnDir(const String& p);

String GitCmd(const char *dir, const char *command);

struct RepoSync : WithRepoSyncLayout<TopWindow> {
	enum {
		MODIFY,
		CONFLICT,
		ADD,
		REMOVE,
		REPLACE,
		
		REPOSITORY,
		MESSAGE,
		
		SVN_IGNORE = -1,
		DELETEC = -2,
		SVN_ACTION = -3,
	};
	
	Array<Switch>            revert;
	Array<DocEdit>           message;
	Array<Button>            diff;
	ArrayMap<String, String> msgmap;

	Index<String>                            svndir;
	VectorMap<String, Tuple<String, String>> svn_credentials;
	bool                                     remember_credentials = true;
	
	struct SvnOptions : WithSvnOptionsLayout<ParentCtrl> {
		SvnOptions() { CtrlLayout(*this); }
	};
	
	struct GitOptions : WithGitOptionsLayout<ParentCtrl> {
		GitOptions() { CtrlLayout(*this); }
	};

	struct Work {
		bool   read_only;
		int    kind;
		String dir;
	};
	
	Array<Work> work;

	String SvnCmd(const char *cmd, const String& dir);
	bool ListGit(const String& path);
	bool ListSvn(const String& path);
	bool GitFile(UrepoConsole& sys, int action, const String& path, bool revert);
	bool SvnFile(UrepoConsole& sys, String& filelist, int action, const String& path, bool revert);
	void SyncCommits();
	void SyncList();
	void DoDiff(int ii);
	void Serialize(Stream& s);

	typedef RepoSync CLASSNAME;

public:
	void   SetMsgs(const String& s);
	String GetMsgs();

	void Dir(bool read_only, const char *dir, int kind);
	void Dir(bool read_only, const char *dir);
	void Dir(const char *dir, int kind)              { Dir(false, dir, kind); }
	void Dir(const char *dir)                        { Dir(false, dir); }
	void DoSync();
	
	RepoSync();
};


void EditCredentials(RepoSync& rs);

void RunRepoDiff(const String& filepath);
