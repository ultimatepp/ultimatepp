#define LAYOUTFILE <ide/urepo.lay>
#include <CtrlCore/lay.h>

class UrepoConsole : public WithUrepoConsoleLayout<TopWindow> {
	typedef UrepoConsole CLASSNAME;
	
	Font font;
	void AddResult(const String& out);
	bool canceled = false;
	int  hide_password_from = 0;
	int  hide_password_to = 0;

public:
	int  System(const char *s);
	int  CheckSystem(const char *s);
	int  Git(const char *dir, const char *command, bool pwd = false);
	void HidePassword(int from, int to)         { hide_password_from = from; hide_password_to = to; }
	void Log(const Value& s, Color ink = SColorText());
	void Perform()	                            { exit.Show(); Execute(); }
	void Clear()                                { list.Clear(); }

	UrepoConsole();
};

String SvnCmd(const char *cmd);

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

	Index<String>            svndir;
	
	struct SvnOptions : WithSvnOptionsLayout<ParentCtrl> {
		SvnOptions() { CtrlLayout(*this); }
	};
	
	struct GitOptions : WithGitOptionsLayout<ParentCtrl> {
		GitOptions() { CtrlLayout(*this); }
	};

	VectorMap<String, int> work;

	String SvnCmd(UrepoConsole& sys, const char *svncmd, const String& dir);
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

	void Dir(const char *dir, int kind);
	void Dir(const char *dir);
	void DoSync();
	
	RepoSync();
	~RepoSync();
};


bool GetCredentials(const String& url, const String& dir, String& username, String& password);
void EditCredentials(const Vector<String>& url_hints);

void RunRepoDiff(const String& filepath, int line = -1);

void LoadBranches(DropList& branch, const String& dir);
void LoadGitRevisions(DropList& r, const String& dir, const String& branch, const String& file);

struct DirRepoDiffDlg : public DirDiffDlg {
	DropList mode[2];
	Button   hash[2];
	DropList r[2], branch[2];
	Vector<String> gitd;
	String         session_id;
	
	void Mode(int i);
	void Revs(int i);
	void AddSelectGit(DropList& dl);
	void Compare();
	void Cleanup();
	void SyncCompare();
	bool IsGit(int i) const { return !IsNull(mode[i]) && IsString(mode[i]); }

	DirRepoDiffDlg();
	~DirRepoDiffDlg();
};
