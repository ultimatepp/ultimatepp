#define LAYOUTFILE <ide/urepo.lay>
#include <CtrlCore/lay.h>

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
	
	Index<String>            skip;
	
	bool has_skips = false;
	
	struct SvnOptions : WithSvnOptionsLayout<ParentCtrl> {
		SvnOptions() { CtrlLayout(*this); }
	};
	
	struct GitOptions : WithGitOptionsLayout<ParentCtrl> {
		GitOptions() { CtrlLayout(*this); }
	};

	VectorMap<String, int> work;

	String SvnCmd(SysConsole& sys, const char *svncmd, const String& dir);
	bool ListGit(const String& path);
	bool ListSvn(const String& path);
	bool GitFile(SysConsole& sys, int action, const String& path, bool revert);
	bool SvnFile(SysConsole& sys, String& filelist, int action, const String& path, bool revert);
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
void CopyGitRevisions(const DropList& dl);

struct DirRepoDiffDlg : public DirDiffDlg {
	DropList mode[2];
	Button   hash[2], log[2];
	DropList r[2], branch[2];
	WithDropChoice<EditString> masks;
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
